
use bevy::prelude::*;
use crossbeam_channel::{bounded, Receiver, Sender};
use json;
use wasm_bindgen::prelude::Closure;
use wasm_bindgen::JsCast;
use web_sys::{MessageEvent, WebSocket};

use crate::components::{
    common::events::{
        Event, Events, SystemEvents
    },
    actions::{
        Actions, string_to_action
    },
    system::config::{
        Config, SystemConfigData
    }
};

#[derive(Resource, Deref)]
pub struct StreamReceiver(Receiver<Actions>);

pub fn setup_client(
    mut commands: Commands,
    mut event_writer: EventWriter<Event>,
    config_query: Query<&Config>
) {
    let (tx, rx) = bounded::<Actions>(10);

    match config_query.get_single() {
        Ok(config) => {
            run_client(tx, config.system_config.clone());
        },
        _ => {
            event_writer.send(Event {
                event_type: Events::SystemEvents(SystemEvents::ClientLoaded(false)),
            });

            return;
        }
    }

    commands.insert_resource(StreamReceiver(rx));

    event_writer.send(Event {
        event_type: Events::SystemEvents(SystemEvents::ClientLoaded(true)),
    });
}

pub fn run_client(sender: Sender<Actions>, config: SystemConfigData) {
    info!("Wait for TCP client");
    let url = String::from(format!("ws://{}:{}", config.chat_bot_url, config.chat_bot_port));

    match WebSocket::new(&url) {
        Ok(socket) => {
            info!("Connected");
            socket.set_binary_type(web_sys::BinaryType::Arraybuffer);
            let cloned = socket.clone();
            let onopen_callback = Closure::<dyn FnMut()>::new(move || {
                info!("socket opened");
                match cloned.send_with_str("ping") {
                    Ok(_) => info!("message successfully sent"),
                    Err(err) => info!("error sending message: {:?}", err),
                }
            });
            let onmessage_callback = Closure::<dyn FnMut(_)>::new(move |e: MessageEvent| {
                // Handle difference Text/Binary,...
                if let Ok(abuf) = e.data().dyn_into::<js_sys::ArrayBuffer>() {
                    info!("message event, received arraybuffer: {:?}", abuf);
                    let array = js_sys::Uint8Array::new(&abuf);
                    let len = array.byte_length() as usize;
                    info!("Arraybuffer received {}bytes: {:?}", len, array.to_vec());
                } else if let Ok(txt) = e.data().dyn_into::<js_sys::JsString>() {
                    info!("message event, received Text: {:?}", txt);
                    sender
                        .send(parse_response(txt.as_string().unwrap()))
                        .unwrap();
                } else {
                    info!("message event, received Unknown: {:?}", e.data());
                }
            });

            socket.set_onmessage(Some(onmessage_callback.as_ref().unchecked_ref()));
            socket.set_onopen(Some(onopen_callback.as_ref().unchecked_ref()));

            onopen_callback.forget();
            onmessage_callback.forget();
        }
        Err(e) => {
            // println!("Filed to connect")
            info!("Failed to connect: {:?}", e);
        }
    }
}

fn parse_response(string: String) -> Actions {
    let mut action: Actions = Actions::Unknown;
    match json::parse(string.as_str()) {
        Ok(content) => {
            if content.has_key("action") {
                action = string_to_action(content["action"].as_str().unwrap());
            }
        }
        Err(e) => {
            info!("Failed to parse response {:?}", e);
        }
    };

    return action;
}
