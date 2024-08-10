use json::JsonValue;

use crate::parsers::request_parser::EventType;

#[derive(Debug, Clone)]
pub struct Action {
    pub name: String,
    pub reward_to_listen: String,
    pub event_type: String,
    pub user_input: String,
    pub action_event_type: EventType
}

impl Action {
    pub fn new() -> Self {
        Self {
            name: String::default(),
            reward_to_listen: String::default(),
            event_type: String::default(),
            user_input: String::default(),
            action_event_type: EventType::Unknown
        }
    }

    pub fn prepare_action(&mut self, event: EventType) {
        match event.clone() {
            EventType::ChannelPointsAction { user, reward_id, user_input} => {
                if self.reward_to_listen != reward_id {
                    return;
                }
            }
            _ => {
                return;
            },
        }

        self.action_event_type = event;
    }

    pub fn get_action_json(&self) -> JsonValue {
        let mut action_obj = JsonValue::new_object();

        action_obj.insert("action", self.name.clone()).unwrap();
        match self.action_event_type.clone() {
            EventType::ChannelPointsAction { user, reward_id, user_input } => {
                if !user_input.is_empty() {
                    let mut message = user_input.clone();
                    if !self.user_input.is_empty() {
                        let format_placeholders = vec!["{user}", "{admin}", "{timeout}", "{count}", "{gifter}", "{viewers}", "{role}", "{input}"];
                        println!("Try to format {} to {}", user_input.clone(), self.user_input.clone());
                        let mut format_message = self.user_input.clone();
                        for placeholder in format_placeholders {
                            if self.user_input.contains(placeholder) {
                                println!("Found placeholder! {}", placeholder.clone());
                                let mut place_holder_text = String::default();
                                match placeholder {
                                    "{user}" => {
                                        place_holder_text = user.clone();
                                    }
                                    "{input}" => {
                                        place_holder_text = user_input.clone();
                                    }
                                    _ => {},
                                }

                                if !place_holder_text.is_empty() {
                                    format_message = format_message.replace(placeholder, place_holder_text.as_str());
                                    println!("Updated format: {}", format_message.clone());
                                }
                            }
                        }

                        message = format_message.clone();
                    }

                    action_obj.insert("message", message.clone()).unwrap();
                }
            }
            _ => {},
        }


        println!("Action prepared for sent! {:?} JSON = {:?}", self, action_obj);
        return action_obj;
    }
}
