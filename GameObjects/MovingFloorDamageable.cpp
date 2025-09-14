#include "MovingFloorDamageable.hpp"

void MovingFloorDamageable::on_collision(GameObject* other) {
    MovingFloor::on_collision(other);
    if (other->tag == "Bullet") {
        marked_destroy = true;
    }
}
