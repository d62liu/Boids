#ifndef BOID_MAIN
#define BOID_MAIN

#include <vector>

class QuadTree;

struct Vec2 {
    double x;
    double y;
    Vec2(void);
    Vec2(double x, double y);
    double mag(void) const;
    Vec2& operator+=(Vec2 const& v);
    friend Vec2 operator+(Vec2 lhs, Vec2 const& rhs);
    Vec2& operator-=(Vec2 const& v);
    friend Vec2 operator-(Vec2 lhs, Vec2 const& rhs);
    Vec2 operator-(void) const;
    Vec2& operator*=(double const scale);
    friend Vec2 operator*(Vec2 v, double const scale);
    friend Vec2 operator*(double const scale, Vec2 v);
    Vec2& operator/=(double const scale);
    friend Vec2 operator/(Vec2 v, double const scale);
    friend Vec2 operator/(double const scale, Vec2 v);
};

struct BoidParams {
    double view_radius;
    double protected_radius;
    double avoid_factor; 
    double matching_factor; 
    double centering_factor; 
    double turn_accel;
    double min_speed; 
    double max_speed;
    BoidParams(void);
};

struct Boid {
    Vec2 pos;
    Vec2 vel;
    BoidParams params;
    Boid(Vec2 pos);
    Boid(Vec2 pos, Vec2 vel);
};

class Scene {
private:

    QuadTree* quadtree;
    int height, width;
public:
    Scene(int w, int h);
    ~Scene(); 
    
    std::vector<Boid> const& getBoids();
    
    void makeBoid(double x, double y);
    void makeBoid(double x, double y, double vx, double vy);
    void setParams(BoidParams params);
    void update(double const dt);
};

#endif 
