use std::collections::VecDeque;
use bevy::prelude::*;
use bevy::text::*;

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
    mut player_query: Query<(&mut MessageManager, &Transform), Without<MessageComponent>>,
    mut message_query: Query<(Entity, &mut Transform), With<MessageComponent>>
) {

    match player_query.get_single_mut() {
        Ok((mut message_manager, player_transform)) => {
            let mut is_finished: bool = false;

            for (entity, mut transform) in message_query.iter_mut() {
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
                transform.translation.y += 100.0;
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

    // TODO: Refactor style
    let other_box_size = Vec2::new(300.0, 200.0);
    let other_box_position = Vec2::new(320.0, -250.0);

    commands
        .spawn(SpriteBundle {
            sprite: Sprite {
                color: Color::rgb(0.20, 0.3, 0.70),
                custom_size: Some(Vec2::new(other_box_size.x, other_box_size.y)),
                ..default()
            },
            transform: Transform::from_translation(other_box_position.extend(0.0)),
            ..default()
        })
        .with_children(|builder| {
            builder.spawn(Text2dBundle {
                text: Text {
                    sections: vec![TextSection::new(
                        message.content,
                        TextStyle { font: text_font.font.clone(), font_size: 42.0, color: Color::RED },
                    )],
                    alignment: TextAlignment::Left,
                    linebreak_behavior: BreakLineOn::AnyCharacter,
                },
                text_2d_bounds: Text2dBounds {
                    // Wrap text in the rectangle
                    size: other_box_size,
                },
                // ensure the text is drawn on top of the box
                transform: Transform::from_translation(Vec3::Z),
                ..default()
            });
        })
    .insert(TransformBundle::default())
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

