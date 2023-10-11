use crate::components::{
    common::events::{Event, Events, SystemEvents},
    gameplay::{Border, BorderType, GameTimer},
};
use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

pub fn setup_game(
    mut commands: Commands,
    windows: Query<&Window>,
    mut event_writer: EventWriter<Event>,
) {
    let window = windows.single();
    let height = window.height();
    let width = window.width();
    let border_size = 10.0;

    // Setup borders
    // Top border
    commands
        .spawn(SpriteBundle {
            sprite: Sprite::default(),
            ..default()
        })
        .insert(Collider::cuboid(width, border_size))
        .insert(Transform::from_xyz(0.0, height / 2.0, 0.0))
        .insert(Border {
            border_type: BorderType::TopBorder,
        });

    // Bottom border
    commands
        .spawn(SpriteBundle {
            sprite: Sprite::default(),
            ..default()
        })
        .insert(Collider::cuboid(width, border_size))
        .insert(Transform::from_xyz(0.0, height / -2.0, 0.0))
        .insert(Border {
            border_type: BorderType::BottomBorder,
        });

    // Left border
    commands
        .spawn(SpriteBundle {
            sprite: Sprite::default(),
            ..default()
        })
        .insert(Collider::cuboid(border_size, height))
        .insert(Transform::from_xyz(width / -2.0 + border_size, 0.0, 0.0))
        .insert(Border {
            border_type: BorderType::LeftBorder,
        });

    // Left border
    commands
        .spawn(SpriteBundle {
            sprite: Sprite::default(),
            ..default()
        })
        .insert(Collider::cuboid(border_size, height))
        .insert(Transform::from_xyz(width / 2.0 + border_size, 0.0, 0.0))
        .insert(Border {
            border_type: BorderType::RightBorder,
        });

    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::GameConfigured(true)),
    });
}
