#pragma once

#include "GameObject.hpp"

struct Bullet : GameObject {
    virtual void init() override;
    virtual void update(float elapsed) override; // called by Mode, should be in update function
    virtual void on_collision(GameObject* other) override; // on collision
    
    void update_position(float elapsed); // Should be in update function
};