#include <stdio.h>
#include <stdbool.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>

#define PLAYER_SIZE 10 


/**
 * drawMap - Draws the map on the window.
 * @instance: A pointer to the SDL_Instance containing the renderer.
 *
 * This function iterates through the map array and draws each tile.
 * White tiles represent walls, and black tiles represent empty spaces.
 */

void drawMap(SDL_Instance *instance) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            SDL_Rect rect = {x * TILE_SIZE , y  * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            if (map[y][x] >= 1) {
                SDL_SetRenderDrawColor(instance->renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(instance->renderer, 0, 0, 0, 255);
            }
        
            SDL_RenderFillRect(instance->renderer, &rect);
        }
    }
}




/**
 * rotate - Rotates the player's direction and the camera plane.
 * @key: The SDL keycode of the key pressed (left or right arrow key).
 * @direction: A pointer to the player's direction vector.
 * @plane: A pointer to the camera plane vector.
 *
 * This function rotates the direction and plane vectors when the left or
 * right arrow keys are pressed. The rotation speed is constant.
 */



void rotate(int key, Direction *direction, Plan *plane) {
    double rotation_speed = 0.05;
    double cos_rot = cos(rotation_speed);
    double sin_rot = sin(rotation_speed);
    double oldDirX, oldPlaneX;

    switch (key) {
        case SDLK_LEFT:
            {
                oldDirX = direction->x;
                direction->x = direction->x * cos_rot - direction->y * sin_rot;
                direction->y = oldDirX * sin_rot + direction->y * cos_rot;

                oldPlaneX = plane->x;
                plane->x = plane->x * cos_rot - plane->y * sin_rot;
                plane->y = oldPlaneX * sin_rot + plane->y * cos_rot;
            }
            break;

        case SDLK_RIGHT:
            {
                oldDirX = direction->x;
                direction->x = direction->x * cos_rot + direction->y * sin_rot;
                direction->y = -oldDirX * sin_rot + direction->y * cos_rot;

                oldPlaneX = plane->x;
                plane->x = plane->x * cos_rot + plane->y * sin_rot;
                plane->y = -oldPlaneX * sin_rot + plane->y * cos_rot;
            }
            break;
    }
}


/**
 * move - Moves the player forward or backward, handling collisions.
 * @key: The SDL keycode of the key pressed (up or down arrow key).
 * @player: A pointer to the player's position.
 * @direction: A pointer to the player's direction vector.
 * @move_speed: The speed at which the player moves.
 *
 * This function moves the player forward or backward based on the direction
 * vector. It also handles collisions, preventing the player from moving
 * through walls, and allows sliding along walls.
 */

void move(SDL_Keycode key, Player *player, Direction *direction, double move_speed) {
    float newX = player->x;
    float newY = player->y;

    if (key == SDLK_UP) {
        newX += direction->x * move_speed;
        newY += direction->y * move_speed;
    } else if (key == SDLK_DOWN) {
        newX -= direction->x * move_speed;
        newY -= direction->y * move_speed;
    }

    int mapX = (int)newX;
    int mapY = (int)newY;

    if (map[mapY][(int)player->x] == 0) { 
        player->y = newY; 
    }

    if (map[(int)player->y][mapX] == 0) {
        player->x = newX; 
    }
}

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