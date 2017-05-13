#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

//include libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "map.h"
#include "Robots.h"

// pre-processor definitions
#define MAP_SIZE 1000
#define WINDOW_POSX 500
#define WINDOW_POSY 250
#define SQUARE_SIZE 34
#define EXTRASPACE 150
#define MARGIN 3
#define delay_change 50 //Constante para alterar a velocidade/delay do robot.
#define initial_delay 300 //Define o delay inicial a 300 ms.

// functions for rendering the graphics declarations
int InitEverything(int , int , SDL_Window** , SDL_Renderer** );
int InitSDL();
int InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window* );
void RenderMap(int , int , MAP **, ROBOT *, SDL_Renderer* );
int RenderText(int , int , const char* , TTF_Font *, SDL_Renderer* );
int RenderLogo(int , int , SDL_Renderer* );
void set_path(SDL_Renderer*, ROBOT *headR); //Função que desenha o caminho que o robot deve percorrer

#endif // GRAPHICS_H_INCLUDED
