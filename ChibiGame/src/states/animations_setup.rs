use bevy::prelude::*;
use crate::components::{
    animation::AnimationTimer,
    common::events::{
        Event, Events, SystemEvents
    }
};

pub fn setup_animation(mut commands: Commands, mut event_writer: EventWriter<Event>) {
    commands.insert_resource(AnimationTimer {
        timer: Timer::from_seconds(0.1, TimerMode::Repeating),
    });
    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::AnimationsConfigured(true)),
    });
}
