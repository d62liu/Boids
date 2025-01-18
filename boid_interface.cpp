#include <SDL2/SDL.h>
#include <vector>
#include "boid_main.h"
#include "boid_interface.h"

//TODO: Doxygen style comments
//the main interface
GameWindow::GameWindow(int w, int h) {
    scene = new Scene(w,h);
}
void GameWindow::update(double dt) {
    scene->update(dt);
}
//Does not free renderer
void GameWindow::render(SDL_Renderer *const renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::vector<Boid> const boids= scene->getBoids();
    for(Boid const& curboid : boids) {
        double x = curboid.pos.x;
        double y = curboid.pos.y;
        SDL_RenderDrawPoint(renderer, x, y);
    }
}
