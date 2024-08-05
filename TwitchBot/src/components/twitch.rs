use std::sync::Arc;

use json::{self, JsonValue};
use serde_json::Value;
use crate::parsers::{
    config::get_value, request_parser::{
        convert_twitch_type_to_enum, EventType, RequestSender
    }
};
use crate::components::action::Action;

#[derive(Debug, Clone)]
pub struct Twitch {
    pub user_token: String,
    pub client_id: String,
    pub session_id: String,
    pub user_id: String,
    pub actions: Vec<Action>,
    pub isConnected: bool,
    pub isSubscribed: bool,
    sender: Option<crossbeam_channel::Sender<Action>>
}

impl Twitch {
    pub fn new()->Self {
        Self{
            user_token: String::new(),
            client_id: String::new(),
            session_id: String::new(),
            user_id: String::new(),
            actions: vec![],
            isConnected: false,
            isSubscribed: false,
            sender: None
        }
    }

    pub fn setup_sender(&mut self, sender: crossbeam_channel::Sender<Action>) {
        self.sender = Some(sender);
    }

    fn parse_welcome_message(&mut self, data: &JsonValue) {
        match get_value(&data, "session") {
            Ok(session) => {
                match get_value(&session, "id") {
                    Ok(id) => {
                        self.session_id = id.to_string();
                        if !self.session_id.is_empty() {
                            self.isConnected = true;
                        }
                    },
                    Err(_) => {},
                }
            },
            Err(_) => {},
        }
    }

    fn parse_notification(&mut self, data: &JsonValue) {
        match get_value(&data, "subscription") {
            Ok(subscription) => {
                match get_value(&subscription, "type") {
                    Ok(event_type) => {
                        for action in self.actions.iter_mut() {
                            if action.event_type == event_type.to_string() {
                                match get_value(&data, "event") {
                                    Ok(event) => {
                                        action.prepare_action(convert_twitch_type_to_enum(event_type.to_string(), &event));
                                    },
                                    Err(_) => {},
                                }
                            }
                        }
                    },
                    Err(_) => {},
                }
            },
            Err(_) => {},
        }
    }

    pub fn do_action(&mut self) {
        for action in self.actions.iter_mut() {
            match action.action_event_type {
                EventType::Unknown => {
                    // Do nothing
                }
                _ => {
                    match &self.sender {
                        Some(request_sender) => {
                            match request_sender.send(action.clone()) {
                                Ok(_) => {
                                    println!("Action sent! {:?}", action.clone());
                                },
                                Err(_) => {
                                    println!("Failed to send action! {:?}", action.clone());
                                },
                            };

                            action.action_event_type = EventType::Unknown;
                        },
                        None => {},
                    }
                },
            }
        }
    }

    pub async fn parse_message(&mut self, data: &JsonValue) {
        match get_value(&data, "metadata") {
            Ok(metadata) => {
                match get_value(&metadata, "message_type") {
                    Ok(message_type) => {
                        match get_value(&data, "payload") {
                            Ok(payload) => {
                                match message_type.to_string().as_ref() {
                                    "session_welcome" => {
                                        self.parse_welcome_message(&payload);
                                    },
                                    "notification" => {
                                        self.parse_notification(&payload);
                                    }
                                    _ => {},
                                }
                            },
                            Err(_) => {},
                        }

                    },
                    Err(_) => {},
                }
            },
            Err(_) => {},
        }
    }

    async fn subscribe_to_event(&self, body: &Value) {
        let mut token : String = String::from("Bearer ");
        token = token + self.user_token.as_str();

        let client = reqwest::Client::new();
        let response = client.post("https://api.twitch.tv/helix/eventsub/subscriptions")
            .header("Authorization", token.as_str())
            .header("Client-Id", self.client_id.as_str())
            .header("Content-Type", "application/json")
            .json(&body)
            .send().await;

        match response {
            Ok(data) => {
                println!("Subscribtion result {:?}", data);
            },
            Err(_) => {},
        }
    }

    async fn subscribe_to_channel_points_rewards(&self) {
        let body = serde_json::json!({
            "type":"channel.channel_points_custom_reward_redemption.add",
            "version": "1",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_follow(&self) {
        let body = serde_json::json!({
            "type":"channel.follow",
            "version": "2",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str(),
                "moderator_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_subscription(&self) {
        let body = serde_json::json!({
            "type":"channel.subscribe",
            "version": "1",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_raid(&self) {
        let body = serde_json::json!({
            "type":"channel.raid",
            "version": "1",
            "condition" : {
                "to_broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_ban(&self) {
        let body = serde_json::json!({
            "type":"channel.ban",
            "version": "1",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_unban(&self) {
        let body = serde_json::json!({
            "type":"channel.unban",
            "version": "1",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_moder_add(&self) {
        let body = serde_json::json!({
            "type":"channel.moderator.add",
            "version": "1",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    async fn subscribe_to_moder_remove(&self) {
        let body = serde_json::json!({
            "type":"channel.moderator.remove",
            "version": "1",
            "condition" : {
                "broadcaster_user_id": self.user_id.as_str()
            },
            "transport": {
                "method": "websocket",
                "session_id": self.session_id.as_str()
            }
        });

        self.subscribe_to_event(&body).await;
    }

    pub async fn subscribe_to_events(&mut self) {
        self.isSubscribed = true;
        self.subscribe_to_channel_points_rewards().await;
        self.subscribe_to_subscription().await;
        self.subscribe_to_follow().await;
        self.subscribe_to_raid().await;
        self.subscribe_to_ban().await;
        self.subscribe_to_unban().await;
        self.subscribe_to_moder_add().await;
        self.subscribe_to_moder_remove().await;
    }
}
