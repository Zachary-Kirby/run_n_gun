#include "pallete.hpp"

void Pallete::init(SDL_Texture *loadedAtlas, SDL_Texture* loadedCollisionAtlas)
{
  atlas = loadedAtlas;
  collisionAtlas = loadedCollisionAtlas;
  SDL_QueryTexture(atlas, NULL, NULL, &atlasWidth, &atlasHeight);
}

int Pallete::getSelectedTile(int mouseX, int mouseY)
{
  //handles the mouse in window coordinates
  int relX = mouseX - x;
  int relY = mouseY - y;
  int atlasTileWidth = atlasWidth/tile_size;
  int atlasTileHeight = atlasHeight/tile_size;
  int tileX = relX / tile_size / scale;
  int tileY = relY / tile_size / scale;
  int tileID = tileX + tileY * atlasTileWidth;
  return tileID;
}

void Pallete::update()
{
  if (!enabled) return;
  selectedTile = getSelectedTile(mouseX, mouseY);
}

void Pallete::draw(SDL_Renderer *renderer, int selectedLayer)
{
  if (!enabled) return;
  SDL_Rect dstRect {x, y, atlasWidth*scale, atlasHeight*scale};
  SDL_Rect clipRect {x, y, 128, 128};
  int relX = mouseX - x;
  int relY = mouseY - y;
  int scaled_tile_size = tile_size * scale;
  SDL_Rect selectedRect {
    x + (relX/scaled_tile_size)*scaled_tile_size, 
    y + (relY/scaled_tile_size)*scaled_tile_size, 
    scaled_tile_size, 
    scaled_tile_size};
  SDL_RenderSetClipRect(renderer, &clipRect);
  SDL_SetRenderDrawColor(renderer, 32, 32, 32, 32);
  SDL_RenderFillRect(renderer, &dstRect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  if (selectedLayer!=1) SDL_RenderCopy(renderer, atlas, NULL, &dstRect);
  else SDL_RenderCopy(renderer, collisionAtlas, NULL, &dstRect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &selectedRect);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderSetClipRect(renderer, NULL);
  
}
