use crate::components::{
    common::{
        events::{Event, Events, SystemEvents},
        ui::UISettings
    },
    gameplay::{
        Border, BorderType, GameTimer,
        actions_strategies::{
            climb_strategy::ClimbStrategy,
            walk_strategy::WalkStrategy
        }
    },
    system::config::Config,
    movement::{WalkComponent, ClimbComponent, MonitorMovementInfo}
};
use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

pub fn setup_game(
    mut commands: Commands,
    mut windows: Query<&mut Window>,
    config_query: Query<&Config>,
    mut event_writer: EventWriter<Event>,
    type_registry: ResMut<AppTypeRegistry>,
    asset_server: Res<AssetServer>
) {
    let config = config_query.single();
    let (width, height) = config.get_screen_resolution();

    // Set Window resolution
    let mut window = windows.single_mut();
    window.resolution.set(width, height);

    // Register movement components traits
    type_registry.write().register::<WalkComponent>();
    type_registry.write().register::<ClimbComponent>();

    // Register actions strategies
    type_registry.write().register::<ClimbStrategy>();
    type_registry.write().register::<WalkStrategy>();

    // Setup borders
    let border_size = 10.0;

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

    // Setup UI
    let font_handle: Handle<Font> = asset_server.load("fonts/ComicSansMS.ttf");
    let msg_settings = config.get_message_settings();
    commands.insert_resource(UISettings {
        font: font_handle,
        message_color: msg_settings.message_color,
        font_size: msg_settings.font_size
    });

    // Init movement monitor resource
    commands.insert_resource(MonitorMovementInfo {
        fall_populated: false
    });

    // Go to next state
    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::GameConfigured(true)),
    });
}
