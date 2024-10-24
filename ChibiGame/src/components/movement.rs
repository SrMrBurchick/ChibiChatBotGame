use bevy::prelude::*;
use bevy_rapier2d::prelude::*;
use rand::prelude::*;

use crate::components::{
    actions::{
        ActionType, WalkDirection, ClimbDirection, MoveType, ActionsManager
    },
    common::events::{
        Event, GameEvents, Events, OverlapType
    },
    gameplay::{
        gameplay_logic::GameplayLogicComponent,
        player::PlayerComponent
    }
};

#[reflect_trait]
pub trait MovementComponent {
    fn get_velocity(&self) -> Vec2;
    fn get_movement_type(&self) -> MoveType;
    fn swap_direction(&mut self);
}

#[derive(Component, Debug, Reflect)]
#[reflect(MovementComponent)]
pub struct WalkComponent {
    pub direction: WalkDirection,
}

impl MovementComponent for WalkComponent {
    fn get_velocity(&self) -> Vec2 {
        let mut velocity = Vec2::ZERO;
        match self.direction {
            WalkDirection::Left => {
                velocity.x -= 1.0;
            }
            WalkDirection::Right => {
                velocity.x += 1.0;
            }
        }

        velocity.normalize_or_zero()
    }

    fn get_movement_type(&self) -> MoveType {
        MoveType::Walk(self.direction)
    }

    fn swap_direction(&mut self) {
        match self.direction {
            WalkDirection::Left => {
                self.direction = WalkDirection::Right;
            }
            WalkDirection::Right => {
                self.direction = WalkDirection::Left;
            },
        }
    }
}

#[derive(Component, Debug, Reflect)]
#[reflect(MovementComponent)]
pub struct ClimbComponent {
    pub direction: ClimbDirection,
}

impl MovementComponent for ClimbComponent {
    fn get_velocity(&self) -> Vec2 {
        let mut velocity = Vec2::ZERO;
        match self.direction {
            ClimbDirection::Up => {
                velocity.y += 1.0;
            }
            ClimbDirection::Down => {
                velocity.y -= 1.0;
            }
        }

        velocity.normalize_or_zero()
    }

    fn get_movement_type(&self) -> MoveType {
        MoveType::Climb(self.direction)
    }

    fn swap_direction(&mut self) {
        // TODO: Fix if player already landed and direction is down than swap directions or change action
        // match self.direction {
        //     ClimbDirection::Up => {
        //         self.direction = ClimbDirection::Down;
        //     },
        //     ClimbDirection::Down => {
        //         self.direction = ClimbDirection::Up;
        //     }
        // }
    }
}

#[derive(Component)]
pub struct PlayerMovementComponent {
    pub movement: Option<Box<dyn Reflect>>,
    pub active: bool,
    pub landed: bool,
    pub can_climb: bool,
    pub speed: f32,
    pub last_walk_direction: WalkDirection
}

impl PlayerMovementComponent {
    pub fn new() -> Self {
        PlayerMovementComponent {
            movement: None,
            active: false,
            landed: false,
            can_climb: false,
            speed: 3500.0,
            last_walk_direction: WalkDirection::Right
        }
    }

    pub fn on_action_changed(
        &mut self,
        action: ActionType,
        velocity: &mut Velocity
    ) {
        self.active = true;
        match action {
            ActionType::Walk => {
                let mut rand = rand::thread_rng();
                let direction_rand: f64 = rand.gen();
                let mut walk_direction: WalkDirection;
                if direction_rand < 0.5 {
                    walk_direction = WalkDirection::Left;
                } else {
                    walk_direction = WalkDirection::Right;
                }

                if self.can_climb {
                    walk_direction = self.last_walk_direction
                }

                self.movement = Some(Box::new(WalkComponent {
                    direction: walk_direction
                }));
            }
            ActionType::Climb => {
                self.movement = Some(Box::new(ClimbComponent {
                    direction: ClimbDirection::Up
                }));
            }
            _ => {
                info!("Disable movement");
                self.active = false;
                self.movement = None;
                velocity.linvel = Vec2::ZERO;
            },
        }
    }

    pub fn swap_direction(
        &mut self,
        type_registry: &AppTypeRegistry,
    ) {
        match self.movement.take() {
            Some(mut movement) => {
                let type_registry = type_registry.read();
                let reflect_movement = type_registry
                    .get_type_data::<ReflectMovementComponent>(movement.type_id())
                    .unwrap();

                let movement_trait: &mut dyn MovementComponent =
                    reflect_movement.get_mut(&mut *movement).unwrap();

                movement_trait.swap_direction();

                self.movement = Some(movement);
            },
            None => {
                match self.last_walk_direction {
                    WalkDirection::Right => {
                        self.last_walk_direction = WalkDirection::Left;
                    },
                    WalkDirection::Left => {
                        self.last_walk_direction = WalkDirection::Right;
                    }
                }
            },
        }
    }

    pub fn get_move_type(&self, type_registry: &AppTypeRegistry) -> Option<MoveType> {
        match self.movement.as_deref() {
            Some(movement) => {
                let type_registry = type_registry.read();
                let reflect_movement = type_registry
                    .get_type_data::<ReflectMovementComponent>(movement.type_id())
                    .unwrap();

                let movement_trait: &dyn MovementComponent =
                    reflect_movement.get(&*movement).unwrap();

                return Some(movement_trait.get_movement_type());
            },
            None => {
                None
            }
        }
    }
}

pub fn move_player(
    time: Res<Time>,
    type_registry: Res<AppTypeRegistry>,
    mut query: Query<(&mut PlayerMovementComponent, &GameplayLogicComponent, &ActionsManager, &mut Velocity)>,
) {
    for (mut component, gameplay, action, mut velocity) in &mut query {
        if component.active == false || gameplay.is_movement_enabled() == false {
            continue;
        }

        let mut direction = component.last_walk_direction;
        let mut can_move: bool = false;

        match component.movement.as_deref() {
            Some(movement) => {
                let type_registry = type_registry.read();
                let reflect_movement = type_registry
                    .get_type_data::<ReflectMovementComponent>(movement.type_id())
                    .unwrap();

                let movement_trait: &dyn MovementComponent =
                    reflect_movement.get(&*movement).unwrap();

                info!("Movement {:?}. Last direction: {:?}", movement_trait.get_movement_type(), direction);

                match action.current_action {
                    ActionType::Walk => {
                        match movement_trait.get_movement_type() {
                            MoveType::Walk(walk) => {
                                direction = walk;
                            }
                            _ => {},
                        }
                        can_move = component.landed;
                    }
                    ActionType::Climb => {
                        can_move = component.can_climb;
                    }
                    _ => {
                        can_move = false;
                    },
                }

                if can_move {
                    velocity.linvel =
                        movement_trait.get_velocity() * component.speed * time.delta_seconds();
                }
            }
            _ => {}
        }

        component.last_walk_direction = direction;
    }
}

pub fn monitor_collisions(
    mut collision_events: EventReader<CollisionEvent>,
    mut event_writer: EventWriter<Event>,
    colliders: Query<Entity, With<Collider>>,
    player: Query<Entity, With<PlayerComponent>>
) {
    match player.single() {
        (player_entity) => {
            for collision in collision_events.iter() {
                match collision {
                    CollisionEvent::Stopped(e1, e2, _) => {
                        for collider_entity in colliders.iter() {
                            let is_player = player_entity == *e1 || player_entity == *e2;
                            let is_collider = collider_entity == *e1 || collider_entity == *e2;

                            if is_player && is_collider {
                                event_writer.send(
                                    Event {
                                        event_type: Events::GameEvents(GameEvents::PlayerOverlapped(OverlapType::Ended(collider_entity)))
                                    }
                                );

                            }
                        }

                    },
                    CollisionEvent::Started(e1, e2, _) => {
                        for collider_entity in colliders.iter() {
                            let is_player = player_entity == *e1 || player_entity == *e2;
                            let is_collider = collider_entity == *e1 || collider_entity == *e2;

                            if is_player && is_collider {
                                event_writer.send(
                                    Event {
                                        event_type: Events::GameEvents(GameEvents::PlayerOverlapped(OverlapType::Started(collider_entity)))
                                    }
                                );

                            }
                        }
                    },
                    _ => {},
                }
            }
        }
        _ => {},
    }
}


#[derive(Resource)]
pub struct MonitorMovementInfo {
    pub fall_populated: bool
}

pub fn monitor_movement(
    mut event_writer: EventWriter<Event>,
    mut monitor_info: ResMut<MonitorMovementInfo>,
    components: Query<(&Velocity, &GameplayLogicComponent, &PlayerMovementComponent), With<PlayerComponent>>
) {
    for (velocity, gameplay, movement) in components.iter() {
        if movement.landed {
            continue;
        }

        // Detect that the player is moving down
        if velocity.linvel.y < 0.0 {
            // Check is the current action is not climb
            if gameplay.get_current_action() != ActionType::Climb &&
               gameplay.get_current_action() != ActionType::Fall
            {
                if monitor_info.fall_populated {
                    continue;
                }

                event_writer.send(
                    Event {
                        event_type: Events::GameEvents(GameEvents::SetNewAction(ActionType::Fall))
                    }
                );

                monitor_info.fall_populated = true;
            } else {
                if monitor_info.fall_populated {
                    monitor_info.fall_populated = false;
                }
            }
        }
    }
}
