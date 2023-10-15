use bevy::prelude::*;
use bevy_rapier2d::prelude::Velocity;
use crate::components::{
    common::{
        GameStates, events::{
            Event, Events, GameEvents, SystemEvents, OverlapType
        }
    },
    gameplay::{
        player::PlayerComponent,
        gameplay_logic::GameplayLogicComponent, Border, BorderType
    },
    actions::{ActionComponent, Actions},
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
                        info!("ConfigLoaded {:?}", result);
                        if result {
                            next_state.set(GameStates::ChatBotConnectionSetup);
                        } else {
                            //TODO: print error
                        }
                    }
                    SystemEvents::ClientLoaded(result) => {
                        info!("ClientLoaded {:?}", result);
                        if result {
                            next_state.set(GameStates::GameSetup);
                        } else {
                            //TODO: print error
                        }
                    },
                    SystemEvents::GameConfigured(result) => {
                        info!("GameConfigured {:?}", result);
                        if result {
                            next_state.set(GameStates::AnimationsSetup);
                        } else {
                            //TODO: print error
                        }
                    }
                    SystemEvents::AnimationsConfigured(result) => {
                        info!("AnimationsConfigured {:?}", result);
                        if result {
                            next_state.set(GameStates::PlayerSetup);
                        } else {
                            //TODO: print error
                        }
                    }
                    SystemEvents::PlayerConfigured(result) => {
                        info!("PlayerConfigured {:?}", result);
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
        &mut GameplayLogicComponent,
        &mut Velocity
    ), With<PlayerComponent>>,
    borders: Query<(Entity, &Border), With<Border>>
) {
    for event in events.iter() {
        match event.event_type {
            Events::GameEvents(game_event) => {
                match game_event {
                    GameEvents::SetNewAction(new_action) => {
                        for (_, _, movement, mut gameplay_logic_component, _) in components.iter_mut() {
                            info!("Set new action {:?}", new_action);
                            if !gameplay_logic_component.try_to_set_action(new_action) {
                                // TODO: send error message
                            }
                        }
                    }
                    GameEvents::ActionChanged(action) => {
                        for (
                            mut action_component,
                            mut animation_component,
                            mut movement_component,
                            _,
                            mut velocity
                        ) in components.iter_mut() {
                            info!("Action changed to {:?}", action);
                            on_action_changed(
                                action,
                                &mut action_component,
                                &mut animation_component,
                                &mut movement_component,
                                &mut velocity
                            );
                        }
                    },
                    GameEvents::PlayerOverlapped(overlap) => {
                        info!("Player has overlapped with: {:?}", overlap);
                        match overlap {
                            OverlapType::Started(overlap_entity) => {
                                for border in borders.iter() {
                                    // Check is overlapped object is border
                                    if border.0 == overlap_entity {
                                        for (
                                            _, _,
                                            mut movement,
                                            mut gameplay_logic_component,
                                            _) in components.iter_mut() {
                                            gameplay_logic_component.on_player_hit_border(border.1, &mut movement);
                                        }

                                    }
                                }
                            }
                            OverlapType::Ended(overlap_entity) => {
                                for border in borders.iter() {
                                    // Check is overlapped object is border
                                    if border.0 == overlap_entity {
                                        for (
                                            _, _,
                                            mut movement,
                                            mut gameplay_logic_component,
                                            _) in components.iter_mut() {
                                            gameplay_logic_component.on_player_detach_from_border(border.1, &mut movement);
                                        }
                                    }
                                }
                            }
                            _ => {},
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
    velocity: &mut Velocity
) {
    let animations = action_component.get_animation_map_by_action(new_action);
    if !animations.is_empty() {
        action_component.current_action = new_action;
        animation_component.set_new_animation_sequence(&animations);
    }
    movement_component.on_action_changed(new_action, velocity);
}
