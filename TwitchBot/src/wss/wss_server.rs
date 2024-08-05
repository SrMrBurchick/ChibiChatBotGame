use std::sync::Arc;

use websocket::OwnedMessage;

use crate::components::action;
use crate::parsers::request_parser::RequestReceiver;
use crate::parsers::config::Config;

pub async fn run_wss_server(config: Config, mut receiver : RequestReceiver) {
    let url = config.host.clone() + ":" + config.port.to_string().as_str();
    let arc_rx = Arc::new(receiver);
    println!("Run server = {:?}", url.clone());
    match websocket::sync::Server::bind(url) {
        Ok(server) => {
            for request in server.filter_map(Result::ok) {
                match request.accept() {
                    Ok(mut client) => {
                        println!("New Client connected");
                        let msg = websocket::Message::text("Tobi Pizda!");
                        match client.send_message(&msg) {
                            Ok(_) => {},
                            Err(_) => {
                                continue;
                            },
                        }
                        let (mut client_rx, mut client_tx) = client.split().unwrap();
                        let (tx1, rx1) = tokio::sync::oneshot::channel();
                        let rx_clone = arc_rx.clone();

                        let sender_handler = tokio::spawn(async move {
                            loop {
                                println!("Try to receive action");
                                match rx_clone.recv() {
                                    Ok(action) => {
                                        println!("Received action: {:?}", action.clone());
                                        match client_tx.send_message(&websocket::Message::text(action.get_action_json().to_string())) {
                                            Ok(_) => {},
                                            Err(_) => {
                                                break;
                                            },
                                        }

                                    },
                                    Err(_) => {},
                                }
                            }
                        });

                        tokio::spawn(async move {
                            loop {
                                println!("Check state");
                                match client_rx.recv_message() {
                                    Ok(msg) => {
                                        match msg {
                                            OwnedMessage::Close(_) => {
                                                tx1.send("Client disconnected").unwrap();
                                                break;
                                            }
                                            _ => {},
                                        }
                                    },
                                    Err(_) => {
                                        break;
                                    },
                                }
                            }
                        });

                        tokio::select! {
                            _ = rx1 => {
                                sender_handler.abort();
                            }
                        };
                    },
                    Err(_) => {},
                }

            }
        },
        Err(e) => {
            println!("Pishow nahyu, {:?}", e);
        },
    }
}
