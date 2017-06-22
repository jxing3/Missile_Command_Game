/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Dome Class
*/


#include "dome.h"
#include <cairomm/context.h>
#include <cairomm/pattern.h>

#define RADIUS 0.15
#define DEFAULT_X 2.0
#define DEFAULT_Y 3.0

/** Constructors and destructor */
Dome::Dome(): radius(RADIUS), x(DEFAULT_X), y(DEFAULT_Y) {}
Dome::Dome(double x, double y): radius(RADIUS), x(x), y(y) {}
Dome::~Dome() {}

/** Draws dome */
bool Dome::draw_dome(const Cairo::RefPtr<Cairo::Context>& cr) {
    Cairo::RefPtr<Cairo::RadialGradient> pat1(Cairo::RadialGradient::create(x, y, 0.4*radius, x,y,radius)); 
    pat1->add_color_stop_rgb(0, .8, .7, 0);
    pat1->add_color_stop_rgb(1, .3, 0.2, 0);
    cr->set_source(pat1);
    cr->arc((this->x), (this->y), (this->radius), M_PI, 2*M_PI);
    cr->fill();
    return true;
}

/** Gets x position of the dome's center */
double Dome::get_x() {
    return this->x;
}

/** Gets y position of the dome's center*/
double Dome::get_y() {
    return this->y;
}

/** Gets radius */
double Dome::get_radius() {
    return this->radius;
}
