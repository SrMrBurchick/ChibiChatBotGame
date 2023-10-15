use bevy::prelude::*;
use crate::components::{
    common::sprite_index::SpriteIndex,
    gameplay::gameplay_logic::GameplayLogicComponent,
    movement::PlayerMovementComponent,
    actions::{
        MoveType, WalkDirection
    }
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
    type_registry: Res<AppTypeRegistry>,
    mut query: Query<(
        &GameplayLogicComponent,
        &PlayerMovementComponent,
        &mut AnimationComponent,
        &mut TextureAtlasSprite,
        &Handle<TextureAtlas>,
    )>,
) {
    for (gameplay_component, movement_component, mut anim_component, mut atlas, handle) in &mut query {
        if !gameplay_component.is_current_action_valid() && !gameplay_component.get_action_running_status() {
            continue;
        }

        animtimer.timer.tick(time.delta());
        if animtimer.timer.just_finished() {
            let texture_atlas = texture_atlases.get(handle).unwrap();
            let next_sprite = anim_component.get_next_sprite();
            let sprite_start_index = next_sprite.row * anim_component.map_columns;
            let next_sprite_index = sprite_start_index + next_sprite.column;
            atlas.index = next_sprite_index as usize % texture_atlas.textures.len();

            let mut inverted: bool = next_sprite.inverted;
            match movement_component.get_move_type(&type_registry) {
                Some(move_type) => {
                    match move_type {
                        MoveType::Walk(direction) => {
                            match direction {
                                WalkDirection::Left => {
                                    inverted = !next_sprite.inverted;
                                }
                                _ => {
                                },
                            }
                        }
                        _ => {
                        },
                    }
                }
                _ => {
                },
            }

            atlas.flip_x = inverted;
        }
    }
}
