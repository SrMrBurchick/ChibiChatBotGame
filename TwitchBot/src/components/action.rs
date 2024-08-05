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
            EventType::ChannelPointsAction { user, reward_id } => {
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

        action_obj.insert("action", self.name.clone());

        return action_obj;
    }
}
