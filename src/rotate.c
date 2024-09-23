#include "../headers/main.h"
#include <math.h>

/**
 * rotate - Rotates the player's direction and the camera plane.
 * @key: The SDL keycode of the key pressed (left or right arrow key).
 * @direction: A pointer to the player's direction vector.
 * @plane: A pointer to the camera plane vector.
 *
 * This function rotates the direction and plane vectors when the left or
 * right arrow keys are pressed. 
 */


void rotateAndMove(const Uint8 *keyState, Player *p, Direction *direction, Plan *plane, double move_speed) {
    if (keyState[SDL_SCANCODE_LEFT]) {
        rotate_left(direction, plane);
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        rotateRight(direction,plane);
    }
    if (keyState[SDL_SCANCODE_UP]) {
        move(SDLK_UP, p, direction, move_speed);
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        move(SDLK_DOWN, p, direction, move_speed);  
    }   
}


/**
 * rotate_left - Rotates the direction and plane vectors to the left.
 * @direction: Pointer to the direction structure, representing the player's direction.
 * @plane: Pointer to the plane structure, representing the camera plane.
 */

void rotate_left(Direction *direction, Plan *plane){
    double oldDirX, oldPlaneX;
    double cos_rotation = cos(rotation_speed);
    double sin_rotation = sin(rotation_speed);

    oldDirX = direction->x;
    direction->x = direction->x * cos_rotation - direction->y * sin_rotation;
    direction->y = oldDirX * sin_rotation + direction->y * cos_rotation;
    oldPlaneX = plane->x;
    plane->x = plane->x * cos_rotation - plane->y * sin_rotation;
    plane->y = oldPlaneX * sin_rotation + plane->y * cos_rotation;
}

/**
 * rotate_right - Rotates the direction and plane vectors to the right.
 * @direction: Pointer to the direction structure, representing the player's direction.
 * @plane: Pointer to the plane structure, representing the camera plane.
 */
void rotateRight(Direction *direction, Plan *plane){
    double oldDirX, oldPlaneX;
    double cos_rotation = cos(rotation_speed);
    double sin_rotation = sin(rotation_speed);

    oldDirX = direction->x;
    direction->x = direction->x * cos_rotation + direction->y * sin_rotation;
    direction->y = -oldDirX * sin_rotation + direction->y * cos_rotation;
    oldPlaneX = plane->x;
    plane->x = plane->x * cos_rotation + plane->y * sin_rotation;
    plane->y = -oldPlaneX * sin_rotation + plane->y * cos_rotation;
}