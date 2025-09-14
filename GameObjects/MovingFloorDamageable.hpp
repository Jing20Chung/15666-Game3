#pragma once

#include "MovingFloor.hpp"

struct MovingFloorDamageable: MovingFloor {
    virtual void on_collision(GameObject* other) override; // on collision
};
