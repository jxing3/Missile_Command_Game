/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

UFO class
*/

#ifndef UFO_H
#define UFO_H

#include <cairomm/context.h>
#include <random>
#include <chrono>
#define UFO_RAD 0.05

class Ufo {
    double i; //Keeps track of progress
    double x_init; //Starting point x
    double y_init; //Starting point y
    bool remove; //state to be removed
    bool drawing; //state whether on the screen
    double speed; 

public: 
    Ufo(double speed); //Constructor
    virtual ~Ufo(); //Destructor
    bool grow_ufo(const Cairo::RefPtr<Cairo::Context> &cr, double time); //Progress and draw the ufo
    bool check_drawing(); //Checks whether the UFO is in the screen 
    double get_x(); // x coordinate of the ufo
    double get_y(); // y coordinate of the ufo
    void true_remove(); // Marks UFO to be removed 
    bool get_remove(); // Checks whether ready to be removed
};
 
#endif
