pub mod player;
pub mod gameplay_logic;
pub mod actions_strategies;
pub mod ai;

use bevy::prelude::*;

#[derive(Component, Deref, DerefMut)]
pub struct GameTimer(pub Timer);

#[derive(Component)]
pub struct Border {
    pub border_type: BorderType
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum BorderType {
    TopBorder,
    BottomBorder,
    LeftBorder,
    RightBorder
}
