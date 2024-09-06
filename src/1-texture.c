#include <stdio.h>
#include <stdbool.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>
#define NUM_TEXTURES 1


/*gcc  -I C:\\mingw_dev_lib\include\SDL2  -L C:\\mingw_dev_lib\lib  -o texture src/1-texture.c src/drawMap.c src/init_sdl.c  -lmingw32 -lSDL2main -lSDL2*/
int map[mapWidth][mapHeight]  ={
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,3,0,0,0,3,0,0,0,1},
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



SDL_Texture *wallTextures[NUM_TEXTURES];


int setTexture(SDL_Instance *instance, char paths[1][1024], int size) {
    if(size < NUM_TEXTURES){
        wallTextures[size] = loadTexture(instance, paths[0]);
        setTexture(instance, paths, ++size);
    }
    return size;
}



int main(int argc, char **argv) {
    SDL_Instance instance;
    SDL_Event e;
    Player p = {22, 12};
    Direction direction = {.x = -1, .y = 0};
    Plan plan = {.x = 0, .y = 0.66};
    Measures mes = {.deltaX = 0, .deltaY = 0, .sideX = 0, .sideY = 0};
    RayDirection ray = {.x = 0, .y = 0};
    double perpWallDist, time = 0, oldTime = 0, cameraX, frameTime, moveSpeed;
    int hitSide, mapX, mapY, game_running = 1;
    char paths[1][1024] = {"../images/wall1.bmp"};
    

    if (!init_instance(&instance)) {
        return 1;
    }

    setTexture(&instance ,paths, 0);

    while (game_running) {
        oldTime = time;
        time = SDL_GetTicks();
        frameTime = (time - oldTime) / 1000.0;
        moveSpeed = frameTime * 10.0;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                game_running = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                rotate(e.key.keysym.sym, &direction, &plan);
                move(e.key.keysym.sym, &p, &direction, moveSpeed);
            }
        }

        SDL_RenderClear(instance.renderer);
        renderCeilAndGround(&instance,  wallTextures[0],  &p,  direction, plan);

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
            ray.x = direction.x + plan.x * cameraX;
            ray.y = direction.y + plan.y * cameraX;
            mes.deltaX = (ray.x == 0) ? 1e30 : fabs(1 / ray.x);
            mes.deltaY = (ray.y == 0) ? 1e30 : fabs(1 / ray.y);
            mapX = (int)p.x;
            mapY = (int)p.y;
            hitSide = 0;

            performDDA(&ray, &mes, &p, &mapX, &mapY, &hitSide);

            if (hitSide == 0) {
                perpWallDist = (mes.sideX - mes.deltaX);
            } else {
                perpWallDist = (mes.sideY - mes.deltaY);
            }

            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;


            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            int wallType = map[mapY][mapX] - 1;
            if (wallType < 0 || wallType >= NUM_TEXTURES) {
                wallType = 0;
            }

            double wallX = (hitSide == 0) ? p.y + perpWallDist * ray.y : p.x + perpWallDist * ray.x;
            wallX -= floor(wallX);
            int textureX = (int)(wallX * 64);
            if ((hitSide == 0 && ray.x > 0) || (hitSide == 1 && ray.y < 0)) {
                textureX = 64 - textureX - 1;
            }

            SDL_Rect srcRect = {textureX, 0, 1, 64}; 
            SDL_Rect destRect = {x, drawStart, 1, drawEnd - drawStart};
            SDL_RenderCopy(instance.renderer, wallTextures[wallType], &srcRect, &destRect);

        }

        SDL_RenderPresent(instance.renderer);

    }

    for (int i = 0; i < NUM_TEXTURES; i++) {
        SDL_DestroyTexture(wallTextures[i]);
    }
    destroy_instance(&instance);
    return 0;
}