#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <sstream>
#include <time.h>

#include "Jugador.h"
#include "Personajes.h"
#include "Tile.h"
#include "WallTile.h"
#include "NPC.h"

#define BWALL 0
#define MWALL 1
#define FLOOR 2

//using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int LEVEL_WIDTH = 32;
const int LEVEL_HEIGHT = 32;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

bool fullscreen = false;
int currentLevel = 1;


SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture *background;
SDL_Rect rect_background;
SDL_Rect camera;
SDL_Event event;
TTF_Font *gFont;
TTF_Font *tFont;
Mix_Chunk *gMedium;
list<NPC*>NPCs;
Tile *tiles[32][32];
Jugador* jg;
NPC* bud;

TTF_Font *typeface;
SDL_Texture* tTexture;
SDL_Surface* tSurf;
SDL_Rect tRect;

SDL_Texture* tTexture2;
SDL_Surface* tSurf2;
SDL_Rect tRect2;

SDL_Texture* wText;
SDL_Rect wRect;

SDL_Color white = {255,255,255};
int hud_event = 0;



int rand_lim(int limit);
int scroll(int direction);
void cameraInit();
void blankSlate();
void hud_init();
void hud_redraw(int happening);
void instructions();

int menu()
{
    int x, y;
    const int NUMENU = 3;
    const char* labels[NUMENU] = {"New Game","Instructions","Quit"};
    SDL_Color color[2] = {{255,255,255},{255,0,0}};
    bool selected[NUMENU] = {0,0};
    gFont = TTF_OpenFont("Runic.ttf", 16 );
    tFont = TTF_OpenFont("RunicBold.ttf", 24);
    gMedium = Mix_LoadWAV("click.wav");

    SDL_Surface* textSurface[NUMENU];
    SDL_Texture* text[NUMENU];

    SDL_Surface* titleSurface;
    SDL_Texture* titleText;

    textSurface[0] = TTF_RenderText_Solid(gFont, labels[0], color[0]);
    textSurface[1] = TTF_RenderText_Solid(gFont, labels[1], color[0]);
    textSurface[2] = TTF_RenderText_Solid(gFont, labels[2], color[0]);


    titleSurface = TTF_RenderText_Solid(tFont,"Wolf-RL",color[0]);

    text[0] = SDL_CreateTextureFromSurface(gRenderer, textSurface[0]);
    text[1] = SDL_CreateTextureFromSurface(gRenderer, textSurface[1]);
    text[2] = SDL_CreateTextureFromSurface(gRenderer, textSurface[2]);

    titleText = SDL_CreateTextureFromSurface(gRenderer, titleSurface);

    SDL_Rect textRect[NUMENU];
    SDL_Rect titleRect;

    textRect[0].x = (SCREEN_WIDTH/2)-(textSurface[0]->w)/2; textRect[0].y = 75+25;
    textRect[1].x = (SCREEN_WIDTH/2)-(textSurface[1]->w)/2; textRect[1].y = 125+25;
    textRect[2].x = (SCREEN_WIDTH/2)-(textSurface[2]->w)/2; textRect[2].y = 175+25;

    titleRect.x = (SCREEN_WIDTH/2)-(titleSurface->w/2) ; titleRect.y = 20;


    SDL_QueryTexture(text[0], NULL, NULL, &textRect[0].w, &textRect[0].h);
    SDL_QueryTexture(text[1], NULL, NULL, &textRect[1].w, &textRect[1].h);
    SDL_QueryTexture(text[2], NULL, NULL, &textRect[2].w, &textRect[2].h);

    SDL_QueryTexture(titleText,NULL,NULL, &titleRect.w, &titleRect.h);

    while(true)
    {
        while( SDL_PollEvent( &event ))
        {
            switch(event.type){
                case SDL_QUIT:
                    SDL_FreeSurface(textSurface[0]);
                    SDL_FreeSurface(textSurface[1]);
                    SDL_FreeSurface(textSurface[2]);
                        return -1;
                   // SDL_FreeSurface(titleSurface);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_f:
                        if(event.key.keysym.sym == SDLK_f)
                        {
                            if(fullscreen)
                            {
                                fullscreen = false;
                                SDL_SetWindowFullscreen(gWindow,0);
                            }
                            else
                            {
                                fullscreen = true;
                                SDL_SetWindowFullscreen(gWindow,SDL_WINDOW_FULLSCREEN);
                            }
                        }
                        break;
                    case SDLK_ESCAPE:
                        SDL_Quit();
                        return -1;
                    case SDLK_RETURN:
                        return 0;
                    }

                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    for(int i = 0; i < NUMENU; i++)
                    {
                        if(x>=textRect[i].x && x<=textRect[i].x+textRect[i].w && y>=textRect[i].y && y<=textRect[i].y+textRect[i].h)
                        {
                            if(!selected[i])
                            {
                                selected[i] = 1;
                                SDL_FreeSurface(textSurface[i]);
                                SDL_DestroyTexture(text[i]);
                                textSurface[i] = TTF_RenderText_Solid(gFont,labels[i],color[1]);
                                text[i] = SDL_CreateTextureFromSurface(gRenderer, textSurface[i]);
                                Mix_PlayChannel(-1, gMedium, 0);
                            }
                        }
                        else{
                                selected[i] = 0;
                                SDL_FreeSurface(textSurface[i]);
                                SDL_DestroyTexture(text[i]);
                                textSurface[i] = TTF_RenderText_Solid(gFont,labels[i],color[0]);
                                text[i] = SDL_CreateTextureFromSurface(gRenderer, textSurface[i]);
                        }

                    }
                break;

                case SDL_MOUSEBUTTONDOWN:
                    x = event.motion.x;
                    y = event.motion.y;
                     for(int i = 0; i < NUMENU; i++) {
                        if(x>=textRect[i].x && x<=textRect[i].x+textRect[i].w && y>=textRect[i].y && y<=textRect[i].y+textRect[i].h){
                            SDL_FreeSurface(textSurface[0]);
                            SDL_FreeSurface(textSurface[1]);
                            SDL_FreeSurface(textSurface[2]);
                            TTF_CloseFont(gFont);
                            return i;
                        }
                     }
                break;
            }
        }

        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, text[0], NULL, &textRect[0]);
        SDL_RenderCopy(gRenderer, text[1], NULL, &textRect[1]);
        SDL_RenderCopy(gRenderer, text[2], NULL, &textRect[2]);
        SDL_RenderCopy(gRenderer, titleText, NULL, &titleRect);
        SDL_RenderPresent(gRenderer);


    }

    SDL_DestroyTexture(text[0]);
    SDL_DestroyTexture(text[1]);
    SDL_DestroyTexture(text[2]);
    SDL_DestroyTexture(titleText);

    TTF_CloseFont(gFont);
    TTF_CloseFont(tFont);
    TTF_Quit();
}

void instructions()
{
    SDL_RenderClear(gRenderer);


    gFont = TTF_OpenFont("Runic.ttf", 16 );
    tFont = TTF_OpenFont("RunicBold.ttf", 24);

    SDL_Surface* titleSurface;
    SDL_Texture* titleText;

    SDL_Surface* normalSurface[6];
    SDL_Texture* normalTexture[6];

    titleSurface = TTF_RenderText_Solid(tFont,"INSTRUCTIONS",white);
    normalSurface[0] = TTF_RenderText_Solid(gFont,"WASD moves around, ",white);
    normalSurface[1] = TTF_RenderText_Solid(gFont,"SPACE interacts, ",white);
    normalSurface[5] = TTF_RenderText_Solid(gFont,"Q switches weapons, ",white);
    normalSurface[2] = TTF_RenderText_Solid(gFont,"ARROW KEYS fire, ",white);
    normalSurface[3] = TTF_RenderText_Solid(gFont,"and ESCAPE, well, escapes.",white);
    normalSurface[4] = TTF_RenderText_Solid(gFont,"Finish all levels to win.",white);

    titleText = SDL_CreateTextureFromSurface(gRenderer,titleSurface);
    normalTexture[0] = SDL_CreateTextureFromSurface(gRenderer,normalSurface[0]);
    normalTexture[1] = SDL_CreateTextureFromSurface(gRenderer,normalSurface[1]);
    normalTexture[2] = SDL_CreateTextureFromSurface(gRenderer,normalSurface[2]);
    normalTexture[3] = SDL_CreateTextureFromSurface(gRenderer,normalSurface[3]);
    normalTexture[4] = SDL_CreateTextureFromSurface(gRenderer,normalSurface[4]);
    normalTexture[5] = SDL_CreateTextureFromSurface(gRenderer,normalSurface[5]);

    SDL_Rect txtRect[6];
    SDL_Rect titleRect;

    txtRect[0].x = (SCREEN_WIDTH/2)-(normalSurface[0]->w)/2; txtRect[0].y = 75+25;
    txtRect[1].x = (SCREEN_WIDTH/2)-(normalSurface[1]->w)/2; txtRect[1].y = 125+25;
    txtRect[5].x = (SCREEN_WIDTH/2)-(normalSurface[5]->w)/2; txtRect[5].y = 175+25;
    txtRect[2].x = (SCREEN_WIDTH/2)-(normalSurface[2]->w)/2; txtRect[2].y = 225+25;
    txtRect[3].x = (SCREEN_WIDTH/2)-(normalSurface[3]->w)/2; txtRect[3].y = 300+25;
    txtRect[4].x = (SCREEN_WIDTH/2)-(normalSurface[4]->w)/2; txtRect[4].y = 350+25;

    titleRect.x = (SCREEN_WIDTH/2)-(titleSurface->w/2) ; titleRect.y = 20;

    SDL_QueryTexture(titleText,NULL,NULL, &titleRect.w, &titleRect.h);
    SDL_QueryTexture(normalTexture[0], NULL, NULL, &txtRect[0].w, &txtRect[0].h);
    SDL_QueryTexture(normalTexture[1], NULL, NULL, &txtRect[1].w, &txtRect[1].h);
    SDL_QueryTexture(normalTexture[2], NULL, NULL, &txtRect[2].w, &txtRect[2].h);
    SDL_QueryTexture(normalTexture[3], NULL, NULL, &txtRect[3].w, &txtRect[3].h);
    SDL_QueryTexture(normalTexture[4], NULL, NULL, &txtRect[4].w, &txtRect[4].h);
    SDL_QueryTexture(normalTexture[5], NULL, NULL, &txtRect[5].w, &txtRect[5].h);

    while(true)
    {
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, titleText, NULL, &titleRect);

        SDL_RenderCopy(gRenderer,normalTexture[0],NULL,&txtRect[0]);
        SDL_RenderCopy(gRenderer,normalTexture[1],NULL,&txtRect[1]);
        SDL_RenderCopy(gRenderer,normalTexture[2],NULL,&txtRect[2]);
        SDL_RenderCopy(gRenderer,normalTexture[3],NULL,&txtRect[3]);
        SDL_RenderCopy(gRenderer,normalTexture[4],NULL,&txtRect[4]);
        SDL_RenderCopy(gRenderer,normalTexture[5],NULL,&txtRect[5]);

        SDL_RenderPresent(gRenderer);
        while( SDL_PollEvent( &event ))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    goto fin;
            }
        }
    }
    fin:SDL_DestroyTexture(normalTexture[0]);
    SDL_DestroyTexture(normalTexture[1]);
    SDL_DestroyTexture(normalTexture[2]);
    SDL_DestroyTexture(normalTexture[3]);
    SDL_DestroyTexture(normalTexture[4]);
    SDL_DestroyTexture(normalTexture[5]);
    SDL_DestroyTexture(titleText);
    TTF_CloseFont(gFont);
    TTF_CloseFont(tFont);
    TTF_Quit();
    SDL_RenderClear(gRenderer);
    SDL_DestroyRenderer(gRenderer);
}
int dunGen(int previousHP,int ammo,int weapon,bool hasSMG)
{
    std::string mapa = "MAP";
    std::string filetype = ".dung";
    std::stringstream ss;
    ss<<mapa<<currentLevel<<filetype;

    std::string butt = ss.str();

    const char* filename = butt.c_str();

    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        printf("\n\nThere's no more maps fam. the end lel");
        //blankSlate();
        return -1;
    }
    for(int i = 0;i<32;i++)
    {
        for(int j = 0;j<32;j++)
        {
            tiles[i][j] = NULL;
        }
    }
    while(!feof(file))
    {

        int tipo,x,y;
        fscanf(file,"%d\t%d %d",&tipo,&x,&y);
        if(tipo == 5 )
        {
            jg = new Jugador(NULL, gRenderer,x*32,y*32,previousHP,ammo,weapon,hasSMG);
        }
        if(tipo == 6)
        {
           NPCs.push_back(new NPC(gRenderer,'g',x*32,y*32));
        }
        if(tipo == 9)
        {
           NPCs.push_back(new NPC(gRenderer,'s',x*32,y*32));
        }

        tiles[x][y] = new Tile(gRenderer,x*32,y*32,tipo);
        //bud = new NPC(gRenderer,'s',2*32,2*32);
        //tiles.push_back(new Tile(gRenderer,x*32,y*32,tipo));
    }
    fclose(file);
    cameraInit();
    hud_init();
    return 0;
}


int main(int argc, char* args[])
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    gWindow = SDL_CreateWindow("WolfRL 0.1.0 - Alpha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    start:gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);

    //SDL_RenderSetLogicalSize(gRenderer,1024,768);
    //float scale = (1280/320);
    //SDL_RenderSetScale(gRenderer,.6,.6);
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_UpdateWindowSurface(gWindow);

    TTF_Init();



    int menuRes = menu();

    if (menuRes ==1)
    {
        instructions();
        goto start;
    }
    else if(menuRes == 0)
    {

        if(dunGen(100,8,0,false) ==-1)
        {
            blankSlate();
            return 0;
        }

        double last_frame=0;



        SDL_Rect GameWindow;
        GameWindow.x = 0; GameWindow.y = 0;
        GameWindow.w = SCREEN_WIDTH; GameWindow.h = 480;//SCREEN_HEIGHT - 100;


        SDL_Rect NarratorWindow;
        NarratorWindow.x = 0; NarratorWindow.y = 480;//SCREEN_HEIGHT-100;
        NarratorWindow.w = SCREEN_WIDTH; NarratorWindow.h = 100;
        hud_init();

        while(true)
        {
            SDL_RenderSetViewport(gRenderer,&GameWindow);
            while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        return 69;

                    case SDL_KEYDOWN:
                        int logic_result = jg->logic(tiles,NPCs);

                        if(logic_result != 0)
                        {
                            if(logic_result == 1488)
                                hud_event = 1488;

                            if(logic_result == 99)
                            {
                                hud_event = 1;
                                hud_redraw(hud_event);
                                 SDL_RenderSetViewport(gRenderer,&NarratorWindow);
                                //SDL_RenderClear(gRenderer);
                                SDL_RenderCopy(gRenderer, background, NULL, &rect_background);
                                 hud_redraw(hud_event);
                                SDL_RenderCopy(gRenderer,tTexture,NULL,&tRect);
                                if(jg->selected_weapon != 0)
                                    SDL_RenderCopy(gRenderer,tTexture2,NULL,&tRect2);
                                SDL_RenderCopy(gRenderer,wText,NULL,&wRect);

                                SDL_RenderPresent(gRenderer);


                                currentLevel++;
                                if(currentLevel == 5)
                                {
                                    hud_redraw(0);
                                    hud_redraw(187);
                                    SDL_RenderSetViewport(gRenderer,&NarratorWindow);
                                    SDL_RenderClear(gRenderer);
                                    SDL_RenderCopy(gRenderer, background, NULL, &rect_background);
                                    SDL_RenderCopy(gRenderer,tTexture,NULL,&tRect);
                                    if(jg->selected_weapon != 0)
                                        SDL_RenderCopy(gRenderer,tTexture2,NULL,&tRect2);
                                    SDL_RenderCopy(gRenderer,wText,NULL,&wRect);

                                    SDL_RenderPresent(gRenderer);
                                    while(true)
                                    {
                                        while( SDL_PollEvent( &event ))
                                        {
                                            switch(event.type)
                                            {
                                                case SDL_KEYDOWN:
                                                    blankSlate();
                                                   SDL_RenderClear(gRenderer);
                                                    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
                                                    SDL_RenderSetViewport(gRenderer,NULL);
                                                    currentLevel = 1;
                                                    hud_redraw(666);
                                                    SDL_DestroyRenderer(gRenderer);
                                                    goto start; //return 0;
                                            }
                                        }
                                    }
                                }
                                blankSlate();

                                if(dunGen(jg->hp,jg->ammo,jg->selected_weapon,jg->hasSMG) ==-1)
                                {
                                   //goto fin;
                                    blankSlate();
                                    SDL_Quit();
                                    return 0;
                                }
                                hud_event = 0;
                                hud_redraw(hud_event);
                            }

                            for(list<NPC*>::iterator e = NPCs.begin(); e!=NPCs.end(); e++)
                            {
                                (*e)->logic(tiles,jg->rect.x,jg->rect.y,jg);
                            }
                            if(jg->hp < 1)
                            {
                                hud_redraw(666);

                                 SDL_RenderSetViewport(gRenderer,&NarratorWindow);
                                //SDL_RenderClear(gRenderer);
                                SDL_RenderCopy(gRenderer, background, NULL, &rect_background);
                                hud_redraw(666);
                                SDL_RenderCopy(gRenderer,tTexture,NULL,&tRect);
                                SDL_RenderCopy(gRenderer,tTexture,NULL,&tRect);
                                    if(jg->selected_weapon != 0)
                                SDL_RenderCopy(gRenderer,wText,NULL,&wRect);

                                SDL_RenderPresent(gRenderer);
                                while(true)
                                {
                                    while( SDL_PollEvent( &event ))
                                    {
                                        switch(event.type)
                                        {
                                            case SDL_KEYDOWN:
                                                blankSlate();
                                               SDL_RenderClear(gRenderer);
                                                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
                                                SDL_RenderSetViewport(gRenderer,NULL);
                                                currentLevel = 1;
                                                hud_redraw(666);
                                                SDL_DestroyRenderer(gRenderer);
                                                goto start; //return 0;
                                        }
                                    }
                                }


                            }
                            scroll(logic_result);


                        }
                        if(event.key.keysym.sym == SDLK_ESCAPE)
                        {
                           blankSlate();
                           SDL_RenderClear(gRenderer);
                            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
                            //SDL_RenderSetViewport(gRenderer,NULL);
                            currentLevel = 1;
                            SDL_DestroyRenderer(gRenderer);
                            goto start; //return 0;
                        }
                        if(event.key.keysym.sym == SDLK_f)
                        {
                            if(fullscreen)
                            {
                                fullscreen = false;
                                SDL_SetWindowFullscreen(gWindow,0);
                                //sdl_renderset
                            }
                            else
                            {
                                fullscreen = true;
                                SDL_SetWindowFullscreen(gWindow,SDL_WINDOW_FULLSCREEN);
                            }
                        }
                        goto cont;
                }
            }

        cont:double diferencia = SDL_GetTicks()-last_frame;
        double ajuste = 17 - diferencia;

        if(ajuste>0)
            SDL_Delay(ajuste);
        last_frame=SDL_GetTicks();

        //Clear screen


        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
        SDL_RenderClear( gRenderer );

        //SDL_RenderFillRect(gRenderer,&camera);
        SDL_RenderCopy(gRenderer, background, NULL, &rect_background);

        for(int i = 0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                if(tiles[i][j] != NULL)
                {
                    if(jg->tilePos(tiles,i,j) || tiles[i][j]->tipo == 30||tiles[i][j]->tipo == 40||tiles[i][j]->tipo == 70)
                        jg->discover(tiles,i,j,&NPCs);
                    if(tiles[i][j]->isVisible)
                        tiles[i][j]->dibujar(jg->rect);
                }
            }
        }

        for(list<NPC*>::iterator e = NPCs.begin(); e!=NPCs.end(); e++)
        {
            if((*e)->isVisible)
                (*e)->render(1,1);

        }

        jg->render(1,1);
        //bud->render(1,1);


        //SDL_RenderPresent(gRenderer);


        SDL_RenderSetViewport(gRenderer,&NarratorWindow);
        //SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, background, NULL, &rect_background);
         hud_redraw(hud_event);
        SDL_RenderCopy(gRenderer,tTexture,NULL,&tRect);
        SDL_RenderCopy(gRenderer,wText,NULL,&wRect);
        if(jg->selected_weapon != 0)
            SDL_RenderCopy(gRenderer,tTexture2,NULL,&tRect2);
        SDL_RenderPresent(gRenderer);

        }
    }
        //blankSlate();

        return 0;
}

void blankSlate()
{
    if(NPCs.empty())
        NPCs.clear();
    if(tiles != NULL)
    {
        for(int i = 0;i<32;i++)
        {
            for(int j = 0;j<32;j++)
            {
                tiles[i][j] = NULL;
            }
        }
    }
    NPCs.clear();
    if(jg != NULL)
        delete(jg);

}

//tomado de:
//https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do
    {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

void cameraInit()
{

    //camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    camera.h = 32*32;
    camera.w = 32*32;
    camera.x = 512;
    camera.y = 512;

    if(jg->x < 13*32)
        camera.x = (jg->rect.x-32);
    else
        while(scroll(1) == 1){}

    if(jg->y < 10*32)
        camera.y = (jg->rect.y-32);
    else
        while(scroll(4) == 1){}


    if(camera.x < 512)
        camera.x = 512;
    if(camera.y < 512)
        camera.y = 512;

    if(camera.x > 736)
        camera.x = 736;
    if(camera.y > 960+65)
        camera.y = 960+65;
}

int scroll(int direction)
{
    switch(direction)
    {
    case 1:
        if(jg->x >= 13*32 && (camera.x) < 736)
        {
            camera.x+=32;
            jg->x -=32;
            for(list<NPC*>::iterator e = NPCs.begin(); e!=NPCs.end(); e++)
                (*e)->x -=32;
            for(int i = 0;i<32;i++)
            {
                for(int j = 0;j<32;j++)
                {
                    if(tiles[i][j] != NULL)
                    tiles[i][j]->rect.x-=32;
                }
            }
        }
        else
            return 0;
        break;
    case 3:
        if(jg->x <= 13*32 && (camera.x> 512))
        {
            camera.x-=32;
            jg->x +=32;
            for(list<NPC*>::iterator e = NPCs.begin(); e!=NPCs.end(); e++)
                (*e)->x +=32;
            for(int i = 0;i<32;i++)
            {
                for(int j = 0;j<32;j++)
                {
                    if(tiles[i][j] != NULL)
                    tiles[i][j]->rect.x+=32;
                }
            }

        }
        else
            return 0;
        break;
    case 2:
        if(jg->y <= 10*32 && (camera.y> 512))
        {
            camera.y-=32;
            jg->y +=32;
            for(list<NPC*>::iterator e = NPCs.begin(); e!=NPCs.end(); e++)
                (*e)->y +=32;
            for(int i = 0;i<32;i++)
            {
                for(int j = 0;j<32;j++)
                {
                    if(tiles[i][j] != NULL)
                         tiles[i][j]->rect.y+=32;
                }
            }
        }
        else
            return 0;
        break;


    case 4:
        if(jg->y >= 10*32 && (camera.y< 960+65))
        {
            camera.y+=32;
            jg->y -=32;
            for(list<NPC*>::iterator e = NPCs.begin(); e!=NPCs.end(); e++)
                (*e)->y -=32;
            for(int i = 0;i<32;i++)
            {
                for(int j = 0;j<32;j++)
                {
                    if(tiles[i][j] != NULL)
                        tiles[i][j]->rect.y-=32;
                }
            }
        }
        else
            return 0;
        break;

    }
    return 1;
}

void hud_init()
{
    SDL_DestroyTexture(tTexture);
    SDL_FreeSurface(tSurf);

    SDL_DestroyTexture(tTexture2);
    SDL_FreeSurface(tSurf2);

    SDL_DestroyTexture(wText);
    int ammo;
    switch(jg->selected_weapon)
    {
    case 0:
         wText = IMG_LoadTexture(gRenderer,"knife.png");
         ammo = -1;
         break;
    case 1:
         wText = IMG_LoadTexture(gRenderer,"pistol.png");
         ammo = jg->ammo;
         break;
    case 2:
         wText = IMG_LoadTexture(gRenderer,"smg.png");
         ammo = jg->ammo;
         break;
    }
    int w = 16,h=16;
    SDL_QueryTexture(wText, NULL, NULL,&w,&h );
     wRect.x = 20; wRect.y = 60;wRect.w = 32;wRect.h = 32;

   //Font by: Adam Moore  http://laemeur.sdf.org/fonts/
   typeface = TTF_OpenFont("MorePerfectDOSVGA.ttf", 32 );

    if(typeface == NULL)
        printf("\nVGA FONT FUCKED UP");

    int health = jg->hp;



    stringstream strs;
      strs <<"HP: "<< health;
      string temp_str = strs.str();
      char* hp = (char*) temp_str.c_str();

      char* balas;
      if(ammo != -1)
      {
          stringstream strs;
        strs<<ammo;
        string temp_str = strs.str();
        balas = (char*) temp_str.c_str();
      }



    tSurf = TTF_RenderText_Solid(typeface,hp,white);
    if( tSurf == NULL ) { printf( "Unable to render text surface!\n SDL_ttf Error: %s\n", TTF_GetError() ); }

   tTexture = SDL_CreateTextureFromSurface(gRenderer,tSurf);
    if( tTexture == NULL ) { printf( "Unable to create texture from rendered text! \nSDL Error: %s\n", SDL_GetError() ); }

   tRect.x = 20;tRect.y =20;
   tRect2.x = 108;tRect2.y =60;

   if(ammo != -1)
    {
        tSurf2 = TTF_RenderText_Solid(typeface,balas,white);
        if( tSurf2 == NULL ) { printf( "Unable to render text surface!\n SDL_ttf Error: %s\n", TTF_GetError() ); }

        tTexture2 = SDL_CreateTextureFromSurface(gRenderer,tSurf2);
        if( tTexture2 == NULL ) { printf( "Unable to create texture from rendered text! \nSDL Error: %s\n", SDL_GetError() ); }
        SDL_QueryTexture(tTexture2,NULL,NULL,&tRect2.w,&tRect2.h);
    }


    SDL_QueryTexture(tTexture,NULL,NULL,&tRect.w,&tRect.h);

}

void hud_redraw(int happening)
{
    char* whatHappened;
    switch(happening)
    {
    case 1:
        whatHappened = "    Loading Level...";
        break;
    case 666:
        whatHappened = "    You Are Dead!";
        break;
    case 187:
        whatHappened = "    You Have Won!";
        break;
    default:
        whatHappened = NULL;
        break;
    }
    SDL_DestroyTexture(tTexture);
    SDL_FreeSurface(tSurf);

    SDL_DestroyTexture(wText);
    int ammo;
    switch(jg->selected_weapon)
    {
    case 0:
         wText = IMG_LoadTexture(gRenderer,"knife.png");
         ammo = -1;
         break;
    case 1:
         wText = IMG_LoadTexture(gRenderer,"pistol.png");
         ammo = jg->ammo;
         break;
    case 2:
         wText = IMG_LoadTexture(gRenderer,"smg.png");
         ammo = jg->ammo;
         break;
    }
    int w = 16,h=16;
    SDL_QueryTexture(wText, NULL, NULL,&w,&h );
     wRect.x = 20; wRect.y = 60;wRect.w = 32;wRect.h = 32;


    int health = jg->hp;

    stringstream strs;
      strs <<"HP: "<< health<<"     "<<whatHappened;
      string temp_str = strs.str();
      char* hp = (char*) temp_str.c_str();
      char* balas;
      if(ammo != -1)
      {
          stringstream strs;
        strs<<ammo;
        string temp_str = strs.str();
        balas = (char*) temp_str.c_str();
      }

    tSurf = TTF_RenderText_Solid(typeface,hp,white);
    if( tSurf == NULL ) { printf( "Unable to render text surface!\n SDL_ttf Error: %s\n", TTF_GetError() ); }

   tTexture = SDL_CreateTextureFromSurface(gRenderer,tSurf);
    if( tTexture == NULL ) { printf( "Unable to create texture from rendered text! \nSDL Error: %s\n", SDL_GetError() ); }

    if(ammo != -1)
    {
        tSurf2 = TTF_RenderText_Solid(typeface,balas,white);
        if( tSurf2 == NULL ) { printf( "Unable to render text surface!\n SDL_ttf Error: %s\n", TTF_GetError() ); }

        tTexture2 = SDL_CreateTextureFromSurface(gRenderer,tSurf2);
        if( tTexture2 == NULL ) { printf( "Unable to create texture from rendered text! \nSDL Error: %s\n", SDL_GetError() ); }
        SDL_QueryTexture(tTexture2,NULL,NULL,&tRect2.w,&tRect2.h);
    }

   tRect.x = 20;tRect.y = 20;
   tRect2.x = 108;tRect2.y =60;
   SDL_QueryTexture(tTexture,NULL,NULL,&tRect.w,&tRect.h);

}




