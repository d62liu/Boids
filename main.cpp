#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "SDL2/SDL.h"
#include "boid_interface.h"
int main(int argc, char*argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    #define WIDTH 400
    #define HEIGHT 400
    SDL_Window *window = SDL_CreateWindow("test",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    if(!window) fprintf(stderr, "could not make window: %s" "\n", SDL_GetError());
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,0);
    if(!renderer) fprintf(stderr, "could not make renderer: %s" "\n", SDL_GetError()); 

    if(!(renderer && window)) return EXIT_FAILURE;

    SDL_Event window_event;
    GameWindow *gamewindow = new GameWindow(WIDTH, HEIGHT);
    double dt =0;
    size_t LAST = 0;
    size_t NOW = SDL_GetPerformanceCounter();
        gamewindow->placeBird(10, 10);
        gamewindow->placeBird(20, 20);
        gamewindow->placeBird(30, 30);
        gamewindow->placeBird(40, 40);
    while(1) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        dt = (double)(NOW-LAST)/(double)SDL_GetPerformanceFrequency();
        if(SDL_PollEvent(&window_event)) {
            if(window_event.type == SDL_QUIT) break;
        }
        gamewindow->update(dt);
        gamewindow->render(renderer);
        SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer,0, 0,0, 255);
        SDL_RenderClear(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return EXIT_SUCCESS;
}
