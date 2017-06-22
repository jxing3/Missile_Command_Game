/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Draws cities 
*/

#include <cairomm/context.h>
#include "city.h"
#include <iostream>

double width = 0.075;
double MAX_HEIGHT = 0.2;

/** Constructors, destructor */
City::City(): leftmost(0), firstkill(false), actkill(false), elapsed(0){}
City::City(double x): leftmost(x), firstkill(false), actkill(false), elapsed(0) {}
City::~City() {}

/** Draws the city*/
void City::draw_city(const Cairo::RefPtr<Cairo::Context>& cr, double seconds) {
    int j[] = {-2, -1, 0, -1, -2};
    double height;
    for (int i = 0; i < 5; i++) {
        height = MAX_HEIGHT + 0.05*j[i];
        cr->rectangle(this->leftmost + width*i, 3 - height, width, height);
        cr->set_source_rgba(0.5, 0.7, 1, 0.6);
        cr->fill() ;
    }

    if(firstkill){
      this->elapsed+=seconds;
    }

    // The city is removed from the screen a little after it is hit.
    if(elapsed>=.35&&firstkill){
       this->actkill=true;
    }
   
}

/** Checks the time elapsed after being hit */
double City::get_elapsed() {
    return this->elapsed;
}

/** Gets the left x coordinate */
double City::get_left() {
    return this->leftmost;
}

/** Gets the right x coordinate */
double City::get_right() {
    return this->leftmost + width*5;
}

/** Gets the maximal height */
double City::get_height() {
    return 3-MAX_HEIGHT;
}

/** Checks whether it is ready to be COMPLETELY removed*/
bool City::get_delayremove() {
    return this->actkill;
}

/** Checks whether the city is hit*/
bool City::get_remove(){
   return this->firstkill;

}

/** Marks the city as hit */
void City::true_remove(){
   this->firstkill= true;
}



