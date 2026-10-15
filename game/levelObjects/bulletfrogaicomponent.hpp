#pragma once

struct BulletFrogAI {
  float range = 200.0f; //const really
  float actionTimer = 0.0f;
  float actionTime = 1.0f; //const really
  float prepareShotTimer = 0.0f;
  bool lookingRight = false;
};