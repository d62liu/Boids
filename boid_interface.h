#ifndef BOID_INTERFACE
#define BOID_INTERFACE
#include "boid_main.h"
#include "SDL2/SDL.h"
class GameWindow {
    Scene *scene;
    public:
    GameWindow(int w, int h);
    void update(double dt);
    //Does not free renderer
    void render(SDL_Renderer *const renderer);
};
#endif