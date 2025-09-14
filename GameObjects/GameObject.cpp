// From my game 2: https://github.com/Jing20Chung/15666-Game2
#define GLM_ENABLE_EXPERIMENTAL
#define DEBUG_EN

#include "GameObject.hpp"
#include "../Scene.hpp"
#include "../Load.hpp"

#include <glm/glm.hpp>
// from https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
#include "glm/gtx/string_cast.hpp"
#include <iostream>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(){
    this->transform = nullptr;
};
GameObject::~GameObject(){};

// init function
void GameObject::init() {}

// bind transform and bounds according to mesh
bool GameObject::bind_drawable(Scene::Drawable* drawable_, Bounds bounds) {
    assert(drawable_->transform);
    this->transform = drawable_->transform;
    this->size.x = bounds.max.x - bounds.min.x;
    this->size.y = bounds.max.y - bounds.min.y;
    this->size.z = bounds.max.z - bounds.min.z;
    this->drawable = drawable_;
    // std::cout << "name: " << transform->name << ", size = " << glm::to_string(this->size) << ", bounds max = " << glm::to_string(this->get_bounds().max) << ", bounds min = " << glm::to_string(this->get_bounds().min)  << std::endl;
    return true;
}

// update input from Mode
void GameObject::update_input(SDL_Event const &evt) { }

// called by Mode
void GameObject::update(float elapsed) { 
    // std::cout <<"GameObject updating" << std::endl;
}

 // on collision
void GameObject::on_collision(GameObject& other) {
#ifdef DEBUG_EN
    std::cout << transform->name << " collide with " << other.transform->name << std::endl;
#endif
}

// get current bounds
Bounds GameObject::get_bounds() {
    // glm::vec3 world_pos = transform->make_world_from_local()[3];
    glm::vec3 world_pos = transform->position;
    glm::vec3 bound_max(world_pos.x + size.x/2, world_pos.y + size.y/2, world_pos.z + size.z/2);
    glm::vec3 bound_min(world_pos.x - size.x/2, world_pos.y - size.y/2, world_pos.z - size.z/2);
    return Bounds(bound_max, bound_min);
}

bool GameObject::check_collision(GameObject& obj_a, GameObject& obj_b) {
    // std::cout << "check_collision: " << obj_a.transform->name << " with " << obj_b.transform->name << std::endl;
    // check collision
	Bounds bounds_a = obj_a.get_bounds();
	Bounds bounds_b = obj_b.get_bounds();
	if (bounds_a.min.x <= bounds_b.max.x && bounds_a.max.x >= bounds_b.min.x) {
		if (bounds_a.min.y <= bounds_b.max.y && bounds_a.max.y >= bounds_b.min.y) {
			if (bounds_a.min.z <= bounds_b.max.z && bounds_a.max.z >= bounds_b.min.z) {
                return true;
			}
		}
	}
    return false;
}
