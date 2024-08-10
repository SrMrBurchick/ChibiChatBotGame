use bevy::prelude::*;
use crossbeam_channel::{Receiver, Sender};
use json;
use wasm_bindgen::prelude::Closure;
use wasm_bindgen::JsCast;
use web_sys::{MessageEvent, WebSocket};

use crate::components::actions::{string_to_action, ActionType};

#[derive(Resource, Deref)]
pub struct BotEventsReceiver(pub Receiver<ActionType>);

pub fn run_client(sender: Sender<ActionType>, url: String, port: u32) {
    info!("Wait for TCP client");
    let url = String::from(format!("ws://{}:{}", url, port));

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

fn parse_response(string: String) -> ActionType {
    let mut action: ActionType = ActionType::Unknown;
    info!("Received response from wss server: {}", string.clone());
    match json::parse(string.as_str()) {
        Ok(content) => {
            if content.has_key("action") {
                let action_str = content["action"].as_str();
                match action_str {
                    Some(value) => {
                        action = string_to_action(value);
                        match action.clone() {
                            ActionType::Unknown => {
                                if !value.is_empty() {
                                    let mut message = String::default();
                                    match content["message"].as_str() {
                                        Some(value) => {
                                            message = String::from(value);
                                        },
                                        None => {
                                        },
                                    }

                                    action = ActionType::UserAction(String::from(value), message);
                                }
                            }
                            _ => {},
                        }
                    },
                    None => {},
                }
            }
        }
        Err(e) => {
            info!("Failed to parse response {:?}", e);
        }
    };

    return action;
}
