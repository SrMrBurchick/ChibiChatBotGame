use std::collections::VecDeque;
use bevy::prelude::*;
use bevy::text::*;
use bevy_stroked_text::*;

use crate::components::system::config::Config;
use crate::components::{
    gameplay::player::PlayerComponent,
    common::ui::UISettings
};

#[derive(Debug, Clone)]
pub struct Message {
    pub content: String,
    pub message_color: String,
    pub message_border_color: String,
    pub timeout: f32,
    pub font_size: f32,
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
    mut timer_query: Query<(Entity, &mut MessageDurationTimer)>,
    text_font: Res<UISettings>,
    config_query: Query<&Config>,
    mut player_query: Query<(&mut MessageManager, &Transform), Without<MessageComponent>>,
    mut message_query: Query<(Entity, &mut Transform), With<MessageComponent>>
) {

    match player_query.get_single_mut() {
        Ok((mut message_manager, player_transform)) => {
            let mut is_finished: bool = false;
            let mut is_running: bool = false;

            for (entity, mut transform) in message_query.iter_mut() {
                info!("Message transform: {:?}", transform);
                match timer_query.get_single_mut() {
                    Ok((timer_entity, mut timer)) => {
                        timer.timer.tick(time.delta());

                        is_finished = timer.timer.just_finished();
                        is_running = true;

                        if is_finished {
                            commands.entity(timer_entity).despawn_recursive();
                        }
                    },
                    Err(_) => {},
                }

                if is_finished {
                    commands.entity(entity).despawn_recursive();
                    continue;
                }
                //
                // // TODO: Move message above character if player is not on the to border
                // // Disable to message go through borders
                // transform.translation = player_transform.translation;
                // transform.translation.y += text_info.logical_size.y;
                //
                // match config_query.get_single() {
                //     Ok(config) => {
                //         match config.get_sprite_size() {
                //             Ok(sprite_size) => {
                //                 let offset = sprite_size.height as f32 * player_transform.scale.y;
                //                 transform.translation.y += offset;
                //             },
                //             Err(_) => {},
                //         }
                //     },
                //     Err(_) => {},
                // }
            }

            if is_finished {
                spawn_notification(
                    &mut commands,
                    &mut message_manager,
                    &text_font
                );
            } else if message_manager.is_have_massage() && !is_running {
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
    ui_settings: &Res<UISettings>,
) {
    let message = message_manager.get_next_notification();
    if (message.is_none()) {
        return;
    }

    info!("Spawn message: {:?}", message.clone());
    let message = message.unwrap();

    commands.spawn(
        StrokedTextBundle {
            text: StrokedText {
                text: message.content,
                font: ui_settings.font.clone(),
                stroke_color: Color::hex(message.message_border_color.as_str()).unwrap(),
                color: Color::hex(message.message_color.as_str()).unwrap(),
                font_size: message.font_size,
                text_anchor: bevy::sprite::Anchor::Center
            },
            ..default()
    })
    .insert(MessageComponent);

    commands.spawn(MessageDurationTimer {
        timer: Timer::from_seconds(message.timeout, TimerMode::Once)
    });
}

pub fn test_message_system(
    mut player_query: Query<&mut MessageManager, With<PlayerComponent>>,
) {
    match player_query.get_single_mut() {
        Ok(mut manager) => {
            let message = Message {
                content: String::from("Tobi pizda!"),
                message_color: String::from("#ff0000"),
                message_border_color: String::from("#000000"),
                timeout: 10.0,
                font_size: 54.0
            };
            manager.add_message(&message);
        },
        Err(_) => {},
    }
}

