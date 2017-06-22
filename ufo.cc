/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu
UFO class
*/

#include <cairomm/pattern.h>
#include "ufo.h"
#include <cairomm/context.h>
#include <ctime>
#include <cmath>
#include <glibmm/main.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cairomm/pattern.h>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

Mix_Chunk *ufos = NULL;
unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator2 (seed2);

/** Constructor & destructor*/
Ufo::Ufo(double speed): i(0), remove(false), drawing(true), speed(speed) {
    std::uniform_int_distribution<int> x_distribution(0, 1);
    std::uniform_int_distribution<int> y_distribution(0, 240);
    x_init = (x_distribution(generator2)*4); //Starts either at right or left
    y_init = (y_distribution(generator2)*3)/(480.0);     

    //SFX
    Mix_OpenAudio(11025, MIX_DEFAULT_FORMAT, 2, 4096);
    ufos = Mix_LoadWAV("ufo2.wav");
    Mix_PlayChannel(-1,ufos, 0);
}
Ufo::~Ufo() {}

/** Grows and draws UFO*/
bool Ufo::grow_ufo(const Cairo::RefPtr<Cairo::Context> &cr, double seconds) {
    Cairo::RefPtr<Cairo::RadialGradient> pat1(Cairo::RadialGradient::create(x_init+i, y_init, 0.1*UFO_RAD,x_init+i, y_init, UFO_RAD)); 
    pat1->add_color_stop_rgb(0.1, 1, 1, 0);
    pat1->add_color_stop_rgb(0.9, 0, 0.3, 0.8);
    double off = 0.07;

    cr->set_line_width(0.01);
    cr->set_source(pat1);
    cr->move_to (x_init + i - off, y_init + off) ;
    cr ->line_to (x_init + i + off, y_init - off);
    cr->move_to (x_init + i - off, y_init - off);
    cr -> line_to(x_init + i + off, y_init + off);
    cr -> stroke();

    cr->set_source(pat1);
    cr->arc( (x_init + i), (y_init), (UFO_RAD), 0, 2*M_PI);
    cr->fill();

    double ex, ey, ew, eh;
    ex = x_init + i;
    ey = y_init;
    ew = 0.2;
    eh = 0.04;

    cr->set_line_width(0.02);
    cr->scale(ew/0.2, eh/0.2);
    cr->arc(ex, ey*ew/eh, 0.1, 0.0, 2*M_PI);
    cr -> stroke();


    if (x_init == 4) { //starting from right
        i += -1*this->speed * seconds;
    } else { //from left
        i += this->speed * seconds;
    }

    // Out of the screen 
    if (i != 0 && (x_init + i <= 0.0 ||  x_init + i >= 4.0)) {
        drawing = false;
        i = 0;
    }
    return true;
}

/** Gets x position */
double Ufo::get_x() {
    return (this->x_init+this->i);
}

/** Gets y position */
double Ufo::get_y() {
    return (this->y_init);
}

/** Checks whether ufo is on screen */
bool Ufo::check_drawing() {
    return drawing;
}

/** Marks the UFO to be removed */
void Ufo::true_remove() {
    this->remove = true;
}

/** Checks whether ufo is ready to be deleted */
bool Ufo::get_remove() {
    return this->remove;
}

