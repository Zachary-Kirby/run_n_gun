
#pragma once

#include <glm/vec2.hpp>

struct Entity {
public:
    unsigned int ID; // NOTE that this is note the slot ID but just something as an identitfier besides the slot
    unsigned int Components;
    glm::vec2 position;
};

