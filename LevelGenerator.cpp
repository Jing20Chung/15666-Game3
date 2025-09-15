#include "LevelGenerator.hpp"
#include "GameObjects/MovingFloor.hpp"
#include "GameObjects/MovingFloorDamageable.hpp"
#include "GameObjects/JumpAbility.hpp"
#include "GameObjects/Player.hpp"
#include "GameObjects/Bullet.hpp"

// from https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "read_write_chunk.hpp"
#include <iostream>
#include <fstream>

LevelGenerator::LevelGenerator () { }

// I am using shared_ptr to manage object, I don't need to delete all the object in the container manually.
LevelGenerator::~LevelGenerator() { }

void LevelGenerator::spawn_object_row(int level_index, int row_index) {
    std::vector<int> cur_row = level_maps[level_index][row_index];
    int n = (int)cur_row.size();
    int half_n = n / 2;
    glm::vec3 pos = glm::vec3(-1 * ((half_n) * C_SIZE_PER_BLOCK + (half_n) * C_SPACING), C_START_Y, 0);
    for (int i = 0; i < n; i++) {
        if (cur_row[i] != C_EMPTY) {
            object_container_ptr->emplace_back(spawn_object(object_type_lookup[cur_row[i]], pos, glm::quat(0,0,0,1), mesh_buffer, pipeline, vao));
        }
        pos.x += C_POS_OFFSET;
    }
}

std::shared_ptr< GameObject > LevelGenerator::spawn_object(ObjectType type, glm::vec3 position, glm::quat rotation, MeshBuffer const * mesh_buffer_, Scene::Drawable::Pipeline pipeline_, GLuint vao_) {
    // Create a new transform entry
    scene_ptr->transforms.emplace_back();
	Scene::Transform* t = &scene_ptr->transforms.back();

    // Use transform's name as instance id.
    t->name = xf_name_lookup[type] + "_" + std::to_string(cur_object_id++);
    t->position = position;
	t->rotation = rotation;

    // Lookup the mesh
    Mesh const &mesh = mesh_buffer_->lookup(xf_name_lookup[type]);
	
    // Add new drawable
	scene_ptr->drawables.emplace_back(t);
	Scene::Drawable &drawable = scene_ptr->drawables.back();
	drawable.pipeline = pipeline_;
	drawable.pipeline.vao = vao_;
	drawable.pipeline.type = mesh.type;
	drawable.pipeline.start = mesh.start;
	drawable.pipeline.count = mesh.count;

    // Use transform name (from scene file) to lookup the bound
    Bounds bound = scene_ptr->bounds_map[xf_name_lookup[type]];

    // Create object according to ObjectType
    std::shared_ptr< GameObject > new_obj_ptr;
    switch (type) {
        case ObjectType::Player: {
            new_obj_ptr = std::make_shared< Player >();
        }
        break;
        case ObjectType::MovingFloor: {
            new_obj_ptr = std::make_shared< MovingFloor >();
        }
        break;
        case ObjectType::MovingFloorDamageable: {
            new_obj_ptr = std::make_shared< MovingFloorDamageable >();
        }
        break;
        case ObjectType::Bullet: {
            new_obj_ptr = std::make_shared< Bullet >();
        }
        break;
        case ObjectType::JumpAbility: {
            new_obj_ptr = std::make_shared< JumpAbility >();
        }
        break;
        default: 
            throw std::runtime_error("LevelGenerator::spawn_object: Unknown ObjectType");
    }

    // Run init method of the game object
    new_obj_ptr->init();

    // Bind drawable and bound to the object. Bind drawable in order to destroy the object later.
    new_obj_ptr->bind_drawable(&drawable, bound);
    return new_obj_ptr;
}

void LevelGenerator::update(float elapsed) {
    if (isSpawning) {
        cur_accumulated_time += elapsed;

        // If now is the time to spawn next row.
        if (cur_accumulated_time >= next_spawn_time) {
            next_spawn_time = cur_accumulated_time + tempo;
            spawn_object_row(cur_level_index, cur_row_index++);
        }
        
        // If spawn completed (aka the row index is greater than the total number of rows)
        if (cur_row_index >= (int)level_maps[cur_level_index].size()) {
            std::cout << "spawn completed" << std::endl;
            isSpawning = false;
        }
    }
}

void LevelGenerator::start_spawn(int level_index) {
    cur_level_index = level_index;
    cur_row_index = 0;
    cur_accumulated_time = 0;
    next_spawn_time = tempo;
    isSpawning = true;
}

void LevelGenerator::stop_spawn() {
    isSpawning = false;
}

void LevelGenerator::init(Scene* scene_, MeshBuffer const * mesh_buffer_, std::list< std::shared_ptr< GameObject > >* object_container_, Scene::Drawable::Pipeline pipeline_, GLuint vao_) {
    this->scene_ptr = scene_;
    this->mesh_buffer = mesh_buffer_;
    this->pipeline = pipeline_;
    this->vao = vao_;
    this->object_container_ptr = object_container_;
    object_type_lookup[0]= ObjectType::MovingFloorDamageable;
    object_type_lookup[1]= ObjectType::MovingFloor;
    object_type_lookup[2]= ObjectType::Bullet;
    object_type_lookup[3]= ObjectType::JumpAbility;
    xf_name_lookup[ObjectType::Player] = "Player";
    xf_name_lookup[ObjectType::MovingFloorDamageable] = "Floor_Damageable";
    xf_name_lookup[ObjectType::MovingFloor] = "Floor";
    xf_name_lookup[ObjectType::Bullet] = "Bullet";
    xf_name_lookup[ObjectType::JumpAbility] = "JumpAbility";
    // level_maps[0] = {{1,0,-1,1},
    //                  {1,0,2,-1,1},
    //                  {-1},
    //                  {0,1,-1,0,0,1}};
}


void LevelGenerator::load(std::string const &filename) {
	std::ifstream file(filename, std::ios::binary);
	std::vector< int > col_size;
	read_chunk(file, "cols", &col_size);
    // std::cout<<"col size = " << col_size[0] << std::endl;
	std::vector< int > data;
	read_chunk(file, "lvl1", &data);
    
    level_maps.clear();
    std::vector<int> row;
    int cnt = 0;
    for(auto d : data) {
        // std::cout << d << " ";
        row.push_back(d);
        cnt++;
        if (cnt >= col_size[0]) {
            cnt = 0;
            level_maps[cur_level_index].push_back(row);
            row.clear();
            // std::cout << std::endl;
        }
    }
}