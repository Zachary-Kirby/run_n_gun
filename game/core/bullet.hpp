#pragma once

#include "entitymanager.hpp"
#include "damagetypes.hpp"

void handleBullets(EntityManager& em, float delta);

int createBullet(const Sprite sprite, EntityManager& em, float x, float y, float xv, float yv, DamageType damageType, DamageType recieveDamageType, float lifetime = 2.0f);