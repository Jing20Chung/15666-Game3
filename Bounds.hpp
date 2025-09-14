#pragma once 

#include <glm/glm.hpp>


struct Bounds {
    Bounds(){}
    Bounds(glm::vec3 max_, glm::vec3 min_): max(max_), min(min_){}
    glm::vec3 max;
    glm::vec3 min;

    Bounds& operator=(const Bounds& other) {
        max = other.max;
        min = other.min;
        return *this;
    }
};