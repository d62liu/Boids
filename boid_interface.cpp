#include <SDL2/SDL.h>
#include <vector>
#include "boid_main.h"
#include "boid_interface.h"
#include <iostream>

//TODO: Doxygen style comments
//the main interface
GameWindow::GameWindow(int w, int h) {
    scene = new Scene(w,h);
}
void GameWindow::update(double dt) {
    scene->update(dt);
}
void GameWindow::placeBird(int x, int y) {
    this->scene->makeBoid(x,y, 1,1);
}
//Does not free renderer
void GameWindow::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::vector<Boid> const boids= scene->getBoids();
    for(Boid const& curboid : boids) {
        double x = curboid.pos.x;
        double y = curboid.pos.y;
        SDL_Rect const rect = {.x=(int)x,.y=(int)y,.w=10,.h=10};
        SDL_RenderDrawRect(renderer,&rect);
    }
}
