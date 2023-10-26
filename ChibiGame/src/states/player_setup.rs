use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

use crate::components::{
    common::events::{
        Events, Event, SystemEvents, GameEvents
    },
    animation::AnimationComponent,
    actions::{ActionComponent, Actions},
    gameplay::{
        player::*,
        gameplay_logic::GameplayLogicComponent,
        ai::{
            AIComponent, AITimer
        }
    },
    system::config::Config
};

pub fn setup_player(
    mut commands: Commands,
    mut event_writer: EventWriter<Event>,
    asset_server: Res<AssetServer>,
    mut texture_atlases: ResMut<Assets<TextureAtlas>>,
    query: Query<&Config>,
) {
    for config in &query {
        let mut animation_component: AnimationComponent = AnimationComponent::new();
        animation_component.map_columns = config.get_table_size().unwrap().columns;

        let texture_handle = asset_server.load("sprite-sheet.png");
        let sprite_size = config.get_sprite_size().unwrap();
        let table_size = config.get_table_size().unwrap();
        let texture_atlas = TextureAtlas::from_grid(
            texture_handle,
            Vec2::new(sprite_size.height as f32, sprite_size.width as f32),
            table_size.columns as usize,
            table_size.rows as usize,
            None,
            None,
        );

        info!("Animation_component {:?}", animation_component);

        let mut action_component = ActionComponent::default();

        action_component.init_animation_map(&config.get_configured_animations_map());
        action_component.init_user_animations(&config.get_user_actions());

        info!("Action component {:?}", action_component);

        let texture_atlas_handle = texture_atlases.add(texture_atlas);

        let mut player = Player::new();
        player.set_texture_atlas(texture_atlas_handle);
        player.set_animation_component(animation_component);
        player.set_action_component(action_component);

        // Setup AI component
        let mut ai_component = AIComponent::new();
        ai_component.add_new_action(Actions::Walk, 0.2); // 20% chance to pick walk action
        ai_component.add_new_action(Actions::Climb, 0.4); // 40% chance to active a climb action
        ai_component.add_new_action(Actions::SwapDirection, 0.01); // 1% chance to swap current direction
        commands.insert_resource(AITimer {
            timer: Timer::from_seconds(15.0, TimerMode::Repeating),
        });

        commands
            .spawn(player)
            .insert(RigidBody::Dynamic)
            .insert(Collider::cuboid(
                sprite_size.width as f32 / 2.0,
                sprite_size.height as f32 / 2.0,
            ))
            .insert(LockedAxes::ROTATION_LOCKED)
            .insert(ActiveEvents::COLLISION_EVENTS)
            .insert(Velocity {
                linvel: Vec2 { x: 0.0, y: 0.0 },
                angvel: 0.0,
            })
            .insert(TransformBundle::from(Transform::from_scale(Vec3::splat(
                config.get_sprite_scale(),
            ))))
            .insert(GameplayLogicComponent::new())
            .insert(ai_component)
            .insert(PlayerComponent);
    }

    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::PlayerConfigured(true)),
    });

    event_writer.send(Event {
        event_type: Events::GameEvents(GameEvents::SetNewAction(Actions::Fall)),
    });
}
