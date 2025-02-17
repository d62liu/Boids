#include <cmath>
#include <vector>
#include <iostream>
#include "boid_main.h"

Vec2::Vec2(void) : x(0), y(0) {}

Vec2::Vec2(double x, double y) : x(x), y(y) {}

double Vec2::mag(void) const { 
    return std::sqrt(x * x + y * y);//(0.96043387*std::max(x,y) + 0.397824734*std::min(x,y);
}

Vec2& Vec2::operator+=(Vec2 const& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vec2 operator+(Vec2 lhs, Vec2 const& rhs) {
    lhs += rhs;
    return lhs;
}

Vec2& Vec2::operator-=(Vec2 const& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vec2 operator-(Vec2 lhs, Vec2 const& rhs) {
    lhs -= rhs;
    return lhs;
}

Vec2 Vec2::operator-(void) const {
    return Vec2(-x, -y);
}

Vec2& Vec2::operator*=(double const scale) { 
    x *= scale;
    y *= scale;
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
    x /= scale;
    y /= scale;
    return *this;
}

Vec2 operator/(Vec2 v, double const scale) {
    v /= scale;
    return v;
}

Vec2 operator/(double const scale, Vec2 v) {
    v /= scale;
    return v;
}

BoidParams::BoidParams(void) {
    view_radius = 200;
    protected_radius = 20;          
    avoid_factor = 0.1; 
    matching_factor = 0.5; 
    centering_factor = 0.5; 
    turn_accel = 1500;
    min_speed = 400; 
    max_speed = 600;
}

Boid::Boid(Vec2 pos) : pos(pos), vel(Vec2()), params(BoidParams()) {}

Boid::Boid(Vec2 pos, Vec2 vel) : pos(pos), vel(vel), params(BoidParams()) {}

struct AABB {
    double left_bound, right_bound, top_bound, bot_bound;
    
    AABB(double left, double right, double top, double bot)
      : left_bound(left), right_bound(right), top_bound(top), bot_bound(bot) {}
    
    bool contains(const Boid& b) const {
        return contains(b.pos);
    }
    
    bool contains(const Vec2& p) const {
        return (p.x >= left_bound && p.x <= right_bound &&
                p.y >= bot_bound && p.y <= top_bound);
    }
    
    bool intersects(const AABB& other) const {
        return (left_bound <= other.right_bound && right_bound >= other.left_bound &&
                bot_bound  <= other.top_bound   && top_bound >= other.bot_bound);
    }
};

class QuadTree {
public:
    AABB boundary;
    bool divided = false;
    std::vector<Boid> birds;
    QuadTree* North_East = nullptr;
    QuadTree* North_West = nullptr;
    QuadTree* South_East = nullptr;
    QuadTree* South_West = nullptr;
    
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
        if (!boundary.contains(boid))
            return false;
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
        double midX = (boundary.left_bound + boundary.right_bound) / 2.0;
        double midY = (boundary.top_bound + boundary.bot_bound) / 2.0;            
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
        } else {
            for (auto& b : birds) {
                if (quadrant.contains(b))
                    found.push_back(b);
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
    
    std::vector<Boid> radius_query(int radius, const Boid& bird) const {
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
                    result.push_back(b);  
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



Scene::Scene(int w, int h) : width(w), height(h) {
    int top_border = height - 1;
    int right_border = width - 1;
    int bottom_border = 0;
    int left_border = 0;
    AABB boundary(left_border, right_border, top_border, bottom_border);
    quadtree = new QuadTree(boundary);
}

Scene::~Scene() {
    delete quadtree;
}

std::vector<Boid> const& Scene::getBoids() {
    return quadtree->birds;
}

void Scene::makeBoid(double x, double y) {
    quadtree->birds.push_back(Boid(Vec2(x, y), Vec2()));
}

void Scene::makeBoid(double x, double y, double vx, double vy) {
    quadtree->birds.push_back(Boid(Vec2(x, y), Vec2(vx, vy)));
}

void Scene::setParams(BoidParams params) {
    for(auto & bird : quadtree->birds) {
        bird.params = params;
    }
}

void Scene::update(double const dt) {
    int const top_border = height - 1;
    int const right_border = width - 1;
    int const bottom_border = 0;
    int const left_border = 0;
    
    size_t const sz = quadtree->birds.size();
    for (size_t i = 0; i < sz; ++i) {
        Boid& bird = quadtree->birds[i];
        
        double const protected_radius = bird.params.protected_radius;
        double const view_radius = bird.params.view_radius;
        double const avoid_factor = bird.params.avoid_factor;
        double const centering_factor = bird.params.centering_factor;
        double const matching_factor = bird.params.matching_factor;
        double const turn_accel = bird.params.turn_accel;
        double const max_speed = bird.params.max_speed;
        double const min_speed = bird.params.min_speed;
        double const wall_radius = 200.0;
        
        Vec2 separation;
        {
            std::vector<Boid> close_boids = quadtree->radius_query(protected_radius, bird);
            for (const auto& neighbor : close_boids) {
                if ((neighbor.pos.x == bird.pos.x) && (neighbor.pos.y == bird.pos.y))
                    continue;
                separation -= (neighbor.pos - bird.pos);
            }
        }
        
        Vec2 position_total;
        Vec2 velocity_total;
        int count = 0;
        {
            std::vector<Boid> neighbors = quadtree->radius_query(view_radius, bird);
            for (const auto& neighbor : neighbors) {
                if ((neighbor.pos.x == bird.pos.x) && (neighbor.pos.y == bird.pos.y))
                    continue;
                position_total += neighbor.pos;
                velocity_total += neighbor.vel;
                ++count;
            }
        }
        bird.vel += dt * separation * avoid_factor;
        if (count > 0) {
            Vec2 average_position = position_total / count;
            Vec2 average_velocity = velocity_total / count;
            bird.vel += dt * (average_position - bird.pos) * centering_factor;
            bird.vel += dt * (average_velocity - bird.vel) * matching_factor;
        }
        
        if (bird.pos.x - wall_radius < left_border)
            bird.vel.x += turn_accel * dt;
        if (bird.pos.x + wall_radius > right_border)
            bird.vel.x -= turn_accel * dt;
        if (bird.pos.y - wall_radius < bottom_border)
            bird.vel.y += turn_accel * dt;
        if (bird.pos.y + wall_radius > top_border)
            bird.vel.y -= turn_accel * dt;
        
        double speed = bird.vel.mag();
        constexpr double EPSILON = 1e-9;
        if (speed < min_speed)
            bird.vel *= min_speed / (speed + EPSILON);
        if (speed > max_speed)
            bird.vel *= max_speed / (speed + EPSILON);
    }
    for (size_t i = 0; i < sz; ++i) {
        quadtree->birds[i].pos += quadtree->birds[i].vel * dt;
    }
}
