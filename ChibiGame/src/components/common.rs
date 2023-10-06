use bevy::utils::HashMap;
use bevy::prelude::*;

pub mod sprite_index;
pub mod events;

use crate::components::{
    actions::Actions,
    common::sprite_index::SpriteIndex
};

#[derive(Debug, Default, Clone)]
pub struct AnimationsMap(pub HashMap<Actions, Vec<SpriteIndex>>);

#[derive(Debug, Default, PartialEq, Eq)]
pub struct TableSize {
    pub columns: u32,
    pub rows: u32
}

#[derive(Debug, Default, PartialEq, Eq)]
pub struct SpriteSize {
    pub height: u32,
    pub width: u32
}

#[derive(Debug, Clone, Copy, Eq, PartialEq, Hash)]
pub enum BorderType {
    TopBorder,
    BottomBorder,
    LeftBorder,
    RightBorder
}

#[derive(Component)]
pub struct Border {
    pub border_type: BorderType
}
