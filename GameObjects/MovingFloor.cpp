// From my game 2: https://github.com/Jing20Chung/15666-Game2
#include "MovingFloor.hpp"

#include "../Scene.hpp"
#include "../Load.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// from https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>

void MovingFloor::init() {
    Floor::init();
    velocity = glm::vec3(0, -3, 0);
}

void MovingFloor::update(float elapsed) {
    update_position(elapsed);
    update_rotation(elapsed);
}

// called by Mode, should be in update function
void MovingFloor::update_position(float elapsed) {
    this->transform->position += this->velocity * elapsed;
} 

// called by Mode, should be in update function
void MovingFloor::update_rotation(float elapsed) {
} 

// on collision
void MovingFloor::on_collision(GameObject* other) {
    // GameObject::on_collision(other);
    if (other->tag != "Player") {
        velocity *= -1;
    }
}
