#pragma once

#include "entitymanager.hpp"
#include "animations.hpp"

void handleFrogAI(EntityManager& em, Animations& animations, float playerX, float delta);

int createFrog(EntityManager& em, Animations& animations, float x, float y);