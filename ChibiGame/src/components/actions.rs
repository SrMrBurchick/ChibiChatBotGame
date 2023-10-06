use bevy::prelude::*;

#[derive(Debug, Clone, Copy, Eq, PartialEq, Hash, Reflect)]
pub enum WalkDirection {
    Left,
    Right
}

#[derive(Debug, Clone, Copy, Eq, PartialEq, Hash, Reflect)]
pub enum ClimbDirection {
    Up,
    Down
}

#[derive(Debug, Clone, Eq, PartialEq, Hash)]
pub enum MoveType {
    Walk(WalkDirection),
    Climb(ClimbDirection)
}

#[derive(Debug, Clone, Copy, Eq, PartialEq, Hash)]
pub enum Actions {
    Climb,
    Eat,
    Fall,
    PickUp,
    Sleep,
    StandBy,
    Walk,
    Unknown
}

pub fn string_to_action(action: &str) -> Actions {
    match action {
        "walk" => Actions::Walk,
        "standby" => Actions::StandBy,
        "pickup" => Actions::PickUp,
        "climb" => Actions::Climb,
        "fall" => Actions::Fall,
        _ => Actions::Unknown,
    }
}

pub fn action_to_string(action: &Actions) -> &str {
    match action {
        Actions::Walk => "walk",
        Actions::Climb => "climb",
        Actions::Fall => "fall",
        Actions::StandBy => "standby",
        _ => "",
    }
}

#[derive(Component, PartialEq, Debug)]
pub struct ActionComponent {
    pub current_action: Actions,
}

impl ActionComponent {
    pub fn on_action_changed(
        &mut self,
    ) {}
}

impl Default for ActionComponent {
    fn default() -> Self {
        Self {
            current_action: Actions::Unknown,
        }
    }
}
