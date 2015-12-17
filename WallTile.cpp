#include "WallTile.h"
#include "Tile.h"


WallTile::WallTile(list<Tile*>* tiles,SDL_Renderer* renderer)
{
    tipo = "walltile";
    this->renderer = renderer;
    SDL_Texture* texture= IMG_LoadTexture(renderer, "BWALL1.png");
    SDL_Rect rect;
    SDL_QueryTexture(texture,NULL, NULL, &rect.w, &rect.h);
}

WallTile::~WallTile()
{
    //dtor
}


