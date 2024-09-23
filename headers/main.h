#ifndef MAZE_H
#define MAZE_H

#include <SDL.h>
#include <SDL_image.h>



#define mapWidth 24
#define mapHeight 24

#define TILE_SIZE 20
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define texWidth 64
#define texHeight 64


#define NUM_TEXTURES 10

#define numSprites 19
extern int map[mapWidth][mapHeight];
extern char *TexturePaths[NUM_TEXTURES];
extern SDL_Texture *textures[NUM_TEXTURES];
extern double zBuffer[SCREEN_WIDTH];
extern double rotation_speed;
/**
 * struct SDL_Instance - Contains all SDL-related elements for rendering
 * @window: Pointer to the SDL window
 * @screenSurface: Pointer to the SDL surface
 * @renderer: Pointer to the SDL renderer
 */

typedef struct {
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer* renderer;
}SDL_Instance;


/**
 * struct Player - Defines the player’s position on the map
 * @x: X-coordinate of the player
 * @y: Y-coordinate of the player
 */
typedef struct {
    double x;
    double y;
} Player;


/**
 * struct Direction - Represents a 2D direction vector
 * @x: X-component of the direction
 * @y: Y-component of the direction
 */

typedef struct{
    double x;
    double y;
}Direction;




/**
 * struct RayDirection - Defines the direction of the ray being cast
 * @x: X-component of the ray's direction
 * @y: Y-component of the ray's direction
 */
typedef struct{
    double x;
    double y; 
} RayDirection;


/**
 * struct Plan - Represents the camera plane (2D plane perpendicular to the direction)
 * @x: X-component of the camera plane
 * @y: Y-component of the camera plane
 */
typedef struct {
    double x;
    double y;
}Plan;


/**
 * struct Measures - Holds values related to raycasting calculations
 * @deltaX: Difference in X for raycasting
 * @deltaY: Difference in Y for raycasting
 * @sideX: X-coordinate where a ray crosses a grid line
 * @sideY: Y-coordinate where a ray crosses a grid line
 */

typedef struct 
{
    double deltaX;
    double deltaY;
    double sideX;
    double sideY;
} Measures;






int init_instance(SDL_Instance * instance);
void destroy_instance(SDL_Instance *instance);
void performDDA(RayDirection *ray, Measures *mes, Player *player, int *mapX, int *mapY, int *hitSid, int *cellValue);
void rotateAndMove(const Uint8 *keyState, Player *p, Direction *direction, Plan *plane, double move_speed) ;
void rotate_left(Direction *direction, Plan *plane);
void rotateRight(Direction *direction, Plan *plane);
void renderCeilAndGround(SDL_Instance *instance, SDL_Texture *groundTexture, Player *p, Direction direction, Plan plan);
void ceilTexture(SDL_Instance *instance, SDL_Texture *ceilingTexture, Player *p, Direction direction, Plan plan);
void move(SDL_Keycode key, Player *player, Direction *direction, double move_speed);
SDL_Texture *loadTexture(SDL_Instance *instance, const char *path);
void initTexturePaths();
void freeTexturePaths();
void drawWalls(SDL_Instance *instance, Player *p, Direction *direction, Plan *plan );
int setTexture(SDL_Instance *instance, char* paths[NUM_TEXTURES], int size);
void renderWeapon(SDL_Instance *instance, SDL_Texture *texture);
SDL_Texture *loadTexture(SDL_Instance *instance, const char *path);
void getTextureSize(SDL_Texture *texture, int *width, int *height);
SDL_Texture  *getTexture(int cellValue);
void freeTextures();

#endif