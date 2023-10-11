use bevy::prelude::*;
use bevy_rapier2d::prelude::Velocity;
use crate::components::{
    common::{
        GameStates, events::{
            Event, Events, GameEvents, SystemEvents
        }
    },
    gameplay::player::PlayerComponent, actions::{ActionComponent, Actions},
    movement::PlayerMovementComponent, animation::AnimationComponent
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
                            next_state.set(GameStates::GameSetup);
                        } else {
                            //TODO: print error
                        }
                    },
                    SystemEvents::GameConfigured(result) => {
                        if result {
                            next_state.set(GameStates::AnimationsSetup);
                        } else {
                            //TODO: print error
                        }
                    }
                    SystemEvents::AnimationsConfigured(result) => {
                        if result {
                            next_state.set(GameStates::PlayerSetup);
                        } else {
                            //TODO: print error
                        }
                    }
                    SystemEvents::PlayerConfigured(result) => {
                        if result {
                            next_state.set(GameStates::RunGame);
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

// Listen for game events
pub fn handle_game_events(
    mut events: EventReader<Event>,
    mut components: Query<(
        &mut ActionComponent,
        &mut AnimationComponent,
        &mut PlayerMovementComponent,
        &Velocity
    ), With<PlayerComponent>>
) {
    for event in events.iter() {
        match event.event_type {
            Events::GameEvents(game_event) => {
                match game_event {
                    GameEvents::ActionChanged(new_action) => {
                        for (mut action_component, mut animation_component, mut movement_component, velocity) in components.iter_mut() {
                            on_action_changed(
                                new_action,
                                &mut action_component,
                                &mut animation_component,
                                &mut movement_component,
                                velocity
                            );
                        }
                    }
                    _ => {},
                }
            }
            _ => {},
        }
    }
}

fn on_action_changed(
    new_action: Actions,
    action_component: &mut ActionComponent,
    animation_component : &mut AnimationComponent,
    movement_component: &mut PlayerMovementComponent,
    velocity: &Velocity
) {
    let animations = action_component.get_animation_map_by_action(new_action);
    if !animations.is_empty() {
        action_component.current_action = new_action;
        animation_component.set_new_animation_sequence(&animations);
        movement_component.on_action_changed(new_action, velocity.linvel);
    }
}
