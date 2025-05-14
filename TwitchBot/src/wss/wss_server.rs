use std::sync::Arc;

use websocket::OwnedMessage;

use crate::components::{self, action};
use crate::parsers::request_parser::RequestReceiver;
use crate::parsers::config::Config;

pub async fn run_wss_server(config: Config, mut receiver : RequestReceiver) {
    let url = config.host.clone() + ":" + config.port.to_string().as_str();
    let arc_rx = Arc::new(receiver);
    println!("Run server = {:?}", url.clone());
    match websocket::sync::Server::bind(url) {
        Ok(server) => {
            println!("Wait for connection!");
            for request in server.filter_map(Result::ok) {
                match request.accept() {
                    Ok(mut client) => {
                        handle_client(client, arc_rx.clone()).await;
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

async fn handle_client(
    mut client: websocket::client::sync::Client<std::net::TcpStream>,
    action_rx: Arc<crossbeam_channel::Receiver<components::action::Action>>
)
{
    println!("New Client connected");
    let msg = websocket::Message::text("Tobi Pizda!");
    match client.send_message(&msg) {
        Ok(_) => {},
        Err(e) => {
            println!("Failed to send message: {:?}", e);
        },
    }
    let (mut client_rx, mut client_tx) = client.split().unwrap();
    let (tx, mut rx) = tokio::sync::mpsc::channel(10);
    let tx_clone = tx.clone();

    // let client_receiver = tokio::spawn(async move {
    //     loop {
    //         println!("Check state");
    //         if !active {
    //             println!("Client not responding, close!");
    //             client_rx.shutdown().unwrap();
    //             break;
    //         }
    //
    //         match client_rx.recv_message() {
    //             Ok(msg) => {
    //                 match msg {
    //                     OwnedMessage::Close(_) => {
    //                         println!("Client disconnected");
    //                         active = false;
    //                     }
    //                     _ => {},
    //                 }
    //             },
    //             Err(_) => {
    //                 active = false;
    //             },
    //         }
    //     }
    // });
    //
    // let action_sender = tokio::spawn(async move {
    //     loop {
    //         if !active {
    //             println!("Client not responding, close!");
    //             client_tx.shutdown().unwrap();
    //             break;
    //         }
    //
    //         match action_rx.recv() {
    //             Ok(action) => {
    //                 println!("Received action: {:?}", action.clone());
    //                 match client_tx.send_message(&websocket::Message::text(action.get_action_json().to_string())) {
    //                     Ok(_) => {
    //                         println!("Message successfully send");
    //                     },
    //                     Err(e) => {
    //                         println!("Failed to send message to client: {:?}", e);
    //                         active = false;
    //                     },
    //                 }
    //             },
    //             Err(e) => {
    //                 println!("Failed to received action, reason = {:?}", e);
    //                 active = false;
    //             },
    //         }
    //     }
    // });


    // let _ = tokio::try_join!(client_receiver, action_sender);
}
