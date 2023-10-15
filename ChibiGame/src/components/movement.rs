use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

use crate::components::{
    actions::{
        Actions, WalkDirection, ClimbDirection, MoveType
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
}

#[derive(Component)]
pub struct PlayerMovementComponent {
    pub movement: Option<Box<dyn Reflect>>,
    pub enabled: bool,
    pub landed: bool,
    pub can_climb: bool,
    pub speed: f32,
}

impl PlayerMovementComponent {
    pub fn new() -> Self {
        PlayerMovementComponent {
            movement: None,
            enabled: false,
            landed: false,
            can_climb: false,
            speed: 3500.0
        }
    }

    pub fn on_action_changed(
        &mut self,
        action: Actions,
        velocity: &mut Velocity
    ) {
        if !self.enabled {
            return;
        }

        let current_move_direction = velocity.linvel;
        match action {
            Actions::Walk => {
                if current_move_direction.x >= 0.0 {
                    self.movement = Some(Box::new(WalkComponent {
                        direction: WalkDirection::Right
                    }));
                } else {
                    self.movement = Some(Box::new(WalkComponent {
                        direction: WalkDirection::Left
                    }));
                }

            }
            Actions::Climb => {
                if current_move_direction.y >= 0.0 {
                    self.movement = Some(Box::new(ClimbComponent {
                        direction: ClimbDirection::Up
                    }));
                } else {
                    self.movement = Some(Box::new(ClimbComponent {
                        direction: ClimbDirection::Down
                    }));
                }

            }
            _ => {
                info!("Disable movement");
                self.enabled = false;
                self.movement = None;
                velocity.linvel = Vec2::ZERO;
            },
        }
    }
}

pub fn move_player(
    time: Res<Time>,
    type_registry: Res<AppTypeRegistry>,
    mut query: Query<(&PlayerMovementComponent, &mut Velocity)>,
) {
    for (component, mut velocity) in &mut query {
        if component.enabled == false {
            continue;
        }

        match component.movement.as_deref() {
            Some(movement) => {
                let type_registry = type_registry.read();
                let reflect_movement = type_registry
                    .get_type_data::<ReflectMovementComponent>(movement.type_id())
                    .unwrap();

                let movement_trait: &dyn MovementComponent =
                    reflect_movement.get(&*movement).unwrap();

                velocity.linvel =
                    movement_trait.get_velocity() * component.speed * time.delta_seconds();
            }
            _ => {}
        }
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

pub fn monitor_movement(
    mut event_writer: EventWriter<Event>,
    components: Query<(&Velocity, &GameplayLogicComponent, &PlayerMovementComponent), With<PlayerComponent>>
) {
    for (velocity, gameplay, movement) in components.iter() {
        if !movement.enabled {
            continue;
        }

        // Detect that the player is moving down
        if velocity.linvel.y < 0.0 {
            // Check is the current action is not climb
            if gameplay.get_current_action() != Actions::Climb {
                event_writer.send(
                    Event {
                        event_type: Events::GameEvents(GameEvents::SetNewAction(Actions::Fall))
                    }
                );
            }
        }
    }
}

