use crate::{
    components::actions::Actions,
    listeners::web_sock_client::StreamReceiver
};
use bevy::prelude::*;

pub fn handle_commands(receiver: Res<StreamReceiver>) {
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
