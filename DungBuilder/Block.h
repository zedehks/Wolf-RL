#ifndef BLOCK_H
#define BLOCK_H
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Block
{
    public:
        int x,y;
        SDL_Renderer* renderer;
        SDL_Rect rect_block;
        SDL_Texture* texture;
        int tipo;
        Block(SDL_Renderer* renderer, int x, int y, int texture);
        bool check_selected(int x,int y);
        void change_type(int tipo);
        virtual ~Block();
    protected:
    private:
};

#endif // BLOCK_H
