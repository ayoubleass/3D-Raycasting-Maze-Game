#include <stdio.h>
#include <math.h>
#include "../headers/main.h"


/**
 * init_instance - Initializes an SDL instance.
 * @instance: A pointer to an SDL_Instance structure to initialize.
 *
 * This function initializes the SDL library and creates a window
 * and a renderer. If any step of the initialization fails, it prints
 * an error message, frees allocated resources, and quits SDL.
 *
 * Return: 1 on success, 0 on failure.
 */
int init_instance(SDL_Instance * instance) {
    
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return (0);
    }
    instance->window = SDL_CreateWindow( "Maze project", 
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if(instance->window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return (0);   
    }
    /* Initialize renderer color*/
    instance->renderer = SDL_CreateRenderer(instance->window, -1, 0);
    if(instance->renderer == NULL) {
        printf("renderer  could not be created! SDL_Error: %s\n",  SDL_GetError());
        return (0);  
    }
    return (1);
}


/**
 * destroy_instance - Destroys an SDL instance and cleans up resources.
 * @instance: A pointer to an SDL_Instance structure to destroy.
 *
 * This function destroys the SDL window and renderer associated with the given instance
 * and cleans up any allocated resources. It then quits the SDL library.
 */

void destroy_instance(SDL_Instance *instance)
{
    SDL_DestroyRenderer(instance->renderer);
    SDL_DestroyWindow( instance->window );
    instance->renderer = NULL;
    instance->window = NULL;
    SDL_Quit();
}



