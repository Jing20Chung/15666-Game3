// From my game 2: https://github.com/Jing20Chung/15666-Game2
#pragma once

#include "../GL.hpp"
#include "../Bounds.hpp"
#include "../Scene.hpp"
#include "../Mesh.hpp"
#include "../Load.hpp"

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string.h>


struct GameObject {
    GameObject();
    virtual ~GameObject();

    Scene::Transform* transform; // current binded transform
    Scene::Drawable* drawable; // current binded drawable

    glm::vec3 size;

    glm::vec3 position; // current position
    glm::quat rotation; // current rotation
    
    glm::vec3 velocity = glm::vec3(0, 0, 0); // current velocity

    GameObject* parent = nullptr;

    std::string tag = "";

    bool marked_destroy = false;
    
    virtual void init();
    virtual bool bind_drawable(Scene::Drawable* drawable, Bounds bounds); // // bind drawable and bounds
    virtual void update_input(SDL_Event const &evt); // update input from Mode
    virtual void update(float elapsed); // called by Mode
    virtual void on_collision(GameObject* other); // on collision

    Bounds get_bounds(); // get current bounds

    static bool check_collision(GameObject* obj_a, GameObject* obj_b);
};