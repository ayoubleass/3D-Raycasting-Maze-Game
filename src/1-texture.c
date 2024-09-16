#include <stdio.h>
#include <dirent.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>
#include <dirent.h> 


/*gcc  -I C:\\mingw_dev_lib\include\SDL2  -L C:\\mingw_dev_lib\lib  -o texture src/1-texture.c  src/rotate.c src/essential.c src/init_sdl.c  src/init-texture_path.c src/move.c -lmingw32 -lSDL2main -lSDL2*/

char *TexturePaths[NUM_TEXTURES];
SDL_Texture *wallTextures[NUM_TEXTURES];


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
        moveSpeed = frameTime * 10.0;
        SDL_RenderClear(instance.renderer);
        renderCeilAndGround(&instance,  wallTextures[9],  &p,  direction, plan);
        drawWalls(&instance,&p, &direction, &plan);
        renderGun(&instance,wallTextures[9] , &direction ,100, 100);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                game_running = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                rotateAndMove(e.key.keysym.sym, &p , &direction, &plan, moveSpeed);
            }
        }
   
        SDL_RenderPresent(instance.renderer);
    }

    for (int i = 0; i < NUM_TEXTURES; i++) {
        SDL_DestroyTexture(wallTextures[i]);
    }
    freeTexturePaths() ;
    destroy_instance(&instance);
    return 0;
}