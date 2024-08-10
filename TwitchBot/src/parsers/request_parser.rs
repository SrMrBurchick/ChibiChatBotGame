use chrono::prelude::*;
use json::{self, JsonValue};

use crate::components::action::Action;
use crate::parsers::config::get_value;

pub type RequestReceiver = crossbeam_channel::Receiver<Action>;
pub type RequestSender = crossbeam_channel::Sender<Action>;

#[derive(Debug, Clone)]
pub enum EventType {
    Subscription {
        user: String,
    },
    SubscriptionsGift {
        gifter: String,
        subscribers: Vec<String>,
    },
    Raid {
        streamer: String,
        viewers: u32,
    },
    Ban {
        user: String,
        reason: String,
        timeout: u32,
        permanent: bool,
    },
    UnBan {
        user: String,
    },
    Follow {
        user: String,
    },
    ModeratorAdded {
        user: String,
    },
    ModeratorRemoved {
        user: String,
    },
    ChannelPointsAction {
        user: String,
        reward_id: String,
        user_input: String,
    },
    Unknown,
}

pub fn convert_twitch_type_to_enum(twitch_type: String, data: &JsonValue) -> EventType {
    let mut event_type: EventType = EventType::Unknown;
    match twitch_type.to_string().as_ref() {
        "channel.follow" => match get_value(&data, "user_name") {
            Ok(user_name) => {
                event_type = EventType::Follow {
                    user: user_name.to_string(),
                };
            }
            Err(_) => {}
        },
        "channel.subscribe" => match get_value(&data, "user_name") {
            Ok(user_name) => {
                event_type = EventType::Subscription {
                    user: user_name.to_string(),
                };
            }
            Err(_) => {}
        },
        "channel.ban" => {
            let user: String;
            let reason: String;
            let timeout: u32;
            let is_permanent: bool;
            match get_value(&data, "user_name") {
                Ok(user_name) => {
                    user = user_name.to_string();
                }
                Err(_) => {
                    user = String::default();
                }
            }
            match get_value(&data, "reason") {
                Ok(value) => {
                    reason = value.to_string();
                }
                Err(_) => {
                    reason = String::default();
                }
            }
            match get_value(&data, "is_permanent") {
                Ok(value) => {
                    is_permanent = value.as_bool().unwrap();
                }
                Err(_) => {
                    is_permanent = false;
                }
            }

            match get_value(&data, "banned_at") {
                Ok(ban_start) => match get_value(&data, "ends_at") {
                    Ok(ban_end) => {
                        let datetime1 =
                            DateTime::parse_from_rfc3339(ban_start.to_string().as_str())
                                .unwrap()
                                .with_timezone(&Utc);
                        let datetime2 = DateTime::parse_from_rfc3339(ban_end.to_string().as_str())
                            .unwrap()
                            .with_timezone(&Utc);
                        let duration = datetime2.signed_duration_since(datetime1);
                        timeout = duration.num_seconds() as u32;
                    }
                    Err(_) => {
                        timeout = 0;
                    }
                },
                Err(_) => {
                    timeout = 0;
                }
            }

            event_type = EventType::Ban {
                user,
                reason,
                timeout,
                permanent: is_permanent,
            };
        }
        "channel.unban" => match get_value(&data, "user_name") {
            Ok(user_name) => {
                event_type = EventType::UnBan {
                    user: user_name.to_string(),
                };
            }
            Err(_) => {}
        },
        "channel.raid" => {
            let raid_channel: String;
            let viewers: u32;

            match get_value(&data, "from_broadcaster_user_name") {
                Ok(value) => {
                    raid_channel = value.to_string();
                }
                Err(_) => raid_channel = String::default(),
            }

            match get_value(&data, "viewers") {
                Ok(value) => {
                    viewers = value.as_u32().unwrap();
                }
                Err(_) => {
                    viewers = 0;
                }
            }

            event_type = EventType::Raid {
                streamer: raid_channel,
                viewers,
            };
        }
        "channel.channel_points_custom_reward_redemption.add" => {
            let mut user: String = String::default();
            let mut reward_id: String = String::default();
            let mut user_input: String = String::default();

            match get_value(&data, "user_name") {
                Ok(value) => {
                    user = value.to_string();
                }
                Err(_) => {}
            }

            match get_value(&data, "user_input") {
                Ok(value) => {
                    user_input = value.to_string();
                }
                Err(_) => {}
            }

            match get_value(&data, "reward") {
                Ok(reward) => match get_value(&reward, "id") {
                    Ok(value) => {
                        reward_id = value.to_string();
                    }
                    Err(_) => {}
                },
                Err(_) => {}
            }

            event_type = EventType::ChannelPointsAction { user, reward_id, user_input };
        }
        _ => {}
    }

    event_type
}
