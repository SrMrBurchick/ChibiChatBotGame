use bevy::prelude::*;
use rand::prelude::*;
use crate::components::{
    actions::Actions,
    movement::PlayerMovementComponent,
    gameplay::player::PlayerComponent,
    common::events::{
        Event, Events, GameEvents
    }
};


#[derive(Resource, Debug)]
pub struct AITimer {
    pub timer: Timer,
}

#[derive(Debug, PartialEq, Eq)]
struct AIActionComponent {
    pub action: Actions,
    pub weight: u32
}

#[derive(Component)]
pub struct AIComponent {
    actions: Vec<AIActionComponent>,
    total_weight: u32
}

impl AIComponent {
    pub fn new() -> Self {
        AIComponent {
            actions: vec![],
            total_weight: 0
        }
    }

    pub fn add_new_action(&mut self, action: Actions, weight: u32) {
        let action_comp = AIActionComponent{
            action,
            weight
        };

        if !self.actions.contains(&action_comp) {
            self.total_weight += action_comp.weight;
            self.actions.push(action_comp);

            self.actions.sort_by_key(|item| item.weight);
        }
    }

    pub fn generate_action(&self) -> Actions {
        let mut rand = rand::thread_rng();
        let mut new_action_weight = rand.gen_range(0..self.total_weight);
        for action in self.actions.iter() {
            if new_action_weight < action.weight {
                return action.action;
            }

            new_action_weight -= action.weight;
        }

        return Actions::Unknown;
    }
}

pub fn ai_system(
    mut event_writer: EventWriter<Event>,
    mut components: Query<(&AIComponent, &mut PlayerMovementComponent), With<PlayerComponent>>,
    time: Res<Time>,
    mut ai_timer: ResMut<AITimer>,
    type_registry: Res<AppTypeRegistry>,
) {
    ai_timer.timer.tick(time.delta());
    if ai_timer.timer.finished() {
        for (ai, mut movement) in components.iter_mut() {
            let new_action = ai.generate_action();
            match new_action {
                Actions::SwapDirection => {
                    movement.swap_direction(&type_registry);
                }
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

