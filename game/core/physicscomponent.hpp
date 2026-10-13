#ifndef PHYSICSCOMPONENT
#define PHYSICSCOMPONENT

struct Physics{
  float gravity = 800.0f;
  float friction = 800.0f;
  bool useAirFriction = false;
  int collisionMask = 0b00000;
  bool grounded = false;
};

#endif
