#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>




int map[mapHeight][mapWidth]  ={
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,0,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
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






/**/


int main(int argc, char **argv) {
    (void)argc, (void)argv;
    SDL_Instance instance;
    SDL_Event e;
    int game_running = init_instance(&instance);
    if (!game_running) return 1;

    Player p = {9, 5};
    Direction direction = {.x = -1, .y = 0};
    Plan plan = {.x = 0, .y = 0.66};
    Measures mes = {.deltaX = 0, .deltaY = 0, .sideX = 0, .sideY = 0};
    RayDirection ray = {.x = 0, .y = 0};

    double perpWallDist,time = 0, oldTime = 0;
        

    while (game_running) {
        oldTime = time;
        time = SDL_GetTicks();
        double frameTime = (time - oldTime) / 1000.0;
        double moveSpeed = frameTime * 2.0;


        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                game_running = 0;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {
                    rotate(e.key.keysym.sym, &direction, &plan);
                } else {
                    move(e.key.keysym.sym, &p, &direction, moveSpeed);
                }
            }
        }

        SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 255);
        SDL_RenderClear(instance.renderer);   
        drawMap(&instance);

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
            ray.x = direction.x + plan.x * cameraX;
            ray.y = direction.y + plan.y * cameraX;

            mes.deltaX = (ray.x == 0) ? 1e30 : fabs(1 / ray.x);
            mes.deltaY = (ray.y == 0) ? 1e30 : fabs(1 / ray.y);

            int playerScreenPosX = p.x * TILE_SIZE  + TILE_SIZE / 2;
            int playerScreenPosY = p.y * TILE_SIZE  + TILE_SIZE / 2;

            int endX = playerScreenPosX + ray.x * 50;
            int endY = playerScreenPosY + ray.y * 50;

            SDL_SetRenderDrawColor(instance.renderer, 200, 200, 0, 255);
            SDL_RenderDrawLine(instance.renderer, playerScreenPosX, playerScreenPosY, endX, endY);
        }

        SDL_RenderPresent(instance.renderer);
        SDL_Delay(16); // Roughly 60 FPS
    }

    destroy_instance(&instance);
    return 0;
}
