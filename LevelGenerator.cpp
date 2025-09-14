#include "LevelGenerator.hpp"
#include "GameObjects/MovingWall.hpp"
#include "GameObjects/JumpAbility.hpp"

// from https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>

// I am using shared_ptr to manage object, I don't need to delete all the object in the container manually.
LevelGenerator::~LevelGenerator() { }

void LevelGenerator::spawn_object_row(MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao) {
    std::vector<int> cur_row = level_maps[cur_level_index][cur_row_index];
    int n = (int)cur_row.size();
    glm::vec3 pos = init_pos;
    for (int i = 0; i < n; i++) {
        spawn_object(object_type_lookup[cur_row[i]], pos, glm::quat(0,0,0,1), mesh_buffer, pipeline, vao);
        pos.x += spacing;
    }
}

void LevelGenerator::spawn_object(ObjectType type, glm::vec3 position, glm::quat rotation, MeshBuffer const * mesh_buffer, Scene::Drawable::Pipeline pipeline, GLuint vao) {
    scene_ptr->transforms.emplace_back();
	Scene::Transform* t = &scene_ptr->transforms.back();
    t->name = std::to_string(cur_object_id++);
    Mesh const &mesh = mesh_buffer->lookup(xf_name_lookup[type]);
	t->position = position;
	t->rotation = rotation;
	scene_ptr->drawables.emplace_back(t);
	Scene::Drawable &drawable = scene_ptr->drawables.back();
	drawable.pipeline = pipeline;
	drawable.pipeline.vao = vao;
	drawable.pipeline.type = mesh.type;
	drawable.pipeline.start = mesh.start;
	drawable.pipeline.count = mesh.count;
    Scene::Bounds bound = scene_ptr->bounds_map[xf_name_lookup[type]];
    std::shared_ptr< GameObject > new_obj_ptr;
    switch (type) {
        case ObjectType::MovingWall: {
            new_obj_ptr = std::make_shared< MovingWall >();
        }
        break;
        case ObjectType::MovingWallDamageable: {
            new_obj_ptr = std::make_shared< MovingWall >();
        }
        break;
        case ObjectType::Bullet: {
            new_obj_ptr = std::make_shared< MovingWall >();
        }
        break;
        case ObjectType::JumpAbility: {
            new_obj_ptr = std::make_shared< JumpAbility >();
        }
        break;
        default: 
            throw std::runtime_error("LevelGenerator::spawn_object: Unknown ObjectType");
    }
    new_obj_ptr->init();
    new_obj_ptr->bind_drawable(&drawable, bound);
    level_objects.emplace_back(new_obj_ptr);
}

// reference: https://stackoverflow.com/questions/42545826/deleting-objects-within-a-list-with-shared-ptr
void LevelGenerator::clean_up_destroyed_object() {
    auto iterator = level_objects.begin();
    while (iterator != level_objects.end()) {
        if ((*iterator)->marked_destroy) {
            // std::cout << "clean object! id = " << (*iterator)->transform->name << std::endl;
            scene_ptr->drawables.remove(*((*iterator)->drawable));
            scene_ptr->transforms.remove(*((*iterator)->transform));
            level_objects.erase(iterator++);
        }
        else {
            iterator++;
        }
    }
}

void LevelGenerator::update(float elapsed) {
    for (auto obj : level_objects) {
        obj->update(elapsed);
    }

    clean_up_destroyed_object();
}

void LevelGenerator::load(Scene* scene) {
    this->scene_ptr = scene;
    object_type_lookup[0]= ObjectType::MovingWallDamageable;
    object_type_lookup[1]= ObjectType::MovingWall;
    object_type_lookup[2]= ObjectType::Bullet;
    object_type_lookup[3]= ObjectType::JumpAbility;
    xf_name_lookup[ObjectType::MovingWallDamageable] = "Wall_Damageable";
    xf_name_lookup[ObjectType::MovingWall] = "Wall";
    xf_name_lookup[ObjectType::Bullet] = "Bullet";
    xf_name_lookup[ObjectType::JumpAbility] = "JumpAbility";
    level_maps[0] = {{1,0,1},{1,0,1}};
}