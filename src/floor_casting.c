#include <stdio.h>
#include <stdbool.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>
#include <string.h>

/**
 * setTexture - Recursively loads textures into wallTextures array
 * @instance: Pointer to the SDL instance that contains the renderer
 * @paths: Array of file paths for the textures
 * @size: Current index of the texture being loaded
 * 
 * This function loads textures from the provided paths array into the 
 * wallTextures array recursively. If a texture fails to load, it prints
 * an error message and returns the current size of the texture array.
 * 
 * Return: The current size of the texture array.
 */

int setTexture(SDL_Instance *instance, char* paths[NUM_TEXTURES], int size) {
    if (size >= NUM_TEXTURES) {
        return size;
    }
    textures[size] = loadTexture(instance, paths[size]);
    if (textures[size] == NULL) {
        printf("Failed to load texture at index %d: %s\n", size, paths[size]);
        return size; 
    }
    return setTexture(instance, paths, size + 1);
}


/**
 * loadTexture - Loads a texture from a file path
 * @instance: Pointer to the SDL instance that contains the renderer
 * @path: The file path to the texture
 * 
 * This function loads an image from the specified path and converts it 
 * to an SDL texture that can be rendered. If it fails to load the image 
 * or create the texture, it prints an error message.
 * 
 * Return: The loaded SDL texture, or NULL if loading failed.
 */

SDL_Texture *loadTexture(SDL_Instance *instance, const char *path) {
    SDL_Surface *tempSurface = IMG_Load(path); 
    if (!tempSurface) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(instance->renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!texture) {
        printf("Failed to create texture: %s\n", IMG_GetError());
    }
    return texture;
}

/**
 * renderCeilAndGround - Renders the ceiling and ground textures
 * @instance: Pointer to the SDL instance that contains the renderer
 * @groundTexture: SDL texture to be used for the ground
 * @p: Pointer to the player position
 * @direction: Direction of the player
 * @plan: Projection plane for raycasting
 * 
 * This function renders the ground textures by calculating the floor
 * projection based on the player's position and direction. It also 
 * renders the ceiling by calling the `ceilTexture()` function.
 */


void renderCeilAndGround(SDL_Instance *instance, SDL_Texture *groundTexture, Player *p, Direction direction, Plan plan) {
    RayDirection ray0 = {.x = direction.x - plan.x , .y= direction.y - plan.y};
    RayDirection ray1 = {.x = direction.x + plan.x , .y= direction.y + plan.y};
    ceilTexture(instance,textures[9], p, direction, plan);
    int y = SCREEN_HEIGHT / 2;
    if (groundTexture) {
        for (; y < SCREEN_HEIGHT; y++) {
            int pY = y - SCREEN_HEIGHT / 2;  
            double rowDistance = (0.5 * SCREEN_HEIGHT) / pY;
            double floorStepX = rowDistance *  (ray1.x - ray0.x) / SCREEN_WIDTH;
            double floorStepY = rowDistance *  (ray1.y - ray0.y) / SCREEN_WIDTH; 
            double floorX = p->x + rowDistance * ray0.x;
            double floorY = p->y + rowDistance * ray0.y;
            if(y > 240){
                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    int cellX = (int)floorX;
                    int cellY = (int)floorY;
                    int tx = (int)(64 * (floorX - cellX)) & (64 - 1);
                    int ty = (int)(64 * (floorY - cellY)) & (64 - 1);
                    floorX += floorStepX;
                    floorY += floorStepY;
                    SDL_Rect srcRect = {tx, ty, 1, 1};
                    SDL_Rect destRect = {x, y, 1, 1};
                    SDL_RenderCopy(instance->renderer,textures[1], &srcRect, &destRect);
                }
            }
            

        }
    }
}



/**
 * renderGun - Renders a gun texture at the bottom of the screen
 * @instance: Pointer to the SDL instance that contains the renderer
 * @gunTexture: SDL texture of the gun
 * @dir: Pointer to the player's direction (used for rotating the gun)
 * @gunTextureWidth: Width of the gun texture
 * @gunTextureHeight: Height of the gun texture
 * 
 * This function renders the gun texture in the middle bottom of the screen. 
 * It adds a slight movement to the gun based on the time and rotates it based 
 * on the player's direction.
 */
void renderGun(SDL_Instance *instance, SDL_Texture *gunTexture, int gunTextureWidth, int gunTextureHeight) {
    if (gunTexture == NULL) {
        printf("Gun texture is NULL.\n");
        return;
    }

    // Calculate the position at the bottom center of the screen
    int gunX = SCREEN_WIDTH / 2 - gunTextureWidth / 2;
    int gunY = SCREEN_HEIGHT - gunTextureHeight;

    // Define source and destination rectangles
    SDL_Rect srcRect = {0, 0, gunTextureWidth, gunTextureHeight };
    SDL_Rect destRect = {gunX, gunY, gunTextureWidth, gunTextureHeight };

    // Render the gun texture without rotation
    SDL_RenderCopyEx(instance->renderer, gunTexture, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);
    
}
