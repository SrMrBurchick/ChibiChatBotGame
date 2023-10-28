use bevy::prelude::*;
use bevy::utils::HashMap;
use crate::components::common::{
    sprite_index::SpriteIndex, AnimationsMap
};

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

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct UserAction {
    pub action: String,
    pub animations: Vec<SpriteIndex>
}

#[derive(Debug, Clone, Eq, PartialEq, Hash)]
pub enum Actions{
    Climb,
    Eat,
    Fall,
    PickUp,
    Sleep,
    StandBy,
    SwapDirection,
    Walk,
    UserAction(String),
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

#[derive(Component, Debug)]
pub struct ActionComponent {
    pub current_action: Actions,
    user_actions: Vec<UserAction>,
    animation_map: HashMap<Actions, Vec<SpriteIndex>>
}

impl ActionComponent {
    pub fn on_action_changed(
        &mut self,
    ) {}

    pub fn add_user_action(&mut self, user_action: UserAction) {
        if self.user_actions.contains(&user_action) {
            return;
        }

        self.user_actions.push(user_action.clone());
    }

    pub fn init_animation_map(&mut self, animations_map: &AnimationsMap) {
        if !self.animation_map.is_empty() {
            self.animation_map.clear();
        }

        self.animation_map = animations_map.0.clone();
    }

    pub fn init_user_animations(&mut self, user_actions: &Vec<UserAction>) {
        if !self.user_actions.is_empty() {
            self.user_actions.clear();
        }

        self.user_actions = user_actions.clone();

        for user_action in &self.user_actions {
            self.animation_map.insert(
                Actions::UserAction(user_action.action.clone()),
                user_action.animations.clone()
            );
        }
    }

    pub fn get_animation_map_by_action(&self, action: Actions) -> Vec<SpriteIndex> {
        let animations = vec![];

        match self.animation_map.get(&action) {
            Some(animations) => {
                return animations.clone();
            },
            None => {
                error!("Not registered action {:?}", action);
            },
        }

        return animations;
    }
}

impl Default for ActionComponent {
    fn default() -> Self {
        Self {
            // TODO: Remove current action
            current_action: Actions::Unknown,
            user_actions: vec![],
            animation_map: HashMap::default()
        }
    }
}

pub fn is_action_can_interrupt(action: Actions) -> bool {
    match action {
        Actions::Walk => false,
        Actions::Climb => false,
        Actions::Unknown => false,
        Actions::SwapDirection => false,
        _ => true
    }
}
