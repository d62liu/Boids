#include <cmath>
#include <vector>

//Don't include sdl2 

//vector arithmetic is done naturally, so its ok to ignore this class and just use it.
struct Vec2 {
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
struct Boid {
    Vec2 pos;
    Vec2 vel;
    double view_radius, protected_radius;
    double avoid_factor, matching_factor, centering_factor, turn_factor;
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
     //parameters for boids 
public:
    Scene(int, int) {
        height = 0; width = 0; //etc
    }
    double getParam1() {}; //...etc
    double setParam1() {}; //...etc

    void update(double const dt) {
        size_t const sz = birds.size();
        for(size_t i = 0; i < sz; ++i) {
            Vec2 p_tot = Vec2(); //Cohesion
            int neighbouring_boids = 0;
            Vec2 distance = Vec2();//Separation
            Vec2 v_tot = Vec2(); //Alignment
            for(size_t j = 0; j < sz; ++j) {
                Vec2 p1 = birds[i].pos;
                Vec2 v1 = birds[i].vel;
                Vec2 p2 = birds[j].pos;
                Vec2 v2 = birds[j].vel;
                if(((v1-v2).mag()) <= birds[i].view_radius){ //setting variables for Cohesion and Alignment
                    p_tot = p_tot + birds[i].pos;
                    v_tot = v_tot + birds[i].vel;
                    neighbouring_boids = neighbouring_boids + 1;
                if(((v1-v2).mag()) <= birds[i].protected_radius){ //setting variables for Separation
                    distance = distance + (birds[i].pos - birds[j].pos);
                }
            birds[i].pos = birds[i].pos + distance * birds[i].avoid_factor; //calculation for Separation
            if (neighbouring_boids > 0){ //Calculation for Cohesion and Alignment
                Vec2 const p_avg = p_tot/neighbouring_boids;
                Vec2 const v_avg = v_tot/neighbouring_boids;
                birds[i].vel = birds[i].vel + (p_avg - birds[i].pos) * birds[i].centering_factor;
                birds[i].vel = birds[i].vel + (v_avg - birds[i].vel) * birds[i].matching_factor;

            }
                }    
        }
    }

};
}

