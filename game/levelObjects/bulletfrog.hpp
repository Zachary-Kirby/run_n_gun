#pragma once

#include "entitymanager.hpp"
#include "animations.hpp"
#include "sound.hpp"

void handleFrogAI(EntityManager& em, Animations& animations, float playerX, float delta, Sound& ribbitSound, Sound& frogHurtSound);

int createFrog(EntityManager& em, Animations& animations, float x, float y);