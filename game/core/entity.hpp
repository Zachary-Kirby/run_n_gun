
#pragma once

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

class Entity {
public:
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer, glm::vec2 camera) = 0;
    virtual ~Entity() = default;
};