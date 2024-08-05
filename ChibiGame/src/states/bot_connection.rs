use crate::components::{
    actions::Actions,
    common::events::{Event, Events, SystemEvents},
    system::config::Config,
};
use bevy::prelude::*;

use crate::listeners::web_sock_client::{run_client, BotEventsReceiver};

pub fn setup_bot_client(
    mut commands: Commands,
    mut event_writer: EventWriter<Event>,
    query: Query<&Config>,
) {
    for config in query.iter() {
        let (sender, receiver) = crossbeam_channel::bounded::<Actions>(10);
        run_client(
            sender,
            config.system_config.bot_url.clone(),
            config.system_config.bot_port,
        );

        // At that channel will be sender chat bot commands
        commands.insert_resource(BotEventsReceiver(receiver));

        event_writer.send(Event {
            // By default set to true
            // TODO: Add result if client was connected to chat bot
            event_type: Events::SystemEvents(SystemEvents::ClientLoaded(true)),
        });
    }
}
