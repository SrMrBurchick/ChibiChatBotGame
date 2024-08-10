use crate::components::{
    actions::{ActionType, is_action_can_interrupt},
    common::events::{Event, Events, GameEvents},
    gameplay::{
        player::PlayerComponent,
        Border, BorderType,
        actions_strategies::{
            ActionLogicStrategy, climb_strategy::ClimbStrategy,
            walk_strategy::WalkStrategy, ReflectActionLogicStrategy
        }
    }, movement::PlayerMovementComponent,
};
use bevy::prelude::*;
use std::collections::VecDeque;

#[derive(Component, Debug)]
pub struct ActionDurationTimer {
    pub timer: Timer,
}

#[derive(Component)]
pub struct GameplayLogicComponent {
    pending_actions: VecDeque<ActionType>,
    high_priority_actions: VecDeque<ActionType>,
    is_action_running: bool,
    current_action: ActionType,
    can_be_interrupted: bool,
    movement_enabled: bool,
    current_strategy: Option<Box<dyn Reflect>>,
    action_execution_time: f32,
    reset_current_action: bool,
}

impl GameplayLogicComponent {
    pub fn new() -> Self {
        GameplayLogicComponent {
            pending_actions: VecDeque::new(),
            high_priority_actions: VecDeque::new(),
            is_action_running: false,
            current_action: ActionType::Unknown,
            can_be_interrupted: false,
            current_strategy: None,
            movement_enabled: false,
            action_execution_time: 5.0,
            reset_current_action: false,
        }
    }

    fn reset_current_action_timer(&mut self) {
        info!("Reset current action {:?}", self.current_action);
        self.reset_current_action = true;
    }

    pub fn try_to_set_action(&mut self, action: ActionType) -> bool {
        if action == ActionType::Unknown || action == ActionType::SwapDirection {
            return false;
        }

        if self.current_action == action {
            self.reset_current_action_timer();
            return false;
        }

        // If current action cannot be interrupted than any action must be pass
        // to the low_priority actions. Even if the next action cannot be interrupted either
        if !self.can_be_interrupted && self.is_current_action_valid() {
            if !self.pending_actions.contains(&action) {
                self.pending_actions.push_back(action.clone());
                info!("Add low priority action: {:?}", action.clone());
                return true;
            }
            return false;
        }

        // If current action can be interrupted than any other action must be pass
        // to hight priority, even if the new action can be interrupted
        if self.can_be_interrupted && self.is_current_action_valid() {
            if !self.high_priority_actions.contains(&action) {
                self.high_priority_actions.push_back(action.clone());

                info!("Add high priority action: {:?}", action.clone());
                return true;
            }
            return false;
        }

        // Set only when current action is unknown
        info!("Set action {:?}", action.clone());
        self.current_action = action.clone();
        self.can_be_interrupted = !is_action_can_interrupt(action.clone());

        // Assign action strategy if exists
        match action {
            ActionType::Climb => {
                self.current_strategy = Some(Box::new(
                    ClimbStrategy::new()
                ));
            }
            ActionType::Walk => {
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

    pub fn get_current_action(&self) -> ActionType {
        self.current_action.clone()
    }

    pub fn get_next_action(&mut self) -> ActionType {
        if !self.high_priority_actions.is_empty() {
            return self.high_priority_actions.pop_front().unwrap();
        }

        if self.pending_actions.is_empty() {
            self.is_action_running = false;
            self.can_be_interrupted = true;
            return ActionType::StandBy;
        }

        return self.pending_actions.pop_front().unwrap();
    }

    pub fn set_action_duration(&mut self, action_duration: f32) {
        self.action_execution_time = action_duration;
    }
    pub fn get_action_duration(&self) -> f32 {
        return self.action_execution_time;
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
                self.try_to_set_action(ActionType::StandBy);
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
        match self.current_action {
            ActionType::Climb => {
                self.high_priority_actions.push_front(ActionType::Fall);
            }
            _ => {},
        }

        self.current_action = ActionType::Unknown;
        self.is_action_running = false;
        self.can_be_interrupted = true;
        self.current_strategy = None;
    }

    pub fn is_interruption_available(&self) -> bool {
        !self.high_priority_actions.is_empty()
    }

    pub fn is_current_action_valid(&self) -> bool {
        return self.current_action != ActionType::Unknown;
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

    pub fn prepare_next_action(&mut self) {
        self.current_action = self.get_next_action().clone();
        self.can_be_interrupted = !is_action_can_interrupt(self.current_action.clone());

        // Assign action strategy if exists
        match self.current_action {
            ActionType::Climb => {
                self.current_strategy = Some(Box::new(
                    ClimbStrategy::new()
                ));
            }
            ActionType::Walk => {
                self.current_strategy = Some(Box::new(
                    WalkStrategy::new()
                ));
            }
            _ => {
                self.current_strategy = None;
            },
        }
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
                Ok(mut gameplay_component) => {
                    if !gameplay_component.is_strategy_preparing(&type_registry) {
                        action_timer.timer.tick(time.delta());
                    }

                    if gameplay_component.reset_current_action {
                        action_timer.timer.reset();
                        gameplay_component.reset_current_action = false;
                    }
                }
                _ => {
                    action_timer.timer.tick(time.delta());
                }
            }

            is_action_timer_finished = action_timer.timer.just_finished();

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
                if gameplay_component.get_current_action() == ActionType::Fall {
                    match movement_query.get_single() {
                        Ok(movement) => {
                            if movement.landed {
                                on_action_completed(&mut gameplay_component);
                            }
                        }
                        _ => {},
                    }
                } else {
                    on_action_completed(&mut gameplay_component);
                }

                return;
            }

            // If new action can interrupt the current
            if gameplay_component.is_interruption_available() {
                interrupt_current_action(
                    &mut commands,
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
    info!("Start new action {:?}", gameplay_component.get_current_action());
    if gameplay_component.get_current_action() != ActionType::Fall {
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
) {
    gameplay_component.stop_action();
    gameplay_component.prepare_next_action();
}

fn interrupt_current_action(
    commands: &mut Commands,
    gameplay_component: &mut GameplayLogicComponent,
    timer_entity: Option<Entity>,
) {
    if timer_entity != None {
        commands.entity(timer_entity.unwrap()).despawn();
    }

    gameplay_component.stop_action();
    gameplay_component.prepare_next_action();
}
