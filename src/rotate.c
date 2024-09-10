#include "../headers/main.h"
#include <math.h>

/**
 * rotate - Rotates the player's direction and the camera plane.
 * @key: The SDL keycode of the key pressed (left or right arrow key).
 * @direction: A pointer to the player's direction vector.
 * @plane: A pointer to the camera plane vector.
 *
 * This function rotates the direction and plane vectors when the left or
 * right arrow keys are pressed. The rotation speed is constant.
 */




/*void move(SDL_Keycode key, Player *player, Direction *direction, double move_speed)*/

void rotateAndMove(int key, Player *p , Direction *direction, Plan *plane, double move_speed) {
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
        case SDLK_UP:
            move(key, p, direction, move_speed); 
            break;
        case SDLK_DOWN:
            move(key, p, direction, move_speed); 
            break;
    }
}