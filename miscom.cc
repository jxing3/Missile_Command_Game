/* CS120 Team CX HW9 
Kareem Fakhoury kareem@jhu.edu
David Lee dlee140@jhu.edu
Jesse Xing jxing3@jhu.edu

Missile Command. The Main Driver
*/

#include "game.h"
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h> 
#include <gtkmm/spinbutton.h>
#include <gtkmm/main.h> 
#include <gtkmm/adjustment.h>
#include <gtkmm/label.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/box.h>
#include <glibmm.h>
#include <gdkmm/general.h>
#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"


/* Music player */
Mix_Music *music = NULL;
/* Level */
int level=1;

/** Sets the difficulty of the game as the user increments/decrements difficulty. 
*g: the game, *sb: spinbutton
*/
void sbc(Game *g, Gtk::SpinButton *sb) {
    int a = sb->get_value();
    g->set_difficulty(a);
}

/** Menu configuration ("New Game" button, "Next Level" button)
*g : Game, *b : New Game button, *c : Next Level button
*/
bool changeNG(Game *g, Gtk::Button *b, Gtk::Button *c) {
    if (g->get_end() ) {
        b->set_label("New Game");  
        if (!g->get_lost()) { // if this level was won, 
            c->show(); // show the "Next Level" button
        }     
    } else if(g->get_end() && g->get_lost() ) { // when "game over" ie, lost, 
        c->hide(); // hide the "Next Level" button
    } 
    return true;
}

/** When the "Next Level" button is clicked, sets up the next level. 
*g : Game, *c : Next level button, *b : New Game button
*/ 
void on_button_nextlvl(Game *g, Gtk::Button *c, Gtk::Button *b){
    level++;
    int score = g->get_score();
    int diffc = g->get_difficulty();
    std::vector<City> cv = g->get_cities();
  
    *g =  Game(level, score, cv); // Sets up the game at the next level
    g->set_difficulty(diffc);
    c->hide(); // Once "New Level" is pressed, it goes away. 
    b->set_label("Pause"); // Since now the gaming is being played, set the "Play" label to "Pause"
}

/** When the "Play" button is clicked... 
*g : game, *b : play/pause/new game button, *sb : The box that wraps difficulty , *c : next level button, *sb : spin difficulty button
*/
void on_button_played(Game *g, Gtk::Button *b, Gtk::Box *sb, Gtk::Button *c, Gtk::SpinButton *spb) {
    sb->hide(); // When in "Play", difficulty cannot be set. Difficulty box is hidden.
    c->hide(); // So is "Next Level" button.

    if (g->get_unstarted()) { // If the game has not been started
        g->toggle_start(); // Mark the game  started. 
        b->set_label("Pause"); // The "Play" button now becomes "Pause" button. 
        
        
    } else if (g->get_end()) { // If game is done (won/lost), at this point the Label is "New Game". If this is clicked,
         level=1; // Level gets reinitialized to 1
         *g = Game(); // New game
         spb->set_value(1); // Sets the Spinbutton to 1
         sb->show(); // Difficulty box reappears.
         b->set_label("Play"); 
  
    } else {
        if (g->toggle_pause()) { // Pause or not
            b->set_label("Pause");
        } else {
            b->set_label("Resume");
        }
    }

}

/** Quit button operation */
void on_button3_closed() {
    exit(EXIT_SUCCESS);
}

/** MAIN DRIVER */
int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "TeamCXMISSILECOMMAND.d");
    Gtk::ApplicationWindow window; // Main window

    //Buttons
    Gtk::Button m_button("Play"); // This Button is : "Play", "Pause", "Resume", "New Game"
    Gtk::Button m_button2("Difficulty"); // This Button is : "Difficulty" 
    Gtk::Button m_button3("Quit"); // This Button is : "Quit"
    Gtk::Button m_button4("Next Level"); // this Button is : "Next Level"

    //Boxes
    Gtk::Box box1(Gtk::ORIENTATION_VERTICAL); // Game
    Gtk::Box box2(Gtk::ORIENTATION_HORIZONTAL); // Menu buttons... Play, Difficulty, Quit
    Gtk::Box box3(Gtk::ORIENTATION_HORIZONTAL); // Difficulty


    //SpinButton
    Glib::RefPtr<Gtk::Adjustment> m_adjustment_difficulty( Gtk::Adjustment::create(1.0, 1.0, 3.0, 1.0, 5.0, 0.0)); //Spinbutton stuff
    Gtk::SpinButton m_SpinButton_Difficulty(m_adjustment_difficulty);
    Gtk::Label m_Label_Difficulty("Difficulty : ");
    box3.pack_start(m_Label_Difficulty);
    box3.pack_start(m_SpinButton_Difficulty);
    m_Label_Difficulty.override_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);

/**************************/
    Game game;   /*********   Our Game ******************/
/**************************/

    //Window configurations
    window.set_size_request(640,480);
    window.set_title("Missile Command");
    window.set_border_width(0);

    //Packing Menu buttons 
    box2.pack_start(m_button4, Gtk::PACK_EXPAND_WIDGET);
    box2.pack_start(m_button, Gtk::PACK_EXPAND_WIDGET);
    box2.pack_start(box3, Gtk::PACK_EXPAND_WIDGET);
    box2.pack_start(m_button3, Gtk::PACK_EXPAND_WIDGET);
   
    window.add(box1);
    //Packing Game
    box1.pack_start(game);
    game.show();
    //Packing Menubox
    box1.pack_start(box2, Gtk::PACK_SHRINK);
    box2.show(); // shows buttons
    box1.show(); //shows game
 
    window.show_all_children();    
    m_button4.hide(); //Next level button hidden for a while


    //Signal for Spinbutton. When value changed, sets the difficulty
    sigc::slot<void> sslot = sigc::bind<Game *, Gtk::SpinButton *> ( sigc::ptr_fun(&sbc), &game, &m_SpinButton_Difficulty);
    m_adjustment_difficulty->signal_value_changed().connect(sslot);

    //Idle Signal to check that game's done and change the labels
    sigc::slot<bool> tslot = sigc::bind<Game *, Gtk::Button *, Gtk::Button *> ( sigc::ptr_fun(&changeNG), &game, &m_button, &m_button4);
    Glib::signal_timeout().connect(tslot, 50 );

    //Signal when the "Play/Pause/Resume/NewGame" button clicked
    m_button.signal_clicked().connect( sigc::bind<Game *, Gtk::Button *, Gtk::Box *, Gtk::Button *, Gtk::SpinButton *> ( sigc::ptr_fun(&on_button_played), &game, &m_button, &box3, &m_button4, &m_SpinButton_Difficulty)  );

    //Quit signal
    m_button3.signal_clicked().connect(sigc::ptr_fun(&on_button3_closed ));

    //Next Level signal
    m_button4.signal_clicked().connect( sigc::bind<Game *, Gtk::Button *, Gtk::Button *> ( sigc::ptr_fun(&on_button_nextlvl), &game, &m_button4, &m_button)  );

    //BGM
    Mix_OpenAudio(11025, MIX_DEFAULT_FORMAT, 2, 512);
    music = Mix_LoadMUS("synth.mp3");
    Mix_PlayMusic(music, -1);
   
    return app->run(window); //Time to play
    Mix_FreeMusic(music);
    Mix_CloseAudio();    
}
