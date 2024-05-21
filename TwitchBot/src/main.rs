use tokio::sync::mpsc;

pub mod parsers;
pub mod wss;

#[tokio::main]
async fn main() {
    let (tx, rx) = mpsc::channel::<parsers::request_parser::EventType>(32);

    let blocking_task = tokio::task::spawn_blocking(|| {

    });

    // tokio::spawn(wss_server::run_wss_server(rx));

    // We can wait for the blocking task like this:
    // If the blocking task panics, the unwrap below will propagate the
    // panic.
    blocking_task.await.unwrap();
}
