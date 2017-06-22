/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Dome clas
*/

#ifndef DOME_H
#define DOME_H

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

class Dome {
    double radius, x, y; //radius, center x, y coordinate

public:
    Dome(); //Constructor
    Dome(double x, double y); //Constructor
    ~Dome(); //Destructor
    bool draw_dome(const Cairo::RefPtr<Cairo::Context>& cr); //Draw
    double get_x(); // Gets x coordinate of the center
    double get_y(); // Gets y coordinate of the center
    double get_radius(); //Gets the radius
};

#endif
