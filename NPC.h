#ifndef NPC_H
#define NPC_H


#include "Jugador.h"
#include <stdlib.h>
#include <list>
#include "Personajes.h"
#include "NPC.h"

class Personajes;

class NPC : public Personajes
{
    public:
        int hp;
        bool isAwake;
        bool isVisible;
        bool wasHit;
        bool isDead;
        int wait;
        char tipo;
        double x, y;
        SDL_Texture* texture;
        NPC(SDL_Renderer* renderer,char tipo,int x,int y);
        void render(int camX, int camY);
        void getHit(int damage,Jugador*jg);
        void wake();
        virtual ~NPC();
        void logica();
        void logic(Tile *tile[][32],int jx, int jy,Jugador*jg);
    protected:
    private:
};

#endif // NPC_H
