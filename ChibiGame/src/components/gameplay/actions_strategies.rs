use bevy::prelude::*;
use crate::components::{
    movement::PlayerMovementComponent,
    gameplay::gameplay_logic::GameplayLogicComponent,
    common::events::Event
};

pub mod climb_strategy;
pub mod walk_strategy;

#[reflect_trait]
pub trait ActionLogicStrategy {
    fn do_action(
        &mut self,
        movement: &PlayerMovementComponent,
        gameplay: &GameplayLogicComponent,
        event_writer: &mut EventWriter<Event>
    );
    fn is_valid_strategy(
        &self,
        gameplay: &GameplayLogicComponent
    ) -> bool;
    fn is_in_preparing_phase(&self) -> bool;
}
