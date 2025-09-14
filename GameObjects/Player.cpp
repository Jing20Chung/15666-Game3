// From my game 2: https://github.com/Jing20Chung/15666-Game2
#include "Player.hpp"
#include "../Scene.hpp"
#include "../Mode.hpp"
#include "../PlayMode.hpp"
#include "../Ray.hpp"
#include "../data_path.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// from https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

Load< Sound::Sample > move_left_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("honk.wav"));
});

Load< Sound::Sample > move_right_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("honk.wav"));
});

Load< Sound::Sample > shoot_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("honk.wav"));
});

void Player::init() {
	tag = "Player";
    velocity = glm::vec3(0,0,0);
	parent = nullptr;
}

// update input from Mode
void Player::update_input(SDL_Event const &evt) {
    // Copy from PlayMode.cpp
    if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_SPACE) { // Jump
            input.space = true;
			return;
		} else if (evt.key.key == SDLK_A) {
            input.left = true;
			if (move_oneshot) move_oneshot->stop();
			move_oneshot = Sound::play_3D(*move_left_sample, 0.3f, glm::vec3(4.6f, -7.8f, 6.9f)); //hardcoded position of front of car, from blender
			return;
		} else if (evt.key.key == SDLK_D) {
            input.right = true;
			if (move_oneshot) move_oneshot->stop();
			move_oneshot = Sound::play_3D(*move_right_sample, 0.3f, glm::vec3(4.6f, -7.8f, 6.9f)); //hardcoded position of front of car, from blender
			return;
		} else if (evt.key.key == SDLK_W) {
            input.up = true;
			return;
		} else if (evt.key.key == SDLK_S) {
            input.down = true;
			return;
		}
	} else if (evt.type == SDL_EVENT_KEY_UP) {
		if (evt.key.key == SDLK_SPACE) {
            input.space = false;
			return;
		} else if (evt.key.key == SDLK_A) {
            input.left = false;
			return;
		} else if (evt.key.key == SDLK_D) {
            input.right = false;
			return;
		} else if (evt.key.key == SDLK_W) {
            input.up = false;
			return;
		} else if (evt.key.key == SDLK_S) {
            input.down = false;
			return;
		} 
	} else if (evt.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		// reference: https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent
		if (evt.button.button == SDL_BUTTON_LEFT) {
			input.mouse_left = true;
			if (shoot_oneshot) shoot_oneshot->stop();
			shoot_oneshot = Sound::play_3D(*shoot_sample, 0.3f, glm::vec3(4.6f, -7.8f, 6.9f)); //hardcoded position of front of car, from blender
			isRequestShootBullet = true;
			return;
		}
	} else if (evt.type == SDL_EVENT_MOUSE_BUTTON_UP) {
		if (evt.button.button == SDL_BUTTON_LEFT) {
			input.mouse_left = false;
			return;
		}
	} else if (evt.type == SDL_EVENT_MOUSE_MOTION) {
		// if (SDL_GetWindowRelativeMouseMode(Mode::window) == true) {
		// 	glm::vec2 motion = glm::vec2(
		// 		evt.motion.xrel / float(window_size.y),
		// 		-evt.motion.yrel / float(window_size.y)
		// 	);
		// 	camera->transform->rotation = glm::normalize(
		// 		camera->transform->rotation
		// 		* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
		// 		* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
		// 	);
		// 	return true;
		// }
	}
}

// called by Mode
void Player::update(float elapsed) {
	update_position(elapsed);
	update_rotation(elapsed);
}

// Should be in update function
void Player::update_position(float elapsed) {
	if (isTopDownView) 
	{
		static float move_dist = 2.0f;
		if (input.left) {
			this->transform->position -= glm::vec3(move_dist, 0, 0);
		}
		else if (input.right) {
			this->transform->position += glm::vec3(move_dist, 0, 0);
		}
		else if (input.mouse_left) {

		}

		input.left = false;
		input.right = false;
		input.mouse_left = false;

	} else { // Velocity calculation
		static float move_speed = 30.0f;
		static float move_speed_max = 10.0f;
		static float jump_speed = 30.0f;
		static float max_fall_speed = -30.0f;
		static float slowdown_speed = 0.0f;
		constexpr float C_VELOCITY_EPSILON = 0.001f;
		constexpr float C_GRAVITY = -9.18f * 8;

		// try to implement is on ground detection and move along with the ground.
		if (parent != nullptr) {
			this->velocity += parent->velocity;
		}

		// decrease velocity by elapsed time
		float slowdown_this_frame = slowdown_speed * elapsed;
		velocity.x += velocity.x < 0? slowdown_this_frame: -slowdown_this_frame;
		velocity.y += velocity.y < 0? slowdown_this_frame: -slowdown_this_frame;
		// set to zero if the value is small enough
		if (velocity.x != 0 && glm::abs(velocity.x) < C_VELOCITY_EPSILON) velocity.x = 0;
		if (velocity.y != 0 && glm::abs(velocity.y) < C_VELOCITY_EPSILON) velocity.y = 0;
		if (velocity.z != 0 && glm::abs(velocity.z) < C_VELOCITY_EPSILON) velocity.z = 0;

		// apply new input
		if (input.left) {
			velocity += glm::vec3(-1, 0, 0) * move_speed;
		}
		if (input.right) {
			velocity += glm::vec3(1, 0, 0) * move_speed;
		}
		if (input.up) {
			velocity += glm::vec3(0, 1, 0) * move_speed;
		}
		if (input.down) {
			velocity += glm::vec3(0, -1, 0) * move_speed;
		}
		if (input.space && parent != nullptr) {
			velocity += glm::vec3(0, 0, 1) * jump_speed;
		}

		// apply gravity
		velocity.z += C_GRAVITY * elapsed;

		// clamp veloctiy
		velocity.x = velocity.x < 0? glm::max(velocity.x, -move_speed_max): glm::min(velocity.x, move_speed_max);
		velocity.y = velocity.y < 0? glm::max(velocity.y, -move_speed_max): glm::min(velocity.y, move_speed_max);
		velocity.z = glm::max(velocity.z, max_fall_speed);
    
		// Update position
		this->transform->position += this->velocity * elapsed;

		if (parent != nullptr) {
			// check leave parent
			if (GameObject::check_collision(static_cast<GameObject*>(this), parent)) {
				// hard code set z on the floor
				this->transform->position.z = size.z / 2 + parent->get_bounds().max.z;
			}
			else {
				// detach parent
				parent = nullptr;
			}
		}
	}

	if (this->transform->position.z < -10) {
		std::cout << "die!" << std::endl;
		isDead = true;
	}

	isRequestShootBullet = false;
} 

// Should be in update function
void Player::update_rotation(float elapsed) {
} 

// on collision
void Player::on_collision(GameObject* other) {
	if (other->tag == "Floor") {
		if (isTopDownView) {
			isDead = true;
		}
		else {
			parent = other;
		}
	}
	else {
		std::cout << "Player collide with " << other->transform->name << std::endl;
	}
}
