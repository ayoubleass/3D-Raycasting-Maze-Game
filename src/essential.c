#include <stdio.h>
#include <stdbool.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>


/**
 * rendreceilAndGround - Renders the sky and ground on the window.
 * @instance: A pointer to the SDL_Instance containing the renderer.
 *
 * This function clears the renderer with a sky color and then draws
 * a rectangle to represent the ground.
 */
// void rendreceilAndGround(SDL_Instance *instance){
//         SDL_SetRenderDrawColor(instance->renderer, 135, 206, 235, 0); 
//         SDL_RenderClear(instance->renderer);
//         SDL_Rect groundRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
//         SDL_SetRenderDrawColor(instance->renderer, 65, 152, 10, 0);
//         SDL_RenderFillRect(instance->renderer, &groundRect);
// }


/**
 * performDDA - Performs Digital Differential Analysis (DDA) to detect wall hits.
 * @ray: A pointer to the RayDirection structure containing the ray's direction.
 * @mes: A pointer to the Measures structure containing DDA variables.
 * @player: A pointer to the player's position.
 * @mapX: A pointer to the current x-coordinate on the map.
 * @mapY: A pointer to the current y-coordinate on the map.
 * @hitSide: A pointer to the variable indicating which side of the wall was hit.
 *
 * This function uses the DDA algorithm to step through the grid until a wall is hit.
 * The function updates the mapX, mapY, and hitSide variables accordingly.
 */

void performDDA(RayDirection *ray, Measures *mes, Player *player, int *mapX, int *mapY, int *hitSide) {
    int stepX, stepY;
    double initSideX = mes->sideX;
    double initSideY = mes->sideY;

    if (ray->x < 0) {
        stepX = -1;
        mes->sideX = (player->x - *mapX) * mes->deltaX;
    } else {
        stepX = 1;
        mes->sideX = (*mapX + 1.0 - player->x) * mes->deltaX;
    }
    if (ray->y < 0) {
        stepY = -1;
        mes->sideY = (player->y - *mapY) * mes->deltaY;
    } else {
        stepY = 1;
        mes->sideY = (*mapY + 1.0 - player->y) * mes->deltaY;
    }

    while (1) {
        if (mes->sideX < mes->sideY) {
            mes->sideX += mes->deltaX;
            *mapX += stepX;
            *hitSide = 0;
        } else {
            mes->sideY += mes->deltaY;
            *mapY += stepY;
            *hitSide = 1;
        }
        if (map[*mapY][*mapX] > 0) break;
    }
}




SDL_Texture *loadTexture(SDL_Instance *instance, const char *path) {
    SDL_Surface *tempSurface = SDL_LoadBMP(path); 
    if (!tempSurface) {
        printf("Failed to load texture: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(instance->renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
    }
    return texture;
}


void renderCeilAndGround(SDL_Instance *instance, SDL_Texture *groundTexture, Player *p, Direction direction, Plan plan) {
    SDL_SetRenderDrawColor(instance->renderer, 135, 206, 235, 255); 
    SDL_RenderClear(instance->renderer);
    SDL_Rect groundRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    int y = SCREEN_HEIGHT / 2;
    if (groundTexture) {
        for (; y < SCREEN_HEIGHT; y++) {
            double rayDirX0 = direction.x - plan.x;
            double rayDirX1 = direction.x + plan.x;
            double rayDirY0 = direction.y - plan.y;
            double rayDirY1 = direction.y + plan.y;
            int pY = y - SCREEN_HEIGHT / 2;  
            double posZ = 0.5 * SCREEN_HEIGHT;      
            double rowDistance = posZ / pY;
            double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
            double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;
            /*printf("rowDistance  : %lf\n", rowDistance);*/
            double floorX = p->x + rowDistance * rayDirX0;
            double floorY = p->y + rowDistance * rayDirY0;
            
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                int cellX = (int)floorX;
                int cellY = (int)floorY;              
                int tx = (int)(64 * (floorX - cellX)) & (64 - 1);
                int ty = (int)(64 * (floorY - cellY)) & (64 - 1);
                floorX += floorStepX;
                floorY += floorStepY;
                SDL_Rect srcRect = {tx, ty, 1, 1};
                SDL_Rect destRect = {x, y, 1, 1};
                SDL_RenderCopy(instance->renderer, groundTexture, &srcRect, &destRect);
            }
        }
    }
}