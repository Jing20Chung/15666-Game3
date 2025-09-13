// From my game 2: https://github.com/Jing20Chung/15666-Game2
#pragma once

#include "Floor.hpp"
#include "../Load.hpp"

struct MovingFloor : Floor {
    virtual void init() override; // init funciton
    virtual void update(float elapsed) override; // called by Mode, should be in update function
    virtual void on_collision(GameObject& other) override; // on collision
    
    void update_position(float elapsed); // Should be in update function
    void update_rotation(float elapsed); // Should be in update function
};