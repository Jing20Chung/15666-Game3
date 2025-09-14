#pragma once

#include "GameObject.hpp"
// Reference: https://stackoverflow.com/questions/9029548/abstract-base-struct-in-c
struct Item: GameObject {
    Item() {}
    ~Item() {}
    virtual void OnGet() = 0; // force child to implement this function.
};