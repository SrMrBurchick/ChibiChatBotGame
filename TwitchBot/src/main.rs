use std::{sync::Arc, time::Duration};

use wss::{wss_server, wss_twitch_client};

pub mod parsers;
pub mod wss;
pub mod components;

#[tokio::main]
async fn main() {
    let (tx, mut rx) = crossbeam_channel::unbounded::<components::action::Action>();
    let config = parsers::config::parse_config(String::from("Game/config/ChibiChatBotConfig.json"));

    let twitch_client = tokio::spawn(wss_twitch_client::run_wss_client(config.clone(), tx));
    let game_server = tokio::spawn(wss_server::run_wss_server(config.clone(), rx));

    let _ = tokio::try_join!(twitch_client, game_server);
}
