#include "Jugador.h"
#include "NPC.h"
#include"Math.h"
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Jugador::Jugador(list<Entidad*>* entidades,SDL_Renderer* renderer,int x, int y, int hp, int ammo,int weapon, bool hasSMG)
{

    tipo = "Jugador";
    this->hp = hp;
    this->renderer = renderer;
    this->ammo = ammo;
    this->selected_weapon = weapon;
    this->hasSMG = hasSMG;

    this->texture = IMG_LoadTexture(renderer, "BJ_STAND.png");


    SDL_QueryTexture(this->texture, NULL, NULL, &rect.w, &rect.h);
    this->x = x;
    this->y = y;
    //x = 0; y = 0;
    rect.x = this->x; rect.y = this->y;

    animation_velocity=20;

    current_texture=0;

    state="down";

    //this->entidades = entidades;
}

Jugador::~Jugador()
{
    //dtor
}
void Jugador :: logica()
{

}

int Jugador :: logic(Tile *tiles[][32],list<NPC*>&npcs)
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    if(currentKeyStates[SDL_SCANCODE_D])
    {
        for(list<NPC*>::iterator e = npcs.begin(); e!=npcs.end(); e++)
        {
            if((*e)->x== this->rect.x+32 && (*e)->y== this->rect.y && !(*e)->isDead)
                return 0;
        }
        for(int i=0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                if(tiles[i][j]->rect.x == this->x+32 && tiles[i][j]->rect.y == this->y)
                    if(tiles[i][j]->isBlocking)
                         return 0;

            }
        }
        x+=32;
        return 1;
    }
    if(currentKeyStates[SDL_SCANCODE_A])
    {
        for(list<NPC*>::iterator e = npcs.begin(); e!=npcs.end(); e++)
        {
            if((*e)->x== this->rect.x-32 && (*e)->y== this->rect.y&& !(*e)->isDead)
                return 0;
        }
        for(int i=0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                if(tiles[i][j]->rect.x == this->x-32 && tiles[i][j]->rect.y == this->y)
                    if(tiles[i][j]->isBlocking)
                         return 0;
            }
        }
        x-=32;
        return 3;
    }
    if(currentKeyStates[SDL_SCANCODE_S])
    {
        for(list<NPC*>::iterator e = npcs.begin(); e!=npcs.end(); e++)
        {
            if((*e)->x== this->rect.x && (*e)->y== this->rect.y+32&& !(*e)->isDead)
                return 0;
        }
        for(int i=0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                if(tiles[i][j]->rect.x == this->x && tiles[i][j]->rect.y == this->y+32)
                    if(tiles[i][j]->isBlocking)
                         return 0;
            }
        }
        y+=32;
        return 4;

    }
    if(currentKeyStates[SDL_SCANCODE_W])
    {
        for(list<NPC*>::iterator e = npcs.begin(); e!=npcs.end(); e++)
        {
            if((*e)->x== this->rect.x && (*e)->y== this->rect.y-32&& !(*e)->isDead)
                return 0;
        }
        for(int i=0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                if(tiles[i][j]->rect.x == this->x && tiles[i][j]->rect.y == this->y-32)
                    if(tiles[i][j]->isBlocking)
                         return 0;
            }
        }
        y-=32;
        return 2;
    }
    if(currentKeyStates[SDL_SCANCODE_SPACE])
    {
        for(int i=0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                {
                    int k = 0;
                    if(tiles[i][j]->rect.x == this->x && tiles[i][j]->rect.y == this->y-32)
                       k = tiles[i][j]->use();
                    if(tiles[i][j]->rect.x == this->x && tiles[i][j]->rect.y == this->y+32)
                       k = tiles[i][j]->use();
                    if(tiles[i][j]->rect.x == this->x+32 && tiles[i][j]->rect.y == this->y)
                       k = tiles[i][j]->use();
                    if(tiles[i][j]->rect.x == this->x-32 && tiles[i][j]->rect.y == this->y)
                       k = tiles[i][j]->use();
                    if(k == 1)
                        return 99;
                }
            }
        }
    }
    if(currentKeyStates[SDL_SCANCODE_K])
    {
        this->getHit(10);
        return 69;
    }
    if(currentKeyStates[SDL_SCANCODE_N])
    {
        return 99;
    }
    if(currentKeyStates[SDL_SCANCODE_I])
    {
        this->ammo = 99;
        return 69;
    }
    if(currentKeyStates[SDL_SCANCODE_Q])
    {
        if(this->selected_weapon == 2)
            selected_weapon = 0;
        else
            this->selected_weapon++;
    }
    bool couldHit = false;
    if(currentKeyStates[SDL_SCANCODE_RIGHT])
        couldHit = attack(1,&npcs,tiles);
    if(currentKeyStates[SDL_SCANCODE_UP])
        couldHit =attack(2,&npcs,tiles);
    if(currentKeyStates[SDL_SCANCODE_LEFT])
        couldHit =attack(3,&npcs,tiles);
    if(currentKeyStates[SDL_SCANCODE_DOWN])
        couldHit =attack(4,&npcs,tiles);

    if(couldHit)
        return 1488;
    else
        return -5;


    return 0;
}

bool Jugador::tilePos(Tile *tiles[][32], int i,int j)
{
    if(tiles[i][j] != NULL)
        return (tiles[i][j]->rect.x == this->x &&tiles[i][j]->rect.y == this->y);
    return false;
}

void Jugador::discover(Tile *tiles[][32],int i, int j,list<NPC*>*npcs)
{
    if(i>31 || j > 31 || i < 0 || j < 0)
        return;
    if(tiles[i][j] != NULL)
    {
        if(!tiles[i][j]->isVisible||tiles[i][j]->tipo == 30||tiles[i][j]->tipo == 40||tiles[i][j]->tipo == 70)
        {
            tiles[i][j]->isVisible = true;
            for(list<NPC*>::iterator e = npcs->begin(); e!=npcs->end(); e++)
            {
               if((*e)->x== tiles[i][j]->rect.x &&(*e)->y== tiles[i][j]->rect.y)
                    (*e)->isVisible = true;
            }


            if(!tiles[i][j]->isBlocking)
            {
                discover(tiles,i+1,j,npcs);
                discover(tiles,i-1,j,npcs);
                discover(tiles,i,j+1,npcs);
                discover(tiles,i,j-1,npcs);
                discover(tiles,i+1,j+1,npcs);
                discover(tiles,i-1,j-1,npcs);
                discover(tiles,i+1,j-1,npcs);
                discover(tiles,i-1,j+1,npcs);
            }
        }
    }
}

void Jugador ::alert(Tile *tiles[][32],int i, int j,list<NPC*>*npcs)
{
    for(int i = 0;i<32;i++)
    {
        for(int j = 0;j<32;j++)
        {
          if(tiles[i][j] != NULL)
            {
                if(tiles[i][j]->isVisible)
                {
                    for(list<NPC*>::iterator e = npcs->begin(); e!=npcs->end(); e++)
                    {
                       if((*e)->x== tiles[i][j]->rect.x &&(*e)->y== tiles[i][j]->rect.y)
                            (*e)->wake();

                    }
                }
            }
        }
    }

}
void Jugador ::render(int camX, int camY)
{
    rect.x = x;
    rect.y = y;

    SDL_RenderCopyEx(renderer, this->texture, NULL, &rect, 0.0, NULL, SDL_FLIP_NONE);
}

bool Jugador::attack(int direction,list<NPC*>*NPCs, Tile *tiles[][32])
{
    int fLength = 0;
    int currentTime = 0;
    int framecount = 0;
    bool animation = true;
    switch(selected_weapon)
    {
    case 0:

        while(animation)
        {
            switch(framecount)
            {
            case 0:
                this->texture = IMG_LoadTexture(renderer,"BJ_K0.png");
                break;
            case 2:
                this->texture = IMG_LoadTexture(renderer,"BJ_K1.png");
                break;
            case 4:
                this->texture = IMG_LoadTexture(renderer,"BJ_K2.png");
                break;
            case 18:
                this->texture = IMG_LoadTexture(renderer,"BJ_STAND1.png");
                break;
            case 30:
                this->texture = IMG_LoadTexture(renderer,"BJ_STAND.png");
                break;
            }

            SDL_QueryTexture(this->texture, NULL, NULL, &rect.w, &rect.h);

            this->render(1,1);

            SDL_RenderPresent(renderer);
            fLength = SDL_GetTicks();
            if( (fLength - currentTime) < SCREEN_TICKS_PER_FRAME )
            {
                SDL_Delay(SCREEN_TICKS_PER_FRAME - (fLength - currentTime));
            }
            if(framecount == 30)
                animation = false;
            else
                framecount++;
        }
        switch(direction)
        {
        case 1:
            for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
            {
                if((*e)->x== this->rect.x+32 && (*e)->y== this->rect.y)
                {
                    (*e)->getHit(15,this);
                    return true;
                }
            }
            break;
        case 2:
            for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
            {
                if((*e)->x== this->rect.x && (*e)->y+32== this->rect.y)
                {
                    (*e)->getHit(15,this);
                    return true;
                }
            }
            break;
        case 3:
            for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
            {
                if((*e)->x== this->rect.x-32 && (*e)->y== this->rect.y)
                {
                    (*e)->getHit(15,this);
                    return true;
                }
            }
            break;
        case 4:
            for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
            {
                if((*e)->x== this->rect.x && (*e)->y-32== this->rect.y)
                {
                    (*e)->getHit(15,this);
                    return true;
                }
            }
            break;
        }
        break;
    case 1:
        alert(tiles,0,0,NPCs);
        if(this->ammo > 0)
            this->ammo--;
        else
            break;
        while(animation)
        {
            switch(framecount)
            {
            case 0:
                this->texture = IMG_LoadTexture(renderer,"BJ_P0.png");
                break;
            case 2:
                this->texture = IMG_LoadTexture(renderer,"BJ_P1.png");
                break;
            case 4:
                this->texture = IMG_LoadTexture(renderer,"BJ_P2.png");
                break;
            case 10:
                this->texture = IMG_LoadTexture(renderer,"BJ_P0.png");
                break;
            case 18:
                this->texture = IMG_LoadTexture(renderer,"BJ_STAND1.png");
                break;
            case 30:
                this->texture = IMG_LoadTexture(renderer,"BJ_STAND.png");
                break;
            }

            SDL_QueryTexture(this->texture, NULL, NULL, &rect.w, &rect.h);

            this->render(1,1);

            SDL_RenderPresent(renderer);
            fLength = SDL_GetTicks();
            if( (fLength - currentTime) < SCREEN_TICKS_PER_FRAME )
            {
                SDL_Delay(SCREEN_TICKS_PER_FRAME - (fLength - currentTime));
            }
            if(framecount == 30)
                animation = false;
            else
                framecount++;
        }
        switch(direction)
        {
        case 1:
            {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                    if((*e)->x> this->rect.x && (*e)->y== this->rect.y  && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x > this->rect.x && tiles[i][j]->rect.x < (*e)->rect.x)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                        (*e)->getHit(rand()%30+15,this);
                        return true;
                    }
                }
                }
            break;
        case 2:
            {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                   if((*e)->x== this->rect.x && (*e)->y< this->rect.y && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y < this->rect.y && tiles[i][j]->rect.y > (*e)->rect.y)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                        (*e)->getHit(rand()%30+15,this);
                        return true;
                    }
                }
                }
            break;
        case 3:
             {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                    if((*e)->x< this->rect.x && (*e)->y== this->rect.y  && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x < this->rect.x && tiles[i][j]->rect.x > (*e)->rect.x)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                        (*e)->getHit(rand()%30+15,this);
                        return true;
                    }
                }
                }
            break;
        case 4:
            {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                   if((*e)->x== this->rect.x && (*e)->y> this->rect.y && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y > this->rect.y && tiles[i][j]->rect.y < (*e)->rect.y)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                        (*e)->getHit(rand()%30+15,this);
                        return true;
                    }
                }
                }
            break;
        }
        break;
    case 2:
        alert(tiles,0,0,NPCs);
        if(this->ammo > 1)
            this->ammo-=2;
        else
            break;

        while(animation)
        {
            switch(framecount)
            {
            case 0:
                this->texture = IMG_LoadTexture(renderer,"BJ_S0.png");
                break;
            case 2:
                this->texture = IMG_LoadTexture(renderer,"BJ_S1.png");
                break;
            case 4:
                this->texture = IMG_LoadTexture(renderer,"BJ_S2.png");
                break;
            case 6:
                this->texture = IMG_LoadTexture(renderer,"BJ_S0.png");
                break;
            case 8:
                this->texture = IMG_LoadTexture(renderer,"BJ_S2.png");
                break;
            case 12:
                this->texture = IMG_LoadTexture(renderer,"BJ_S2.png");
                break;
            case 18:
                this->texture = IMG_LoadTexture(renderer,"BJ_STAND1.png");
                break;
            case 30:
                this->texture = IMG_LoadTexture(renderer,"BJ_STAND.png");
                break;
            }


            SDL_QueryTexture(this->texture, NULL, NULL, &rect.w, &rect.h);

            this->render(1,1);

            SDL_RenderPresent(renderer);
            fLength = SDL_GetTicks();
            if( (fLength - currentTime) < SCREEN_TICKS_PER_FRAME )
            {
                SDL_Delay(SCREEN_TICKS_PER_FRAME - (fLength - currentTime));
            }
            if(framecount == 30)
                animation = false;
            else
                framecount++;
        }
        switch(direction)
        {
        case 1:
            {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                    if((*e)->x> this->rect.x && (*e)->y== this->rect.y  && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x > this->rect.x && tiles[i][j]->rect.x < (*e)->rect.x)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                         (*e)->getHit(rand()%55+15,this);
                        return true;
                    }
                }
                }
            break;
        case 2:
            {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                   if((*e)->x== this->rect.x && (*e)->y< this->rect.y && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y < this->rect.y && tiles[i][j]->rect.y > (*e)->rect.y)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                         (*e)->getHit(rand()%55+15,this);
                        return true;
                    }
                }
                }
            break;
        case 3:
             {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                    if((*e)->x< this->rect.x && (*e)->y== this->rect.y  && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x < this->rect.x && tiles[i][j]->rect.x > (*e)->rect.x)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                         (*e)->getHit(rand()%55+15,this);
                        return true;
                    }
                }
                }
            break;
        case 4:
            {
                bool loopList = false;
                for(list<NPC*>::iterator e = NPCs->begin(); e!=NPCs->end(); e++)
                {
                   if((*e)->x== this->rect.x && (*e)->y> this->rect.y && !(*e)->isDead)
                    {
                        for(int i=0;i<32;i++)
                        {
                            for(int j = 0;j<32;j++)
                            {
                                if(tiles[i][j] != NULL)
                                {
                                   if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y > this->rect.y && tiles[i][j]->rect.y < (*e)->rect.y)
                                    if(tiles[i][j]->isBlocking)
                                        loopList = true;
                                }
                                if(loopList)
                                    break;
                            }
                            if(loopList)
                                    break;
                        }
                        if(loopList)
                                    break;
                        (*e)->getHit(rand()%55+15,this);
                        return true;
                    }
                }
                }
            break;
        }
        break;
    }
    //printf("\nSWING AND A MISS");
    return false;
}

void Jugador:: getHit(int damage)
{
//    if(this->isDead)
        //return;
    SDL_Texture *blood;
    SDL_Rect rect_blood;
    rect_blood.x = this->x+8;rect_blood.y = this->y+8;
    rect_blood.w = 32;rect_blood.h = 32;

    int fLength = 0;
    int currentTime = 0;
    int framecount = 0;
    bool animation = true;
    while(animation)
    {
        switch(framecount)
        {
        case 0:
            blood = IMG_LoadTexture(renderer,"blood1.png");
            break;
        case 1:
            blood = IMG_LoadTexture(renderer,"blood2.png");
            break;
        case 2:
            blood = IMG_LoadTexture(renderer,"blood3.png");
            break;
        case 3:
            blood = IMG_LoadTexture(renderer,"blood4.png");
            break;
        }
        int w=32;
        SDL_QueryTexture(blood,NULL,NULL,&w,&w);
        SDL_RenderCopy(this->renderer,blood,NULL,&rect_blood);

        SDL_RenderPresent(renderer);

            fLength = SDL_GetTicks();
            if( (fLength - currentTime) < SCREEN_TICKS_PER_FRAME )
            {
                SDL_Delay(SCREEN_TICKS_PER_FRAME - (fLength - currentTime));
            }

         if(framecount == 9)
                animation = false;
            else
                framecount++;
    }
    SDL_DestroyTexture(blood);

    this->hp -=damage;
//    this->wasHit = true;

    //if(this->hp <= 0)
        //this->isDead  = true;
}
