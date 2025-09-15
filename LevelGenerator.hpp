#pragma once

#include "GameObjects/GameObject.hpp"
#include "Scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <list>

enum struct ObjectType {
    Player,
    MovingFloor,
    MovingFloorDamageable,
    Bullet,
    JumpAbility
};

struct LevelGenerator {
    LevelGenerator();
    ~LevelGenerator();
    void spawn_object_row(int level_index, int row_index);
    std::shared_ptr< GameObject > spawn_object(ObjectType type, glm::vec3 position, glm::quat rotation, MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao);
    void clean_up_destroyed_object();
    void update(float elapsed);
    void start_spawn(int level_index);
    void stop_spawn();
    void init(Scene* scene_, MeshBuffer const * mesh_buffer_, std::list< std::shared_ptr< GameObject > >* object_container_, Scene::Drawable::Pipeline pipeline_, GLuint vao_);
    void load(std::string const &filename);
    
    Scene* scene_ptr;
    MeshBuffer const * mesh_buffer;
    Scene::Drawable::Pipeline pipeline;
    GLuint vao;


    std::unordered_map< uint32_t, std::vector< std::vector< int > > > level_maps;
	// manage level objects
	std::list< std::shared_ptr< GameObject > >* object_container_ptr;
	std::unordered_map< ObjectType, std::string > xf_name_lookup;
	std::unordered_map< int, ObjectType > object_type_lookup;
    uint32_t cur_row_index = 0;
    uint32_t cur_level_index = 0;
    uint32_t cur_object_id = 0;
    const float C_START_Y = 5.0f;
    const float C_SIZE_PER_BLOCK = 1.0f;
    const float C_SPACING = 1.0f;
    const float C_POS_OFFSET = C_SIZE_PER_BLOCK + C_SPACING;
    const int C_EMPTY = -1;

    bool isSpawning = false;
    float next_spawn_time = 0;
    float cur_accumulated_time = 0;
    float tempo = 1.0f; // second
};