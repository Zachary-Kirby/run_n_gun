#pragma once

#include "damagetypes.hpp"

struct Health {
  int health;
  int pendingDamage = 0;
  DamageType recievableDamageType = DamageType::Player;
};