#pragma once

#include "entitymanager.hpp"
#include "rect.hpp"
#include "sound.hpp"

void contactDamageSystem(EntityManager& em, fRect& playerHitbox, int& playerDamage, Sound& dingSound);