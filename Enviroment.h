#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include "NPC.h"
#include <stdlib.h>
#include <list>

class Enviroment : public NPC
{
    public:
        Enviroment(list<Entidad*>* entidades,SDL_Renderer* renderer);
        virtual ~Enviroment();
        void logica();
    protected:
    private:
};

#endif // ENVIROMENT_H
