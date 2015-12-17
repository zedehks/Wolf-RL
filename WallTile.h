#ifndef WALLTILE_H
#define WALLTILE_H

#include "Tile.h"


class WallTile : public Tile
{
    public:
        WallTile(list<Tile*>* tiles,SDL_Renderer* renderer);
        virtual ~WallTile();
    protected:
    private:
};

#endif // WALLTILE_H
