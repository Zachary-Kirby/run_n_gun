#pragma once

#include "rect.hpp"
#include "entitymanager.hpp"
#include "animations.hpp"
#include "sound.hpp"
#include "sprite.hpp"
#include "glm/vec2.hpp"

//playerPush is added to (not overwritten) with the velocity change the player should get this frame.
void handleWimdyBirdAI(EntityManager& em, Animations& spriteAnimations, fRect playerHitbox, float time, float delta,
  glm::vec2& playerPush, Sound& flapSound, Sound& hurtSound, Sound& deathSound);

int spawnWimdyBird(EntityManager& em, Sprite sprite, float x, float y);
