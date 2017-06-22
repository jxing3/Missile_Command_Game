/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Explosion class
*/


#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

class Explosion {
    float x, y; // x, y coordinate of explosion center
    bool shrink, remove; // states 
    float radius; 
    double speed; //speed of explosion

public:
    Explosion(float x, float y, double speed); //Constructor
    ~Explosion(); //Destructor
    bool draw_explosion(const Cairo::RefPtr<Cairo::Context>& cr); //Draw
    void grow(double seconds); //Grows explosion
    bool get_remove(); //Checks to be removed
    float ret_x(); //Gets the center x coordinate of explosion
    float ret_y(); //Gets the center y coordinate of explosion
    float ret_radius(); //Gets radius
};

#endif 
