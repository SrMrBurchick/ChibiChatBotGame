use crate::parsers::request_parser::RequestReceiver;

pub async fn run_wss_server(mut Receiver : RequestReceiver) {
    // This is running on a core thread.
    match websocket::sync::Server::bind("127.0.0.1:1234") {
        Ok(mut server) => {
            server.find_map(Result::ok).into_iter().for_each(|connection| {
                println!("New Connection received");
                // tokio::task::spawn(async {
                    match connection.accept() {
                        Ok(mut client) => {
                            let msg = websocket::Message::text("Tobi Pizda!");
                            match client.send_message(&msg) {
                                Ok(_) => {},
                                Err(_) => {},
                            }
                        },
                        Err(_) => {

                        },
                    }
                // });
            });
        },
        Err(_) => {
            println!("Pishow nahyu");
        },
    }
}
