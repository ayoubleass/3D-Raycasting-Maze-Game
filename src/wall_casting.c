#include "../headers/main.h"
#include <stdio.h>
#include <math.h>


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

void drawWalls(SDL_Instance *instance, Player *p, Direction *direction, Plan *plan ){
    double cameraX;
    int cellValue = 1;
    Measures mes = {.deltaX = 0, .deltaY = 0, .sideX = 0, .sideY = 0};
    RayDirection ray = {.x = 0, .y = 0};
    int hitSide, mapX, mapY, lineHeight, drawStart, drawEnd ;
    double perpWallDist , wallX ,intensity;
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        hitSide = 0;
        cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
        ray.x = direction->x + plan->x * cameraX;
        mes.deltaX = (ray.x == 0) ? 1e30 : fabs(1 / ray.x);
        mes.deltaY = (ray.y == 0) ? 1e30 : fabs(1 / ray.y);
        ray.y = direction->y + plan->y * cameraX;
        mapX = (int)p->x;
        mapY = (int)p->y;
        performDDA(&ray, &mes, p, &mapX, &mapY, &hitSide, &cellValue);
        if (hitSide == 0) {
            perpWallDist = (mes.sideX - mes.deltaX);
        } else {
            perpWallDist = (mes.sideY - mes.deltaY);
        }
        lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
        wallX = (hitSide == 0) ? p->y + perpWallDist * ray.y : p->x + perpWallDist * ray.x;
        wallX -= floor(wallX);
        int textureX = (int)(wallX * 64);
        if ((hitSide == 0 && ray.x > 0) || (hitSide == 1 && ray.y < 0)) {
            textureX = 64 - textureX - 1;
        }
        intensity = 1.0 / (perpWallDist * perpWallDist);
        SDL_Rect srcRect = {textureX, 0, 1, 64}; 
        SDL_Rect destRect = {x, drawStart, 2, drawEnd - drawStart};
        SDL_SetRenderDrawColor(instance->renderer, 255 * intensity, 255 * intensity, 255 * intensity, 255);
        SDL_RenderCopy(instance->renderer, getTexture(cellValue), &srcRect, &destRect);

    }
}



/**
 * getTexture - Returns the appropriate texture based on the cell value.
 * @cellValue: The integer representing the value of a cell in the game map.
 *
 * Return: A pointer to an SDL_Texture corresponding to the given cell value.
 */


SDL_Texture  *getTexture(int cellValue){
    if(cellValue == 1){
        return textures[3];
    }
    if(cellValue == 2){
        return textures[2];
    }
    return textures[7];
 
}
