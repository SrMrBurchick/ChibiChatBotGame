use core::str;
use std::{
    process::Command,
    path::Path,
    fmt::Debug
};
use json::{self, JsonValue};
use crate::parsers::config::get_value;
use crate::components::action::Action;

#[derive(Debug, Clone)]
pub enum ConditionOperator {
    Equal,
    Greater,
    Lower,
    NotEqual,
    Undefined
}

#[derive(Debug, Clone)]
pub enum ExecutionType {
    Random {
        chance: u32
    },
    Condition {
        value: f32,
        operator: ConditionOperator
    },
    Sequence {
        index: u8
    }
}

#[derive(Debug, Clone)]
pub enum OutputType {
    Bool,
    Number,
    String,
    Undefined
}

#[derive(Debug, Clone)]
pub enum BindPoolResultType {
    Bool {
        value: bool
    },
    Number {
        value: f32,
        condition: ConditionOperator
    },
    String {
        value: String
    },
    Undefined
}

#[derive(Debug, Clone)]
pub struct OutputParam {
    pub name: String,
    pub result_type: OutputType,
}

#[derive(Debug, Clone)]
pub struct InputParam {
    pub name: String,
    pub value: String
}

impl InputParam {
    pub fn new() -> Self {
        Self {
            name: String::default(),
            value: String::default()
        }
    }
}

impl OutputParam {
    pub fn new() -> Self {
        Self {
            name: String::default(),
            result_type: OutputType::Undefined
        }
    }

    pub fn parse_config(&mut self, config: &JsonValue) {
        match get_value(&config, "name") {
            Ok(value) => {
                self.name = value.to_string();
            },
            Err(_) => {},
        }

        match get_value(&config, "type") {
            Ok(value) => {
                match value.to_string().as_str() {
                    "bool" => {
                        self.result_type = OutputType::Bool;
                    },
                    "number" => {
                        self.result_type = OutputType::Number;
                    },
                    "string" => {
                        self.result_type = OutputType::String;
                    }
                    _ => {},
                }
            },
            Err(_) => {},
        }
    }
}

// =========== Game Config ===============
#[derive(Debug, Clone)]
pub struct ModuleBindPoolResult {
    action: String,
    result_value: BindPoolResultType
}

#[derive(Debug, Clone)]
pub struct ModuleBindPool {
    pub postfix: String,
    pub results: Vec<ModuleBindPoolResult>
}

impl ModuleBindPoolResult {
    pub fn new() -> Self {
        Self {
            action: String::default(),
            result_value: BindPoolResultType::Undefined
        }
    }

    pub fn parse_config(&mut self, config: &JsonValue, pool: &ModuleBindPool, output: &OutputParam) {
        match get_value(&config, "name") {
            Ok(value) => {
                self.action = value.to_string();
            },
            Err(_) => {},
        }

        match output.result_type {
            OutputType::Bool => {
                match pool.postfix.as_str() {
                    "OnTrue" => {
                        self.result_value = BindPoolResultType::Bool { value: true };
                    },
                    "OnFalse" => {
                        self.result_value = BindPoolResultType::Bool { value: false };
                    }
                    _ => {},
                }
            },
            OutputType::Number => {
                // TODO
            },
            OutputType::String => {
                // TODO
            },
            _ => {},
        }
    }

    pub fn get_action(&self) -> String {
        self.action.clone()
    }

    pub fn check_result(&self, result: &JsonValue) -> bool {
        match &self.result_value {
            BindPoolResultType::Bool { value } => {
                match result.as_bool() {
                    Some(result_value) => {
                        return result_value == *value;
                    },
                    None => {},
                }
            },
            BindPoolResultType::Number { value, condition } => {
                // TODO
            },
            BindPoolResultType::String { value } => {
                // TODO
            }
            _ => {},
        }

        return false;
    }
}

#[derive(Debug, Clone)]
pub struct ModuleBindResult {
    pub target_output: OutputParam,
    pub pools: Vec<ModuleBindPool>
}

#[derive(Debug, Clone)]
pub struct ModuleBind {
    pub target_action: String,
    pub results: Vec<ModuleBindResult>
}

#[derive(Debug, Clone)]
pub struct Module {
    pub name: String,
    pub path: String,
    pub config_path: String,
    pub input_params: Vec<InputParam>,
    pub output_params: Vec<OutputParam>,
    pub module_directory: String,

    // Game
    pub binds: Vec<ModuleBind>
}

impl ModuleBindPool {
    fn new() -> Self {
        Self {
            postfix: String::default(),
            results: vec![]
        }
    }

    fn parse_config(&mut self, config: &JsonValue, output: &OutputParam) {
        match get_value(&config, "postfix") {
            Ok(value) => {
                self.postfix = value.to_string();
            },
            Err(_) => {},
        }

        match get_value(&config, "results") {
            Ok(value) => {
                if value.is_array() {
                    for element in value.members() {
                        let mut result: ModuleBindPoolResult = ModuleBindPoolResult::new();
                        result.parse_config(element, self, output);
                        self.results.push(result);
                    }
                }
            },
            Err(_) => {},
        }
    }
}

impl ModuleBindResult {
    pub fn new() -> Self {
        Self {
            target_output: OutputParam::new(),
            pools: vec![]
        }
    }

    pub fn parse_config(&mut self, config: &JsonValue) {
        self.target_output.parse_config(&config);
        match get_value(&config, "pools") {
            Ok(value) => {
                if value.is_array() {
                    for element in value.members() {
                        let mut pool: ModuleBindPool = ModuleBindPool::new();
                        pool.parse_config(element, &self.target_output);
                        self.pools.push(pool);
                    }
                }
            },
            Err(_) => {},
        }
    }

    pub fn parse_result(&self, result: &JsonValue) -> String {
        match get_value(&result, self.target_output.name.as_str()) {
            Ok(value) => {
                for pool in self.pools.iter() {
                    for result in pool.results.iter() {
                        if result.check_result(&value) {
                            return result.action.clone();
                        }
                    }
                }
            },
            Err(_) => {},
        }

        return String::from("");

    }

}

impl ModuleBind {
    pub fn new() -> Self {
        Self {
            target_action: String::default(),
            results: vec![]
        }
    }

    pub fn parse_config(&mut self, config: &JsonValue) {
        match get_value(&config, "target_action") {
            Ok(value) => {
                self.target_action = value.to_string();
            },
            Err(_) => {},
        }

        match get_value(&config, "results") {
            Ok(value) => {
                if value.is_array() {
                    let mut result: ModuleBindResult = ModuleBindResult::new();
                    result.parse_config(&value);
                    self.results.push(result);
                }
            },
            Err(_) => {},
        }
    }
}

impl Module {
    pub fn new() -> Self {
        Self {
            name: String::default(),
            path: String::default(),
            config_path: String::default(),
            module_directory: String::default(),
            input_params: vec![],
            output_params: vec![],
            binds: vec![]
        }
    }

    pub fn execute(&self, action: &Action) -> Action {
        let mut result_action = Action::new();
        let mut module_process = Command::new(Path::new(self.module_directory.as_str()).join(self.path.as_str()));

        // Args initialization TODO
        for param in self.input_params.clone() {
            module_process.arg(param.name.clone());
            module_process.arg(param.value.clone());
        }

        match module_process.output() {
            Ok(output) => {
                match str::from_utf8(&output.stdout) {
                    Ok(data) => {
                        match json::parse(data) {
                            Ok(json_data) => {
                                for bind in self.binds.iter() {
                                    if bind.target_action == action.name {
                                        for result in bind.results.iter() {
                                            let action_name = result.parse_result(&json_data);
                                            if !action_name.is_empty() {
                                                result_action.name = action_name;
                                                return result_action;
                                            }
                                        }
                                    }
                                }
                            },
                            Err(_) => {},
                        }
                    },
                    Err(_) => {},
                }
            },
            Err(_) => {},
        }
        return result_action;
    }

    pub fn parse_config(&mut self, config: &JsonValue) {
        match get_value(&config, "name") {
            Ok(value) => {
                self.name = value.to_string();
            },
            Err(_) => {},
        }

        match get_value(&config, "directory") {
            Ok(value) => {
                self.module_directory = value.to_string();
            },
            Err(_) => {},
        }

        match get_value(&config, "path") {
            Ok(value) => {
                self.path = value.to_string();
            },
            Err(_) => {},
        }

        match get_value(&config, "binds") {
            Ok(value) => {
                if value.is_array() {
                    for element in value.members() {
                        let mut bind: ModuleBind = ModuleBind::new();
                        bind.parse_config(&element);
                        self.binds.push(bind);
                    }
                }
            },
            Err(_) => {},
        }

        // TOOD: Read module config
        match get_value(&config, "config_path") {
            Ok(value) => {
                self.config_path = value.to_string();
            },
            Err(_) => {},
        }

    }

    pub fn is_binded_to_action(&self, action: &Action) -> bool {
        for bind in self.binds.iter() {
            if bind.target_action == action.name {
                return true;
            }
        }

        return false;
    }
}
