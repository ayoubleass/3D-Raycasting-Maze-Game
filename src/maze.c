#include <stdio.h>
#include <dirent.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>


char *TexturePaths[NUM_TEXTURES];
SDL_Texture *textures[NUM_TEXTURES];
double zBuffer[SCREEN_WIDTH];
double rotation_speed = 0.1;

int map[mapWidth][mapHeight]  ={
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,0,1,0,3,0,0,0,1},
  {1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,2,0,0,2,2,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};






/**
 * main - Entry point of the application
 * @argc: Number of arguments passed
 * @argv: Array of arguments passed
 *
 * This function initializes SDL, sets up textures, and enters the main
 * game loop where it handles rendering and player movement until the game
 * exits. It also manages texture cleanup and SDL instance destruction.
 * 
 * Return: 0 on success, 1 on failure
 */





int main(int argc, char **argv) {
    SDL_Instance instance;
    SDL_Event e;
    Player p = {22, 12};
    Direction direction = {.x = -1, .y = 0};
    Plan plan = {.x = 0, .y = 0.66};
    double  time = 0, oldTime = 0, frameTime, moveSpeed;
    int game_running = 1;
    (void) argc, (void) argv;
    if (!init_instance(&instance))
        return 1;
    initTexturePaths();
    setTexture(&instance ,TexturePaths, 0);  
    while (game_running) {
        oldTime = time;
        time = SDL_GetTicks();
        frameTime = (time - oldTime) / 1000.0;
        moveSpeed = frameTime * 5.0;
        SDL_RenderClear(instance.renderer);
        renderCeilAndGround(&instance,  textures[1],  &p,  direction, plan);
        drawWalls(&instance,&p, &direction, &plan);
        renderWeapon(&instance, textures[6]);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                game_running = 0;
            if (e.type == SDL_KEYDOWN)
                rotateAndMove(SDL_GetKeyboardState(NULL), &p , &direction, &plan, moveSpeed);
        }
        SDL_RenderPresent(instance.renderer);
    }
    freeTextures();
    freeTexturePaths() ;
    destroy_instance(&instance);
    return 0;
}