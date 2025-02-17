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
    double left_bound, right_bound, top_bound, bot_bound;
    AABB(double left, double right, double top, double bot){
        left_bound = left;
        right_bound = right;
        top_bound = top;
        bot_bound = bot; 
    }
    bool contains(Boid b){
        return (b.pos.x >= left_bound && b.pos.x <= right_bound
                && b.pos.y >= bot_bound&& b.pos.y <= top_bound);
        }
    bool intersects(const AABB& other) const {
        return (left_bound <= other.right_bound && right_bound >= other.left_bound &&
                bot_bound  <= other.top_bound   && top_bound >= other.bot_bound);
    };
}

class QuadTree{
public:
    AABB boundary;
    Boid boid;
    bool divided = false;
    std::vector<Boid> birds;
    QuadTree* North_East = 0;
    QuadTree* North_West = 0;
    QuadTree* South_East = 0;
    QuadTree* South_West = 0;
    QuadTree(const AABB& bound)
    : boundary(bound), divided(false),
      North_East(nullptr), North_West(nullptr),
      South_East(nullptr), South_West(nullptr) {}
    ~QuadTree() {
        delete North_East;
        delete North_West;
        delete South_East;
        delete South_West;
    }
        bool insert(Boid& boid) {
            if (!boundary.contains(boid)) {
                return false;
            }
            if (birds.size() < 4) {
                birds.push_back(boid);
                return true;
            }
            if (!divided) {
                subdivide();
            }
            if (North_East->insert(boid)) return true;
            if (North_West->insert(boid)) return true;
            if (South_East->insert(boid)) return true;
            if (South_West->insert(boid)) return true;

            return false;
        }

        void subdivide(){
            double new_width = boundary.width/2;
            double new_height= boundary.height/2;
            double width  = boundary.right_bound - boundary.left_bound;
            double height = boundary.top_bound - boundary.bot_bound;
            double midX   = (boundary.left_bound + boundary.right_bound) / 2.0;
            double midY   = (boundary.top_bound  + boundary.bot_bound) / 2.0;            
            AABB ne(midX, boundary.right_bound, boundary.top_bound, midY);
            AABB nw(boundary.left_bound, midX, boundary.top_bound, midY);
            AABB se(midX, boundary.right_bound, midY, boundary.bot_bound);
            AABB sw(boundary.left_bound, midX, midY, boundary.bot_bound);
            
            North_East = new QuadTree(ne);
            North_West = new QuadTree(nw);
            South_East = new QuadTree(se);
            South_West = new QuadTree(sw);
            divided = true;
            }


        std::vector<Boid> query(const AABB& quadrant) const {
            std::vector<Boid> found;
            if (!boundary.intersects(quadrant)){
                return found;
            }else{
                 for (auto& b : birds) {
                    if (quadrant.contains(b.pos)) {
                        found.push_back(b);
                    }
                }
                if (!divided) return found;
                auto q = North_East->query(quadrant);
                found.insert(found.end(), q.begin(), q.end());
                q = North_West->query(quadrant);
                found.insert(found.end(), q.begin(), q.end());
                q = South_East->query(quadrant);
                found.insert(found.end(), q.begin(), q.end());
                q = South_West->query(quadrant);
                found.insert(found.end(), q.begin(), q.end());
                return found;
        }
        }
        std::vector<Boid> radius_query(int radius, const Boid& bird) const{
            std::vector<Boid> result;
            AABB bound_box(bird.pos.x - radius, bird.pos.x + radius, 
                           bird.pos.y + radius, bird.pos.y - radius);
            if (!boundary.intersects(bound_box)){
                return result;
            }
            if (!divided){
                for (const auto& b : birds) {
                    double dx = b.pos.x - bird.pos.x;
                    double dy = b.pos.y - bird.pos.y;
                    if (dx * dx + dy * dy <= radius * radius)
                        result.push_back(b)
            }
            return result;
        }
            auto r = North_East->radius_query(radius, bird);
            result.insert(result.end(), r.begin(), r.end());
            r = North_West->radius_query(radius, bird);
            result.insert(result.end(), r.begin(), r.end());
            r = South_East->radius_query(radius, bird);
            result.insert(result.end(), r.begin(), r.end());
            r = South_West->radius_query(radius, bird);
            result.insert(result.end(), r.begin(), r.end());
            return result;
        }
    };


void Scene::update(double const dt) {
    int const top_border = this->height-1;
    int const right_border = this->width-1;
    int const bottom_border = 0;
    int const left_border = 0;


    size_t const sz = quadtree.birds.size();
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
            if (p1.x + wall_radius > right_border) birds[i].vel.x -= turn_accel* dt;
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
