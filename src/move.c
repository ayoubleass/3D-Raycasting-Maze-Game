#include "../headers/main.h"


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
    int mapX, mapY; 
    float newX = player->x;
    float newY = player->y;
    if (key == SDLK_UP) {
        newX += direction->x * move_speed;
        newY += direction->y * move_speed;
    } else if (key == SDLK_DOWN) {
        newX -= direction->x * move_speed;
        newY -= direction->y * move_speed;
    }

    mapX = (int)newX;
    mapY = (int)newY;
    if (map[mapY][(int)player->x] == 0) { 
        player->y = newY; 
    }
    if (map[(int)player->y][mapX] == 0) {
        player->x = newX; 
    }
}