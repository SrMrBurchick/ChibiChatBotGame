use crate::components::{
    common::{
        AnimationsMap,
        SpriteSize, TableSize,
        sprite_index::SpriteIndex,
    },
    actions::{
        ActionType, string_to_action, UserAction
    }
};

use bevy::prelude::*;
use json::JsonValue;
use std::error::Error;
use wasm_bindgen::{prelude::*, JsCast};
use wasm_bindgen_futures::JsFuture;

use web_sys::{Request, RequestInit, RequestMode, Response};

#[derive(Debug, Default, Component, Clone)]
pub struct SystemConfigData {
    pub bot_url: String,
    pub bot_port: u32
}

#[derive(Debug)]
pub struct MessageSettings {
    pub message_color: String,
    pub font_size: f32
}

#[derive(Debug, Component)]
pub struct Config {
    content: JsonValue,
    animations_map: AnimationsMap,
    user_actions: Vec<UserAction>,
    loaded: bool,
    pub system_config: SystemConfigData
}

pub fn get_value(content: &JsonValue, key: &str) -> Result<JsonValue, Box<dyn Error>> {
    let value: JsonValue;

    if 0 == key.len() {
        return Err(Box::from(format!("Unknown key {:?}", key)));
    }

    if JsonValue::is_null(content) {
        return Err(Box::from("Config content not initialized!"));
    }

    value = content[key].clone();

    if JsonValue::is_null(&value) {
        return Err(Box::from(format!("Key {} not configured", key)));
    }

    return Ok(value);
}

pub fn init_bot_settings(obj: &JsonValue) -> Result<SystemConfigData, Box<dyn Error>> {
    if JsonValue::is_null(obj) {
        return Err(Box::from("Unknown param"));
    }

    if false == obj.has_key("url") || false == obj.has_key("port") {
        return Err(Box::from("Broken sprite size"));
    }

    Ok(SystemConfigData {
        bot_url: obj["url"].to_string(),
        bot_port: obj["port"].as_u32().unwrap()
    })
}

pub fn init_sprite_size(obj: &JsonValue) -> Result<SpriteSize, Box<dyn Error>> {
    if JsonValue::is_null(obj) {
        return Err(Box::from("Unknown param"));
    }

    if false == obj.has_key("height") || false == obj.has_key("width") {
        return Err(Box::from("Broken sprite size"));
    }

    Ok(SpriteSize {
        height: obj["height"].as_u32().unwrap(),
        width: obj["width"].as_u32().unwrap(),
    })
}

pub fn init_table_size(obj: &JsonValue) -> Result<TableSize, Box<dyn Error>> {
    if JsonValue::is_null(obj) {
        return Err(Box::from("Unknown param"));
    }

    if false == obj.has_key("columns") || false == obj.has_key("rows") {
        return Err(Box::from("Broken table size"));
    }

    Ok(TableSize {
        columns: obj["columns"].as_u32().unwrap(),
        rows: obj["rows"].as_u32().unwrap(),
    })
}

pub fn init_sprite_index(obj: &JsonValue) -> Result<SpriteIndex, Box<dyn Error>> {
    if JsonValue::is_null(obj) {
        return Err(Box::from("Unknown sprite index"));
    }

    if false == obj.has_key("column") || false == obj.has_key("row") || false == obj.has_key("inverted") {
        return Err(Box::from("Broken sprite index"));
    }

    Ok(SpriteIndex {
        column: obj["column"].as_u32().unwrap(),
        row: obj["row"].as_u32().unwrap(),
        inverted: obj["inverted"].as_bool().unwrap()
    })
}

pub async fn wasm_load_config(sender: async_std::channel::Sender<String>) -> Result<(), JsValue> {
    info!("Load config");
    let url = "config/ChibiChatBotConfig.json";

    // let (mut tx, rx) = wasm_bindgen_futures::channel::unbounded();
    let mut opts = RequestInit::new();
    opts.method("GET");
    opts.mode(RequestMode::Cors);

    let request = Request::new_with_str_and_init(&url, &opts)?;
    let window = web_sys::window().ok_or_else(|| "global `window` is undefined")?;
    let resp_value = JsFuture::from(window.fetch_with_request(&request)).await?;
    let resp: Response = resp_value.dyn_into().unwrap();
    sender
        .send(JsFuture::from(resp.text()?).await?.as_string().unwrap())
        .await
        .unwrap();

    Ok(())
}

impl Config {
    pub fn new() -> Self {
        Config {
            content: JsonValue::Null,
            animations_map: AnimationsMap::default(),
            loaded: false,
            system_config: SystemConfigData::default(),
            user_actions: vec![]
        }
    }

    pub fn load(&mut self, source: String) -> Result<(), Box<dyn Error>> {
        info!("Try to load config from: {}", source);
        match json::parse(&source) {
            Ok(content) => {
                self.loaded = true;
                self.content = content;
                return Ok(());
            }
            Err(e) => {
                return Err(Box::from(e));
            }
        }
    }

    fn get_value(&self, key: &str) -> Result<JsonValue, Box<dyn Error>> {
        let value: JsonValue;

        if self.loaded == false {
            return Err(Box::from("Config not loaded!"));
        }

        if 0 == key.len() {
            return Err(Box::from(format!("Unknown key {:?}", key)));
        }

        if JsonValue::is_null(&self.content) {
            return Err(Box::from("Config content not initialized!"));
        }

        value = self.content[key].clone();

        if JsonValue::is_null(&value) {
            return Err(Box::from(format!("Key {} not configured", key)));
        }

        return Ok(value);
    }

    pub fn get_table_size(&self) -> Result<TableSize, Box<dyn Error>> {
        match self.get_value("table-size") {
            Ok(size) => {
                return init_table_size(&size);
            }
            Err(e) => {
                return Err(Box::from(format!("Failed to get sprite-size {:?}", e)));
            }
        }
    }

    pub fn get_sprite_size(&self) -> Result<SpriteSize, Box<dyn Error>> {
        match self.get_value("sprite-size") {
            Ok(size) => {
                return init_sprite_size(&size);
            }
            Err(e) => {
                return Err(Box::from(format!("Failed to get sprite-size {:?}", e)));
            }
        }
    }

    pub fn get_actions(&self) -> Result<JsonValue, Box<dyn Error>> {
        self.get_value("actions")
    }

    pub fn init_actions_map(&mut self) -> Result<(), Box<dyn Error>> {
        let actions_array: JsonValue;

        match self.get_value("actions") {
            Ok(array) => {
                actions_array = array;
            }
            Err(e) => {
                return Err(Box::from(format!("Failed to get animations {:?}", e)));
            }
        }

        if false == actions_array.is_array() {
            return Err(Box::from("Animations must be an array! Corrupted config"));
        }

        let hash_map = &mut self.animations_map.0;

        for item in actions_array.members() {
            match get_value(&item, "name") {
                Ok(name) => {
                    let key = string_to_action(&name.to_string());
                    let mut is_user_action = false;

                    if key == ActionType::Unknown {
                        is_user_action = true;
                    }

                    let mut animations_list: Vec<SpriteIndex> = vec![];
                    match get_value(&item, "animations") {
                        Ok(animations) => {
                            if !animations.is_array() {
                                return Err(Box::from("Broken animation item"));
                            }

                            for animation in animations.members() {
                                match init_sprite_index(animation) {
                                    Ok(sprite_index) => {
                                        animations_list.push(sprite_index);
                                    }
                                    Err(_) => {
                                        // continue;
                                    }
                                }
                            }
                            info!("Detected animation in {:?}: {:?}", key, animations_list);

                            if is_user_action {
                                self.user_actions.push(
                                    UserAction {
                                        action: name.to_string(),
                                        animations: animations_list 
                                    }
                                );
                            } else {
                                hash_map.insert(key, animations_list);
                                info!("Animations has been updated: {:?}", hash_map);
                            }

                        },
                        Err(_) => {},
                    }

                },
                Err(_) => {},
            }
        }

        return Ok(());
    }

    pub fn get_action_animations(&mut self, action: ActionType) -> Vec<SpriteIndex> {
        let mut map: Vec<SpriteIndex> = vec![];

        if self.animations_map.0.is_empty() {
            match self.init_actions_map() {
                Err(_) => { return map; },
                _ => {}
            }
        }

        if self.animations_map.0.contains_key(&action) {
            map = self.animations_map.0.get(&action).unwrap().clone();
        }

        return map;
    }

    pub fn get_img(&self) -> String {
        self.get_value("source-file").unwrap().to_string()
    }

    pub fn get_sprite_scale(&self) -> f32 {
        self.get_value("sprite-scale").unwrap().as_f32().unwrap()
    }

    pub fn get_configured_actions(&self) -> Vec<ActionType> {
        let mut actions: Vec<ActionType> = vec![];

        if self.animations_map.0.is_empty() {
            return actions;
        }

        for (k, _) in self.animations_map.0.iter() {
            actions.push(k.clone());
        }

        return actions;
    }

    pub fn get_configured_animations_map(&self) -> AnimationsMap {
        info!("Configured animations: {:?}", self.animations_map);
        self.animations_map.clone()
    }

    pub fn is_loaded(&self) -> bool {
        self.loaded
    }

    pub fn init_bot_settings(&mut self) -> Result<(), Box<dyn Error>>{
        match self.get_value("twitch-settings") {
            Ok(twitch_settings) => {
                match get_value(&twitch_settings, "bot-settings") {
                    Ok(bot_settings) => {
                        match init_bot_settings(&bot_settings) {
                            Ok(system_config) => {
                                self.system_config = system_config;
                            },
                            Err(e) => {
                                return Err(Box::from(format!("Failed to parse chat-bot {:?}", e)));
                            },
                        }

                    },
                    Err(_) => {},
                }
            }
            Err(e) => {
                return Err(Box::from(format!("Failed to get chat-bot {:?}", e)));
            }
        }

        return Ok(());
    }

    pub fn get_user_actions(&self) -> Vec<UserAction> {
        return self.user_actions.clone();
    }

    pub fn get_screen_resolution(&self) -> (f32, f32) {
        let mut width: f32 = 0.0;
        let mut height: f32 = 0.0;

        match self.get_value("screen-resolution") {
            Ok(resolution) => {
                if resolution.has_key("height") || resolution.has_key("width") {
                    width = resolution["width"].as_f32().unwrap();
                    height = resolution["height"].as_f32().unwrap();
                }
            },
            Err(_) => {},
        }

        return (width, height);
    }

    pub fn get_predefined_actions(&self) -> Vec<(String, u8)> {
        let mut predefined_actions: Vec<(String, u8)> = vec![];

        match self.get_value("predefined-actions") {
            Ok(actions) => {
                if !actions.is_array() {
                    return predefined_actions;
                }

                for item in actions.members() {
                    let action_name = item["action"].as_str().unwrap();
                    let chance = item["chance"].as_u8().unwrap();

                    predefined_actions.push((String::from(action_name), chance));
                }
            },
            Err(_) => {},
        }

        return predefined_actions;
    }

    pub fn get_message_settings(&self) -> MessageSettings {
        let mut settings = MessageSettings{
            font_size: 24.0,
            message_color: String::from("#f32344")
        };

        match self.get_value("message-settings") {
            Ok(msg_settings) => {
                if msg_settings.has_key("text-color") {
                    settings.message_color = String::from(msg_settings["text-color"].as_str().unwrap());
                }

                if msg_settings.has_key("font") {
                    settings.font_size = msg_settings["font"]["size"].as_f32().unwrap();
                }
            },
            Err(_) => {},
        }

        return settings;
    }

    pub fn get_action_execution_time(&self) -> f32 {
        let mut action_execution_time: f32 = 15.0;

        match self.get_value("action-execution-time") {
            Ok(config_action) => {
                action_execution_time = config_action.as_f32().unwrap();
            },
            Err(_) => {},
        }

        return action_execution_time;
    }

    pub fn get_movement_speed(&self) -> f32 {
        let mut movement_speed: f32 = 3500.0;

        match self.get_value("movement-speed") {
            Ok(config_movement) => {
                movement_speed = config_movement.as_f32().unwrap();
            },
            Err(_) => {},
        }

        return movement_speed;
    }

    pub fn get_generate_new_action_timeout(&self) -> f32 {
        let mut new_action_timeout: f32 = 20.0;

        match self.get_value("next-action-timeout") {
            Ok(config_timeout) => {
                new_action_timeout = config_timeout.as_f32().unwrap();
            },
            Err(_) => {},
        }

        return new_action_timeout;
    }
}
