use bevy::prelude::*;
use crate::components::{
    actions::{
        ActionComponent, Actions
    },
    common::sprite_index::SpriteIndex
};

#[derive(Component, Debug)]
pub struct AnimationComponent {
    pub animations: Vec<SpriteIndex>,
    pub map_columns: u32,
    current_index: usize,
}

impl AnimationComponent {
    pub fn new() -> Self {
        Self {
            animations: Vec::default(),
            map_columns: 0,
            current_index: 0,
        }
    }

    pub fn get_next_sprite(&mut self) -> SpriteIndex {
        if self.animations.is_empty() {
            SpriteIndex::default()
        } else {
            self.current_index = (self.current_index + 1) % self.animations.len();
            self.animations[self.current_index].clone()
        }
    }

    pub fn set_new_animation_sequence(&mut self, animations: &Vec<SpriteIndex>) {
        self.animations = animations.clone();
    }
}

#[derive(Resource, Debug)]
pub struct AnimationTimer {
    pub timer: Timer,
}

pub fn animation_system(
    time: Res<Time>,
    texture_atlases: Res<Assets<TextureAtlas>>,
    mut animtimer: ResMut<AnimationTimer>,
    mut query: Query<(
        &ActionComponent,
        &mut AnimationComponent,
        &mut TextureAtlasSprite,
        &Handle<TextureAtlas>,
    )>,
) {
    for (action_component, mut anim_component, mut atlas, handle) in &mut query {
        if action_component.current_action == Actions::Unknown {
            continue;
        }

        animtimer.timer.tick(time.delta());
        if animtimer.timer.just_finished() {
            let texture_atlas = texture_atlases.get(handle).unwrap();
            let next_sprite = anim_component.get_next_sprite();
            let sprite_start_index = next_sprite.row * anim_component.map_columns;
            let next_sprite_index = sprite_start_index + next_sprite.column;
            atlas.index = next_sprite_index as usize % texture_atlas.textures.len();
            atlas.flip_x = next_sprite.inverted;
        }
    }
}
