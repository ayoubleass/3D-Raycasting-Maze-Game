#include "../headers/main.h"
#include <stdio.h>



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

void performDDA(RayDirection *ray, Measures *mes, Player *player, int *mapX, int *mapY, int *hitSide, int *cellValue) {
    int stepX, stepY;
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
        if (map[*mapY][*mapX] > 0) {
            *cellValue = map[*mapY][*mapX];
            break;
        }
    }
}