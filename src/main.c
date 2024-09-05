#include "../headers/main.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include<windows.h> 
#include<unistd.h>



int color = 0;
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






int main(int argc, char **argv) {
    (void)argc; (void)argv; 
    SDL_Instance instance;
    SDL_Event e;
    Player p = {22, 12};
    Direction direction = {.x = -1, .y = 0};
    Plan plan = {.x = 0, .y = 0.66};
    Measures mes = {.deltaX = 0, .deltaY = 0, .sideX = 0, .sideY = 0};
    RayDirection ray = {.x = 0, .y = 0};
    double perpWallDist,time = 0, oldTime = 0,   cameraX,   frameTime, moveSpeed;
    int  hitSide,mapX, mapY, game_running = 1;


    if (!init_instance(&instance)) return 1;
    while (game_running) {

        oldTime = time;
        time = SDL_GetTicks();
        frameTime = (time - oldTime) / 1000.0;
        moveSpeed = frameTime * 15.0;
        while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    game_running = 0;
                }
                if(e.type == SDL_KEYDOWN){
                    rotate(e.key.keysym.sym, &direction, &plan);
                    move(e.key.keysym.sym, &p, &direction, moveSpeed);
                }
        }
        /**/
        rendreceilAndGround(&instance);
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
            ray.x = direction.x + plan.x * cameraX;
            ray.y = direction.y + plan.y * cameraX;
            mes.deltaX = (ray.x == 0) ? 1e30 : fabs(1 / ray.x);
            mes.deltaY = (ray.y == 0) ? 1e30 : fabs(1 / ray.y);
            mapX = (int) p.x;
            mapY = (int) p.y;
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
            SDL_SetRenderDrawColor(instance.renderer, 99, 99, 99, 0);
            SDL_RenderDrawLine(instance.renderer, x, drawStart, x, drawEnd);
        }
        SDL_RenderPresent(instance.renderer);
        system("cls"); 

    }
    destroy_instance(&instance);
    return 0;
}

