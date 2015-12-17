#ifndef ENTIDAD_H
#define ENTIDAD_H

#include<SDL.h>
#include<SDL_image.h>
#include <iostream>
#include <list>

using namespace std;

class Entidad
{
    public:
        Entidad();
        virtual ~Entidad();
        string tipo;
        bool delete_flag;
        list<Entidad*>*entidades;
        bool colision(SDL_Rect r1, SDL_Rect r2);
        virtual void dibujar()=0;
        virtual void logica()=0;
    protected:
    private:
};

#endif // ENTIDAD_H
