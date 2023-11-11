use bevy::prelude::*;

#[derive(Resource)]
pub struct UISettings {
    pub font: Handle<Font>,
    pub message_color: String
}
