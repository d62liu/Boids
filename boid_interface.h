#ifndef BOID_INTERFACE
#define BOID_INTERFACE
#include "boid_main.h"
#include "SDL2/SDL.h"
class GameWindow {
private:
    Scene *scene;
    int FPS; // used for fps display only
    void renderGui(SDL_Renderer *renderer);
    void renderBirds(SDL_Renderer *renderer);
public:
    GameWindow(int w, int h);
    void placeBird(int x,int y);
    void update(double dt);
    //Does not free renderer
    void render(SDL_Renderer *renderer);
};
#endif
