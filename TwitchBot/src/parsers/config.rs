use std::error::Error;
use std::fs;
use json::{self, JsonValue};

use crate::components::action::Action;


#[derive(Debug)]
pub struct Config {
    pub host: String,
    pub port: u32,
    pub actions: Vec<Action>,
    pub client_id: String,
    pub user_token: String
}

fn get_value(content: &JsonValue, key: &str) -> Result<JsonValue, Box<dyn Error>> {
    let value: JsonValue;

    if 0 == key.len() {
        return Err(Box::from(format!("Unknown key {:?}", key)));
    }

    if JsonValue::is_null(content) {
        return Err(Box::from("Config content not initialized!"));
    }

    value = content[key].clone();

    if JsonValue::is_null(&value) {
        return Err(Box::from(format!("Key {} not configured", key)));
    }

    return Ok(value);
}

impl Config {
    pub fn new() -> Self {
        Config {
            host: String::new(),
            port: 0,
            actions: Vec::new(),
            client_id: String::new(),
            user_token: String::new()
        }
    }

    fn init_twitch_info(&mut self, data: &JsonValue) {
        if data.is_object() {
            match get_value(data, "oauth-token") {
                Ok(value) => {
                    self.user_token = value.to_string();
                },
                Err(_) => {},
            }

            match get_value(data, "client-id") {
                Ok(value) => {
                    self.client_id = value.to_string();
                },
                Err(_) => {},
            }
        }

    }

    fn init_base_info(&mut self, data: &JsonValue) {
        if data.is_object() {
            match get_value(data, "port") {
                Ok(value) => {
                    self.port = value.as_u32().unwrap();
                },
                Err(_) => {},
            }

            match get_value(data, "host") {
                Ok(value) => {
                    self.host = value.to_string();
                },
                Err(_) => {},
            }
        }
    }

    fn init_actions(&mut self, actions: &JsonValue) {
        if actions.is_array() {
            for element in actions.members() {
                let mut action: Action = Action::new();
                match get_value(&element, "name") {
                    Ok(name) => {
                        action.name = name.to_string();
                    },
                    Err(_) => {},
                }
                match get_value(&element, "config") {
                    Ok(config) => {
                        match get_value(&config, "twitch-reward-id") {
                            Ok(reward_id) => {
                                action.reward_to_listen = reward_id.to_string();
                            },
                            Err(_) => {},
                        }
                    },
                    Err(_) => {},
                }

                println!("Action parsed: {:?}", action);
                self.actions.push(action);

            }
        }
    }

    pub fn init(&mut self, content: JsonValue) {
        match get_value(&content, "twitch-bot") {
            Ok(bot) => {
                self.init_base_info(&bot);
            },
            Err(_) => {

            },
        }

        match get_value(&content, "actions") {
            Ok(actions) => {
                self.init_actions(&actions);
            },
            Err(_) => {},
        }

        match get_value(&content, "twitch-settings") {
            Ok(settings) => {
                self.init_twitch_info(&settings);
            },
            Err(_) => {},
        }

        println!("Config initialied: {:?}", self);
    }
}

pub fn parse_config(config_path: String) -> Config {
    let mut config = Config::new();

    match fs::read_to_string(&config_path) {
        Ok(data) => {
            match json::parse(&data) {
                Ok(content) => {
                    config.init(content);
                },
                Err(_) => {
                },
            }
        },
        Err(_) => {},
    }

    return config;
}
