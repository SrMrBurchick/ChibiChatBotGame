use bevy::prelude::*;
use crate::components::common::{
    GameStates, events::{
        Event, Events, GameEvents, SystemEvents
    }
};

// Listen to events for states changes
pub fn system_event_listener(
    mut events: EventReader<Event>,
    mut next_state: ResMut<NextState<GameStates>>
) {
    for event in events.iter() {
        match event.event_type {
            Events::SystemEvents(system_event) => {
                match system_event {
                    SystemEvents::ConfigLoaded(result) => {
                        if result {
                            next_state.set(GameStates::ChatBotConnectionSetup);
                        } else {
                            //TODO: print error
                        }
                    }
                    SystemEvents::ConfigLoaded(result) => {
                        if result {
                            next_state.set(GameStates::GameRunning);
                        } else {
                            //TODO: print error
                        }
                    }
                    _ => {
                        // Do nothing
                    }
                }
            }
            _ => {
                // Handle at game events listener
            }
        }
    }
}
