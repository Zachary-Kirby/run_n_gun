#pragma once
#include "glm/vec2.hpp"
#include "entitymanager.hpp"

class Renderer;

void renderSprites(EntityManager& em, Renderer *renderer, int gameplayDrawWidth, glm::vec2 camera, float time);