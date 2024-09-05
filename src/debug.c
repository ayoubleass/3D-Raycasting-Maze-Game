#include <stdio.h>
#include <stdbool.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>


void debug(Plyer p, RayDirection ray){
    int playerScreenPosX = p.x * TILE_SIZE  + TILE_SIZE / 2;
    int playerScreenPosY = p.y * TILE_SIZE  + TILE_SIZE / 2;

    int endX = playerScreenPosX + ray.x * 30;
    int endY = playerScreenPosY + ray.y * 30;

    SDL_SetRenderDrawColor(instance.renderer, 200, 200, 0, 255);
    SDL_RenderDrawLine(instance.renderer, playerScreenPosX, playerScreenPosY, endX, endY);



}