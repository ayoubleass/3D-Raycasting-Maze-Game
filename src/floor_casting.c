#include <stdio.h>
#include <stdbool.h>
#include "../headers/main.h"
#include <math.h>
#include<windows.h> 
#include<unistd.h>
#include <string.h>


int setTexture(SDL_Instance *instance, char* paths[NUM_TEXTURES], int size) {
    if (size >= NUM_TEXTURES) {
        return size;
    }
    wallTextures[size] = loadTexture(instance, paths[size]);
    if (wallTextures[size] == NULL) {
        printf("Failed to load texture at index %d: %s\n", size, paths[size]);
        return size; 
    }
    return setTexture(instance, paths, size + 1);
}



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


void renderCeilAndGround(SDL_Instance *instance, SDL_Texture *groundTexture, Player *p, Direction direction, Plan plan) {
    RayDirection ray0 = {.x = direction.x - plan.x , .y= direction.y - plan.y};
    RayDirection ray1 = {.x = direction.x + plan.x , .y= direction.y + plan.y};
    ceilTexture(instance,groundTexture, p, direction, plan);
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
                    SDL_RenderCopy(instance->renderer, wallTextures[3], &srcRect, &destRect);
                }
            }
            

        }
    }
}




void renderGun(SDL_Instance *instance, SDL_Texture *gunTexture,  Direction *dir ,int gunTextureWidth, int gunTextureHeight) {
    int gunX = SCREEN_WIDTH / 2 - gunTextureWidth / 2;
    int gunY = SCREEN_HEIGHT - gunTextureHeight;
    gunX += sin(SDL_GetTicks() / 1000.0) * 5; 
    double angle = atan2(dir->y, dir->x) * 180.0 / M_PI;

    SDL_Rect srcRect = {0, 0, gunTextureWidth, gunTextureHeight};
    SDL_Rect destRect = {gunX, gunY, gunTextureWidth, gunTextureHeight};
    SDL_RenderCopyEx(instance->renderer, gunTexture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}