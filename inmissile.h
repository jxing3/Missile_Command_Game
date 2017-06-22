/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Inmissile class
*/

#ifndef INMISSILE
#define INMISSILE

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

class inmissile {
    double origx,origy, x, y, movx, movy, speed, time;
    //x oriign, y origin, current x & y, gradient x & y, speed, time 
    bool remove, split;
    //state of removed, missile splitted.
  
public:
    //Constructor
    inmissile(double spd, bool splt);
    inmissile(double spd, double x, double y, bool splt);
    ~inmissile();//Destructor

    double ret_time(); //Gets the current time
    void chg_time(double seconds); //Sets the time
    void grow_missile(double seconds); //Grows missiles 
    bool draw_missile(const Cairo::RefPtr<Cairo::Context>& cr); //Draw
    void true_remove(); //Marks the missile to be removed
    bool get_remove(); //Checks whether removed
    double get_x(); //Gets current x coordinate
    double get_y(); //Gets current y coordinate
    bool get_split(); //Checks missile split
};

#endif 

