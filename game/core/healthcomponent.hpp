#pragma once

#include "damagetypes.hpp"

enum struct VulnerableDirections {
  LEFT  = 0x1,
  RIGHT = 0x2,
  UP    = 0x4,
  DOWN  = 0x8,
  ALL   = 0xf,
};

struct Health {
  int health;
  int pendingDamage = 0;
  DamageType recievableDamageType = DamageType::Player;
  VulnerableDirections vulnerableDirection = VulnerableDirections::ALL;
  int preHealth = 0;
};