#[derive(Debug, Clone)]
pub struct Action {
    pub name: String,
    pub reward_to_listen: String
}

impl Action {
    pub fn new() -> Self {
        Self {
            name: String::default(),
            reward_to_listen: String::default()
        }
    }
}
