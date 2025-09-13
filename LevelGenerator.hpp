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
    MovingWall,
    MovingWallDamageable,
    Bullet,
    JumpAbility
};

struct LevelGenerator {
    
    void spawn_object_row(Scene* scene, MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao);
    void spawn_object(ObjectType type, glm::vec3 position, glm::quat rotation, Scene* scene, MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao);
    void update(float elapsed);
    void load();
    std::unordered_map<uint32_t, std::vector<std::vector<int>>> level_maps;
	// manage level objects
	std::list< GameObject > level_objects;
	std::unordered_map< ObjectType, std::string > xf_name_lookup;
	std::unordered_map< int, ObjectType > object_type_lookup;
    std::unordered_map< std::string, Bounds > bounds_map;
    uint32_t cur_row_index = 0;
    uint32_t cur_level_index = 0;
    float spacing = 2;
    glm::vec3 init_pos = glm::vec3(0, 0, 0);
};