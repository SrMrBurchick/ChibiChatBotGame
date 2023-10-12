use bevy::prelude::*;
use bevy_rapier2d::prelude::*;
use components::actions::Actions;
use components::common::events::{Events, GameEvents};

pub mod components;
pub mod listeners;
pub mod states;

use crate::components::common::{
    GameStates, events::Event,
};
use crate::components::{
    animation, movement
};
use crate::states::*;
use crate::listeners::*;

fn main() {
    App::new()
        .add_plugins(DefaultPlugins.set(WindowPlugin {
            primary_window: Some(Window {
                // Setting `transparent` allows the `ClearColor`'s alpha value to take effect
                transparent: true,
                // Disabling window decorations to make it feel more like a widget than a window
                decorations: false,
                #[cfg(target_os = "macos")]
                composite_alpha_mode: CompositeAlphaMode::PostMultiplied,
                ..default()
            }),
            ..default()
        }))
        .add_plugins((
            RapierPhysicsPlugin::<NoUserData>::pixels_per_meter(100.0),
            RapierDebugRenderPlugin::default()
        ))
        .add_state::<GameStates>()
        .add_event::<Event>()

        // ClearColor must have 0 alpha, otherwise some color will bleed through
        .insert_resource(ClearColor(Color::NONE))
        .add_systems(Startup, setup)

        // Always listen to system events
        .add_systems(Update, event_listener::system_event_listener)

        // Listen Game Events only in RunGame state
        .add_systems(Update, event_listener::handle_game_events.run_if(in_state(GameStates::RunGame)))

        // Setup config state
        .add_systems(OnEnter(GameStates::ConfigLoading), config_state::setup_config_loader)
        .add_systems(Update, config_state::handle_config.run_if(in_state(GameStates::ConfigLoading)))

        .add_systems(OnEnter(GameStates::ChatBotConnectionSetup), chat_bot_connection::setup_chat_bot_client)
        .add_systems(OnEnter(GameStates::GameSetup), game_setup::setup_game)
        .add_systems(OnEnter(GameStates::AnimationsSetup), animations_setup::setup_animation)
        .add_systems(OnEnter(GameStates::PlayerSetup), player_setup::setup_player)
        .add_systems(Update, animation::animation_system.run_if(in_state(GameStates::RunGame)))
        .add_systems(Update, movement::move_player.run_if(in_state(GameStates::RunGame)))

        .run();
}

fn setup(mut commands: Commands) {
    commands.spawn(Camera2dBundle::default());
}
