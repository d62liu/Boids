#include <cmath>
#include <vector>

//Don't include sdl2 

//vector arithmetic is done naturally, so its ok to ignore this class and just use it.
class Vec2 {
public:
    //x,y can be modified from outside
    double x;
    double y;
    Vec2(void) {
        x = 0;
        y = 0;
    }
    Vec2(double x, double y) {
        this->x = x;
        this->y = y;
    }
    double mag(void) {
        return sqrt(x*x + y*y);
    }
    Vec2 operator+(Vec2 const& v) const {
        return Vec2(x+v.x, y+v.y);
    }
    Vec2 operator-(Vec2 const& v) const {
        return Vec2(x-v.x, y-v.y);
    }
    Vec2 operator-(void) const {
        return Vec2(-x, -y);
    }
    double operator*(Vec2 const& v) const { //dot product
        return x*v.x + y*v.y;
    }
    Vec2 operator*(double scale) const { 
        return Vec2(x*scale, y*scale);
    }
    Vec2 operator/(double scale) const {
        return Vec2(x/scale, y/scale);
    }
};
class Boid {
    public:
    Vec2 pos;
    Vec2 vel;
    Boid(Vec2 pos) {
        this->pos = pos;
        this->vel = Vec2();
    }
    Boid (Vec2 pos, Vec2 vel) {
        this->pos = pos;
        this->vel = vel;
    }
};

class Scene {
private:
    std::vector<Boid> birds; //naive vector
    size_t height, width;
    double param1, param2, param3; //parameters for boids (rename them)
public:
    Scene(int, int) {
        height = 0; width = 0; //etc
    }
    double getParam1() {}; //...etc
    double setParam1() {}; //...etc

    void update(void) {
        size_t const sz = birds.size();
        for(size_t i = 0; i < sz; ++i) {
            for(size_t j = 0; < sz; ++j) {
                Vec2 p1 = birds[i].pos;
                Vec2 v1 = birds[i].vel;
                Vec2 p2 = birds[j].pos;
                Vec2 v2 = birds[j].vel;
                if(((v1 + v2)/(v1+v2).mag()).x == 69); //domath

            }
        }
    }

};

