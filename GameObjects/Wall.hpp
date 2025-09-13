// From my game 2: https://github.com/Jing20Chung/15666-Game2
#pragma once

#include "GameObject.hpp"
#include "../Load.hpp"

struct Wall : GameObject{
    virtual void init() override;
};
