#ifndef BOID_INTERFACE
#define BOID_INTERFACE
#include "boid_main.h"
#include "SDL2/SDL.h"
class GameWindow {
    Scene *scene;
    public:
    GameWindow(int w, int h);
    void placeBird(int x,int y);
    void update(double dt);
    //Does not free renderer
    void render(SDL_Renderer *renderer);
};
#endif
