#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include "GameWorld.h"
#include <sstream>
#include <list>
#include "Board.h"

class GameObject;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// Determine if there is a square at the specified location. Used to determine if a gameobject
	// like a player, enemy or vortex can move onto the specified location
	bool is_there_a_square_at_location(int dest_x, int dest_y) const;

	// Get a pointer to the square at the specified location
	//returns nullptr if no square is found
	GameObject* get_square_at_location(double x, double y) const;

	// get # of coins in the bank
	int get_bank_coins() const;

	// add coins to the bank (when player passes over a bank square)
	void deposit_bank_coins(int coins);

	// reset # of coins in the bank to zero
	void reset_bank_coins();

	// Get a random location on the board that contains a square
	GameObject* get_random_square() const;

	//returns a pointer to yoshi
	Player* getYoshi() const;

	//returns a pointer to Peach
	Player* getPeach() const;

	//Adds a vortex projectile at (x,y) moving in direction dir(Passed from Player::do_something())
	void addVortex(int x, int y, int dir);

	//creates a dropping square at (x,y) which is added to the list of actors
	//ONLY CALL AFTER ENSURING THAT THE SQUARE THAT EXISTED AT (x,y) PREVIOUSLY HAS BEEN DELETED USING remove_square_at()!!
	void make_dropping_square(int x, int y);

	//removes the square at (x,y). Called to properly free up memory assosciated with a square before a dropping square is created in it's place
	void remove_square_at(int x, int y);

	//Returns if the location of p is a fork or not
	bool is_a_fork(GameObject* p);

	//returns if two objects overlap->used to check for the overlap of a vortex and a baddie
	//Returns nullptr if no overlap is found.
	GameObject* check_overlap_with(GameObject* p);
private:
	Player* Peach;
	Player* Yoshi;
	std::list<GameObject*> actors;
	int bank_coins, playersx, playersy;
};

#endif // STUDENTWORLD_H_