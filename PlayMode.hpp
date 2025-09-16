#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include "LevelGenerator.hpp"

#include "GameObjects/GameObject.hpp"
#include "GameObjects/Player.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	void clean_up_destroyed_object();
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	glm::vec3 get_leg_tip_position();

	std::shared_ptr< Sound::PlayingSample > game_background_music;

	//car honk sound:
	std::shared_ptr< Sound::PlayingSample > honk_oneshot;

	//camera:
	Scene::Camera *camera = nullptr;

	// LevelGenerator
	LevelGenerator level_gen;

	// object container
	std::list< std::shared_ptr< GameObject > > level_objects;

	// Player
	Player* player;

	// Start cam pos
	Scene::Transform *first_cam_pos = nullptr;
	// Second cam pos
	Scene::Transform *second_cam_pos = nullptr;

	float cur_accumulated_time = 0;
	float time_switch_to_second_cam_pos = 13;
	float song_end_time = 40;
};
