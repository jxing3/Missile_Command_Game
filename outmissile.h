/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Outmissile class
*/

#ifndef OUTMISSILE_H
#define OUTMISSILE_H

#include <cairomm/context.h>
#include <vector>

class Outmissile {
    double i; // Keeps track of progress
    double destx; //x coordinate of destination
    double desty; //y coordinate of destination 
    bool deleted; 
    double speed; 

public:
    //Constructors
    Outmissile(double spd);
    Outmissile(double spd, double x, double y);
    virtual ~Outmissile();//Destructor
    bool get_deleted(); //Checks whether to be deleted 
    double get_destx(); //gets x destination
    double get_desty(); // gets y destination
    static bool show_leftmis(const Cairo::RefPtr<Cairo::Context> &cr,  std::vector<int> &leftmis); //shows remaining missiles
    bool grow_outmissile(const Cairo::RefPtr<Cairo::Context> &cr,int width, int height, double seconds,std::vector<int> &leftmis); //Grows and draws the outmissiles
}; 
#endif
