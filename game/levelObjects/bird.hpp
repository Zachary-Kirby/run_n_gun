#pragma once

#include "rect.hpp"
#include "entitymanager.hpp"
#include "animations.hpp"
#include "sound.hpp"

void handleBirdAI(EntityManager& em, Animations& spriteAnimations, fRect playerHitbox, float time, float delta, Sound& deathSound);

int spawnBird(EntityManager& em, float x, float y);