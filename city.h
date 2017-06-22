/* CS120 Team CX HW8 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu
*/

#ifndef CITY_H
#define CITY_H
#include <cairomm/context.h>

class City {
    double leftmost;
    bool firstkill;
    bool actkill;
    double elapsed;
public:
    City();
    City(double x);
    virtual ~City();
    void draw_city(const Cairo::RefPtr<Cairo::Context>& cr, double seconds);
    double get_elapsed();
    double get_left();
    double get_right();
    double get_height();
    bool get_delayremove();
    bool get_remove();
    void true_remove();

};
#endif
