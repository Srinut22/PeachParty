#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class GameObject : public GraphObject {
public:
    GameObject(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size, int walkdirection, int facedirection);
    virtual void do_something() = 0;
    virtual bool is_a_square() const = 0;
    virtual bool is_a_directional_square() const = 0;
    virtual bool is_a_dropping_square() const = 0;
    virtual bool can_be_hit_by_vortex() const = 0;
    virtual void hit_by_vortex(); //dummy method. does nothing. only used to ensure all derived classes do not become abstract as classes that cant be hit by projectiles
    //don't require this method
    // is a game object still alive or should it be removed from game?
    bool is_active() const;   
    //marks instance of the object for removal by StudentWorld::move()
    void set_inactive();
    //returns a pointer to StudentWorld
    StudentWorld* get_ptr_to_student_world() const;
    //moves the object to a random square
    virtual void teleport_me_to_random_square();
    //forces the object to move in dir with a sprite direction of angle
    virtual void force_walk_direction(int dir, int angle);
    //returns the current walk direction
    int get_walk_direction() const;
    //sets ticks_to_move to tick
    void set_ticks(int tick);
    //reduces ticks_to_move by 1
    void decrement_ticks();
    //returns the value of ticks_to_move
    int get_ticks() const;
    //forces the object to move in a random walk_direction with the appropriate sprite direction
    void pick_a_random_direction();
    virtual ~GameObject();
private:
    StudentWorld* my_world;
    bool activity_status;
    int ticks_to_move, walk_direction, face_direction;
};

class Player : public GameObject {
public:
    Player(StudentWorld* sw, int imageID, int startX, int startY, int player_no);
    //called during every tick of the game
    void do_something();
    //resets player's coins to zero
    void reset_coins();
    //resets player's stars to zero
    void reset_stars();
    //returns false since a player isn't a square
    bool is_a_square() const;
    //returns false since a player isn't a directional square
    bool is_a_directional_square() const;
    //returns false since a player isn't a dropping square
    bool is_a_dropping_square() const;
    //returns false since a player cant be hit by a vortex
    bool can_be_hit_by_vortex() const;
    // used to display stats on status line
    int get_stars() const;
    // used to display stats on status line
    int get_dice() const; 
    // used to display stats on status line
    int get_coins() const; 
    // used to display stats on status line
    bool has_vortex() const;
    //returns true if the player is walking
    bool is_walking() const;
    //changes the players stars by this_much
    void adjust_stars(const int this_much);
    //changes the players coins by this_much
    void adjust_coins(const int this_much);
    //both players exchange positions
    void swap_positions(); 
    //both players exchange their stars
    void swap_stars();
    //both players exchange their coins
    void swap_coins();
    //changes the vortex projectile count to a non-zero value
    void equip_with_vortex_projectile();
    //updates the waiting_to_roll variable ->used when two players swap positions
    void change_waiting_to_roll_status(bool new_status);
    //used when a player is freshly teleported. 
    void make_direction_invalid();
    //sets waiting_to_roll to true.
    void set_stationary();
    //makes the number of vortex projectiles the player is equipped with zero
    void reset_vortex_projectiles();
    virtual ~Player();
private:
    int player_number, no_of_coins, no_of_stars, no_of_vortex_projectiles, last_rolled_no, invalid_walk_direction;
    bool waiting_to_roll;
};


class Vortex : public GameObject {
public:
    Vortex(StudentWorld* sw, int imageID, int startX, int startY, int dir);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //called every tick
    void do_something();
    virtual ~Vortex();
};

class StarSquare : public GameObject {
public:
    StarSquare(StudentWorld* sw, int imageID, int startX, int startY);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    //called every tick
    virtual void do_something();
    virtual ~StarSquare();
};

class CoinSquare : public GameObject {
public:
    CoinSquare(StudentWorld* sw, int imageID, int startX, int startY, int adjust_coins_by);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    //called every tick
    virtual void do_something();
    virtual ~CoinSquare();
private:
    int coins_to_give;
    bool p1_just_landed, p2_just_landed;
};

class DirectionalSquare : public GameObject {
public:
    DirectionalSquare(StudentWorld* sw, int imageID, int startX, int startY, int dir, int angle);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    virtual void do_something();
    //called every tick
    virtual ~DirectionalSquare();
private:
    int force_direction, force_angle;
};

class BankSquare : public GameObject {
public:
    BankSquare(StudentWorld* sw, int imageID, int startX, int startY);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    virtual void do_something();
    //called every tick
    virtual ~BankSquare();
private:
    bool p1_just_landed, p2_just_landed;
};

class EventSquare : public GameObject {
public:
    EventSquare(StudentWorld* sw, int imageID, int startX, int startY);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    //called every tick
    virtual void do_something();
    virtual ~EventSquare();
private:
    bool p1_just_landed, p2_just_landed;
};

class DroppingSquare : public GameObject {
public:
    DroppingSquare(StudentWorld* sw, int imageID, int startX, int startY);
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    virtual void do_something();
    //called every tick
    virtual ~DroppingSquare();
private:
    bool p1_just_landed, p2_just_landed;
};

class Enemy : public GameObject {
public:
    Enemy(StudentWorld* sw, int imageID, int startX, int startY,
        int dir, double size, int depth, int num_sq_to_move, int number_of_ticks_to_pause);
    //ENEMY IS AN ABSTRACT BASE CLASS
    virtual void do_something() = 0;
    //is the object a square?
    bool is_a_square() const;
    //is the object a directional square?
    bool is_a_directional_square() const;
    //is the object a dropping square?
    bool is_a_dropping_square() const;
    //can the object be hit by a vortex?
    bool can_be_hit_by_vortex() const;
    // called when enemy is hit by a vortex projectile (called by vortex projectile)
    void hit_by_vortex();  
    //returns true if the enemy is stationary
    bool is_not_moving() const;
    //changes ticks_to_pause to tick
    void set_ticks_to_pause(int tick);
    //reduces ticks_to_pause by 1
    void decrement_ticks_to_pause();
    //Returns true if peach is newly introduced to the environment of the baddie in the current tick
    bool Peach_just_landed() const;
    //changes a flag to true if peach leaves the square of the baddie
    void Peach_just_left();
    //sets the flag to false to imply that peach isn't new to the environment of the square
    void Peach_arrived();
    //Returns true if yoshi is newly introduced to the environment of the baddie in the current tick
    bool Yoshi_just_landed() const; 
    //changes a flag to true if peach leaves the square of the baddie
    void Yoshi_just_left();
    //changes a flag to true if yoshi leaves the square of the baddie
    void Yoshi_arrived();
    //returns the amount of ticks to pause
    int get_ticks_to_pause() const;
    //changes the enemy's state to the active state
    void make_enemy_unpause();
    //changes the enemy state to the paused state.
    void make_enemy_pause();
    virtual ~Enemy();
private:
    bool is_Paused, p1_just_landed, p2_just_landed;
    int ticks_to_pause, travel_distance;
};

class Bowser : public Enemy {
public:
    Bowser(StudentWorld* sw, int imageID, int startX, int startY);
    //called every tick
    virtual void do_something();
    virtual ~Bowser();
};

class Boo : public Enemy {
public:
    Boo(StudentWorld* sw, int imageID, int startX, int startY);
    //called every tick
    virtual void do_something();
    virtual ~Boo();
};

#endif // ACTOR_H_