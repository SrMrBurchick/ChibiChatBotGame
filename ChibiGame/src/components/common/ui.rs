use bevy::prelude::*;

#[derive(Resource)]
pub struct TextFont {
    pub font: Handle<Font>,
}
