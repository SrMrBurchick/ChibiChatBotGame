use std::collections::VecDeque;

use bevy::prelude::*;

#[derive(Debug, Clone)]
pub struct Message {
    pub content: String
    // TODO: Add types
}

#[derive(Component, Debug)]
pub struct MessageComponent {
    messages: VecDeque<Message>
}

impl MessageComponent {
    pub fn new() -> Self{
        MessageComponent {
            messages: VecDeque::default()
        }
    }

    pub fn add_message(&mut self, message :&Message) {
        self.messages.push_back(message.clone());
    }

    pub fn get_nex_notification(&mut self) -> Option<Message> {
        if self.messages.is_empty() {
            return None;
        }

        return self.messages.pop_front()
    }
}
