#include <stdio.h>
#include <dirent.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>
#include <dirent.h> 


char *TexturePaths[NUM_TEXTURES];
SDL_Texture *textures[NUM_TEXTURES];


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

void getTextureSize(SDL_Texture *texture, int *width, int *height) {
    if (SDL_QueryTexture(texture, NULL, NULL, width, height) != 0) {
        printf("Unable to query texture size: %s\n", SDL_GetError());
    }
}

int main(int argc, char **argv) {
    SDL_Instance instance;
    SDL_Event e;
    Player p = {22, 12};
    Direction direction = {.x = -1, .y = 0};
    Plan plan = {.x = 0, .y = 0.66};
    double  time = 0, oldTime = 0, frameTime, moveSpeed;
    int gunTextureWidth, gunTextureHeight;
    int game_running = 1;
    (void) argc, (void) argv;

    if (!init_instance(&instance))
        return 1;
    initTexturePaths();
    setTexture(&instance ,TexturePaths, 0);  
    getTextureSize(textures[6], &gunTextureWidth, &gunTextureHeight);

    while (game_running) {
        oldTime = time;
        time = SDL_GetTicks();
        frameTime = (time - oldTime) / 1000.0;
        moveSpeed = frameTime * 10.0;
        SDL_RenderClear(instance.renderer);
        renderCeilAndGround(&instance,  textures[1],  &p,  direction, plan);
        drawWalls(&instance,&p, &direction, &plan);
        renderGun(&instance, textures[6],gunTextureWidth, gunTextureHeight - 100);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                game_running = 0;
            if (e.type == SDL_KEYDOWN)
                rotateAndMove(e.key.keysym.sym, &p , &direction, &plan, moveSpeed);
        }
        SDL_RenderPresent(instance.renderer);
    }
    for (int i = 0; i < NUM_TEXTURES; i++) {
        SDL_DestroyTexture(textures[i]);
    }
    freeTexturePaths() ;
    destroy_instance(&instance);
    return 0;
}