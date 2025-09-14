// From my game 2: https://github.com/Jing20Chung/15666-Game2
#include "Bullet.hpp"
#include <glm/glm.hpp>

// called by Mode, should be in update function
void Bullet::init() {
    tag = "Bullet";
    velocity = glm::vec3(0, 1, 0);
}

void Bullet::update(float elapsed) {
    update_position(elapsed);
}

// called by Mode, should be in update function
void Bullet::update_position(float elapsed) {
    this->transform->position += this->velocity * elapsed;
} 

// on collision
void Bullet::on_collision(GameObject* other) {
    // GameObject::on_collision(other);
    // Destroy if its not player
    if (other->tag != "Player") marked_destroy = true;
}