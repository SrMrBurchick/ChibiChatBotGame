use async_std;
use bevy::prelude::*;
use crossbeam_channel::Receiver;

use crate::components::{
    common::events::{Event, Events, SystemEvents},
    system::config::{wasm_load_config, Config},
};

#[derive(Resource, Deref)]
pub struct ConfigReceiver(Receiver<String>);

pub fn setup_config_loader(mut commands: Commands) {
    let (async_tx, async_rx) = async_std::channel::bounded(10);
    // Run async config reading
    async_std::task::block_on(async move {
        match wasm_load_config(async_tx).await {
            _ => {
                // Do nothing
            }
        }
    });

    let (sender, receiver) = crossbeam_channel::bounded::<String>(10);

    // Wait for config readded
    async_std::task::block_on(async move {
        let sender_clone = sender.clone();
        let result = async_rx.recv().await.unwrap();

        sender_clone.send(result).unwrap();
    });

    // Add config receiver resource
    commands.insert_resource(ConfigReceiver(receiver));
}

pub fn handle_config(
    mut commands: Commands,
    receiver: Res<ConfigReceiver>,
    mut event_writer: EventWriter<Event>,
) {
    for result in receiver.try_iter() {
        if result.is_empty() {
            // Wait for valid config
            continue;
        }

        let mut config = Config::new();
        match config.load(result) {
            Err(error) => {
                error!(error);
                event_writer.send(Event {
                    event_type: Events::SystemEvents(SystemEvents::ConfigLoaded(false)),
                });
                continue;
            },
            _ => {
                // Do nothing
            }
        }

        match config.init_actions_map() {
            Err(error) => {
                error!(error);
                event_writer.send(Event {
                    event_type: Events::SystemEvents(SystemEvents::ConfigLoaded(false)),
                });
                continue;
            },
            _ => {
                // Do nothing
            }
        }

        match config.init_bot_settings() {
            Err(error) => {
                error!(error);
                event_writer.send(Event {
                    event_type: Events::SystemEvents(SystemEvents::ConfigLoaded(false)),
                });
                continue;
            },
            _ => {
                // Do nothing
            }
        }

        // If everything ok
        commands.spawn(config);

        event_writer.send(Event {
            event_type: Events::SystemEvents(SystemEvents::ConfigLoaded(true)),
        });
    }
}
