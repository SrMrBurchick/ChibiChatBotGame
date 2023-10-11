use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

use crate::components::{
    common::events::{
        Events, Event, SystemEvents
    },
    animation::AnimationComponent,
    gameplay::player::*,
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

        let texture_handle = asset_server.load(config.get_img());
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

        let texture_atlas_handle = texture_atlases.add(texture_atlas);

        let mut player = Player::new();
        player.set_texture_atlas(texture_atlas_handle);
        player.set_animation_component(animation_component);

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
            .insert(PlayerComponent);
    }

    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::PlayerConfigured(true)),
    });
}
