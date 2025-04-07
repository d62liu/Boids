#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <string>
#include <vector>
#include "boid_main.h"
#include "boid_interface.h"
#include "font8x8_basic.h"
#include <iostream>

//TODO: Doxygen style comments
//the main interface
GameWindow::GameWindow(int w, int h) {
    scene = new Scene(w,h);
    this->FPS = 1;
}
void GameWindow::update(double dt) {
    static float LAST = 0;
    scene->update(dt);
    if(LAST >= 0.05) {
        this->FPS = 1/dt;
        LAST = 0;
    }
    LAST += dt;
}
void GameWindow::placeBird(int x, int y) {
    static unsigned count = 0;
    ++count;
    this->scene->makeBoid(x,y, count * (count&1? -1 : 1),count * (count%3? -1 : 1));
}
void GameWindow::setAllParams(BoidParams params) {
    this->scene->setParams(params);
}
//Does not free renderer

void GameWindow::renderGui(SDL_Renderer *renderer) {
    //fps counter starts at 0,0
    constexpr int FONT_RES = 8;
    constexpr double EPSILON = 1e-10;
    int const FONTSZ = 2;
    int const padding = FONTSZ;
    int const FPS = this->FPS;
    std::string const text_fps = "I love birds + FPS: " + std::to_string(FPS);
    std::vector<SDL_Rect> rectsout;
    //NOTE: idk if i should use sizet or int here, since sizet forces a cast
    for(int charind = 0; charind < text_fps.size(); ++charind) {
        unsigned char const*const curc = font8x8_basic[text_fps[charind]];
        for(int curlayer = 0; curlayer < FONT_RES; ++curlayer) {
            unsigned char const mask = curc[curlayer];
            for(int curbit = 0; curbit < FONT_RES; ++curbit) {
                if(mask & (1<<curbit)) {
                    rectsout.push_back({
                        .x = padding + FONTSZ*(curbit + FONT_RES*charind),
                        .y = padding + FONTSZ* curlayer,
                        .w = FONTSZ,
                        .h = FONTSZ,
                    });
                }
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(renderer, rectsout.data(), rectsout.size());
}

void GameWindow::renderBirds(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    std::vector<Boid> const boids= scene->getBoids();
    constexpr double scale = 0.75;
    constexpr double BOID_HEAD = 7.5 * scale;
    constexpr double BOID_WINGSPAN = scale * 8/2.0;
    constexpr double BOID_TAIL = scale * 10/2.0;
    constexpr double BOID_BUTT = scale * 5/2.0;
    for(Boid const& curboid : boids) {
        double const speed = curboid.vel.mag();
        Vec2 head= BOID_HEAD*curboid.vel/speed;
        Vec2 wing1 = (BOID_WINGSPAN/BOID_HEAD)*Vec2(-head.y, head.x);
        Vec2 wing2= -wing1;
        Vec2 butt = -(BOID_BUTT/BOID_HEAD)*head;
        Vec2 offs = curboid.pos;
        wing1 += offs - (BOID_TAIL/BOID_HEAD)*head;
        wing2 += offs- (BOID_TAIL/BOID_HEAD)*head;
        head += offs;
        butt += offs;

        SDL_Vertex verts[4] = {
            {SDL_FPoint{static_cast<float>(head.x), static_cast<float>(head.y)}, SDL_Color{255,255,255,255}, SDL_FPoint{0}},
            {SDL_FPoint{static_cast<float>(wing1.x), static_cast<float>(wing1.y)}, SDL_Color{255,255,255,255}, SDL_FPoint{0}},
            {SDL_FPoint{static_cast<float>(butt.x), static_cast<float>(butt.y)}, SDL_Color{255,255,255,255}, SDL_FPoint{0}},
            {SDL_FPoint{static_cast<float>(wing2.x), static_cast<float>(wing2.y)}, SDL_Color{255,255,255,255}, SDL_FPoint{0}},
        };
        int const indices[] = {0,1,2,0,3,2};
        SDL_RenderGeometry(renderer, 0, verts, 4, indices, sizeof(indices)/sizeof(indices[0]));
        auto centerofmass = SDL_FRect{static_cast<float>(curboid.pos.x-2), static_cast<float>(curboid.pos.y-2), 4,4};
        SDL_RenderFillRectF(renderer, &centerofmass);
    }
}

void GameWindow::render(SDL_Renderer *renderer) {
    renderBirds(renderer);
    renderGui(renderer);
}
