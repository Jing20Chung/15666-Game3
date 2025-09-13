#include "LevelGenerator.hpp"
#include "GameObjects/MovingWall.hpp"
#include "GameObjects/JumpAbility.hpp"

// from https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>

void LevelGenerator::spawn_object_row(Scene* scene, MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao) {
    std::vector<int> cur_row = level_maps[cur_level_index][cur_row_index];
    int n = (int)cur_row.size();
    glm::vec3 pos = init_pos;
    for (int i = 0; i < n; i++) {
        std::cout << "Spawn at " << glm::to_string(pos) << std::endl;
        spawn_object(object_type_lookup[cur_row[i]], pos, glm::quat(0,0,0,1), scene, mesh_buffer, pipeline, vao);
        pos.x += spacing;
    }
}

void LevelGenerator::spawn_object(ObjectType type, glm::vec3 position, glm::quat rotation, Scene* scene, MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao) {
    scene->transforms.emplace_back();
	Scene::Transform* t = &scene->transforms.back();
    Mesh const &mesh = mesh_buffer->lookup(xf_name_lookup[type]);
	t->position = position;
	t->rotation = rotation;
	scene->drawables.emplace_back(t);
	Scene::Drawable &drawable = scene->drawables.back();
	drawable.pipeline = pipeline;
	drawable.pipeline.vao = vao;
	drawable.pipeline.type = mesh.type;
	drawable.pipeline.start = mesh.start;
	drawable.pipeline.count = mesh.count;

    GameObject* new_gameobject;
    switch (type) {
        case ObjectType::MovingWall: {
            MovingWall obj;
            new_gameobject = &obj;
        }
        break;
        case ObjectType::MovingWallDamageable: {
            MovingWall obj;
            new_gameobject = &obj;
        }
        break;
        case ObjectType::Bullet: {
            MovingWall obj;
            new_gameobject = &obj;
        }
        break;
        case ObjectType::JumpAbility: {
            JumpAbility obj;
            new_gameobject = &obj;
        }
        break;
        default: 
            throw std::runtime_error("LevelGenerator::spawn_object: Unknown ObjectType");
    }

    new_gameobject->bind_drawable(&drawable, bounds_map[xf_name_lookup[type]]);
    level_objects.emplace_back(*new_gameobject);
}

void LevelGenerator::update(float elapsed) {
    for (auto& obj : level_objects) {
        obj.update(elapsed);
    }
}

void LevelGenerator::load() {
    object_type_lookup[0]= ObjectType::MovingWallDamageable;
    object_type_lookup[1]= ObjectType::MovingWall;
    object_type_lookup[2]= ObjectType::Bullet;
    object_type_lookup[3]= ObjectType::JumpAbility;
    xf_name_lookup[ObjectType::MovingWallDamageable] = "Wall_Damageable";
    xf_name_lookup[ObjectType::MovingWall] = "Wall";
    xf_name_lookup[ObjectType::Bullet] = "Bullet";
    xf_name_lookup[ObjectType::JumpAbility] = "JumpAbility";
    level_maps[0] = {{1,1,1},{1,0,1}};
}