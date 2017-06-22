/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Explosion class
*/


#include "explosion.h"
#include <cairomm/context.h>
#include <cairomm/pattern.h>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#define DEFAULT_RAD 0.03
#define MAX_RAD 0.15
#define END_RAD 0.03

Mix_Chunk *exps = NULL;

/** Constructor, Destructor*/
Explosion::Explosion(float x, float y, double speed): x(x), y(y), shrink(false), remove(false), radius(DEFAULT_RAD), speed(speed) {
    //SFX
    exps = Mix_LoadWAV("boom.wav");
    Mix_PlayChannel(-1,exps, 0);
}

Explosion::~Explosion(){}

/** Draws explosion*/
bool Explosion::draw_explosion(const Cairo::RefPtr<Cairo::Context>& cr) {
    Cairo::RefPtr<Cairo::RadialGradient> pat1(Cairo::RadialGradient::create(x, y, 0.1*radius, x,y,radius)); 
    pat1->add_color_stop_rgb(0.1, 1, 1, 0);
    pat1->add_color_stop_rgb(0.9, 1, 0.3, 0);
    cr->set_source(pat1);
    cr->arc((this->x), (this->y), (this->radius), 0, 2 * M_PI);
    cr->fill();
    return true;
}

/** grows explosion */
void Explosion::grow(double seconds) {
    if (!this->shrink && this->radius < MAX_RAD) {
        this->radius = this->radius + this->speed*seconds; 
    }
    if (!this->shrink && this->radius >= MAX_RAD) {
        this->shrink = true;
    }
    if (this->shrink) {
        this->radius = this->radius - this->speed*seconds; 
    }
    if (this->shrink && this->radius <= END_RAD) {
        this->remove = true;
    }
}

/** Checks if ready to be removed */
bool Explosion::get_remove() {
    return this->remove;
}

/** Gets x position */ 
float Explosion::ret_x(){
    return this->x;
}

/** Gets y position */
float Explosion::ret_y(){
    return this->y;
}

/** Gets the radius */
float Explosion::ret_radius(){
    return this->radius;
}

