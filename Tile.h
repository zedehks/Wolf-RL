#include <list>
#ifndef TILE_H
#define TILE_H
#include<SDL.h>
#include<SDL_image.h>
#include <iostream>
#include <list>
#define BWALL 0

using namespace std;

class Tile
{
    public:

        SDL_Renderer* renderer;
        //list<Tile*>*tiles;
        int tipo;
        bool isBlocking;
        bool isVisible;
        SDL_Rect rect;
        SDL_Texture* texture;

        Tile(SDL_Renderer* renderer, int x, int y, int tipo);
        void dibujar(SDL_Rect source);
        int use();

        virtual ~Tile();
    protected:
    private:
};

#endif // TILE_H
