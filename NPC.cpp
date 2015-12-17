#include "NPC.h"
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

NPC::NPC(SDL_Renderer* renderer, char tipo,int x,int y)
{
    this->renderer = renderer;
    this->tipo = tipo;
    this->isDead = false;

    switch(tipo)
    {
    case 'g':
        this->hp = 40;
        this->texture = IMG_LoadTexture(renderer, "GUARD.png");
        this->wait = 5;
        break;
    case 's':
        this->hp = 60;
        this->texture = IMG_LoadTexture(renderer, "SS.png");
        this->wait = 2;
        break;
    }
    this->isAwake = false;
    SDL_QueryTexture(this->texture,NULL,NULL,&rect.w,&rect.h);
     this->x = x;
    this->y = y;
    this->isVisible = false;
    //x = 0; y = 0;
    rect.x = this->x; rect.y = this->y;
    //this->entidades = entidades;
}

NPC::~NPC()
{
    //dtor
}

void NPC :: logica()
{

}
void NPC::logic(Tile *tiles[][32],int jx, int jy,Jugador*jg)
{
   if(this->isDead || !this->isAwake)
        return;
    if(this->wait > 1)
    {
        this->wait--;
        return;
    }
    if(this->x+32 == jx || this->x-32 == jx)
        goto ytest;
    if(this->y == jy)
        goto ytest;
    if(this->x == jx)
    {
      switch(this->tipo)
        {
        case 's':
        {
            int fLength = 0;
            int currentTime = 0;
            int framecount = 0;
            bool animation = true;
            while(animation)
            {
                switch(framecount)
                {
                case 0:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                case 2:
                    this->texture = IMG_LoadTexture(renderer,"SS_F1.png");
                    break;
                case 4:
                    this->texture = IMG_LoadTexture(renderer,"SS_F2.png");
                    break;
                case 6:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                case 8:
                    this->texture = IMG_LoadTexture(renderer,"SS_F2.png");
                    break;
                case 12:
                    this->texture = IMG_LoadTexture(renderer,"SS_F2.png");
                    break;
                case 18:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                case 30:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                }


                SDL_QueryTexture(this->texture, NULL, NULL, &this->rect.w, &this->rect.h);

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
        }
            break;
        case 'g':
        {
            int fLength = 0;
            int currentTime = 0;
            int framecount = 0;
            bool animation = true;
            while(animation)
            {
                switch(framecount)
                {
                case 0:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F0.png");
                    break;
                case 2:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F1.png");
                    break;
                case 4:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F2.png");
                    break;
                case 18:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F0.png");
                    break;
                case 30:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F0.png");
                    break;
                }


                SDL_QueryTexture(this->texture, NULL, NULL, &this->rect.w, &this->rect.h);

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
        }


        }
        for(int i=0;i<32;i++)
            {
                for(int j = 0;j<32;j++)
                {
                    if(tiles[i][j] != NULL)
                    {
                       if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x > this->rect.x && tiles[i][j]->rect.x < jg->rect.x)
                       {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }

                        else if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x < this->rect.x && tiles[i][j]->rect.x > jg->rect.x)
                        {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }
                         if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y < this->rect.y && tiles[i][j]->rect.y > jg->rect.y)
                        {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }
                        if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y > this->rect.y && tiles[i][j]->rect.y < jg->rect.y)
                        {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }
                    }
                }
            }
            jg->getHit(rand()%7+5);
        return;
    }

    else if(this->x < jx)
    {
        for(int i = 0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                {
                    if(tiles[i][j]->rect.x == this->x+32 && tiles[i][j]->rect.y == this->y)
                        if(tiles[i][j]->isBlocking)
                         goto ytest;
                }
            }
        }
        this->x+=32;
        if(this->x == jx && this->y == jy)
            this->x-=32;
        return;
    }
    else if (this->x > jx)
    {
        for(int i = 0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                {
                    if(tiles[i][j]->rect.x == this->x-32 && tiles[i][j]->rect.y == this->y)
                        if(tiles[i][j]->isBlocking)
                         goto ytest;
                }
            }
        }
       this->x-=32;
       if(this->x == jx && this->y == jy)
            this->x+=32;
       return;
    }


    if(this->y+32 == jy || this->y-32 == jy)
        return;
    ytest:if(this->y == jy)
    {
      switch(this->tipo)
        {

        case 's':
        {
            int fLength = 0;
            int currentTime = 0;
            int framecount = 0;
            bool animation = true;
            while(animation)
            {
                switch(framecount)
                {
                case 0:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                case 2:
                    this->texture = IMG_LoadTexture(renderer,"SS_F1.png");
                    break;
                case 4:
                    this->texture = IMG_LoadTexture(renderer,"SS_F2.png");
                    break;
                case 6:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                case 8:
                    this->texture = IMG_LoadTexture(renderer,"SS_F2.png");
                    break;
                case 12:
                    this->texture = IMG_LoadTexture(renderer,"SS_F2.png");
                    break;
                case 18:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                case 30:
                    this->texture = IMG_LoadTexture(renderer,"SS_F0.png");
                    break;
                }


                SDL_QueryTexture(this->texture, NULL, NULL, &this->rect.w, &this->rect.h);

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
        }
        break;
        case 'g':
        {
            int fLength = 0;
            int currentTime = 0;
            int framecount = 0;
            bool animation = true;
            while(animation)
            {
                switch(framecount)
                {
                case 0:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F0.png");
                    break;
                case 2:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F1.png");
                    break;
                case 4:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F2.png");
                    break;
                case 18:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F0.png");
                    break;
                case 30:
                    this->texture = IMG_LoadTexture(renderer,"GUARD_F0.png");
                    break;
                }


                SDL_QueryTexture(this->texture, NULL, NULL, &this->rect.w, &this->rect.h);

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
        }

        }
        for(int i=0;i<32;i++)
            {
                for(int j = 0;j<32;j++)
                {
                    if(tiles[i][j] != NULL)
                    {
                       if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x > this->rect.x && tiles[i][j]->rect.x < jg->rect.x)
                       {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }

                        else if(tiles[i][j]->rect.y == this->rect.y && tiles[i][j]->rect.x < this->rect.x && tiles[i][j]->rect.x > jg->rect.x)
                        {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }
                         if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y < this->rect.y && tiles[i][j]->rect.y > jg->rect.y)
                        {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }
                        if(tiles[i][j]->rect.x == this->rect.x && tiles[i][j]->rect.y > this->rect.y && tiles[i][j]->rect.y < jg->rect.y)
                        {
                         if(tiles[i][j]->isBlocking)
                            return;
                       }
                    }
                }
            }
            jg->getHit(rand()%7+5);
        return;
    }

    if(this->y < jy)
    {
        for(int i = 0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                {
                    if(tiles[i][j]->rect.x == this->x && tiles[i][j]->rect.y == this->y+32)
                        if(tiles[i][j]->isBlocking)
                         return;
                }
            }
        }
        this->y+=32;
        if(this->x == jx && this->y == jy)
            this->y-=32;
        return;
    }
    else if (this->y > jy)
    {
        for(int i = 0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                {
                    if(tiles[i][j]->rect.x == this->x && tiles[i][j]->rect.y == this->y-32)
                        if(tiles[i][j]->isBlocking)
                         return;
                }
            }
        }
        this->y-=32;
        if(this->x == jx && this->y == jy)
            this->y+=32;
        return;
    }



}

void NPC ::render(int camX, int camY)
{
    if(this->isDead)
        return;
    rect.x = x;
    rect.y = y;

    //SDL_RenderCopyEx(renderer,this->texture, NULL, &rect, 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopy(renderer,this->texture,NULL,&rect);
    this->wasHit = false;
}
void NPC::wake()
{
    isAwake = true;
    switch(this->tipo)
        {
        case 'g':
            this->texture = IMG_LoadTexture(renderer, "GUARD_F0.png");
            break;
        case 's':
            this->texture = IMG_LoadTexture(renderer, "SS_F0.png");
            break;
        }
        SDL_QueryTexture(this->texture,NULL,NULL,&rect.w,&rect.h);
}
void NPC :: getHit(int damage,Jugador*jg)
{
    if(this->isDead)
        return;
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
    this->wasHit = true;
    if(!isAwake)
        wake();
    if(this->hp <= 0)
    {
        this->isDead  = true;
        jg->ammo+=rand()%4+1;
        jg->hp+=5;
        if(jg->hp > 100)
            jg->hp =100;
    }

}
