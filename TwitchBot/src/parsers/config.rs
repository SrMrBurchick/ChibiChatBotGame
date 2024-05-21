use std::error::Error;
use std::fs;
use json::{self, JsonValue};


#[derive(Debug)]
pub struct Config {
    pub host: String,
    pub port: u32,
    pub actions: Vec<String>
}

fn get_value(content: &JsonValue, key: &str) -> Result<JsonValue, Box<dyn Error>> {
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

impl Config {
    pub fn new() -> Self {
        Config {
            host: String::new(),
            port: 0,
            actions: Vec::new()
        }
    }

    fn init_base_info(&mut self, data: &JsonValue) {
        if data.is_object() {
            match get_value(data, "port") {
                Ok(value) => {
                    self.port = value.as_u32().unwrap();
                },
                Err(_) => {},
            }

            match get_value(data, "host") {
                Ok(value) => {
                    self.host = value.to_string();
                },
                Err(_) => {},
            }
        }
    }

    fn init_actions(&mut self, actions: &JsonValue) {
        if actions.is_array() {
            for element in actions.members() {
                for entry in element.entries() {
                    self.actions.push(String::from(entry.0));
                    println!("{}", entry.0);
                }
            }
        }
    }

    pub fn init(&mut self, content: JsonValue) {
        match get_value(&content, "twitch-bot") {
            Ok(bot) => {
                self.init_base_info(&bot);
            },
            Err(_) => {

            },
        }

        match get_value(&content, "animations") {
            Ok(animations) => {
                self.init_actions(&animations);
            },
            Err(_) => {},
        }
    }
}

pub fn parse_config(config_path: String) -> Config {
    let mut config = Config::new();

    match fs::read_to_string(&config_path) {
        Ok(data) => {
            match json::parse(&data) {
                Ok(content) => {
                    config.init(content);
                },
                Err(_) => {
                },
            }
        },
        Err(_) => {},
    }

    return config;
}
