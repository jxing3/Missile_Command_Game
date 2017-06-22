/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

User missile
*/

#include <cairomm/pattern.h>
#include "outmissile.h"
#include <cairomm/context.h>
#include <ctime>
#include <cmath>
#include <glibmm/main.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

Mix_Chunk *laser = NULL;

//Constructors, Destructor
Outmissile::Outmissile(double spd):i(0), destx(-1), desty(-1), deleted(false), speed(spd) {} 
Outmissile::Outmissile(double spd, double x, double y):i(0), destx(x), desty(y), deleted(false), speed(spd) {
    //SFX
    laser = Mix_LoadWAV("laser.wav");
    Mix_PlayChannel(-1, laser, 0);
}
Outmissile::~Outmissile() {}


/** Checks deleted*/
bool Outmissile::get_deleted() {
    return this->deleted;
}

/** Gets x coordinate of the destination (user-click)*/
double Outmissile::get_destx() {
    return this->destx;
}

/** Gets y coordinate of the destination (user-click)*/
double Outmissile::get_desty() {
    return this->desty;
}

/** Displays remaining missiles */
bool Outmissile::show_leftmis(const Cairo::RefPtr<Cairo::Context> &cr,  std::vector<int> &leftmis) {
    cr->set_font_size(0.1);
    cr->set_source_rgba(0.2, 0.2, 0.2, 1);
    cr->move_to(0.15-0.05,3);
    cr->show_text(std::to_string(leftmis[0]));
    cr->move_to(2.0-0.05,3);
    cr->show_text(std::to_string(leftmis[1]));
    cr->move_to(4.0-0.15-0.05,3);
    cr->show_text(std::to_string(leftmis[2]));
    return true;
}

/** Draws missiles */
bool Outmissile::grow_outmissile(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height, double seconds, std::vector<int> &leftmis) {

    double xc = -1.0;
    double yc = 3;
    double  xdest = this->destx*4/width;
    double  ydest = this->desty*3/height;
    int ind = std::floor(xdest/2 + 0.5);
    bool found = false;


    // Deciding which dome to shoot from : Shoots from the nearest dome with missiles remaining.
    if (leftmis[ind] >0) { 
        xc = 2.0*ind;
        found = true;
    } else {
        if (ind == 2) {
           for (int i = 0; i < 3 && !found; i++) { 
               if (leftmis[ind-i] > 0) {
                   ind = ind-i;
                   found = true;
               }
           } 

        } else {

           for (int i = 0; i < 3 && !found; i++) { 
               if (leftmis[(ind+i)%3] > 0) {
                   ind = (ind+i)%3;
                   found = true;
               }
           } 

        }
    }
 
    if (found) {
        xc = 2.0*ind;
        if (i == 0) {
            leftmis[ind]--; 
        }
    } else {
        return false;
    }

    if (xc ==0) {
        xc += 0.15;
    } else if (xc == 4) {
        xc -= 0.15;
    }

    double slope =  (ydest - yc)/((xdest-xc));
    double velocity = this->speed/std::max(width, height);
    double increment = seconds * velocity/sqrt(1+pow(slope,2));

    Cairo::RefPtr<Cairo::LinearGradient> pat1(Cairo::LinearGradient::create(xc, yc, xc+i, yc+slope*i)); 
    pat1->add_color_stop_rgb(0.3, 0.2, 1, 0);
    pat1->add_color_stop_rgb(0.8, 1, 1, 0);

    cr->set_line_width(0.01); 
    cr->set_source(pat1);
    cr->move_to(xc+i, yc+slope*i);
    cr->line_to(xc, yc);
    cr->stroke();

    // Updating the path
    if (slope < 0) {   
        i += increment;
        slope *= -1;
        if ( xc + i >= xdest || yc + slope*i <= ydest ) {
            i = 0;
             deleted = true;
        }
    } else {
        i -= increment;
        if ( xc + i <= xdest || yc + slope*i <= ydest ) {
            i = 0;
            deleted = true;
        }
    }
    return true;
} 
