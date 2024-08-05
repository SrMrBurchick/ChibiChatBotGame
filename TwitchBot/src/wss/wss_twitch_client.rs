use std::{borrow::BorrowMut, net::TcpStream, ops::DerefMut, sync::Arc};

use json;
use websocket::{native_tls::TlsStream, sync::Client, OwnedMessage};

use crate::{components::{self, twitch::Twitch}, parsers::{config::Config, request_parser::RequestSender}};

async fn twitch_loop(mut client: Client<TlsStream<TcpStream>>, config: &Config, sender: RequestSender) {
    let mut twitch = Twitch::new();
    config.init_twitch(&mut twitch);
    twitch.setup_sender(sender);


    loop {
        match client.recv_message() {
            Ok(response) => match response {
                OwnedMessage::Ping(data) => match client.send_message(&OwnedMessage::Pong(data)) {
                    Ok(_) => {
                        println!("Send pong response");
                    }
                    Err(_) => {
                        println!("Failed to send pong response");
                    }
                },
                OwnedMessage::Text(data) => {
                    println!("Received text: {:?}", data);
                    match json::parse(data.as_str()) {
                        Ok(json_data) => {
                            let _ = twitch.parse_message(&json_data).await;
                            if twitch.isConnected && !twitch.isSubscribed {
                                twitch.subscribe_to_events().await;
                            } else {
                                twitch.parse_message(&json_data).await;
                                twitch.do_action();
                            }
                        }
                        Err(_) => {
                            println!("Failed to parse text");
                        }
                    }
                }
                OwnedMessage::Close(data) => {
                    println!("Connection closed: {:?}", data);
                    break;
                }
                _ => {}
            },
            Err(_) => {
                break;
            }
        }
    };

}

pub async fn run_wss_client(config: Config, sender: RequestSender) {
    // This is running on a core thread.
    //
    match websocket::ClientBuilder::new(
        "wss://eventsub.wss.twitch.tv/ws?keepalive_timeout_seconds=30",
    ) {
        Ok(client) => {
            match client.add_protocol("rust-websocket").connect_secure(None) {
                Ok(connection) => {
                    twitch_loop(connection, &config, sender).await;
                }
                Err(_) => {}
            };
        }
        Err(_) => {}
    }
}
