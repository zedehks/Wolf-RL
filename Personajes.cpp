#include "Personajes.h"
#include "Math.h"

Personajes::Personajes()
{
    tipo = "Personaje";
    isVisible = false;
}

Personajes::~Personajes()
{
    //dtor
}

void Personajes::dibujar()
{
    rect.x = x;
    rect.y = y;

    SDL_RenderCopyEx(renderer, textures[state][current_texture], NULL, &rect, 0.0, NULL, SDL_FLIP_NONE);
}
