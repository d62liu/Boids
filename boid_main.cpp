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
        return (sqrt(x*x + y*y));//(0.96043387*std::max(x,y) + 0.397824734*std::min(x,y));
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
    double min_speed, max_speed;
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
    int height, width;
    Vec2 const initial_vel; 
     //parameters for boids 
public:
    Scene(int h, int w, Vec2 const& init_vel) {
        height = h; 
        width = w; //etc
        Vec2 initial_vel = init_vel;
    }
    double getParam1() {}; //...etc
    double setParam1() {}; //...etc
    std::vector<Boid> const& getBoids() {
        return birds;
    }
    void update(double const dt) {
        int const bottom_border = height + 1;
        int const top_border = height - 1;
        int const right_border = width + 1;
        int const left_border = width - 1;

        size_t const sz = birds.size();
        for(size_t i = 0; i < sz; ++i) {
            Vec2 p_tot = Vec2(); //Cohesion
            int neighbouring_boids = 0;
            Vec2 distance = Vec2();//Separation
            Vec2 v_tot = Vec2(); //Alignment
            for(size_t j = 0; j < sz; ++j) {
                Vec2 const p1 = birds[i].pos;
                Vec2 const v1 = birds[i].vel;
                Vec2 const p2 = birds[j].pos;
                Vec2 const v2 = birds[j].vel;
                const double turn_accel = birds[i].turn_factor;
                if(((p1-p2).mag()) <= birds[i].view_radius){ //setting variables for Cohesion and Alignment
                    p_tot = p_tot + p2;
                    v_tot = v_tot + v2;
                    neighbouring_boids = neighbouring_boids + 1;
                }

                if(((p1-p2).mag()) <= birds[i].protected_radius)distance = distance + (p1 - p2);  //setting variables for Separation

            birds[i].pos = p1 + distance * birds[i].avoid_factor; //calculation for Separation

            if (neighbouring_boids > 0){ //Calculation for Cohesion and Alignment
                Vec2 const p_avg = p_tot/neighbouring_boids;
                Vec2 const v_avg = v_tot/neighbouring_boids;
                birds[i].vel = v1 + (p_avg - p1) * birds[i].centering_factor;
                birds[i].vel = v1 + (v_avg - v1) * birds[i].matching_factor;

            }
            if (p1.x < left_border) birds[i].vel.x = v1.x + turn_accel*dt;
            if (p1.x > right_border)birds[i].vel.x = v1.x - turn_accel*dt;
            if (p1.y > top_border)birds[i].vel.y = v1.y - turn_accel*dt;
            if (p1.y < bottom_border)birds[i].vel.y = v1.y + turn_accel*dt;

            double speed = birds[i].vel.mag(); 

            if (speed < birds[i].min_speed) birds[i].vel = v1 * birds[i].min_speed/speed;
            if (speed > birds[i].max_speed) birds[i].vel = v1 * birds[i].max_speed/speed;
            
        }
    }
    for(size_t i = 0; i < sz; ++i) {
        birds[i].pos = birds[i].pos + birds[i].vel * dt;
    }
    }
    void makeBoid(double, double){
        birds.push_back(Boid(initial_vel, Vec2(0,0))); //intiialize at 0,0
    }
};
