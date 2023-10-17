use bevy::prelude::*;
use crate::components::{
    gameplay::{
        gameplay_logic::GameplayLogicComponent,
        actions_strategies::{
            ActionLogicStrategy, ReflectActionLogicStrategy
        }
    },
    movement::PlayerMovementComponent,
    common::events::{
        Event, Events, GameEvents
    },
    actions::Actions
};

#[derive(Component, Debug, Reflect)]
#[reflect(ActionLogicStrategy)]
pub struct WalkStrategy{
    preparing: bool,
    swap_direction_delay: u8
}

impl WalkStrategy {
    pub fn new() -> Self {
        WalkStrategy {
            preparing: false,
            swap_direction_delay: 0
        }
    }
}

impl ActionLogicStrategy for WalkStrategy {
    fn do_action(
        &mut self,
        movement: &PlayerMovementComponent,
        gameplay: &GameplayLogicComponent,
        event_writer: &mut EventWriter<Event>
    ) {
        if gameplay.get_current_action() != Actions::Walk {
            error!("Selected wrong strategy for action: {:?}", gameplay.get_current_action());
            return;
        }

        if movement.landed {
            self.preparing = false;
            if movement.can_climb {
                if gameplay.get_current_action() == Actions::Walk && self.swap_direction_delay == 0 {
                    event_writer.send(
                        Event {
                            event_type: Events::GameEvents(GameEvents::ActionChanged(Actions::SwapDirection))
                        }
                    );
                    self.swap_direction_delay = 3;
                }
            } else {
                if self.swap_direction_delay != 0 {
                    self.swap_direction_delay -= 1;
                }
            }
        } else {
            self.preparing = true;
        }
    }
    fn is_valid_strategy(
        &self,
        gameplay: &GameplayLogicComponent
    ) -> bool {
        gameplay.get_current_action() == Actions::Walk
    }

    fn is_in_preparing_phase(&self) -> bool {
        self.preparing
    }

}

