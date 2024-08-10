use bevy::prelude::*;
use bevy::utils::HashMap;
use json::JsonValue;
use crate::components::{
    system::config::{
        get_value, init_sprite_index
    },
    common::{
        sprite_index::SpriteIndex, AnimationsMap
    }
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
pub enum ActionType{
    Climb,
    Eat,
    Fall,
    PickUp,
    Say(String),
    Sleep,
    StandBy,
    SwapDirection,
    Walk,
    UserAction(String /*Action name*/, String /*Message*/),
    Unknown
}

pub fn string_to_action(action: &str) -> ActionType {
    match action {
        "walk" => ActionType::Walk,
        "standby" => ActionType::StandBy,
        "pickup" => ActionType::PickUp,
        "climb" => ActionType::Climb,
        "fall" => ActionType::Fall,
        _ => ActionType::Unknown,
    }
}

pub fn action_to_string(action: &ActionType) -> &str {
    match action {
        ActionType::Walk => "walk",
        ActionType::Climb => "climb",
        ActionType::Fall => "fall",
        ActionType::StandBy => "standby",
        _ => "",
    }
}

#[derive(Debug, Clone)]
pub struct ActionMessageSettings {
    pub color: String,
    pub border_color: String,
    pub font_size: f32,
    pub timeout: f32
}

impl ActionMessageSettings {
    pub fn new() -> Self {
        Self {
            color: String::default(),
            border_color: String::default(),
            font_size: 24.0,
            timeout: 10.0,
        }
    }
}

#[derive(Component, Debug, Clone)]
pub struct ActionComponent {
    pub action_type: ActionType,
    pub message_settings: ActionMessageSettings,
    pub can_interrupt: bool,
    animations: Vec<SpriteIndex>,
}

impl ActionComponent {
    pub fn new() -> Self {
        Self {
            action_type: ActionType::Unknown,
            message_settings: ActionMessageSettings::new(),
            can_interrupt: false,
            animations: vec![]
        }
    }

    pub fn add_animation(&mut self, animation: &SpriteIndex) {
        self.animations.push(animation.clone());
    }

    pub fn get_animations(&self) -> &Vec<SpriteIndex> {
        return &self.animations;
    }
}

impl PartialEq<ActionComponent> for ActionType {
    fn eq(&self, other: &ActionComponent) -> bool {
        match self {
            ActionType::UserAction(name, _) => {
                match &other.action_type {
                    ActionType::UserAction(other_name, _) => {
                        return *name == *other_name;
                    }
                    _ => {
                        return false;
                    },
                }
            }
            _ => {
                return *self == other.action_type;
            },
        }
    }
}

impl PartialEq<ActionType> for ActionComponent {
    fn eq(&self, other: &ActionType) -> bool {
        return *other == *self;
    }
}

impl PartialEq for ActionComponent {
    fn eq(&self, other: &Self) -> bool {
        return self.action_type == *other;
    }
}

#[derive(Component, Debug)]
pub struct ActionsManager {
    pub current_action: ActionType,
    actions: Vec<ActionComponent>,
}

impl ActionsManager {
    pub fn on_action_changed(
        &mut self,
    ) {}

    fn add_new_action(&mut self, action: &ActionComponent) {
        if !self.actions.contains(action) {
            self.actions.push(action.clone());
        }
    }

    pub fn parse_actions(&mut self, data: &JsonValue) {
        if !data.is_array() {
            error!("Actions not configured");
            return;
        }

        for item in data.members() {
            match get_value(&item, "name") {
                Ok(name) => {
                    let mut action = ActionComponent::new();
                    let key = string_to_action(&name.to_string());

                    if key == ActionType::Unknown {
                        if !name.is_empty() {
                            action.action_type = ActionType::UserAction(name.to_string(), String::default());
                        }
                    } else {
                        action.action_type = key;
                    }

                    match get_value(&item, "animations") {
                        Ok(animations) => {
                            if !animations.is_array() {
                                return;
                            }

                            for animation in animations.members() {
                                match init_sprite_index(animation) {
                                    Ok(sprite_index) => {
                                        action.add_animation(&sprite_index);
                                    }
                                    Err(_) => {
                                        // continue;
                                    }
                                }
                            }
                        },
                        Err(_) => {},
                    }

                    match get_value(&item, "config") {
                        Ok(config) => {
                            match get_value(&config, "can-interrupt") {
                                Ok(can_interrupt) => {
                                    action.can_interrupt = can_interrupt.as_bool().unwrap();
                                },
                                Err(_) => {},
                            }
                            match get_value(&config, "text-settings") {
                                Ok(text_settings) => {
                                    match get_value(&text_settings, "color") {
                                        Ok(color) => {
                                            action.message_settings.color = color.to_string();
                                        },
                                        Err(_) => {},
                                    }

                                    match get_value(&text_settings, "text-border-color") {
                                        Ok(color) => {
                                            action.message_settings.border_color = color.to_string();
                                        },
                                        Err(_) => {},
                                    }

                                    match get_value(&text_settings, "display-time") {
                                        Ok(value) => {
                                            action.message_settings.timeout = value.as_f32().unwrap();
                                        },
                                        Err(_) => {},
                                    }

                                    match get_value(&text_settings, "size") {
                                        Ok(value) => {
                                            action.message_settings.font_size = value.as_f32().unwrap();
                                        },
                                        Err(_) => {},
                                    }
                                },
                                Err(_) => {},
                            }

                        },
                        Err(_) => {},
                    }

                    self.add_new_action(&action);
                },
                Err(_) => {},
            }
        }
    }

    pub fn get_action_by_type(&self, action_type: ActionType) -> Option<&ActionComponent> {
        self.actions.iter().find(|action| *action == &action_type)
    }

    pub fn get_animation_map_by_action_type(&self, action_type: ActionType) -> Vec<SpriteIndex> {
        let animations = vec![];

        match self.get_action_by_type(action_type.clone()) {
            Some(action) => {
                return action.get_animations().clone();
            },
            None => {
                error!("Not registered action {:?}", action_type);
            },
        }

        return animations;
    }

    pub fn get_action_type_from_string(&self, action_string: String) -> ActionType {
        let action: ActionType = string_to_action(action_string.as_str());

        match action {
            ActionType::Unknown => {
                if !action_string.is_empty() {
                    return ActionType::UserAction(action_string, String::default());
                }
            }
            _ => {},
        }

        return action;
    }
}

impl Default for ActionsManager {
    fn default() -> Self {
        Self {
            // TODO: Remove current action
            current_action: ActionType::Unknown,
            actions: vec![]
        }
    }
}

pub fn is_action_can_interrupt(action: ActionType) -> bool {
    match action {
        ActionType::Walk => false,
        ActionType::Unknown => false,
        ActionType::SwapDirection => false,
        ActionType::StandBy => false,
        _ => true
    }
}
