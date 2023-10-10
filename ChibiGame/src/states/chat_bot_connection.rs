use bevy::prelude::*;
use crossbeam_channel::{bounded, Receiver, Sender};
use crate::components::{
    common::events::{
        Event, SystemEvents
    },
    system::config::Config
};

pub fn setup_chat_bot_client(
    mut commands: Commands,
    mut event_writer: EventWriter<Event>,
    query: Query<&Config>
) {
    for config in query.iter() {
        // TODO: Add to config websock port and url reading
        // Run web socket client with configured url and port
    }
}
