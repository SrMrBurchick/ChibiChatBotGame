use tokio::sync::mpsc;
use json::{self, array};

pub type RequestReceiver = mpsc::Receiver<EventType>;
pub type RequestSender = mpsc::Sender<EventType>;

#[derive(Debug)]
pub enum EventType {
    Subscription {
        user: String
    },
    SubscriptionsGift {
        gifter: String,
        subscribers: Vec<String>
    },
    Raid {
        streamer: String,
        viewers: u32
    },
    Ban {
        user: String,
        timeout: u32
    },
    Follow {
        user: String
    },
    ModeratorAdded {
        user: String
    },
    ModeratorRemoved {
        user: String
    },
    ChannelPointsAction {
        user: String,
        action: String
    }
}
