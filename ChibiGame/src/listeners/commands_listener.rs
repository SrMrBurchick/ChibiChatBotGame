use crate::{
    components::common::events::{
        Event, Events, GameEvents
    },
    listeners::web_sock_client::BotEventsReceiver
};
use bevy::prelude::*;

pub fn handle_commands(
    receiver: Res<BotEventsReceiver>,
    mut event_writer: EventWriter<Event>,
) {
    for action in receiver.try_iter() {
        info!("Received action: {:?}", action);

        event_writer.send(Event {
            event_type: Events::GameEvents(GameEvents::SetNewAction(action))
        });
    }
}
