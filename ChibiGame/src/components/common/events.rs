use bevy::prelude::*;
use crate::components::actions::Actions;

#[derive(Debug, PartialEq, Eq, Clone, Copy, Hash)]
pub enum OverlapType {
    Started(Entity),
    Ended(Entity)
}

#[derive(Debug, PartialEq, Eq, Clone, Copy, Hash)]
pub enum GameEvents {
    ActionChanged(Actions),
    SetNewAction(Actions),
    PlayerOverlapped(OverlapType)
}

#[derive(Debug, PartialEq, Eq, Clone, Copy, Hash)]
pub enum SystemEvents {
    ConfigLoaded(bool),
    ClientLoaded(bool),
    GameConfigured(bool),
    AnimationsConfigured(bool),
    PlayerConfigured(bool),
}

#[derive(Debug, PartialEq, Eq, Clone, Copy, Hash)]
pub enum Events {
    GameEvents(GameEvents),
    SystemEvents(SystemEvents)
}

#[derive(Event)]
pub struct Event {
    pub event_type: Events
}
