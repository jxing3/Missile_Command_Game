/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu
Game class
*/
#include <glibmm/fileutils.h>
#include <gdkmm/general.h>
#include <chrono>
#include "ufo.h"
#include "explosion.h"
#include "outmissile.h"
#include "inmissile.h"
#include "game.h"
#include "dome.h"
#include "city.h"
#include <cairomm/context.h>
#include <ctime>
#include <cmath>
#include <glibmm/main.h>
#include <vector>
#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#define MIS_SCORE 50 
#define UFO_SCORE 25 

Glib::RefPtr<Gdk::Pixbuf> m_image; //Background image
double xdest; //y coordinate of user-click
double ydest; //x coordinate of user-click
std::vector<Ufo> ufov; //Vector of UFO

//Random generator
unsigned seed3 = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator3 (seed3);

/** Defines a line for a collision detector */
double Game::line(double m, double b, double x) {
    return m*x + b;
}

/** Finds distance */
double Game::distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x2 - x1),2) + pow((y2 - y1),2));
}


/** Game Default Constructor */
Game::Game(): paused(false), unstarted(true), won(false), lost(false), m_last(std::chrono::system_clock::now()), numhit(0), level(1), score(0), difficulty(1),  timer(5), speed_in(0.3), speed_out(3000), speed_exp(.3), speed_ufo(1.0), mistime(4000), waves(3) {
    //Background Image
    try {
        m_image = Gdk::Pixbuf::create_from_file("stars.png");
    } catch (const Glib::FileError &ex) {
        std::cout << "File Error" << std::endl;
    } catch (const Gdk::PixbufError &ex) {
        std::cout << "Pixbuf Error" << std::endl;
    }

    add_events(Gdk::BUTTON_PRESS_MASK); //Event for recording the user mouse-click input
 
    double numcity = 3; // Number of cities on each half of the screen
    double space = 2.0 - 0.45; // Spacing in each half of the screen (2: half the screen, 0.45 : space occupied by the dome
    double cityspace = space/numcity; // Space per a city
    double offset = (cityspace - 0.075*5)/2; // Offset for city placement. 0.075*5 = width of city without the blank space

    //Putting the Cities
    //left half
    for (int i =0; i<numcity; i++){
      cityvec.push_back(City(0.3 + offset + cityspace*i));
    }
    //right half
    for (int i=0; i<numcity; i++) {
      cityvec.push_back(City(2.15 + offset + cityspace*i));
    }

    //Putting the Domes
    domevec.push_back(Dome(2, 3)); //Middile
    domevec.push_back(Dome(0.15, 3)); //Left
    domevec.push_back(Dome(4.0-0.15,3)); //Right
    
    //Missile Counter
    for (int i = 0; i < 3; i++) {
        leftOutmis.push_back(10);
    }

    Glib::signal_timeout().connect(sigc::mem_fun(*this, &Game::on_timeout), 20 );
}

/** Another constructor to call the "next level". So some stuff are carried over from the previous level*/
Game::Game(int lvl, int score, std::vector<City> cityvec): paused(false), unstarted(false), won(false), lost(false), m_last(std::chrono::system_clock::now()), cityvec(cityvec),  numhit(0), level(lvl), score(score), difficulty(1), timer(5), speed_in(0.3), speed_out(3000), speed_exp(.3), speed_ufo(1.0), mistime(4000), waves(3) {

    //Background Image
    try {
        m_image = Gdk::Pixbuf::create_from_file("stars.png");
    } catch (const Glib::FileError &ex) {
        std::cout << "File Error" << std::endl;
    } catch (const Gdk::PixbufError &ex) {
        std::cout << "Pixbuf Error" << std::endl;
    }

    add_events(Gdk::BUTTON_PRESS_MASK);

    domevec.push_back(Dome(2.0, 3));
    domevec.push_back(Dome(0.15, 3));
    domevec.push_back(Dome(4.0-0.15,3));

    for (int i = 0; i < 3; i++) {
        leftOutmis.push_back(10);
    }

    this->mistime = this->mistime/(pow(1.1,level)); //Updates the inter-enemymissile time according to level. 
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &Game::on_timeout), 20 );
}

/** = operator*/
Game &Game::operator=(const Game &other) {
    this->misvec = other.misvec;
    this->omv = other.omv; 
    this->exp = other.exp;
    this->paused = other.paused;
    this->unstarted = other.unstarted;
    this->won = other.won;
    this->lost = other.lost;
    this->m_last = other.m_last;
    this->cityvec = other.cityvec;
    this->domevec = other.domevec;
    this->numhit = other.numhit;
    this->level= other.level;
    this->score = other.score; 
    this->difficulty = other.difficulty;
    this->leftOutmis = other.leftOutmis;
    this->mistime = other.mistime;
    this->waves = other.waves;
    return *this;
}

/** Destructor */
Game::~Game()
{
}

/** Returns difficulty */
int Game::get_difficulty() {
    return this->difficulty;
}

/** Sets difficulty and gameplay based on that difficulty */
void Game::set_difficulty(int d) {
    this->difficulty = d;

    if (this->difficulty == 1) {
       this->speed_in = 0.3;
       this->speed_out = 3000;
       this->speed_exp =0.3;
       this->speed_ufo = 1.0;
       this->mistime = 4000;

    } else if (this->difficulty == 2) {
       this->speed_in = 0.4;
       this->speed_out = 2000;
       this->speed_exp = .5;
       this->speed_ufo = 1.5;
       this->mistime = 1000;

    } else if (this->difficulty == 3) {
       this->speed_in = 0.5;
       this->speed_out = 1000;
       this->speed_exp = 0.7;
       this->speed_ufo = 2.0;
       this->mistime = 10;
    }
}


/** Gets the x, y coordinate from the user's mouse-click.
When clicked and the game's going on, the User-missile (Outmissile) is generated.*/
bool Game::on_button_press_event(GdkEventButton* event) {
    xdest = event->x;
    ydest = event->y;
    if (!paused && !this->get_end() && !this->unstarted) {    
        Outmissile b(speed_out, xdest, ydest); // User's missiles. 
        omv.push_back(b);
    }
    return true;
}

/** DRAWS EVERYTHING.......... */
bool Game::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    //Clock
    std::chrono::system_clock::time_point m_now(std::chrono::system_clock::now());
    std::chrono::system_clock::duration delta = m_now - m_last;
    int millis = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
    double seconds = millis / 1000.0;
    this->timer+= seconds;

    //Game dimension
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    cr->set_source_rgba(0,0,0,1);
    cr->paint(); 

    //Background Image
    Gdk::Cairo::set_source_pixbuf(cr, m_image, 4, 3); 
    cr->paint();

    cr->scale(width/4, height/3);

    //Draws the screen for different conditions: Main, Pause, Won, Lost. 
    if (this->screen(cr)) {
        m_last = m_now;
        return true;   
    } 

    //Score Display
    cr->select_font_face("Helvetica", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(0.1);
    cr->set_source_rgb(1,1,1);
    cr->move_to(2,0.1);
    cr->show_text(std::to_string(this->score));
   
    //Outmissile (User)
    for (int i = 0; (unsigned) i < omv.size(); i++)  {
        Outmissile &b = omv[i];
        if (b.get_destx() != -1 && b.get_desty() != -1){ 
            b.grow_outmissile(cr, width, height, seconds, leftOutmis);
        }
        if (b.get_deleted()) {
            exp.push_back(Explosion(b.get_destx()*4/width, b.get_desty()*3/height, speed_exp ));
            omv.erase(omv.begin()+i);  
            i--;      
        }
    }
    
    //City
    for(std::vector<City>::size_type i = 0; i < cityvec.size(); i++) {
        cityvec.at(i).draw_city(cr, seconds);
        colli_detect2(i); 
        if(cityvec.at(i).get_delayremove()){
            cityvec.erase(cityvec.begin()+i);
            i--;
        }
    }

    //Inmissile (Enemy) 
    for (std::vector<inmissile>::size_type i = 0; i < misvec.size(); i++) {
        misvec.at(i).grow_missile(seconds);
        misvec.at(i).draw_missile(cr);
        splitmissile(i); 
        if (misvec.at(i).get_remove()){
            exp.push_back(Explosion(misvec.at(i).get_x(), misvec.at(i).get_y(),speed_exp  ));
            misvec.erase(misvec.begin()+i);
            i--;
        }
    }

    //Dome 
    for(std::vector<Dome>::iterator i = domevec.begin() ; i < domevec.end();++i)  {
       i->draw_dome(cr);
       colli_detect3(i);
    }

    //Left missiles
    Outmissile::show_leftmis(cr, leftOutmis);

    //Determining loss - City's all broken
    if ((cityvec.size() == 1 && cityvec[0].get_remove()) || cityvec.size() == 0) {
        this->lost = true;
        this->won = false;

    }
    
    //Determing a win - The city survived all the waves of missiles.
    if (!lost && misvec.empty() && waves <= 0) {
        this->won = true;
        this->lost = false;
    }

    //Explosion
    for (std::vector<Explosion>::iterator i = exp.begin() ; i < exp.end(); ++i) {
        i->grow(seconds);
        i->draw_explosion(cr);
        colli_detect(i); 
        if (i->get_remove()){
            exp.erase(i);
            --i;
        }
    }
    
    //UFO
    std::uniform_int_distribution<int> distribution(1, 100);
    if (distribution(generator3) == 1 && ufov.size() == 0){
        ufov.push_back(Ufo(speed_ufo));
    }
    if (ufov.size() > 0 && (!ufov[0].check_drawing() || ufov[0].get_remove())) {
        ufov.erase(ufov.begin());  
    }  
    if (ufov.size() > 0 && ufov[0].check_drawing()) {  
        ufov[0].grow_ufo(cr, seconds);
    }

    m_last = m_now;
    
    //Shooting enemy missiles
    send_missile();
    return true;
}

/** Spliting the missiles*/
void Game::splitmissile(std::vector<inmissile>::size_type j) {  
    if(misvec.at(j).ret_time() > 4 && misvec.at(j).get_split()) {
        misvec.push_back(inmissile(speed_in, misvec.at(j).get_x(), misvec.at(j).get_y(), false));  
        misvec.at(j).chg_time(0);
    }
}

/** Sends out the Enemy missiles*/
bool Game::send_missile() {
    if (waves > 0) {
        if(!paused && !unstarted){
            if(timer>=(mistime/1000)){
                std::uniform_int_distribution<int> distribution1(2, 5);
                int rand1 = distribution1(generator3);
                for (int i=0; i<rand1;i++){
                    misvec.push_back(inmissile(speed_in, false));
                }
                std::uniform_int_distribution<int> distribution(0, 2);
                int rand= distribution(generator3);
                for( int i=0;i<rand;i++){
                    misvec.push_back(inmissile(speed_in, true));    
                }
                this->timer=0;
                waves--;
            }
        }
    }
    return true;
}

/** Timeout*/
bool Game::on_timeout()
{
    Glib::RefPtr<Gdk::Window> win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    return true;
}

/** Checks paused */
bool Game::get_paused() {
    return this->paused;
}

/** Toggles Paused/Resume state*/
bool Game::toggle_pause() {
    this->paused = !paused; 
    return !paused;
}

/** Collision detector for Explosions. (Outmissile vs. Inmissile, UFO)*/
void Game::colli_detect(std::vector<Explosion>::iterator j) {
    float radius = j->ret_radius();
    float x = j->ret_x();
    float y= j->ret_y();

    for(std::vector<inmissile>::iterator i = misvec.begin() ; i < misvec.end(); ++i) {
        double dist = distance(x, y, i->get_x(), i->get_y());
        if(dist <= radius) {
            i->true_remove();
            numhit++;
            this->score = this->score + MIS_SCORE;
        }
    }

    for(std::vector<Ufo>::iterator i = ufov.begin() ; i < ufov.end(); ++i) {
        double dist = distance(x, y, i->get_x(), i->get_y());
        if(dist <= radius + UFO_RAD) {
            i->true_remove();
            this->score = this->score + UFO_SCORE;
        }
    }
}

/** Collision detector for city vs inmissile*/
void Game::colli_detect2(std::vector<City>::size_type j) {
    double right = cityvec.at(j).get_right();
    double left = cityvec.at(j).get_left();
    double height = cityvec.at(j).get_height();
    double middle = (right - left) / 2;

    double m_right = height / (middle - right);
    double m_left = height / (middle - left);
    double b_right = -1 * m_right * right;
    double b_left = -1 * m_left * left;

    for(std::vector<inmissile>::iterator i = misvec.begin() ; i < misvec.end(); ++i){
        double x = i->get_x();
        double y = i->get_y();
        if (y < height) {
          continue;
        }
        if (x > right || x < left) {
          continue;
        }
        if (x < middle) {
            double y_line = line(m_left, b_left, x);
            if (y > y_line) {
                i->true_remove();
                cityvec.at(j).true_remove();
            }
        }
        else {
            double y_line = line(m_right, b_right, x);
            if (y > y_line) {
                i->true_remove();
                cityvec.at(j).true_remove();
            }    
        } 
    }
}

/** Collision Detector at the Dome vs inmissile */
void Game::colli_detect3(std::vector<Dome>::iterator j) {
    double radius = j->get_radius();
    double x = j->get_x();
    double y = j->get_y();

    for(std::vector<inmissile>::iterator i = misvec.begin(); i < misvec.end(); ++i){
        double dist = distance(x, y, i->get_x(), i->get_y());
        if(dist <= radius) {
            i->true_remove();
            int j  = std::floor(i->get_x()+0.5)/2;
            leftOutmis[j] = 0;
        }
    }
}

/** Drawing screens for Paused/Resume/Won/Lost states */
bool Game::screen(const Cairo::RefPtr<Cairo::Context>& cr) {
    //Main. Before game starts
    if (this->unstarted) {         
        cr->set_source_rgba(0,0.9,0,0);
        cr->paint(); 
        cr->select_font_face("FreeMono", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(0.4);
        cr->set_source_rgba(0.65,0.16,0.16,0.6);
        cr->move_to(.25,1.4);
        cr->show_text("Missile Command");
        return true;
    }

    //Paused
    if (this->paused) {
        cr->set_source_rgba(0.2,0.2,0.2,0.4);
        cr->paint(); 
        cr->select_font_face("FreeMono", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(0.2);
        cr->set_source_rgba(0.18,0.55,0.34,.6);
        cr->move_to(1.2,1.4);
        cr->show_text("Paused");
        cr->move_to(1.2,1.6);
        std::string str2 = "Current Score : " + std::to_string(this->score);
        cr->show_text(str2);
        return true;
    }

    if (this->won) {
        cr->select_font_face("FreeMono", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(0.2);
        cr->set_source_rgba(0.6,0.6,0.7,.6);
        cr->move_to(1.2,1.4);
        std::string str ="You Win Level "+ std::to_string(level);
        cr->show_text(str);
        cr->move_to(1.2,1.6);
        std::string str2 = "Current Score : " + std::to_string(this->score);
        cr->show_text(str2);
        return true;
    }

    if (lost) {
        cr->set_source_rgba(0.5,0.3,0.3,0.2);
        cr->paint(); 
        cr->select_font_face("FreeMono", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(0.2);
        cr->set_source_rgba(0.5,0.0,0.0,.8);
        cr->move_to(1.2,1.4);
        std::string str ="You Lose Level "+ std::to_string(level);
        cr->show_text(str);
        cr->move_to(1.2,1.6);
        std::string str2 = "Final Score : " + std::to_string(this->score);
        cr->show_text(str2);
        return true;
    }
    return false;
}

/** Checks whether game has started or not*/
bool Game::get_unstarted() {
    return this->unstarted;
}

/** Checks the game lost */
bool Game::get_lost() {
    return this->lost;
}

/** Checks game is done */
bool Game::get_end() {
    return this->won || this->lost;
}

/** Toggles pause and play */
void Game::toggle_start() {
    this->unstarted = false;
}

/** Gets the score */
int Game::get_score() {
    return this->score;
}

/** Gets the city vector */
std::vector<City> Game::get_cities() {
    return this->cityvec;
}
