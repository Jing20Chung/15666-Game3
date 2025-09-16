// From my game 2: https://github.com/Jing20Chung/15666-Game2
#pragma once

#include "GameObject.hpp"
#include "../Load.hpp"
#include "../Sound.hpp"

struct Player : GameObject {
    virtual void init() override; // init funciton
    virtual void update_input(SDL_Event const &evt) override; // update input from Mode
    virtual void update(float elapsed) override; // called by Mode, should be in update function
    virtual void on_collision(GameObject* other) override; // on collision

    struct InputSet {
        bool left, right, up, down, space, mouse_left;
        InputSet() {
            left = right = up = down = space = mouse_left = false;
        }
    } input;

    void update_position(float elapsed); // Should be in update function
    void update_rotation(float elapsed); // Should be in update function

    bool isDead = false;
    bool isWin = false;
    bool isRequestShootBullet = false;
    bool isTopDownView = true;

    // from PlayMode
	std::shared_ptr< Sound::PlayingSample > move_left_oneshot;
	std::shared_ptr< Sound::PlayingSample > move_right_oneshot;
	std::shared_ptr< Sound::PlayingSample > jump_oneshot;
	std::shared_ptr< Sound::PlayingSample > land_oneshot;
	std::shared_ptr< Sound::PlayingSample > shoot_oneshot;
};