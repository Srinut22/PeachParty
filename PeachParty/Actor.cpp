#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												GAME OBJECT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

GameObject::GameObject(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size, int walkdirection, int facedirection)
	:GraphObject(imageID, startX, startY, dir, depth, size), my_world(sw), activity_status(true),
ticks_to_move(0), walk_direction(walkdirection), face_direction(facedirection)
{}

GameObject::~GameObject()
{
}

void GameObject::hit_by_vortex()
{
	return;//dummy method
}

bool GameObject::is_active() const
{
	return activity_status; //returns whether or not object is active
}

void GameObject::set_inactive()
{
	activity_status = false; //object is now inactive
}

void GameObject::teleport_me_to_random_square()
{
	GameObject* square = get_ptr_to_student_world()->get_random_square();
	moveTo(square->getX(), square->getY());//move the object to the random square on the board
}

void GameObject::force_walk_direction(int dir, int angle)
{
	walk_direction = dir;//set walk direction and sprite direction to dir and angle
	setDirection(angle);
}

int GameObject::get_walk_direction() const
{
	return walk_direction;//getter method->return walk_Direction
}

void GameObject::set_ticks(int ticks)
{
	ticks_to_move = ticks;//changes ticks_to_move to ticks
}

int GameObject::get_ticks() const
{
	return ticks_to_move; //getter method->return ticks_to_move
}

void GameObject::decrement_ticks()
{
	ticks_to_move--;//reduce ticks to move by one.
}

//forces the object to move in a random direction
void GameObject::pick_a_random_direction()
{
	int new_x, new_y;
	StudentWorld* world = get_ptr_to_student_world();
	while (true)//repeats until a direction with a square is found
	{
		int i = randInt(1, 4);
		switch (i)
		{
		case 1:
			getPositionInThisDirection(up, SPRITE_HEIGHT, new_x, new_y);
			if (world->is_there_a_square_at_location(new_x, new_y))//if there is a valid square in a particular direction, then
				//force the character to move in that direction
			{
				force_walk_direction(up, right);
				return;
			}
			break;
		case 2:
			getPositionInThisDirection(right, SPRITE_WIDTH, new_x, new_y);
			if (world->is_there_a_square_at_location(new_x, new_y))
			{
				force_walk_direction(right, right);
				return;
			}
			break;
		case 3:
			getPositionInThisDirection(down, SPRITE_HEIGHT, new_x, new_y);
			if (world->is_there_a_square_at_location(new_x, new_y))
			{
				force_walk_direction(down, right);
				return;
			}
			break;
		case 4:
			getPositionInThisDirection(left, SPRITE_WIDTH, new_x, new_y);
			if (world->is_there_a_square_at_location(new_x, new_y))
			{
				force_walk_direction(left, left);
				return;
			}
			break;
		}
	}
}

StudentWorld* GameObject::get_ptr_to_student_world() const
{
	return my_world;//returns pointer to StudentWorld
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												PLAYER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Stores player number, Sets both no_of_coins and no_of_stars to zero, Default walk direction right,
/// Player stars with no vortex projectiles, Ticks_to_move will be 0 until user input is given,
/// Player will be waiting for user input when constructed
/// </summary>
/// <param name="sw"> pointer to Student world</param>
/// <param name="imageID">either IID_Peach or IID_Yoshi -> Passed to Game Object ->Passed to Graph Object</param>
/// <param name="startX">Passed from Student World</param>
/// <param name="startY">Passed from StudentWorld</param>
/// <param name="player_no">1 for Peach, 2 for Yoshi</param>
Player::Player(StudentWorld* sw, int imageID, int startX, int startY, int player_no)
	:GameObject(sw, imageID, startX, startY, 0, 0, 1, right, right), last_rolled_no(0),
	player_number(player_no), no_of_coins(0), no_of_stars(0), no_of_vortex_projectiles(0), waiting_to_roll(true), invalid_walk_direction(-2)
{
	set_ticks(0);
}

void Player::do_something()
{
	int new_x = 0, new_y = 0, PIXELS_TO_MOVE = 2;
	StudentWorld* world = get_ptr_to_student_world();
	if (get_walk_direction() == invalid_walk_direction)
	{
		pick_a_random_direction();//pick a random direction to move in after being teleported
	}
	if (!is_walking())
	{
		int action = world->getAction(player_number);
		if (action == ACTION_ROLL) 
		{
			last_rolled_no = randInt(1, 10);
			set_ticks(last_rolled_no*8);//changes ticks_to_move to last_rolled_no * 8
			change_waiting_to_roll_status(false);
		}
		 if (action == ACTION_FIRE)
		 {
			if (has_vortex())//only do the following if the player has a projectile
			{
				//places the vortex projectile 1 pixel in front of the player character
				getPositionInThisDirection(get_walk_direction(), 1, new_x, new_y);
				world->addVortex(new_x, new_y, get_walk_direction());
				world->playSound(SOUND_PLAYER_FIRE);
				//resets vortex projectile count to zero once fired.
				reset_vortex_projectiles();
			}
		 }
		else
			return;
	}
	//if there is a fork and there is no directional square, then use player input to gauge direction of motion for the player.
	//if the character is on a square
	if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0)
	{
		if (world->is_a_fork(this) && !(get_ptr_to_student_world()->get_square_at_location(getX(), getY())->is_a_directional_square()))
		{
			int action = world->getAction(player_number);
			if (action == ACTION_RIGHT)
			{
				if (get_walk_direction() != left)//then the player tries to return in the direction they came from
				{
					getPositionInThisDirection(right, SPRITE_WIDTH, new_x, new_y);
					//if it is a valid direction, then force_walk_direction in the required direction
					get_ptr_to_student_world()->is_there_a_square_at_location(new_x, new_y);
					force_walk_direction(right, right);
				}
				else
					return;
			}
			//same code as above!!
			else if (action == ACTION_LEFT)
			{
				if (get_walk_direction() != right)
				{
					getPositionInThisDirection(left, SPRITE_WIDTH, new_x, new_y);
					get_ptr_to_student_world()->is_there_a_square_at_location(new_x, new_y);
					force_walk_direction(left, left);
				}
				else
					return;
			}
			else if (action == ACTION_UP)
			{
				if (get_walk_direction() != down)
				{
					getPositionInThisDirection(up, SPRITE_WIDTH, new_x, new_y);
					get_ptr_to_student_world()->is_there_a_square_at_location(new_x, new_y);
					force_walk_direction(up, right);
				}
				else
					return;
			}
			else if (action == ACTION_DOWN)
			{
				if (get_walk_direction() != up)
				{
					getPositionInThisDirection(down, SPRITE_WIDTH, new_x, new_y);
					get_ptr_to_student_world()->is_there_a_square_at_location(new_x, new_y);
					force_walk_direction(down, right);
				}
				else
					return;
			}
			else
				return;
		}
	}
	if (is_walking())
	{
		//if the character is on a square
		if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0)
		{
			//makes distinction of sprite width and sprite height although those are the same currently:
			if (get_walk_direction() == up || get_walk_direction() == down)
				getPositionInThisDirection(get_walk_direction(), SPRITE_HEIGHT, new_x, new_y);
			else if (get_walk_direction() == left || get_walk_direction() == right)
				//get position in the required direction
				getPositionInThisDirection(get_walk_direction(), SPRITE_WIDTH, new_x, new_y);

			//if there is a square in this direction, then it is a valid direction of motion
			//thus do nothing
			if (world->is_there_a_square_at_location(new_x, new_y))
			{
			}
			//else update the direction of moiton following spec
			//up prefered over down
			//right prefered over left
			else if (get_walk_direction() == right || get_walk_direction() == left)
			{
				if (world->is_there_a_square_at_location(getX(), getY() + SPRITE_HEIGHT))
					force_walk_direction(up, right);
				else
					force_walk_direction(down, right);

			}
			else if (get_walk_direction() == up || get_walk_direction() == down)
			{
				if (world->is_there_a_square_at_location(getX() + SPRITE_WIDTH, getY()))
					force_walk_direction(right, right);
				else
					force_walk_direction(left, left);
			}
			//move in the updated walk_direction
			moveAtAngle(get_walk_direction(), 2);
		}
		else
			//then there we aren't on top of a square, thus move in the old walk_direction.
			moveAtAngle(get_walk_direction(), 2);
	}
	decrement_ticks();//reduces ticks_to_move
	if (get_ticks() <= 0)
		change_waiting_to_roll_status(true);//stops the player from moving
}

void Player::reset_coins()
{
	no_of_coins = 0;
}

void Player::reset_vortex_projectiles()
{
	no_of_vortex_projectiles = 0;
}

void Player::reset_stars()
{
	no_of_stars = 0;
}

bool Player::is_a_square() const
{
	return false;
}

bool Player::is_a_directional_square() const
{
	return false;
}

bool Player::is_a_dropping_square() const
{
	return false;
}

bool Player::can_be_hit_by_vortex() const
{
	return false;
}

int Player::get_dice() const
{
	return last_rolled_no;
}

int Player::get_stars() const
{
	return no_of_stars;
}

int Player::get_coins() const
{
	return no_of_coins;
}

bool Player::has_vortex() const
{
	return no_of_vortex_projectiles != 0;
}

bool Player::is_walking() const
{
	return !waiting_to_roll;
}

void Player::adjust_stars(const int this_much)
{
	no_of_stars += this_much;
}

void Player::adjust_coins(const int this_much)
{
	no_of_coins += this_much;
}

void Player::change_waiting_to_roll_status(bool new_status)
{
	waiting_to_roll = new_status;
}

void Player::swap_positions()
{
	//swaps the x and y coordinates, ticks_to_move, 
	//waiting to roll status, walk direction, and sprite direction of peach and yoshi
	StudentWorld* world = get_ptr_to_student_world();
	int peach_x = world->getPeach()->getX(), peach_y = world->getPeach()->getY();
	world->getPeach()->moveTo(world->getYoshi()->getX(), world->getYoshi()->getY());//move Peach to Yoshi's location
	world->getYoshi()->moveTo(peach_x, peach_y); //move Yoshi to Peach's location
	int peach_ticks_to_move = world->getPeach()->get_ticks();
	world->getPeach()->set_ticks(world->getYoshi()->get_ticks());
	world->getYoshi()->set_ticks(peach_ticks_to_move);
	bool peach_status = !(world->getPeach()->is_walking());
	world->getPeach()->change_waiting_to_roll_status(!(world->getYoshi()->is_walking()));
	world->getYoshi()->change_waiting_to_roll_status(peach_status);
	int peach_walk_direction = world->getPeach()->get_walk_direction();
	int peach_sprite_direction = world->getPeach()->getDirection();
	world->getPeach()->force_walk_direction(world->getYoshi()->get_walk_direction(), world->getYoshi()->getDirection());
	world->getYoshi()->force_walk_direction(peach_walk_direction, peach_sprite_direction);
} 

void Player::swap_stars()
{
	//swaps the stars of the two players
	StudentWorld* world = get_ptr_to_student_world();
	int peach_stars = world->getPeach()->get_stars();
	world->getPeach()->reset_stars();
	world->getPeach()->adjust_stars(world->getYoshi()->get_stars());
	world->getYoshi()->reset_stars();
	world->getYoshi()->adjust_stars(peach_stars);
}

void Player::swap_coins()
{
	//swaps the coins of the two players
	StudentWorld* world = get_ptr_to_student_world();
	int peach_coins = world->getPeach()->get_coins();
	world->getPeach()->reset_coins();
	world->getPeach()->adjust_coins(world->getYoshi()->get_coins());
	world->getYoshi()->reset_coins();
	world->getYoshi()->adjust_coins(peach_coins);
}

void Player::equip_with_vortex_projectile()
{
	no_of_vortex_projectiles = 1;
}

void Player::make_direction_invalid()
{
	//invalid_walk_direction is an indicatior that the player has been teleported
	force_walk_direction(invalid_walk_direction, getDirection());
}


void Player::set_stationary()
{
	waiting_to_roll = true;
}

Player::~Player()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													STAR SQUARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

StarSquare::StarSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:GameObject(sw, imageID, startX, startY, 0, 1, 1, right, right)
{}
bool StarSquare::is_a_square() const
{
	return true;
}

bool StarSquare::is_a_directional_square() const
{
	return false;
}

bool StarSquare::is_a_dropping_square() const
{
	return false;
}

bool StarSquare::can_be_hit_by_vortex() const
{
	return false;
}

void StarSquare::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();

	//if a player overlaps with the square
	if (world->getYoshi()->getX() == getX() && world->getYoshi()->getY() == getY())
	{
		//of they have more than 20 coins, give them a star
		if (world->getYoshi()->get_coins() >= 20)
		{
			world->getYoshi()->adjust_coins(-20);
			world->getYoshi()->adjust_stars(1);
			world->playSound(SOUND_GIVE_STAR);
		}
	}
	//same as above except for peach
	if (world->getPeach()->getX() == getX() && world->getPeach()->getY() == getY())
	{
		if (world->getPeach()->get_coins() >= 20)
		{
			world->getPeach()->adjust_coins(-20);
			world->getPeach()->adjust_stars(1);
			world->playSound(SOUND_GIVE_STAR);
		}
	}
}

StarSquare:: ~StarSquare()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													COINSQUARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
CoinSquare::CoinSquare(StudentWorld* sw, int imageID, int startX, int startY, int adjust_coins_by)
	:GameObject(sw, imageID, startX, startY, 0, 1, 1, right, right), coins_to_give(adjust_coins_by), p1_just_landed(true), p2_just_landed(true)
{
	//if peach and yoshi have the same start position, dont increment coins. I chose to keep it at 0 for the start of the game
	if (sw->getPeach() != nullptr && sw->getPeach()->getX() == startX && sw->getPeach()->getY() == startY)
		p1_just_landed = false;
	if (sw->getYoshi() != nullptr && sw->getYoshi()->getX() == startX && sw->getYoshi()->getY() == startY)
		p2_just_landed = false;
}

void CoinSquare::do_something()
{
	//if it hasn't been destroyed by anything
	if (is_active())
	{
		StudentWorld* world = get_ptr_to_student_world();
		//if the player was initially on the square and then left, revert back the 
		//flag that indicates the player's first occurance on a square to its original value
		if (!p1_just_landed && (world->getPeach()->getX() != getX() || world->getPeach()->getY() != getY()))
			p1_just_landed = true;
		if (!p2_just_landed && (world->getYoshi()->getX() != getX() || world->getYoshi()->getY() != getY()))
			p2_just_landed = true;

		if (p1_just_landed && !(world->getPeach()->is_walking()) && (world->getPeach()->getX() == getX() && world->getPeach()->getY() == getY()))
		{
			if (coins_to_give < 0) //its a red coin square
			{
				if (world->getPeach()->get_coins() >= 3) //coins can never be negative
					world->getPeach()->adjust_coins(coins_to_give);
				else
					world->getPeach()->adjust_coins(-world->getPeach()->get_coins());//take away all the coins peach has otherwise

				world->playSound(SOUND_TAKE_COIN);
			}
			else
			{
				world->getPeach()->adjust_coins(coins_to_give);
				world->playSound(SOUND_GIVE_COIN);
			}

			p1_just_landed = false;//p1 isn't new to the square anymore.
		}

		//same code as above except for yoshi
		if (p2_just_landed && !(world->getYoshi()->is_walking()) && (world->getYoshi()->getX() == getX() && world->getYoshi()->getY() == getY()))
		{
			if (coins_to_give < 0)
			{
				if (world->getYoshi()->get_coins() >= 3)
					world->getYoshi()->adjust_coins(coins_to_give);
				else
					world->getYoshi()->adjust_coins(-world->getYoshi()->get_coins());
				world->playSound(SOUND_TAKE_COIN);
			}
			else
			{
				world->getYoshi()->adjust_coins(coins_to_give);
				world->playSound(SOUND_GIVE_COIN);
			}

			p2_just_landed = false;
		}
	}
	else
		return;
}
bool CoinSquare::is_a_square() const
{
	return true;
}

bool CoinSquare::is_a_dropping_square() const
{
	return false;
}

bool CoinSquare::is_a_directional_square() const
{
	return false;
}

bool CoinSquare::can_be_hit_by_vortex() const
{
	return false;
}

CoinSquare::~CoinSquare()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													DIRECTIONAL SQUARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

DirectionalSquare::DirectionalSquare(StudentWorld* sw, int imageID, int startX, int startY, int dir, int angle)
	:GameObject(sw, imageID, startX, startY, dir, 1, 1, right, right), force_direction(dir), force_angle(angle)
{
}

bool DirectionalSquare::is_a_square() const
{
	return true;
}

bool DirectionalSquare::is_a_dropping_square() const
{
	return false;
}

bool DirectionalSquare::is_a_directional_square() const
{
	return true;
}

bool DirectionalSquare::can_be_hit_by_vortex() const
{
	return false;
}

void DirectionalSquare::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	//if a player has the same coordinates as the square, force the player to move in the specified direction and angle 
	//as indicated by the square
	if (world->getPeach()->getX() == getX() && world->getPeach()->getY() == getY())
		world->getPeach()->force_walk_direction(force_direction, force_angle);
	if (world->getYoshi()->getX() == getX() && world->getYoshi()->getY() == getY())
		world->getYoshi()->force_walk_direction(force_direction, force_angle);
}

DirectionalSquare::~DirectionalSquare()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													BANK SQUARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BankSquare::BankSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:GameObject(sw, imageID, startX, startY, 0, 1, 1, right, right), p1_just_landed(true), p2_just_landed(true)
{
}

bool BankSquare::is_a_square() const
{
	return true;
}

bool BankSquare::is_a_dropping_square() const
{
	return false;
}

bool BankSquare::is_a_directional_square() const
{
	return false;
}

void BankSquare::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	//if the player was initially on the square and then left, revert back the 
		//flag that indicates the player's first occurance on a square to its original value
	if (!p1_just_landed && world->getPeach()->getX() != getX() || world->getPeach()->getY() != getY())
		p1_just_landed = true;
	if (!p2_just_landed && world->getYoshi()->getX() != getX() || world->getYoshi()->getY() != getY())
		p2_just_landed = true;
	//if the player is on the square for the first time
	if (world->getYoshi()->getX() == getX() && world->getYoshi()->getY() == getY() && p2_just_landed)
	{
		if (!world->getYoshi()->is_walking())//if the player is stationary
		{
			//give them all the coins stored in the bank
			world->getYoshi()->adjust_coins(world->get_bank_coins());
			world->reset_bank_coins();
			world->playSound(SOUND_WITHDRAW_BANK);
		}
		else
		{
			//else, take away 5 coins from them if possible, otherwise set their coin balance to 0
			if (world->getYoshi()->get_coins() > 5)
			{
				world->getYoshi()->adjust_coins(-5);
				world->deposit_bank_coins(5);
				world->playSound(SOUND_DEPOSIT_BANK);
			}
			else
			{
				int coins = world->getYoshi()->get_coins();
				world->getYoshi()->reset_coins();
				world->deposit_bank_coins(coins);
				world->playSound(SOUND_DEPOSIT_BANK);
			}
		}
		p2_just_landed = false; //player is no longer new to the square
	}

	//same code as above
	if (world->getPeach()->getX() == getX() && world->getPeach()->getY() == getY() && p1_just_landed)
	{
		if (!world->getPeach()->is_walking())
		{
			world->getPeach()->adjust_coins(world->get_bank_coins());
			world->reset_bank_coins();
			world->playSound(SOUND_WITHDRAW_BANK);
		}
		else
		{
			if (world->getPeach()->get_coins() > 5)
			{
				world->getPeach()->adjust_coins(-5);
				world->deposit_bank_coins(5);
				world->playSound(SOUND_DEPOSIT_BANK);
			}
			else
			{
				int coins = world->getPeach()->get_coins();
				world->getPeach()->reset_coins();
				world->deposit_bank_coins(coins);
				world->playSound(SOUND_DEPOSIT_BANK);
			}
		}
		p1_just_landed = false;
	}

}

bool BankSquare::can_be_hit_by_vortex() const
{
	return false;
}

BankSquare::~BankSquare()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													EVENT SQUARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
EventSquare::EventSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:GameObject(sw, imageID, startX, startY, 0, 1, 1, right, right), p1_just_landed(true), p2_just_landed(true)
{

}
bool EventSquare::is_a_square() const
{
	return true;
}

bool EventSquare::is_a_dropping_square() const
{
	return false;
}

bool EventSquare::is_a_directional_square() const
{
	return false;
}

bool EventSquare::can_be_hit_by_vortex() const
{
	return false;
}

void EventSquare::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	//if the player was initially on the square and then left, revert back the 
		//flag that indicates the player's first occurance on a square to its original value
	if (!p1_just_landed && world->getPeach()->getX() != getX() || world->getPeach()->getY() != getY())
		p1_just_landed = true;
	if (!p2_just_landed && world->getYoshi()->getX() != getX() || world->getYoshi()->getY() != getY())
		p2_just_landed = true;
	if (world->getPeach()->getX() == getX() && world->getPeach()->getY() == getY() && !(world->getPeach()->is_walking()) && p1_just_landed)
	{
		int i = randInt(1, 3);
		if (i == 1)
		{
			world->getPeach()->teleport_me_to_random_square(); //first case, teleport player to a random square
			world->getPeach()->make_direction_invalid(); //make direction invalid after teleporting
		}
		else if (i == 2)
		{
			world->getPeach()->swap_positions(); //swap positions of the two players
			p2_just_landed = false; //ensures the square doesn't act on the new player that lands here
		}
		else
		{
			if(!(world->getPeach()->has_vortex()))//if the player doesn't already have a vortex, then give them one
			{
				world->getPeach()->equip_with_vortex_projectile();
				world->playSound(SOUND_GIVE_VORTEX);
			}
		}
		p1_just_landed = false;
	}
	//same code as above except for Yoshi
	if (world->getYoshi()->getX() == getX() && world->getYoshi()->getY() == getY() && !(world->getYoshi()->is_walking()) && p2_just_landed)
	{
		int i = randInt(1, 3);
		if (i == 1)
		{
			world->getYoshi()->teleport_me_to_random_square();
			world->getYoshi()->make_direction_invalid();
		}
		else if (i == 2)
		{
			world->getYoshi()->swap_positions();
			p1_just_landed = false;
		}
		else
		{
			if(!(world->getYoshi()->has_vortex()))
			world->getYoshi()->equip_with_vortex_projectile();
			world->playSound(SOUND_GIVE_VORTEX);
		}
		p2_just_landed = false;
	}
}

EventSquare::~EventSquare()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													DROPPING SQUARE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
DroppingSquare::DroppingSquare(StudentWorld* sw, int imageID, int startX, int startY)
	:GameObject(sw, imageID, startX, startY, 0, 1, 1, right, right), p1_just_landed(true), p2_just_landed(true)
{

}

bool DroppingSquare::is_a_dropping_square() const
{
	return true;
}

bool DroppingSquare::is_a_square() const
{
	return true;
}

bool DroppingSquare::is_a_directional_square() const
{
	return false;
}

bool DroppingSquare::can_be_hit_by_vortex() const
{
	return false;
}

void DroppingSquare::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	//if the player was initially on the square and then left, revert back the 
		//flag that indicates the player's first occurance on a square to its original value
	if (!p1_just_landed && world->getPeach()->getX() != getX() || world->getPeach()->getY() != getY())
		p1_just_landed = true;
	if (!p2_just_landed && world->getYoshi()->getX() != getX() || world->getYoshi()->getY() != getY())
		p2_just_landed = true;
	//if there is overlap between a player and the square and the player just landed on the square and isn't walking
	if (world->getYoshi()->getX() == getX() && world->getYoshi()->getY() == getY() && p2_just_landed && !(world->getYoshi()->is_walking()))
	{
		if (randInt(1,2) == 1)
		{
			if (world->getYoshi()->get_coins() >= 10)
			{
				world->getYoshi()->adjust_coins(-10); //removes 10 coins from the player
			}
			else
				world->getYoshi()->adjust_coins(-world->getYoshi()->get_coins());
		}
		else
		{
			if (world->getYoshi()->get_stars() >= 1)
				world->getYoshi()->adjust_stars(-1); //else, removes one star 

		}
		world->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		p2_just_landed = false; //indicates that the player is no longer new to the environment of the square
	}

	//SAME CODE AS ABOVE
	if (world->getPeach()->getX() == getX() && world->getPeach()->getY() == getY() && p1_just_landed && !(world->getPeach()->is_walking()))
	{
		if (randInt(1,2) == 1)
		{
			if (world->getPeach()->get_coins() >= 10)
			{
				world->getPeach()->adjust_coins(-10);
			}
			else
				world->getPeach()->adjust_coins(-world->getPeach()->get_coins());
		}
		else
		{
			if (world->getPeach()->get_stars() >= 1)
				world->getPeach()->adjust_stars(-1);

		}
		p1_just_landed = false;
		world->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
}
DroppingSquare::~DroppingSquare()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													CLASS ENEMY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::Enemy(StudentWorld* sw, int imageID, int startX, int startY,
	int dir, double size, int depth, int num_sq_to_move, int number_of_ticks_to_pause)
	:GameObject(sw, imageID, startX, startY, dir, depth, size, right, right), is_Paused(true), travel_distance(0), ticks_to_pause(number_of_ticks_to_pause)
	, p1_just_landed(true), p2_just_landed(true)
{
	set_ticks(0);
}

bool Enemy::is_a_dropping_square() const
{
	return false;
}

bool Enemy::is_a_square() const
{
	return false;
}
bool Enemy::is_a_directional_square() const
{
	return false;
}

bool Enemy::can_be_hit_by_vortex() const
{
	return true;
}

void Enemy::set_ticks_to_pause(int tick)
{
	ticks_to_pause = tick;
}

bool Enemy::Peach_just_landed() const
{
	return p1_just_landed;
}

void Enemy::Peach_arrived()
{
	p1_just_landed = false;
}

void Enemy::Peach_just_left()
{
	p1_just_landed = true;
}

void Enemy::Yoshi_just_left()
{
	p2_just_landed = true;
}

void Enemy::Yoshi_arrived()
{
	p2_just_landed = false;
}

bool Enemy::Yoshi_just_landed() const
{
	return p2_just_landed;
}


void Enemy::hit_by_vortex() // called when enemy is hit by a vortex projectile (called by vortex projectile)
{
	GameObject* square = get_ptr_to_student_world()->get_random_square(); //teleports the baddie to this square
	moveTo(square->getX(), square->getY());
	setDirection(right);
	set_ticks_to_pause(180); //makes them wait for 180 ticks after teleporting as required by the spec
}

bool Enemy::is_not_moving() const
{
	return is_Paused;
}

void Enemy::decrement_ticks_to_pause()
{
	ticks_to_pause--;
}

int Enemy::get_ticks_to_pause() const
{
	return ticks_to_pause;
}

void Enemy::make_enemy_pause()
{
	is_Paused = true;
}

void Enemy::make_enemy_unpause()
{
	is_Paused = false;
}


Enemy::~Enemy()
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													CLASS BOWSER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bowser::Bowser(StudentWorld* sw, int imageID, int startX, int startY)
	:Enemy(sw, imageID, startX, startY, 0, 1, 0, 0, 180)
{
}

void Bowser::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	int PIXELS_TO_MOVE = 2, new_x = 0, new_y = 0;
	//if the player was initially on the square and then left, revert back the 
		//flag that indicates the player's first occurance on a square to its original value
	if (!Peach_just_landed() && world->getPeach()->getX() != getX() || world->getPeach()->getY() != getY())
		Peach_just_left();
	if (!Yoshi_just_landed() && world->getYoshi()->getX() != getX() || world->getYoshi()->getY() != getY())
		Yoshi_just_left();
	if (is_not_moving())
	{
		if (getX() == world->getPeach()->getX() && getY() == world->getPeach()->getY() && !(world->getPeach()->is_walking()) && Peach_just_landed())
		{
			//50% chance of happening
			if (randInt(1, 2) == 1)
			{
				//in this case, reset the players coins and stars to 0.
				world->getPeach()->reset_coins();
				world->getPeach()->reset_stars();
				world->playSound(SOUND_BOWSER_ACTIVATE);
			}
			Peach_arrived();
			world->playSound(SOUND_BOWSER_ACTIVATE);
		}
		if (getX() == world->getYoshi()->getX() && getY() == world->getYoshi()->getY() && !(world->getYoshi()->is_walking()) && Yoshi_just_landed())
		{
			//same code as above
			if (randInt(1, 2) == 1)
			{
				world->getYoshi()->reset_coins();
				world->getYoshi()->reset_stars();
				world->playSound(SOUND_BOWSER_ACTIVATE);
			}
			Yoshi_arrived();
			world->playSound(SOUND_BOWSER_ACTIVATE);
		}
		decrement_ticks_to_pause();
		if (get_ticks_to_pause() == 0)
		{
			//makes bowser move by the required amount
			int squares_to_move = randInt(1, 3);
			set_ticks(squares_to_move * 8);
			pick_a_random_direction();
			make_enemy_unpause();
		}
	}

	if (!is_not_moving())
	{
		if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0 && world->is_a_fork(this))
			pick_a_random_direction();
		else if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0)
		{
			if (get_walk_direction() == right || get_walk_direction() == left)
				getPositionInThisDirection(get_walk_direction(), SPRITE_WIDTH, new_x, new_y);
			else if (get_walk_direction() == up || get_walk_direction() == down)
				getPositionInThisDirection(get_walk_direction(), SPRITE_HEIGHT, new_x, new_y);

			//implies bowser cant keep moving in the direction currently moving
			if (!(world->is_there_a_square_at_location(new_x, new_y)))
			{
				//if bowser cant keep moving in current direction, updates walk_direction as required by the spec.
				//prefers up over down
				//prefers right over left
				if (get_walk_direction() == up || get_walk_direction() == down)
				{
					getPositionInThisDirection(right, SPRITE_WIDTH, new_x, new_y);
					if (world->is_there_a_square_at_location(new_x, new_y))
						force_walk_direction(right, right);
					else
						force_walk_direction(left, left);
				}
				else
				{
					getPositionInThisDirection(up, SPRITE_WIDTH, new_x, new_y);
					if (world->is_there_a_square_at_location(new_x, new_y))
						force_walk_direction(up, right);
					else
						force_walk_direction(down, right);
				}
			}
		}
		moveAtAngle(get_walk_direction(), PIXELS_TO_MOVE);//moves in the required direction which might've been changed by above code
		decrement_ticks();
		if (get_ticks() == 0)
		{
			make_enemy_pause();
			set_ticks_to_pause(180);
			//25% chance
			if (randInt(1, 4) == 1)
			{
				//makes dropping square at getX(), getY() after removing the square currently ocupying that spot
				world->remove_square_at(getX(), getY());
				world->make_dropping_square(getX(), getY());
			}
			else
				return;
		}
	}
}

Bowser::~Bowser()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													CLASS BOO
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Boo::Boo(StudentWorld* sw, int imageID, int startX, int startY)
	:Enemy(sw, imageID, startX, startY, 0, 1, 0, 0, 180)
{

}
void Boo::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	int PIXELS_TO_MOVE = 2, new_x = 0, new_y = 0;
	//if the player was initially on the square and then left, revert back the 
		//flag that indicates the player's first occurance on a square to its original value
	if (!Peach_just_landed() && world->getPeach()->getX() != getX() || world->getPeach()->getY() != getY())
		Peach_just_left();
	if (!Yoshi_just_landed() && world->getYoshi()->getX() != getX() || world->getYoshi()->getY() != getY())
		Yoshi_just_left();
	if (is_not_moving())
	{
		//if they have the same coordinates and the player just arrived at the square boo is in and isn't moving
		if (getX() == world->getPeach()->getX() && getY() == world->getPeach()->getY() && !(world->getPeach()->is_walking()) && Peach_just_landed())
		{
			//50% chance
			if (randInt(1, 2) == 1)
			{
				int yoshi_coins = world->getYoshi()->get_coins();
				world->getYoshi()->reset_coins();
				world->getYoshi()->adjust_coins(world->getPeach()->get_coins());
				world->getPeach()->reset_coins();
				world->getPeach()->adjust_coins(yoshi_coins);
				//swaps peach and yoshi's coins
			}
			else
			{
				int yoshi_stars = world->getYoshi()->get_stars();
				world->getYoshi()->reset_stars();
				world->getYoshi()->adjust_stars(world->getPeach()->get_stars());
				world->getPeach()->reset_stars();
				world->getPeach()->adjust_stars(yoshi_stars);
				//swaps peach and yoshis stars
			}
			Peach_arrived();//Peach is no longer new to the squares environment
			world->playSound(SOUND_BOO_ACTIVATE);
		}

		//same code as above
		if (getX() == world->getYoshi()->getX() && getY() == world->getYoshi()->getY() && !(world->getYoshi()->is_walking()) && Yoshi_just_landed())
		{
			if (randInt(1, 2) == 1)
			{
				int yoshi_coins = world->getYoshi()->get_coins();
				world->getYoshi()->reset_coins();
				world->getYoshi()->adjust_coins(world->getPeach()->get_coins());
				world->getPeach()->reset_coins();
				world->getPeach()->adjust_coins(yoshi_coins);
			}
			else
			{
				int yoshi_stars = world->getYoshi()->get_stars();
				world->getYoshi()->reset_stars();
				world->getYoshi()->adjust_stars(world->getPeach()->get_stars());
				world->getPeach()->reset_stars();
				world->getPeach()->adjust_stars(yoshi_stars);
			}
			Yoshi_arrived();
			world->playSound(SOUND_BOO_ACTIVATE);
		}
		decrement_ticks_to_pause();
		if (get_ticks_to_pause() == 0)
		{
			//if boo has waited long enough, then move boo as required
			int squares_to_move = randInt(1, 3);
			set_ticks(squares_to_move * 8);
			pick_a_random_direction();
			make_enemy_unpause();
		}
	}

	if (!is_not_moving())
	{
		//of boo is at a fork, then pick a random direction to move in
		if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0 && world->is_a_fork(this))
			pick_a_random_direction();
		else if (getX() % SPRITE_WIDTH == 0 && getY() % SPRITE_HEIGHT == 0)
		{
			if (get_walk_direction() == right || get_walk_direction() == left)
				getPositionInThisDirection(get_walk_direction(), SPRITE_WIDTH, new_x, new_y);
			else if(get_walk_direction() == up || get_walk_direction() == down)
				getPositionInThisDirection(get_walk_direction(), SPRITE_HEIGHT, new_x, new_y);
			//if boo can't keep moving in the current direction, then update the direction
			//as required by the spec
			//right over left
			//up over down
			if (!(world->is_there_a_square_at_location(new_x, new_y)))
			{
				if (get_walk_direction() == up || get_walk_direction() == down)
				{
					getPositionInThisDirection(right, SPRITE_WIDTH, new_x, new_y);
					if (world->is_there_a_square_at_location(new_x, new_y))
						force_walk_direction(right, right);
					else
						force_walk_direction(left, left);
				}
				else
				{
					getPositionInThisDirection(up, SPRITE_WIDTH, new_x, new_y);
					if (world->is_there_a_square_at_location(new_x, new_y))
						force_walk_direction(up, right);
					else
						force_walk_direction(down, right);
				}
			}
		}
		moveAtAngle(get_walk_direction(), PIXELS_TO_MOVE);
		decrement_ticks();
		if (get_ticks() == 0)
		{
			make_enemy_pause();
			set_ticks_to_pause(180);
		}
	}
}
Boo::~Boo()
{}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													VORTEX
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vortex::Vortex(StudentWorld* sw, int imageID, int startX, int startY, int dir)
	:GameObject(sw, imageID, startX, startY, dir, 0, 1, dir, dir)
{
}

bool Vortex::is_a_square() const
{
	return false;
}

bool Vortex::is_a_directional_square() const
{
	return false;
}

bool Vortex::can_be_hit_by_vortex() const
{
	return false;
}

bool Vortex::is_a_dropping_square() const
{
	return false;
}

void Vortex::do_something()
{
	StudentWorld* world = get_ptr_to_student_world();
	//if the vortex is inactive, dont do anything
	if (!is_active())
		return;

	moveAtAngle(get_walk_direction(), 2);

	//if the vortex has left the span of the screen, then set it to be inactive
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
		set_inactive();

	GameObject* baddie = world->check_overlap_with(this);
	if (baddie != nullptr) //baddie == nullptr implies there is no overlap currently
	{
		baddie->hit_by_vortex(); //ask the baddie to do as needed
		set_inactive(); //sets the baddie to be inactive
		world->playSound(SOUND_HIT_BY_VORTEX); 
	}
}
Vortex::~Vortex()
{
}