#include <cmath>
#include <vector>
#include <iostream>
#include "boid_main.h"

//Don't include sdl2 

//vector arithmetic is done naturally, so its ok to ignore this class and just use it.
Vec2::Vec2(void) {
    this->x = 0;
    this->y = 0;
}
Vec2::Vec2(double x, double y) {
    this->x = x;
    this->y = y;
}
double Vec2::mag(void) const { 
    return (sqrt(x*x + y*y));//(0.96043387*std::max(x,y) + 0.397824734*std::min(x,y));
}
Vec2& Vec2::operator+=(Vec2 const& v) {
    this->x += v.x;
    this->y += v.y;
    return *this;
}
Vec2 operator+(Vec2 lhs, Vec2 const& rhs){
    lhs += rhs;
    return lhs;
}
Vec2& Vec2::operator-=(Vec2 const& v) {
    this->x -= v.x;
    this->y -= v.y;
    return *this;
}
Vec2 operator-(Vec2 lhs, Vec2 const& rhs){
    lhs -= rhs;
    return lhs;
}
Vec2 Vec2::operator-(void) const {
    return Vec2(-x,-y);
}
/*
double operator*(Vec2 const& v) const { //dot product
    return x*v.x + y*v.y;
}
*/
Vec2& Vec2::operator*=(double const scale) { 
    this->x *= scale;
    this->y *= scale;
    return *this;
}
Vec2 operator*(Vec2 v, double const scale) { 
    v *= scale;
    return v;
}
Vec2 operator*(double const scale, Vec2 v) { 
    v *= scale;
    return v;
}
Vec2& Vec2::operator/=(double const scale) {
    this->x/= scale;
    this->y/=scale;
    return *this;
}

Vec2 operator/(Vec2 v, double const scale){
    v/= scale;
    return v;
}
Vec2 operator/(double const scale, Vec2 v){
    v/= scale;
    return v;
}


BoidParams::BoidParams(void) {
    this->view_radius = 600;
    this->protected_radius = 20,
    this->avoid_factor = 0.1, 
    this->matching_factor = 0.005, 
    this->centering_factor = 0.0005, 
    this->turn_accel = 5,
    this->min_speed= 400, 
    this->max_speed = 600;
}

Boid::Boid(Vec2 pos) {
    this->pos = pos;
    this->vel = Vec2();
    this->params = BoidParams();
}
Boid::Boid (Vec2 pos, Vec2 vel) {
    this->pos = pos;
    this->vel = vel;
    this->params = BoidParams();
}

Scene::Scene(int w, int h) {
    this->width = w; 
    this->height = h; 
}

std::vector<Boid> const& Scene::getBoids() {
    return birds;
}

void Scene::makeBoid(double x, double y){
    birds.push_back(Boid(Vec2(x,y), Vec2()));
}
void Scene::makeBoid(double x, double y, double vx, double vy){
    birds.push_back(Boid(Vec2(x,y), Vec2(vx, vy)));
}

void Scene::setParams(BoidParams params) {
    for(auto & bird : this->birds) {
        bird.params = params;
    }
}

struct AABB{
    double x, y, width, height;
    AABB(double center_x, double center_y, double half_width, double half_height){
        x = center_x;
        y = center_y;
        width = half_height;
        height = half_width; 
    }
        bool contains(Vec2 p){
            return (p.x >= x - width && p.x <= x + width
                    && p.y >= y - height && p.y <= y + height);
        }
    bool intersects(AABB other){
        return  !(other.x - other.width > x + width ||
                    other.x + other.width < x - width ||
                    other.y - other.height > y + height ||
                    other.y + other.height < y - height);
    };
};

class QuadTree{
    AABB boundary;
    Vec2 boid;
    bool divided;
    QuadTree* North_East;
    QuadTree* North_West;
    QuadTree* South_East;
    QuadTree* South_West;
    QuadTree(AABB bound, bool divided = false, QuadTree* NE = nullptr, QuadTree* NW = nullptr, QuadTree* SE = nullptr, QuadTree* SW = nullptr){
        

    }

}









void Scene::update(double const dt) {
    int const top_border = this->height-1;
    int const right_border = this->width-1;
    int const bottom_border = 0;
    int const left_border = 0;

    size_t const sz = birds.size();
    for(size_t i = 0; i < sz; ++i) {
        Vec2 p_tot = Vec2(); //Cohesion
        //TODO: there has to be a better name for this
        Vec2 distance = Vec2();//Separation
        Vec2 v_tot = Vec2(); //Alignment
        size_t num_neighbors = 0;
        for(size_t j = 0; j < sz; ++j) {
            //for the current time frame.
            Vec2 const& p1 = birds[i].pos;
            Vec2 const& v1 = birds[i].vel;
            Vec2 const& p2 = birds[j].pos;
            Vec2 const& v2 = birds[j].vel;

            if(((p2-p1).mag()) <= birds[i].params.protected_radius)
                distance += (p2 - p1);  //setting variables for Separation

            birds[i].vel -= dt * distance * birds[i].params.avoid_factor; //calculation for Separation

            //TODO: fix overflow error using "rolling" average
            if((p1-p2).mag() < birds[i].params.view_radius) { //setting variables for Cohesion and Alignment
                p_tot += p2;
                v_tot += v2;
                ++num_neighbors;
            }

            if (num_neighbors > 0) { //Calculation for Cohesion and Alignment
                Vec2 const p_avg = p_tot/num_neighbors;
                Vec2 const v_avg = v_tot/num_neighbors;
                birds[i].vel +=  dt * (p_avg - p1) * birds[i].params.centering_factor;
                birds[i].vel +=  dt * (v_avg - v1) * birds[i].params.matching_factor;

            }

            double const& wall_radius = 200;
            double const& turn_accel = birds[i].params.turn_accel;

            if (p1.x - wall_radius < left_border) birds[i].vel.x  += turn_accel * dt;
            if (p1.x +wall_radius > right_border) birds[i].vel.x -= turn_accel* dt;
            if (p1.y - wall_radius < bottom_border) birds[i].vel.y += turn_accel* dt;
            if (p1.y + wall_radius > top_border) birds[i].vel.y -= turn_accel* dt;


            double const& reqspeed = birds[i].vel.mag(); 
            constexpr double EPSILON = 1e-9;
            if (reqspeed < birds[i].params.min_speed)
                birds[i].vel *= birds[i].params.min_speed/(reqspeed+EPSILON);
            if (reqspeed > birds[i].params.max_speed)
                birds[i].vel *= birds[i].params.max_speed/(reqspeed+EPSILON);

        }
    }
    //update positions
    for(size_t i = 0; i < sz; ++i) {
        birds[i].pos += birds[i].vel * dt;
    }
}
