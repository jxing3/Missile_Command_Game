/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Game class
*/

#ifndef GAME_H
#define GAME_H
#include <chrono>
#include <gtkmm/drawingarea.h>
#include "outmissile.h"
#include "inmissile.h"
#include "explosion.h"
#include <vector>
#include "dome.h"
#include "city.h"
class Game : public Gtk::DrawingArea {
    std::vector<inmissile> misvec; //Vector of enemy missiles
    std::vector<Outmissile> omv; //Vector of user missiles  
    std::vector<Explosion> exp; //Vector of explosions
    bool paused; //Game paused?
    bool unstarted; //Game not started?
    bool won; //Game won?
    bool lost; //Game lost? 
    std::chrono::system_clock::time_point m_last; //Timepoints to update each on-draw accurately
    std::vector<City> cityvec; //Vector of cities
    std::vector<Dome> domevec; //Vector of domes
    int numhit; //number of hits
    int level; 
    int score;  
    int difficulty;
    double timer;
    std::vector<int> leftOutmis; //Vector that keeps track of remaining Outmissiles 
    double speed_in; //Speed of inmissile
    double speed_out; //Speed of outmissile
    double speed_exp; //Speed of explosion
    double speed_ufo; //Speed of ufo
    double mistime; //Inter-inmissile period
    int waves; //number of waves of inmissiles
    double line(double m, double b, double x); // Defines line 
    double distance(double x1, double y1, double x2, double y2); //Defines distance

 
public:
    //Constructors
    Game();
    Game(int lvl, int score, std::vector<City> cityvec);
    Game &operator=(const Game &other);
    virtual ~Game(); //destructor
    bool on_button_press_event(GdkEventButton* event); //User-click input
    bool toggle_pause(); //Toggeles pause/play state
    bool get_paused(); //Checks whether game paused
    void toggle_start();  //Toggles start of the game
    bool get_unstarted();  //Checks game has started 
    bool get_lost(); //Checks game lost
    bool get_end(); //Checks game done
    int get_difficulty(); //Gets the difficulty of game
    void set_difficulty(int d); //Sets the difficulty of the game
    int get_score(); //Gets the score
    std::vector<City> get_cities(); //Retrieves current state of the cities

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr); //DRAW
    void splitmissile(std::vector<inmissile>::size_type j); //Splits inmissiles
    bool send_missile(); //Shoots inmissiles
    bool on_timeout(); 
    void colli_detect(std::vector<Explosion>::iterator j); //Collision detection of outmissile vs inmissile & UFO
    void colli_detect2(std::vector<City>::size_type); //Collsion detection of cities vs inmissile
    void colli_detect3(std::vector<Dome>::iterator j); //Collision detection of domes vs inmissile
    bool screen(const Cairo::RefPtr<Cairo::Context>& cr); //Draws the game screen at different states of the game
};

#endif
