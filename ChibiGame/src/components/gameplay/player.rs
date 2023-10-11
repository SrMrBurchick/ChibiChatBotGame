use bevy::{prelude::*, sprite::SpriteSheetBundle};

use crate::components::{
    actions::{ActionComponent, Actions},
    animation::AnimationComponent,
    movement::PlayerMovementComponent,
};

#[derive(Component)]
pub struct PlayerComponent;

#[derive(Component, PartialEq, Debug)]
pub struct PlayerXp(pub u32);

#[derive(Bundle)]
pub struct Player {
    // #[bundle]
    sprite: SpriteSheetBundle,

    // Components
    animation_component: AnimationComponent,
    action_component: ActionComponent,
    movement_component: PlayerMovementComponent,
    xp: PlayerXp,
}

impl Player {
    pub fn new() -> Self {
        Self {
            sprite: SpriteSheetBundle::default(),
            xp: PlayerXp(0),
            action_component: ActionComponent {
                current_action: Actions::Unknown
            },
            animation_component: AnimationComponent::new(),
            movement_component: PlayerMovementComponent::new()
        }
    }

    pub fn set_texture_atlas(&mut self, texture_atlas: Handle<TextureAtlas>) {
        self.sprite.texture_atlas = texture_atlas;
    }

    pub fn set_animation_component(&mut self, animation_component: AnimationComponent) {
        self.animation_component = animation_component;
    }
}
