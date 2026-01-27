
#include "constants.hpp"
#include "SDL2/SDL.h"

void renderCircle(SDL_Renderer *renderer, const SDL_FPoint &center, float radius)
{
  SDL_FPoint transformedCirclePoints[17];
  memcpy(transformedCirclePoints, circlePoints, sizeof(circlePoints));
  transformPoints(transformedCirclePoints, sizeof(circlePoints)/sizeof(SDL_FPoint), radius, 
    center.x, 
    center.y);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 32);
  SDL_RenderDrawLinesF(renderer, transformedCirclePoints, sizeof(circlePoints)/sizeof(SDL_FPoint));
}
