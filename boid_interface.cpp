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
    static int count = 0;
    ++count;
    this->scene->makeBoid(x,y, count * (count&1? -1 : 1),count * (count%3? -1 : 1));
}
//Does not free renderer
void GameWindow::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::vector<Boid> const boids= scene->getBoids();
    for(Boid const& curboid : boids) {
        double x = curboid.pos.x;
        double y = curboid.pos.y;
        SDL_FRect const rect = {.x=x,.y=y,.w=5,.h=5};
        SDL_RenderDrawRectF(renderer,&rect);
    }
}
