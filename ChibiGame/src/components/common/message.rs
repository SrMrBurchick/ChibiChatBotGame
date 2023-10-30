use std::collections::VecDeque;
use bevy::prelude::*;
use bevy::text::*;

use crate::components::system::config::Config;
use crate::components::{
    gameplay::player::PlayerComponent,
    common::ui::TextFont
};

#[derive(Debug, Clone)]
pub struct Message {
    pub content: String
    // TODO: Add types
}

#[derive(Component)]
pub struct MessageComponent;

#[derive(Component, Debug)]
pub struct MessageManager {
    messages: VecDeque<Message>
}

#[derive(Component, Debug)]
pub struct MessageDurationTimer {
    pub timer: Timer,
}

impl MessageManager {
    pub fn new() -> Self{
        MessageManager {
            messages: VecDeque::default()
        }
    }

    pub fn add_message(&mut self, message :&Message) {
        self.messages.push_back(message.clone());
    }

    pub fn get_next_notification(&mut self) -> Option<Message> {
        if self.messages.is_empty() {
            return None;
        }

        return self.messages.pop_front()
    }

    pub fn is_have_massage(&self) -> bool {
        !self.messages.is_empty()
    }
}

pub fn message_system(
    mut commands: Commands,
    time: Res<Time>,
    mut timer_query: Query<&mut MessageDurationTimer>,
    text_font: Res<TextFont>,
    config_query: Query<&Config>,
    mut player_query: Query<(&mut MessageManager, &Transform), Without<MessageComponent>>,
    mut message_query: Query<(Entity, &mut Transform, &TextLayoutInfo), With<MessageComponent>>
) {

    match player_query.get_single_mut() {
        Ok((mut message_manager, player_transform)) => {
            let mut is_finished: bool = false;

            for (entity, mut transform, text_info) in message_query.iter_mut() {
                match timer_query.get_single_mut() {
                    Ok(mut timer) => {
                        timer.timer.tick(time.delta());

                        is_finished = timer.timer.just_finished();
                    },
                    Err(_) => {},
                }

                if is_finished {
                    commands.entity(entity).despawn_recursive();
                    continue;
                }

                // TODO: Move message above character if player is not on the to border
                // Disable to message go through borders
                transform.translation = player_transform.translation;
                transform.translation.y += text_info.size.y;

                match config_query.get_single() {
                    Ok(config) => {
                        match config.get_sprite_size() {
                            Ok(sprite_size) => {
                                let offset = sprite_size.height as f32 * player_transform.scale.y;
                                transform.translation.y += offset;
                            },
                            Err(_) => {},
                        }
                    },
                    Err(_) => {},
                }
            }

            if is_finished {
                spawn_notification(
                    &mut commands,
                    &mut message_manager,
                    &text_font
                );
            } else if message_manager.is_have_massage() {
                spawn_notification(
                    &mut commands,
                    &mut message_manager,
                    &text_font
                );
            }
        },
        Err(_) => {},
    }
}

fn spawn_notification(
    commands: &mut Commands,
    message_manager: &mut MessageManager,
    text_font: &Res<TextFont>,
) {
    let message = message_manager.get_next_notification();
    info!("Spawn message: {:?}", message.clone());
    if (message.is_none()) {
        return;
    }

    let message = message.unwrap();

    commands.spawn(
        Text2dBundle {
            text: Text {
                sections: vec![TextSection::new(
                    message.content,
                    TextStyle { font: text_font.font.clone(), font_size: 24.0, color: Color::RED },
                )],
                alignment: TextAlignment::Center,
                linebreak_behavior: BreakLineOn::AnyCharacter,
            },
            transform: Transform::from_translation(Vec3::Z),
            ..default()
    })
    .insert(MessageComponent);

    commands.spawn(MessageDurationTimer {
        timer: Timer::from_seconds(5.0, TimerMode::Once)
    });
}

pub fn test_message_system(
    mut player_query: Query<&mut MessageManager, With<PlayerComponent>>,
) {
    match player_query.get_single_mut() {
        Ok(mut manager) => {
            let message = Message { content: String::from("Tobi pizda!") };
            manager.add_message(&message);
        },
        Err(_) => {},
    }
}

