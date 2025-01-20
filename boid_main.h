#ifndef BOID_MAIN
#define BOID_MAIN

#include <vector>
struct Vec2 {
    //x,y can be modified from outside
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

//TODO: not so sure about if i should do this
struct BoidParams {
    double view_radius,
        protected_radius,
        avoid_factor, 
        matching_factor, 
        centering_factor, 
        turn_accel,
        min_speed, 
        max_speed;
    BoidParams(void);
};

//TODO: float will do
struct Boid {
    Vec2 pos;
    Vec2 vel;
    BoidParams params;
    Boid(Vec2 pos);
    Boid (Vec2 pos, Vec2 vel);
};


class Scene {
private:
    std::vector<Boid> birds; //naive vector
    int height, width;
public:
    Scene(int w, int h);
    
    std::vector<Boid> const& getBoids();

    void makeBoid(double x, double y);
    void makeBoid(double x, double y, double vx, double vy);
    void setParams(BoidParams params);

    void update(double const dt);
};

#endif
