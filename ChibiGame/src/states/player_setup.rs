use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

use crate::components::{
    common::{
        message::MessageManager,
        events::{
            Events, Event, SystemEvents, GameEvents
        }
    },
    animation::AnimationComponent,
    actions::{ActionComponent, Actions},
    movement::PlayerMovementComponent,
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
        let texture_atlas_handle = texture_atlases.add(texture_atlas);

        info!("Animation_component {:?}", animation_component);

        let mut action_component = ActionComponent::default();

        action_component.init_animation_map(&config.get_configured_animations_map());
        action_component.init_user_animations(&config.get_user_actions());

        info!("Action component {:?}", action_component);

        // Setup AI component
        let mut ai_component = AIComponent::new();
        let predefined_actions = config.get_predefined_actions();
        for predefined_action in predefined_actions.iter() {
            ai_component.add_new_action(
                action_component.get_action_from_string(predefined_action.0.clone()),
                predefined_action.1
            );
        }

        ai_component.add_new_action(Actions::SwapDirection, 1); // 1% chance to swap current direction

        commands.insert_resource(AITimer {
            timer: Timer::from_seconds(config.get_generate_new_action_timeout(), TimerMode::Repeating),
        });

        // Setup gameplay logic component
        let mut gameplay_logic = GameplayLogicComponent::new();
        gameplay_logic.set_action_duration(config.get_action_execution_time());

        // Setup movement
        let mut movement_component = PlayerMovementComponent::new();
        movement_component.speed = config.get_movement_speed();

        // Setup player
        let mut player = Player::new();
        player.set_texture_atlas(texture_atlas_handle);
        player.set_animation_component(animation_component);
        player.set_action_component(action_component);
        player.set_movement_component(movement_component);

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
            .insert(gameplay_logic)
            .insert(ai_component)
            .insert(MessageManager::new())
            .insert(PlayerComponent);
    }

    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::PlayerConfigured(true)),
    });

    event_writer.send(Event {
        event_type: Events::GameEvents(GameEvents::SetNewAction(Actions::Fall)),
    });
}
