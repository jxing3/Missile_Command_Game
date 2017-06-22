/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Enemy Missiles
*/


#include <random>
#include <cairomm/pattern.h>
#include "inmissile.h"
#include <iostream>
#include <cairomm/context.h>
#include <random>
#include <chrono>
#include <cmath>

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);


/** Constructors, Destructor*/
inmissile::inmissile(double spd, bool splt): origx(0), origy(0), x(0), y(0), movx(0), movy(0), speed(spd), time(0), remove(false), split(splt) {
    std::uniform_int_distribution<int> distribution(0, 640);
    origx = (distribution(generator)*4)/(640.0);
    double tmpdestx = (distribution(generator)*4)/(640.0);
    double tmplength = sqrt(pow((tmpdestx-origx),2) + 9);
    movx = (tmpdestx-origx)/tmplength;
    movy = 3/tmplength;
    x = origx;
}

inmissile::inmissile(double spd, double x, double y, bool splt): origx(x), origy(y), x(0), y(0), movx(0), movy(0), speed(spd), time(0), remove(false), split(splt) {
    std::uniform_int_distribution<int> distribution(0, 640);
    double tmpdestx = (distribution(generator)*4)/(640.0);
    double tmplength = sqrt(pow((tmpdestx-origx),2) + pow((3.0-origy),2));
    movx = (tmpdestx-origx)/tmplength;
    movy = (3.0-origy)/tmplength;
    this->x = origx;
    this->y= origy;
}

inmissile::~inmissile() {}

/** gets the current time */
double inmissile::ret_time() {
    return this->time; 
}

/** sets time */
void inmissile::chg_time(double seconds) {
    time = seconds;
}

/** grows missile*/ 
void inmissile::grow_missile(double seconds) {
    time+=seconds;
    x+= movx*speed*seconds;
    y+= movy*speed*seconds;
    if(y>=3){
        remove = true;
    }
}
 
/** Draws inmissile*/ 
bool inmissile::draw_missile(const Cairo::RefPtr<Cairo::Context>& cr) {
    Cairo::RefPtr<Cairo::LinearGradient> pat1(Cairo::LinearGradient::create(origx, origy, x, y)); 
    pat1->add_color_stop_rgb(0.3, 0, 0.2, 0.9);
    pat1->add_color_stop_rgb(0.8, 1, 0, 0);
    cr->set_source(pat1);
    cr->set_line_width(.01);
    cr->move_to(this->origx, this->origy);
    cr->line_to(this->x,this->y);
    cr->stroke();
    return true;
}

/** marks to be removed */
void inmissile::true_remove() {
    this->remove = true;
}

/** Checks whether ready to be removed */
bool inmissile::get_remove() {
    return this->remove;
}

/** x position */
double inmissile::get_x() {
    return this->x;
}

/** y position*/
double inmissile::get_y() {
    return this->y;
}

/** Checks whether missile is split */
bool inmissile::get_split() {
    return this->split;
}

