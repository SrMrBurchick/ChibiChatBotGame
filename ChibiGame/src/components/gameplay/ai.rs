use bevy::prelude::*;
use rand::prelude::*;
use crate::components::{
    actions::Actions,
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
    pub action: Actions,
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

    pub fn add_new_action(&mut self, action: Actions, weight: u8) {
        info!("Add new Action: {:?} with chance {:?} %", action, weight);
        let action_comp = AIActionComponent{
            action,
            weight: weight as f32 / 100.0
        };

        if !self.actions.contains(&action_comp) {
            self.total_weight += action_comp.weight;
            info!("Total weight {:?} %", self.total_weight);
            self.actions.push(action_comp);
        }
    }

    pub fn generate_action(&self) -> Actions {
        let mut rand = rand::thread_rng();
        let new_action_weight = rand.gen_range(0.0.. if self.actions.len() == 1 { 100.0 } else {self.total_weight});
        let mut cumulative_weight = 0.0;
        for action in self.actions.iter() {
            cumulative_weight += action.weight;
            if new_action_weight < cumulative_weight {
                return action.action.clone();
            }
        }

        return Actions::Unknown;
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
                Actions::Unknown => {
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

