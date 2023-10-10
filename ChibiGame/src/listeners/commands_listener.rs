use crate::{
    components::actions::Actions,
    listeners::web_sock_client::ChatBotEventsReceiver
};
use bevy::prelude::*;

pub fn handle_commands(receiver: Res<ChatBotEventsReceiver>) {
    for action in receiver.try_iter() {
        match action {
            Actions::Unknown => {

            }
            _ => {
                info!("Received action: {:?}", action);
            }
        }
    }
}
