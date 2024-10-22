use bevy::prelude::*;
use bevy_rapier2d::na::ComplexField;
use rand::prelude::*;
use crate::components::{
    actions::ActionType,
    gameplay::player::PlayerComponent,
    common::events::{
        Event, Events, GameEvents
    }
};


#[derive(Resource, Debug)]
pub struct AITimer {
    pub timer: Timer,
}

#[derive(Debug, PartialEq)]
struct AIActionComponent {
    pub action: ActionType,
    pub weight: f32
}

#[derive(Component)]
pub struct AIComponent {
    actions: Vec<AIActionComponent>,
    total_weight: f32
}

impl AIComponent {
    pub fn new() -> Self {
        AIComponent {
            actions: vec![],
            total_weight: 0.0
        }
    }

    pub fn get_total_weight(&self) -> f32 {
        self.total_weight
    }

    pub fn add_new_action(&mut self, action: ActionType, weight: u8) {
        info!("Add new Action: {:?} with chance {:?} %", action, weight);
        let action_comp = AIActionComponent{
            action,
            weight: weight as f32
        };

        if !self.actions.contains(&action_comp) {
            self.total_weight += action_comp.weight;
            self.actions.push(action_comp);
        }

        self.actions.sort_by(|a, b| a.weight.total_cmp(&b.weight));
        info!("Predefined actions: {:?}", self.actions);
    }

    pub fn normalize_chances(&mut self) {
        if (self.total_weight - 100.0).abs() > std::f32::EPSILON {
            for action in self.actions.iter_mut() {
                action.weight = (action.weight / self.total_weight) * 100.0;
            }
        }
    }

    pub fn generate_action(&self) -> ActionType {
        let mut rand = rand::thread_rng();
        let new_action_weight = rand.gen_range(0.0..100.0);
        let mut cumulative_weight = 0.0;
        for action in self.actions.iter() {
            cumulative_weight += action.weight;
            info!("Try to chose action {:?}, CurrentWeight = {}, Cumulative = {}", action, new_action_weight, cumulative_weight);
            if new_action_weight < cumulative_weight {
                return action.action.clone();
            }
        }

        return ActionType::Unknown;
    }
}

pub fn ai_system(
    mut event_writer: EventWriter<Event>,
    mut components: Query<&AIComponent, With<PlayerComponent>>,
    time: Res<Time>,
    mut ai_timer: ResMut<AITimer>,
) {
    ai_timer.timer.tick(time.delta());
    if ai_timer.timer.finished() {
        for ai in components.iter_mut() {
            let new_action = ai.generate_action();
            match new_action {
                ActionType::Unknown => {
                    // Do nothing
                }
                _ => {
                    event_writer.send(
                        Event {
                            event_type: Events::GameEvents(GameEvents::SetNewAction(new_action))
                        }
                    );
                },
            }
        }
    }
}

