#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"

#include "Block.h"


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
Block* blocks[32][32];
int x,y;
int xclick,yclick;
int previous_selection;
int selected_block;
bool lpressed;
bool rpressed;
bool bucket;
bool hlock;
bool vlock;
bool rectangle = false;

int init_all();
void printSelection(int i);
void bucketFill(int x, int y, int previousType);

void export_map();
int real_export(int i);

void import_map();
int real_import(int i);

void new_map();

int main(int argc, char *argv[])
{
    int result = init_all();
    if(result != 0)
        return result;

    //init array
    new_map();
    //init cursor
    SDL_Texture* Cursor = IMG_LoadTexture(renderer,"CURSOR.png");
    SDL_Rect rect_Cursor;
    rect_Cursor.x = 0;rect_Cursor.y = 0;
    SDL_QueryTexture(Cursor,NULL,NULL,&rect_Cursor.w,&rect_Cursor.h);

    xclick=-1;yclick=-1;
    //Main Loop
    while(true)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
                goto fin;
            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    goto fin;
                    break;
                case SDLK_1:
                    selected_block = 0;
                    break;
                case SDLK_2:
                    selected_block = 1;
                    break;
                case SDLK_3:
                    selected_block = 2;
                    break;
                case SDLK_4:
                    selected_block = 3;
                    break;
                case SDLK_5:
                    selected_block = 4;
                    break;
                case SDLK_6:
                    selected_block = 5;
                    break;
                case SDLK_7:
                    selected_block = 6;
                    break;
                case SDLK_8:
                    selected_block = 7;
                    break;
                case SDLK_9:
                    selected_block = 8;
                    break;
                case SDLK_0:
                    selected_block = 9;
                    break;
                case SDLK_e:
                    selected_block = -1;
                    break;
                case SDLK_b:
                    rectangle = false;
                    hlock = false;
                    vlock = false;
                    printSelection(selected_block);
                    if(bucket)
                        bucket = false;
                    else
                        bucket = true;
                    break;
                case SDLK_o:
                    export_map();
                    break;
                case SDLK_i:
                    import_map();
                    break;
                case SDLK_n:
                    new_map();
                    break;
                case SDLK_h:
                    printSelection(selected_block);
                    rectangle = false;
                    bucket=false;
                    if(hlock)
                        hlock = false;
                    else
                    {
                        hlock = true;
                        vlock = false;
                    }
                    break;
                case SDLK_v:
                    printSelection(selected_block);
                    rectangle = false;
                    bucket=false;
                    if(vlock)
                        vlock = false;
                    else
                    {
                        vlock = true;
                        hlock = false;
                    }
                    break;
                /*case SDLK_r:
                    printSelection(selected_block);
                    bucket = false;vlock = false;hlock=false;
                    if(!rectangle)
                        rectangle = true;
                    else
                        rectangle = false;
                    break;*/
                case SDLK_c:
                    bucket=false;vlock=false;hlock=false;rectangle=false;
                    selected_block = 0;
                    break;
                }
                printSelection(selected_block);
            }
            if(Event.type == SDL_MOUSEMOTION)
            {
                if(!vlock)
                {
                    x = Event.motion.x;
                }
                if(!hlock)
                {
                    y = Event.motion.y;
                }
                if(lpressed || rpressed)
                {
                    if(!vlock)
                    {
                        xclick = Event.motion.x;
                        yclick = y;
                    }
                    if(!hlock)
                    {
                        yclick = Event.motion.y;
                        xclick = x;
                    }
                }
            }
            if(Event.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(Event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    lpressed = true;
                    if(!vlock)
                        xclick = Event.motion.x;
                    if(!hlock)
                        yclick = Event.motion.y;
                    break;
                case SDL_BUTTON_RIGHT:
                    printf("\nERASING");
                    rpressed = true;
                    previous_selection = selected_block;
                    selected_block = -1;
                    if(!vlock)
                        xclick = Event.motion.x;
                    if(!hlock)
                        yclick = Event.motion.y;
                }
            }
            if(Event.type == SDL_MOUSEBUTTONUP)
            {
                xclick = -1; yclick = -1;
                switch(Event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    lpressed = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    rpressed = false;
                    selected_block = previous_selection;
                    printSelection(selected_block);
                }
            }
        }

        for(int i = 0; i < 32; i++)
        {
            for(int j = 0; j < 32; j++)
            {
                if(blocks[i][j]->check_selected(x,y) )
                {
                    rect_Cursor.x = blocks[i][j]->rect_block.x;
                    rect_Cursor.y = blocks[i][j]->rect_block.y;
                    SDL_RenderCopy(renderer,Cursor,NULL,&rect_Cursor);
                }
                if(blocks[i][j]->check_selected(xclick,yclick))
                {
                    if(bucket )
                    {
                        if(blocks[i][j]->tipo != selected_block)
                            bucketFill(i,j,blocks[i][j]->tipo);
                        //bucket = false;
                        printSelection(selected_block);
                    }

                    else
                        blocks[i][j]->change_type(selected_block);
                }

                SDL_RenderCopy(renderer,blocks[i][j]->texture, NULL, &blocks[i][j]->rect_block);
            }
        }

         SDL_RenderCopy(renderer,Cursor,NULL,&rect_Cursor);
        SDL_RenderPresent(renderer);
    }

    //Clear everything
    fin:
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            free(blocks[i][j]);
        }
    }
    SDL_Quit();
    system("cls");
    printf("I seriously hope you remembered to save.");
    printf("\nI also seriously hope we finish this shit by the deadline.\n\n");
    printf("\nThank you for putting the \"dung\" in \"dungeon\"! \n\n\n\n\n");
    printf("\n\n\n\n\n\n\nDungbuilder v1.1.0 - Copyup (c)1984 SOME AMOUNT OF RIGHTS RESERVED, BITCH\n\"Fuck FLOSS, Fuck Liberty, and especially, Fuck You :^)\"\n\n");
    system("pause");
    return 0;
}

int init_all()
{

    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("DungBuilder v1.0.0", 500, 20, 720/*WIDTH*/, 720/*HEIGHT*/, SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    SDL_SetWindowIcon(window, IMG_Load("BJ_STAND.png"));


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }
    SDL_RenderSetLogicalSize(renderer,512,512);
    //SDL_RenderSetScale(renderer,1.5,1.5);
    selected_block = 0;
    printSelection(selected_block);
    bucket = false;
    //SDL_ShowCursor(0);
    return 0;
}
void printSelection(int i)
{
    system("cls");
    printf("\t~~DUNGBUILDER v1.1.0~~\n\n");
    printf("\nBrushes: number keys\nBucket: b\nEraser: e");
    printf("\nVertical Mouse Lock: v\nHorizontal Mouse Lock: h");//\nRectangle Tool: r");
    printf("\n\nLeft Mouse: draw/use tool\nRight Mouse: quick eraser\nClear Tools (Default Brush): c\n");
    printf("\n\nImport: i\nExport: o\nNew Map (without saving!): n\nExit (without saving!): escape\n\n");

    printf("\nSelected block: ");
    switch(i)
    {
    case 0:
        printf("Blue Brick");
        break;
    case 1:
        printf("Metal Wall");
        break;
    case 2:
        printf("Floor");
        break;
    case 3:
        printf("Vertical Door");
        break;
    case 4:
        printf("Horizontal Door");
        break;
    case 5:
        printf("Player Start");
        break;
    case 6:
        printf("Guard");
        break;
    case 7:
        printf("Elevator Door");
        break;
    case 8:
        printf("Exit Switch");
        break;
    case 9:
        printf("SS");
        break;
    case -1:
        printf("Blank");
        break;
    }
    if(i == -1)
        printf("\nERASER");
    if(bucket)
        printf("\nBUCKET");
    if(vlock)
        printf("\nVERTICAL MOUSE LOCK");
    if(hlock)
        printf("\nHORIZONTAL MOUSE LOCK");
    /*if(rectangle)
        printf("\nRECTANGLE");*/

}

void bucketFill(int x, int y, int previousType)
{
    if(x>31 || y > 31 || x < 0 || y < 0)
        return;
    if(blocks[x][y]->tipo == previousType)
    {
        blocks[x][y]->change_type(selected_block);
        bucketFill(x+1,y,previousType);
        bucketFill(x-1,y,previousType);
        bucketFill(x,y+1,previousType);
        bucketFill(x,y-1,previousType);
    }
}

//Map I/O
void export_map()
{
    system("cls");
    printf("EXPORTING MAP\nEnter map number (or enter -1 to cancel): ");
    int selection = 0;
    scanf("%d",&selection);
    if(selection > -1)
    {
        real_export(selection);
        printf("\nMap was saved as: MAP");
        printf("%d.dung\n",selection);
        system("pause");
    }
    else
        printSelection(selected_block);
    return;

}
int real_export(int i)
{
    std::string mapa = "MAP";
    std::string filetype = ".dung";
    std::stringstream ss;
    ss<<mapa<<i<<filetype;

    std::string butt = ss.str();

    const char* filename = butt.c_str();

    FILE* file = fopen(filename,"w");

    for(int i = 0;i<32;i++)
    {
        for(int j = 0;j<32;j++)
        {
            if(blocks[i][j]->tipo != -1)
                fprintf(file,"%d\t%d %d\n",blocks[i][j]->tipo,blocks[i][j]->x/16,blocks[i][j]->y/16);
        }
    }
    fclose(file);
    return 0;
}

void import_map()
{
    system("cls");
    printf("IMPORTING MAP\nType the map number(or press -1 to cancel): ");
    int selection = 0;
    scanf("%d", &selection);

    if(selection > -1)
    {
        int result = real_import(selection);
        if(result == 0)
        {
            printf("\nLoaded map: MAP%d.dung\n\n",selection);
            system("pause");
        }
    }
}

int real_import(int i)
{
    std::string mapa = "MAP";
    std::string filetype = ".dung";
    std::stringstream ss;
    ss<<mapa<<i<<filetype;

    std::string butt = ss.str();

    const char* filename = butt.c_str();

    FILE* file = fopen(filename,"r");

    if(file == NULL)
    {
        system("cls");
        printf("\nFile doesn't exist or isn't a dung (lel).\n\n");
        system("pause");
        return -1;
    }
    new_map();
    while(!feof(file))
    {

        int tipo,x,y;
        fscanf(file,"%d\t%d %d",&tipo,&x,&y);
        printf("tipo: %d,x: %d,y: %d\n",tipo,x,y);
        blocks[x][y] = new Block(renderer,x*16,y*16,tipo);
    }
    fclose(file);
    return 0;
}

void new_map()
{
    for(int i = 0;i<32;i++)
    {
        for(int j = 0;j<32;j++)
        {
            blocks[i][j] = NULL;
            blocks[i][j] = new Block(renderer,(i*16),(j*16),-1);
        }
    }
}
