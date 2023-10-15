#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;


const int BLUE_COIN_SQUARE_COINS = 3;
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), bank_coins(0), Yoshi(nullptr), Peach(nullptr), playersx(0), playersy(0)
{
}

int StudentWorld::init()
{
    //Following the spec to load the board into bd to create the environment of the game within the code framework
    Board bd;
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_bad_format || result == Board::load_fail_file_not_found)
        return GWSTATUS_BOARD_ERROR;
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int k = 0; k < BOARD_HEIGHT; k++)
        {
            Board::GridEntry ge = bd.getContentsOf(i, k);
            //Ensure a blueCoinSquare is pushed with any Player/Baddie when creating the environment
            switch (ge)
            {
            case Board::empty:
                break;
            case Board::bank_square:
                actors.push_back(new BankSquare(this, IID_BANK_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                break;
            case Board::blue_coin_square:
                actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, BLUE_COIN_SQUARE_COINS));
                break;
            case Board::boo:
                actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, BLUE_COIN_SQUARE_COINS));
                actors.push_back(new Boo(this, IID_BOO, i * SPRITE_WIDTH, k * SPRITE_WIDTH));
                break;
            case Board::bowser:
                 actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, BLUE_COIN_SQUARE_COINS));
                 actors.push_back(new Bowser(this, IID_BOWSER, i * SPRITE_WIDTH, k * SPRITE_WIDTH));
                break;
            case Board::down_dir_square:
                actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, 270, 0));
                break;
            case Board::event_square:
                actors.push_back(new EventSquare(this, IID_EVENT_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                break;
            case Board::left_dir_square:
                actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, 180, 180));
                break;
            case Board::player:
                Peach = new Player(this, IID_PEACH, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, 1);
                Yoshi = new Player(this, IID_YOSHI, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, 2);
                playersx = i * SPRITE_WIDTH;
                playersy = k * SPRITE_HEIGHT;
                actors.push_back(new CoinSquare(this, IID_BLUE_COIN_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, BLUE_COIN_SQUARE_COINS));
                break;
            case Board::red_coin_square:
                actors.push_back(new CoinSquare(this, IID_RED_COIN_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, -BLUE_COIN_SQUARE_COINS));
                break;
            case Board::right_dir_square:
                actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, 0, 0));
                break;
            case Board::star_square:
                actors.push_back(new StarSquare(this, IID_STAR_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT));
                break;
            case Board::up_dir_square:
                actors.push_back(new DirectionalSquare(this, IID_DIR_SQUARE, i * SPRITE_WIDTH, k * SPRITE_HEIGHT, 90, 0));
                break;
            }
        }
    }
    //start the game timer for 99 seconds
    startCountdownTimer(15);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    int winner_coins, winner_stars;
    if (timeRemaining() <= 0)
    {
        //then the game has finished, thus play the required sound, and then check who has more stars/coins
        playSound(SOUND_GAME_FINISHED);
        if (Yoshi->get_stars() > Peach->get_stars())
        {
            winner_coins = Yoshi->get_coins();
            winner_stars = Yoshi->get_stars();
            setFinalScore(winner_stars, winner_coins);
            return GWSTATUS_YOSHI_WON;
        }
        else if (Yoshi->get_stars() < Peach->get_stars())
        {
            winner_coins = Peach->get_coins();
            winner_stars = Peach->get_stars();
            setFinalScore(winner_stars, winner_coins);
            return GWSTATUS_PEACH_WON;
        }
        else if(Yoshi->get_stars() == Peach->get_stars())
        {
            if (Yoshi->get_coins() > Peach->get_coins())
            {
                winner_coins = Yoshi->get_coins();
                winner_stars = Yoshi->get_stars();
                setFinalScore(winner_stars, winner_coins);
                return GWSTATUS_YOSHI_WON;
            }
            else if (Yoshi->get_coins() < Peach->get_coins())
            {
                winner_coins = Peach->get_coins();
                winner_stars = Peach->get_stars();
                setFinalScore(winner_stars, winner_coins);
                return GWSTATUS_PEACH_WON;
            }
            //when the 2 players have the same stars and coins, randomly assign a winner
            else
            {
                if (randInt(1, 2) == 1)
                {
                    winner_coins = Peach->get_coins();
                    winner_stars = Peach->get_stars();
                    setFinalScore(winner_stars, winner_coins);
                    return GWSTATUS_PEACH_WON;
                }
                else
                {
                    winner_coins = Yoshi->get_coins();
                    winner_stars = Yoshi->get_stars();
                    setFinalScore(winner_stars, winner_coins);
                    return GWSTATUS_YOSHI_WON;
                }
            }
        }
    }
    //calls the do_something() methods independently as the players aren't stored in the actors list.
    Peach->do_something();
    Yoshi->do_something();
    string display;
    ostringstream oss;
    oss << "P1 Roll: " << Peach->get_dice() << " Stars: " << Peach->get_stars() << " $$: " << Peach->get_coins() << " ";
    if (Peach->has_vortex()) //only add the VOR if the player has a vortex
    {
        oss << "VOR ";
    }
    oss << "| Time: " << timeRemaining() <<" | Bank: " << get_bank_coins() << " | P2 Roll: " << Yoshi->get_dice() << " Stars: " << Yoshi->get_stars() << " $$: " << Yoshi->get_coins() << " ";
    if (Yoshi->has_vortex())//only add the VOR is the player has the vortex
    {
        oss << "VOR ";
    }

    display = oss.str();
    setGameStatText(display);//displays the required text at the top of the game
    for (list<GameObject*>::const_iterator it = actors.begin(); it != actors.end(); it++)
    {
        if ((*it)->is_active()) //asks all active objects to do_something.
            (*it)->do_something();
    }

    for (list<GameObject*>::const_iterator it = actors.begin(); it != actors.end();)
    {
        if (!(*it)->is_active())
        {
            GameObject* temp = *it;
            it = actors.erase(it); //removes the actor from the list
            delete temp;//deallocates any memory that was allocated for the object
        }
        else
            it++;
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //deletes any dynamically allocated data
    for (list<GameObject*>::iterator it = actors.begin(); it != actors.end(); it++)
    {
        GameObject* temp = *it;
        delete temp;
    }
    delete Peach;
    delete Yoshi;
}

bool StudentWorld::is_there_a_square_at_location(int dest_x, int dest_y) const
{
    for (list<GameObject*>::const_iterator it = actors.begin(); it != actors.end(); it++)
    {
        //returns true if there is an object that is a square with the same x and y coordinate as the functions of the parameter
        if ((*it)->getX() == dest_x && (*it)->getY() == dest_y && (*it)->is_a_square())
            return true;
    }
    return false;
}

GameObject* StudentWorld::get_square_at_location(double x, double y) const
{
    for (list<GameObject*>::const_iterator it = actors.begin(); it != actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->is_a_square())
            return *it;
    }
    return nullptr;
}

int StudentWorld::get_bank_coins() const
{
    return bank_coins;
}

void StudentWorld::deposit_bank_coins(int coins)
{
    bank_coins += coins;
}

void StudentWorld::reset_bank_coins()
{
    bank_coins = 0;
}

GameObject* StudentWorld::get_random_square() const
{
    //creates a random x and y, and checks if there's a square that matches those coordinates until a match is found.
    while (true)
    {
        int x = randInt(0, BOARD_WIDTH - 1);
        int y = randInt(0, BOARD_HEIGHT - 1);

        for (list<GameObject*>::const_iterator it = actors.begin(); it != actors.end(); it++)
            if ((*it)->getX() == x * SPRITE_WIDTH && (*it)->getY() == y * SPRITE_HEIGHT && (*it)->is_a_square())
                return *it;
    }
}

Player* StudentWorld::getYoshi() const
{
    return Yoshi;
}

Player* StudentWorld::getPeach() const
{
    return Peach;
}

bool StudentWorld::is_a_fork(GameObject* p)
{
    int c = 0;
    int new_x = 0, new_y = 0;
    if (p->getX() == playersx && p->getY() == playersy)
        return false;

    for (int i = 0; i <= 270; i += 90)
    {
        if (!(i == p->get_walk_direction() + 180 || i == p->get_walk_direction() - 180))//ensures that the direction the player is coming from isn't counted as a valid square
            //making corners not be classified as a fork
        {
            p->getPositionInThisDirection(i, SPRITE_WIDTH, new_x, new_y);
            if (is_there_a_square_at_location(new_x, new_y))
                c++;
        }
        if (c == 2)
            return true;
    }
    return false;
}

void StudentWorld::remove_square_at(int x, int y)
{
    for (list<GameObject*>::iterator it = actors.begin(); it != actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->is_a_square())
        {
            GameObject* temp = *it;
            it = actors.erase(it);//removes the required square from the list
            delete temp;//deallocates memory
        }
    }
}

void StudentWorld::make_dropping_square(int x, int y)
{
    actors.push_back(new DroppingSquare(this, IID_DROPPING_SQUARE, x, y)); //creates a new dropping square at x,y, and pushes it to the list
    playSound(SOUND_DROPPING_SQUARE_CREATED);
}

GameObject* StudentWorld::check_overlap_with(GameObject* p)
{
    for (list<GameObject*>::iterator it = actors.begin(); it != actors.end(); it++)
    {
        if (((*it)->getX() >= p->getX() && (*it)->getX() < p->getX() + SPRITE_WIDTH) &&
            ((*it)->getY() >= p->getY() && (*it)->getY() < p->getY() + SPRITE_HEIGHT) &&
            (*it)->can_be_hit_by_vortex())
            return *it;//returns a pointer to the reuqired gameObject.
    }
    return nullptr;
}

void StudentWorld::addVortex(int x, int y, int dir)
{
    actors.push_back(new Vortex(this, IID_VORTEX, x, y, dir)); //ceates a new vortex which is added to the list of actors.
}