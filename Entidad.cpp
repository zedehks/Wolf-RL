#include "Entidad.h"

Entidad::Entidad()
{
    tipo = "Entidad";
    delete_flag=false;
}

Entidad::~Entidad()
{
    //dtor
}

bool Entidad::colision(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x+r1.w<r2.x)//Muy a la izquierda
        return false;

    if(r1.x>r2.x+r2.w)//Muy a la derecha
        return false;

    if(r1.y+r1.h<r2.y)//Muy arriba
        return false;

    if(r1.y>r2.y+r2.h)//Muy abajo
        return false;

    return true;
}
