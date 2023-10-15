use crate::components::{
    actions::{Actions, is_action_can_interrupt},
    common::events::{Event, Events, GameEvents},
    gameplay::{
        player::PlayerComponent,
        Border, BorderType
    }, movement::PlayerMovementComponent,
};
use bevy::prelude::*;

#[derive(Component, Debug)]
pub struct ActionDurationTimer {
    pub timer: Timer,
}

#[derive(Component)]
pub struct GameplayLogicComponent {
    pending_actions: Vec<Actions>,
    high_priority_actions: Vec<Actions>,
    is_action_running: bool,
    current_action: Actions,
    can_be_interrupted: bool,
}

impl GameplayLogicComponent {
    pub fn new() -> Self {
        GameplayLogicComponent {
            pending_actions: vec![],
            high_priority_actions: vec![],
            is_action_running: false,
            current_action: Actions::Unknown,
            can_be_interrupted: false
        }
    }

    pub fn try_to_set_action(&mut self, action: Actions) -> bool {
        if self.current_action == action && action != Actions::Unknown {
            return false;
        }

        // If current action cannot be interrupted than any action must be pass
        // to the low_priority actions. Even if the next action cannot be interrupted either
        if !self.can_be_interrupted && self.is_current_action_valid() {
            self.pending_actions.push(action);
            return true;
        }

        // If current action can be interrupted than any other action must be pass
        // to hight priority, even if the new action can be interrupted
        if self.can_be_interrupted && self.is_current_action_valid() {
            self.high_priority_actions.push(action);
            return true;
        }

        // Set only when current action is unknown
        info!("Set action {:?}", action);
        self.current_action = action;
        self.can_be_interrupted = !is_action_can_interrupt(action);

        return true;
    }

    pub fn get_current_action(&self) -> Actions {
        self.current_action
    }

    pub fn get_next_action(&mut self) -> Actions {
        if !self.high_priority_actions.is_empty() {
            return self.high_priority_actions.pop().unwrap();
        }

        if self.pending_actions.is_empty() {
            self.is_action_running = false;
            self.can_be_interrupted = true;
            return Actions::Unknown;
        }

        return self.pending_actions.pop().unwrap();
    }

    pub fn get_action_duration(&self) -> f32 {
        // TODO: Provide random
        return 5.0;
    }

    pub fn on_player_hit_border(
        &mut self,
        border: &Border,
        movement: &mut PlayerMovementComponent
    ) {
        match border.border_type {
            BorderType::LeftBorder => {
                movement.can_climb = true;
            },
            BorderType::RightBorder => {
                movement.can_climb = true;
            }
            BorderType::BottomBorder => {
                movement.landed = true;
                movement.enabled = true;
            }
            BorderType::TopBorder => {
                movement.landed = false;
            }
            _ => {},
        }
    }

    pub fn on_player_detach_from_border(
        &mut self,
        border: &Border,
        movement: &mut PlayerMovementComponent
    ) {
        match border.border_type {
            BorderType::LeftBorder => {
                movement.can_climb = false;
            },
            BorderType::RightBorder => {
                movement.can_climb = false;
            }
            BorderType::BottomBorder => {
                movement.landed = false;
            }
            BorderType::TopBorder => {
                if !movement.can_climb {
                    movement.enabled = false;
                }

                movement.landed = false;
            }
            _ => {},
        }
    }

    pub fn get_action_running_status(&self) -> bool {
        self.is_action_running
    }

    pub fn stop_action(&mut self) {
        self.current_action = Actions::Unknown;
        self.is_action_running = false;
        self.can_be_interrupted = true;
    }

    pub fn is_interruption_available(&self) -> bool {
        !self.high_priority_actions.is_empty()
    }

    pub fn is_current_action_valid(&self) -> bool {
        return self.current_action != Actions::Unknown;
    }
}

pub fn game_logic_system(
    mut commands: Commands,
    event_writer: EventWriter<Event>,
    time: Res<Time>,
    mut timer_query: Query<(Entity, &mut ActionDurationTimer)>,
    mut gameplay_query: Query<&mut GameplayLogicComponent, With<PlayerComponent>>,
    movement_query: Query<&PlayerMovementComponent, With<PlayerComponent>>,
) {
    let is_action_timer_finished: bool;
    let mut timer_entity: Option<Entity> = None;

    // Timer logic
    match timer_query.get_single_mut() {
        Ok((entity, mut action_timer)) => {
            timer_entity = Some(entity);
            action_timer.timer.tick(time.delta());
            is_action_timer_finished = action_timer.timer.finished();

            if is_action_timer_finished {
                commands.entity(entity).despawn();
            }
        }
        Err(_) => {
            is_action_timer_finished = true;
        }
    }

    // Gameplay logic
    match gameplay_query.get_single_mut() {
        Ok(mut gameplay_component) => {
            // If action setted but still not running
            if !gameplay_component.get_action_running_status()
                && gameplay_component.is_current_action_valid()
            {
                start_new_action(&mut gameplay_component, &mut commands, event_writer);

                return;
            }

            // If action timer completed
            if is_action_timer_finished && gameplay_component.get_action_running_status() {
                // Fall action may stopped only when player is landed
                if gameplay_component.get_current_action() == Actions::Fall {
                    match movement_query.get_single() {
                        Ok(movement) => {
                            if movement.landed {
                                on_action_completed(&mut gameplay_component, event_writer);
                            }
                        }
                        _ => {},
                    }
                } else {
                    on_action_completed(&mut gameplay_component, event_writer);
                }

                return;
            }

            // If new action can interrupt the current
            if gameplay_component.is_interruption_available() {
                interrupt_current_action(
                    &mut commands,
                    event_writer,
                    &mut gameplay_component,
                    timer_entity
                );

                return;
            }

        }
        Err(_) => {}
    }
}

fn start_new_action(
    gameplay_component: &mut GameplayLogicComponent,
    commands: &mut Commands,
    mut event_writer: EventWriter<Event>,
) {
    if gameplay_component.get_current_action() != Actions::Fall {
        commands.spawn(ActionDurationTimer{
            timer: Timer::from_seconds(
                gameplay_component.get_action_duration(), TimerMode::Once
            )
        });
    }

    event_writer.send(Event {
        event_type: Events::GameEvents(
            GameEvents::ActionChanged(gameplay_component.get_current_action())
        ) }
    );

    gameplay_component.is_action_running = true;
}

fn on_action_completed(
    gameplay_component: &mut GameplayLogicComponent,
    mut event_writer: EventWriter<Event>,
) {
    gameplay_component.stop_action();

    let next_action = gameplay_component.get_next_action();
    if next_action != Actions::Unknown {
        event_writer.send(Event {
            event_type: Events::GameEvents(GameEvents::SetNewAction(next_action))
        });
    } else {
        event_writer.send(Event {
            event_type: Events::GameEvents(
                GameEvents::ActionChanged(next_action)
            )}
        );
    }
}

fn interrupt_current_action(
    commands: &mut Commands,
    mut event_writer: EventWriter<Event>,
    gameplay_component: &mut GameplayLogicComponent,
    timer_entity: Option<Entity>,
) {
    if timer_entity != None {
        commands.entity(timer_entity.unwrap()).despawn();
    }

    gameplay_component.stop_action();
    event_writer.send(Event {
        event_type: Events::GameEvents(
            GameEvents::SetNewAction(gameplay_component.get_next_action())
        ) }
    );
}
