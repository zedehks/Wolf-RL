#include "Block.h"
#include "SDL.h"
#include "SDL_image.h"

Block::Block(SDL_Renderer* renderer, int x, int y, int texture)
{
    this->renderer = renderer;
    this->x = x; this->y = y;
    this->tipo = texture;


    switch(tipo)
    {
    case 0:
        this->texture = IMG_LoadTexture(renderer, "BWALL1.png");
        break;
    case 1:
        this->texture = IMG_LoadTexture(renderer, "MWALL1.png");
        break;
    case 2:
        this->texture = IMG_LoadTexture(renderer, "FLOOR.png");
        break;
    case 3:
        this->texture = IMG_LoadTexture(renderer,"VDOOR.png");
        break;
    case 4:
        this->texture = IMG_LoadTexture(renderer,"HDOOR.png");
        break;
    case 5:
        this->texture = IMG_LoadTexture(renderer,"PSTART.png");
        break;
    case 6:
        this->texture = IMG_LoadTexture(renderer,"GUARD.png");
        break;
    case 7:
        this->texture = IMG_LoadTexture(renderer,"EDOOR.png");
        break;
    case 8:
        this->texture = IMG_LoadTexture(renderer,"EXIT.png");
        break;
    case 9:
        this->texture = IMG_LoadTexture(renderer,"SS.png");
        break;
    case -1:
        this->texture = IMG_LoadTexture(renderer, "EMPTY.png");
    }

    this->rect_block.x = x; this->rect_block.y = y;
    SDL_QueryTexture(this->texture,NULL,NULL,&rect_block.w,&rect_block.h);
}

Block::~Block()
{
    //dtor
}

bool Block::check_selected(int x, int y)
{
    if(x>= this->rect_block.x && x< this->rect_block.x+16)
    {
       if(y>= this->rect_block.y && y< this->rect_block.y+16)
        {
            return true;
        }
    }
    return false;
}

void Block::change_type(int tipo)
{
    SDL_DestroyTexture(this->texture);
    this->tipo = tipo;

    switch(this->tipo)
    {
    case 0:
        this->texture = IMG_LoadTexture(renderer, "BWALL1.png");
        break;
    case 1:
        this->texture = IMG_LoadTexture(renderer, "MWALL1.png");
        break;
    case 2:
        this->texture = IMG_LoadTexture(renderer, "FLOOR.png");
        break;
    case 3:
        this->texture = IMG_LoadTexture(renderer,"VDOOR.png");
        break;
    case 4:
        this->texture = IMG_LoadTexture(renderer,"HDOOR.png");
        break;
    case 5:
        this->texture = IMG_LoadTexture(renderer,"PSTART.png");
        break;
    case 6:
        this->texture = IMG_LoadTexture(renderer,"GUARD.png");
        break;
    case 7:
        this->texture = IMG_LoadTexture(renderer,"EDOOR.png");
        break;
    case 8:
        this->texture = IMG_LoadTexture(renderer,"EXIT.png");
        break;
    case 9:
        this->texture = IMG_LoadTexture(renderer,"SS.png");
        break;
    case -1:
        this->texture = IMG_LoadTexture(renderer, "EMPTY.png");
    }
    SDL_QueryTexture(this->texture,NULL,NULL,&rect_block.w,&rect_block.h);
}
