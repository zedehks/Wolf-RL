#ifndef JUGADOR_H
#define JUGADOR_H


#include <list>
#include <fstream>
#include "Personajes.h"
#include "Tile.h"
//#include "NPC.h"

class NPC;

class Jugador : public Personajes
{
    public:
        Jugador();
        Jugador(list<Entidad*>* entidades,SDL_Renderer* renderer,int x, int y, int hp, int ammo,int weapon,bool hasSMG);
        SDL_Texture* texture;
        //int x,y;
        virtual ~Jugador();
        void logica();
        int hp;
        int ammo;
        int selected_weapon;
        bool hasSMG;
        int logic(Tile *tile[][32], list<NPC*>&npcs);

        bool tilePos(Tile *tiles[][32], int i,int j);
        void discover(Tile *tiles[][32],int i, int j,list<NPC*>*npcs);
        bool attack(int direction,list<NPC*>*NPCs);
        void getHit(int damage);
        void rotateTexture(int mposx, int mposy);
        void render(int camX, int camY);
    protected:
    private:
};

#endif // JUGADOR_H
