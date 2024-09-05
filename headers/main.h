#ifndef MAZE_H
#define MAZE_H

#include <SDL.h>



#define mapWidth 24
#define mapHeight 24

#define TILE_SIZE 20
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 500


extern int map[mapWidth][mapHeight];
extern int color;

typedef struct {
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;
}SDL_Instance;


typedef struct {
    double x;
    double y;
} Player;


typedef struct{
    double x;
    double y;
}Direction;



typedef struct {
    double directionX;
    double directionY;
    double planX;
    double planY;
} MovementVectors;


typedef struct{
    double x;
    double y; 
} RayDirection;


typedef struct {
    double x;
    double y;
}Plan;



typedef struct 
{
    double deltaX;
    double deltaY;
    double sideX;
    double sideY;
} Measures;



int init_instance(SDL_Instance * instance);
void destroy_instance(SDL_Instance *instance);
void drawMap(SDL_Instance *instance);
void performDDA(RayDirection *ray, Measures *mes, Player *p, int *mapX, int *mapY, int *hitSide);
void rotate(int key, Direction *direction, Plan *plane) ;
void rendreceilAndGround(SDL_Instance *instance);
void move(int key, Player *player, Direction *direction, double move_speed);
#endif