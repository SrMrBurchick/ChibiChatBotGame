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
    preparing: bool
}

impl WalkStrategy {
    pub fn new() -> Self {
        WalkStrategy { preparing: false }
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
            self.preparing = true;
            if movement.can_climb {
                event_writer.send(
                    Event {
                        event_type: Events::GameEvents(GameEvents::ActionChanged(Actions::SwapDirection))
                    }
                );
            }
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

