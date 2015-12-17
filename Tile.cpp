#include "Tile.h"

Tile::Tile(SDL_Renderer* renderer,int x, int y, int tipo)
{
    this->renderer = renderer;
    this->tipo = tipo;
    isBlocking = true;
    isVisible = false;
    switch(tipo)
    {
        case 0:
            this->texture = IMG_LoadTexture(renderer, "BWALL.png");
            break;
        case 1:
            this->texture = IMG_LoadTexture(renderer, "MWALL1.png");
            break;
        case 2:
            this->texture = IMG_LoadTexture(renderer, "FLOOR.png");
            isBlocking = false;
            break;
        case 3:
            this->texture = IMG_LoadTexture(renderer,"VDOOR.png");
            break;
        case 4:
            this->texture = IMG_LoadTexture(renderer,"HDOOR.png");
            break;
        case 5:
            this->texture = IMG_LoadTexture(renderer,"FLOOR.png");
            isBlocking = false;
            break;
        case 6:
            this->texture = IMG_LoadTexture(renderer,"FLOOR.png");
            isBlocking = false;
            break;
        case 7:
            this->texture = IMG_LoadTexture(renderer,"EDOOR.png");
            break;
        case 8:
            this->texture = IMG_LoadTexture(renderer,"EXIT.png");
            break;
        case 9:
            this->texture = IMG_LoadTexture(renderer,"FLOOR.png");
            isBlocking = false;
            break;
    }

    rect.x = x;
    rect.y = y;

    SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
}

Tile::~Tile()
{
    //dtor
}

void Tile::dibujar(SDL_Rect source)
{
    SDL_RenderCopy(renderer,texture,NULL ,&rect );
}

int Tile::use()
{
    switch(this->tipo)
    {
    case 3:
        isBlocking = false;
        tipo = 30;
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;

        this->texture = IMG_LoadTexture(renderer,"VDOORO.png");
        SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
        break;

    case 30:
        isBlocking = true;
        tipo = 3;
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;

        this->texture = IMG_LoadTexture(renderer,"VDOOR.png");
        SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
        break;

    case 4:
        isBlocking = false;
        tipo = 40;
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;

        this->texture = IMG_LoadTexture(renderer,"HDOORO.png");
        SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
        break;

    case 40:
        isBlocking = true;
        tipo = 4;
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;

        this->texture = IMG_LoadTexture(renderer,"HDOOR.png");
        SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
        break;

    case 7:
        isBlocking = false;
        tipo = 70;
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;

        this->texture = IMG_LoadTexture(renderer,"EDOORO.png");
        SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
        break;

    case 70:
        isBlocking = true;
        tipo = 7;
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;

        this->texture = IMG_LoadTexture(renderer,"EDOOR.png");
        SDL_QueryTexture(this->texture,NULL, NULL,&rect.w,&rect.h);
        break;

    case 8:
        return 1;
    }
    return 0;
}
