use crate::components::{
    actions::{Actions, is_action_can_interrupt},
    common::events::{Event, Events, GameEvents},
    gameplay::{
        player::PlayerComponent,
        Border, BorderType,
        actions_strategies::{
            ActionLogicStrategy, climb_strategy::ClimbStrategy,
            walk_strategy::WalkStrategy
        }
    }, movement::PlayerMovementComponent,
};
use bevy::prelude::*;

use super::actions_strategies::ReflectActionLogicStrategy;

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
    movement_enabled: bool,
    current_strategy: Option<Box<dyn Reflect>>,
}

impl GameplayLogicComponent {
    pub fn new() -> Self {
        GameplayLogicComponent {
            pending_actions: vec![],
            high_priority_actions: vec![],
            is_action_running: false,
            current_action: Actions::Unknown,
            can_be_interrupted: false,
            current_strategy: None,
            movement_enabled: false
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

        // Assign action strategy if exists
        match action {
            Actions::Climb => {
                self.current_strategy = Some(Box::new(
                    ClimbStrategy::new()
                ));
            }
            Actions::Walk => {
                self.current_strategy = Some(Box::new(
                    WalkStrategy::new()
                ));
            }
            _ => {
                self.current_strategy = None;
            },
        }

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
            return Actions::StandBy;
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
                self.movement_enabled = true;
                self.try_to_set_action(Actions::StandBy);
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
                    self.movement_enabled = false;
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
        self.current_strategy = None;
    }

    pub fn is_interruption_available(&self) -> bool {
        !self.high_priority_actions.is_empty()
    }

    pub fn is_current_action_valid(&self) -> bool {
        return self.current_action != Actions::Unknown;
    }

    pub fn execute_strategy(
        &mut self,
        movement: &PlayerMovementComponent,
        mut event_writer: EventWriter<Event>,
        type_registry: &AppTypeRegistry,
    ) {
        match self.current_strategy.take() {
            Some(mut strategy) => {
                let type_registry = type_registry.read();

                let reflect_strategy = type_registry
                    .get_type_data::<ReflectActionLogicStrategy>(strategy.type_id())
                    .unwrap();

                let strategy_trait: &mut dyn ActionLogicStrategy =
                    reflect_strategy.get_mut(&mut *strategy).unwrap();

                strategy_trait.do_action(movement, self, &mut event_writer);

                //Put strategy back
                self.current_strategy = Some(strategy);

            }
            _ => {}
        }
    }

    pub fn is_valid_strategy(
        &self,
        type_registry: &AppTypeRegistry,
    ) -> bool {
        match self.current_strategy.as_deref() {
            Some(strategy) => {
                let type_registry = type_registry.read();
                let reflect_strategy = type_registry
                    .get_type_data::<ReflectActionLogicStrategy>(strategy.type_id())
                    .unwrap();

                let strategy_trait: &dyn ActionLogicStrategy =
                    reflect_strategy.get(&*strategy).unwrap();

                return strategy_trait.is_valid_strategy(self);
            }
            _ => {
                return false;
            }
        }
    }

    pub fn is_strategy_preparing(
        &self,
        type_registry: &AppTypeRegistry,
    ) -> bool {
        match self.current_strategy.as_deref() {
            Some(strategy) => {
                let type_registry = type_registry.read();
                let reflect_strategy = type_registry
                    .get_type_data::<ReflectActionLogicStrategy>(strategy.type_id())
                    .unwrap();

                let strategy_trait: &dyn ActionLogicStrategy =
                    reflect_strategy.get(&*strategy).unwrap();

                return strategy_trait.is_in_preparing_phase();
            }
            _ => {
                return false;
            }
        }
    }

    pub fn is_movement_enabled(&self) -> bool {
        self.movement_enabled
    }
}

pub fn game_logic_system(
    mut commands: Commands,
    event_writer: EventWriter<Event>,
    time: Res<Time>,
    mut timer_query: Query<(Entity, &mut ActionDurationTimer)>,
    mut gameplay_query: Query<&mut GameplayLogicComponent, With<PlayerComponent>>,
    movement_query: Query<&PlayerMovementComponent, With<PlayerComponent>>,
    type_registry: Res<AppTypeRegistry>,
) {
    let is_action_timer_finished: bool;
    let mut timer_entity: Option<Entity> = None;

    // Timer logic
    match timer_query.get_single_mut() {
        Ok((entity, mut action_timer)) => {
            timer_entity = Some(entity);
            match gameplay_query.get_single_mut() {
                Ok(gameplay_component) => {
                    if !gameplay_component.is_strategy_preparing(&type_registry) {
                        action_timer.timer.tick(time.delta());
                    }
                }
                _ => {
                    action_timer.timer.tick(time.delta());
                }
            }

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

            // If action strategy running
            if gameplay_component.is_valid_strategy(&type_registry) {
                match movement_query.get_single() {
                    Ok(movement) => {
                        gameplay_component.execute_strategy(&movement, event_writer, &type_registry);
                    }
                    _ => {},
                }

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
