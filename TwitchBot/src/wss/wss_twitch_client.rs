use std::{io::Cursor, net::TcpStream};

use json;
use websocket::{header::{Authorization, Bearer, Headers}, native_tls::TlsStream, server::NoTlsAcceptor, sync::Client, ws::{Message, Receiver}, OwnedMessage};

use crate::{
    components::twitch::Twitch,
    parsers::{
        request_parser::RequestReceiver, config::{
            Config, get_value
        }
    }
};

async fn twitch_loop(mut client: Client<TlsStream<TcpStream>>, config: Config) {
    let mut twitch = Twitch::new();
    let mut is_initialized: bool = false;
    config.init_twitch(&mut twitch);

    let client_task = tokio::spawn(async move {
        loop {
            match client.recv_message() {
                Ok(response) => {
                    match response {
                        OwnedMessage::Ping(data) => {
                            match client.send_message(&OwnedMessage::Pong(data)) {
                                Ok(_) => {
                                    println!("Send pong response");
                                },
                                Err(_) => {
                                    println!("Failed to send pong responce");
                                },
                            }
                        },
                        OwnedMessage::Text(data) => {
                            println!("Received text: {:?}", data);
                            match json::parse(data.as_str()) {
                                Ok(json_data) => {
                                    match get_value(&json_data, "metadata") {
                                        Ok(metadata) => {
                                            match get_value(&metadata, "message_type") {
                                                Ok(message_type) => {
                                                    match get_value(&json_data, "payload") {
                                                        Ok(payload) => {
                                                            match message_type.to_string().as_ref() {
                                                                "session_welcome" => {
                                                                    twitch.parse_welcome_message(&payload);
                                                                },
                                                                "notification" => {
                                                                }
                                                                _ => {},
                                                            }
                                                        },
                                                        Err(_) => {},
                                                    }
                                                },
                                                Err(_) => {

                                                },
                                            }
                                            if twitch.session_id.is_empty() {
                                            }
                                        },
                                        Err(_) => {
                                            println!("Failed to parse data");
                                        },
                                    }

                                },
                                Err(_) => {
                                    println!("Failed to parese text");
                                },
                            }
                        }
                        OwnedMessage::Close(data) => {
                            println!("Connection closed: {:?}", data);
                            break;
                        }
                        _ => {

                        }
                    }
                },
                Err(_) => {},
            };

            if !is_initialized && !twitch.session_id.is_empty() {
                twitch.subscribe_to_events().await;
                is_initialized = true;
            }
        }
    });

    let _ = client_task.await;
}

pub async fn run_wss_client(config: Config, mut Receiver : RequestReceiver) {

    // This is running on a core thread.
    match websocket::ClientBuilder::new("wss://eventsub.wss.twitch.tv/ws?keepalive_timeout_seconds=30") {
        Ok(client) => {
            match client.add_protocol("rust-websocket")
                   .connect_secure(None) {
                Ok(connection) => {
                    twitch_loop(connection, config.clone()).await;
                },
                Err(_) => {},
            };
        },
        Err(_) => {},
    }
}
