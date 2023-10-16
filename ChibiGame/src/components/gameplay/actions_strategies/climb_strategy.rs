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
pub struct ClimbStrategy{
    preparing: bool
}

impl ClimbStrategy {
    pub fn new() -> Self {
        ClimbStrategy { preparing: false }
    }
}

impl ActionLogicStrategy for ClimbStrategy {
    fn do_action(
        &mut self,
        movement: &PlayerMovementComponent,
        gameplay: &GameplayLogicComponent,
        event_writer: &mut EventWriter<Event>
    ) {
        if gameplay.get_current_action() != Actions::Climb {
            error!("Selected wrong strategy for action: {:?}", gameplay.get_current_action());
            return;
        }

        if movement.can_climb {
            if self.preparing {
                self.preparing = false;
                event_writer.send(
                    Event {
                        event_type: Events::GameEvents(GameEvents::ActionChanged(Actions::Climb))
                    }
                );
            }
        } else if !self.preparing {
            self.preparing = true;
            if movement.landed {
                event_writer.send(
                    Event {
                        event_type: Events::GameEvents(GameEvents::ActionChanged(Actions::Walk))
                    }
                );
            }
        }
    }
    fn is_valid_strategy(
        &self,
        gameplay: &GameplayLogicComponent
    ) -> bool {
        gameplay.get_current_action() == Actions::Climb
    }

    fn is_in_preparing_phase(&self) -> bool {
        self.preparing
    }

}

