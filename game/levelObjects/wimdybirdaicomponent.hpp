#pragma once

struct WimdyBirdAI {
  bool facingRight = false;
  bool flapping = false;
  float flapTimer = 0.0f;
  int lastHealth = 0; //used to notice when the bird got hurt
};
