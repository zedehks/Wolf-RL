#ifndef PERSONAJES_H
#define PERSONAJES_H

#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<vector>
#include<map>

#include "Entidad.h"
//#include "NPC.h"

//using namespace std;

class Personajes : public Entidad
{
    public:
        Personajes();
        SDL_Renderer* renderer;
        map<string,vector<SDL_Texture*> >textures;
        string state;
        SDL_Rect rect;
        int current_texture;
        int frames;
        bool isVisible;
        double x, y;
        double velocity;
        double mousepositionx;
        double mousepositiony;
        double delta_x;
        double delta_y;
        double angle;
        int animation_velocity;
        virtual ~Personajes();
        virtual void dibujar();
    protected:
    private:
};

#endif // PERSONAJES_H
