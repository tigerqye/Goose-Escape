/*
START OF gooseEscapeActors.hpp
*/

#ifndef GOOSE_ESCAPE_ACTORS
#define GOOSE_ESCAPE_ACTORS
#include <cmath>
#include <BearLibTerminal.h>
#include <time.h>
#include "gooseEscapeUtil.hpp"

//General actor class with code that apply to all moving elements in the game
class Actor
{  	
  protected: 
		int actorChar;      
    	int location_x, location_y;

  public:

	Actor()
    {
       	actorChar = int('A');
       	location_x = MIN_SCREEN_X;
       	location_y = MIN_SCREEN_Y;
       	//we don't want to print on terminal unless called for (to control number of g/meese)
    }

    Actor(char initPlayerChar, int x0, int y0)
    {
        change_char(initPlayerChar);
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        update_location(x0,y0);
    }
    
    int get_x() const
    {
        return location_x;
    }
    
    int get_y() const
    {
        return location_y;
    }
    
    string get_location_string() const
    {
        char buffer[80];
        itoa(location_x,buffer,10);
        string formatted_location = "(" + string(buffer) + ",";
        itoa(location_y,buffer,10);
        formatted_location += string(buffer) + ")";
        return formatted_location;
    }
    
    void change_char(char new_actor_char)
    {
        actorChar = min(int('~'),max(int(new_actor_char),int(' ')));
    }

    bool can_move(int delta_x, int delta_y) const
    {
        int new_x = location_x + delta_x;
        int new_y = location_y + delta_y;

        return new_x >= MIN_BOARD_X && new_x <= MAX_BOARD_X 
			   && new_y >= MIN_BOARD_Y && new_y <= MAX_BOARD_Y;
    }

    void update_location(int delta_x, int delta_y)
    {
        if (can_move(delta_x, delta_y))
        {
            terminal_clear_area(location_x, location_y, 1, 1);
            location_x += delta_x;
            location_y += delta_y;
            put_actor();
        }
    }
    
    void put_actor() const
    {
        terminal_put(location_x, location_y, actorChar);
        terminal_refresh();
    }
};

//child classes of the Actor class that give properties to different characters

//just initializes player with its proper character
class Protagonist : public Actor
{
  public:
	Protagonist()
	{
		actorChar = PLAYER_CHAR;
	}
		
	Protagonist(int x0, int y0)
	{
		actorChar = PLAYER_CHAR;
		update_location(x0,y0);
	}
};

//goose, monster that chases protagonist and flies over walls
class Flier : public Actor
{	
  public: 	
	Flier()
	{
		actorChar = GOOSE_CHAR;
	}
	
	Flier(int x0, int y0)
    {
        actorChar = GOOSE_CHAR;
        update_location(x0,y0);
    }
	
	//function to chase the player over walls 
	void chase(Actor & player, int game_board[NUM_BOARD_Y][NUM_BOARD_X], int turn)
	{
		//move every other (odd) turn
		if(turn%2 == 1) 
		{
			int yMove = 0, xMove = 0;
			//calcualtes the difference in positions
			int y_diff = location_y-player.get_y();
			int x_diff = location_x-player.get_x();

			//avoid scenario where both are 0 and we divide by 0
			if(y_diff != 0 || x_diff != 0) 
			{
				//moves one tile in the direction that the goose is furthest
				if(abs(y_diff) < abs(x_diff))
					xMove = -(x_diff) / abs(x_diff);
				else
					yMove = -(y_diff) / abs(y_diff);
			}
			
			//check if possible to move
			if (can_move(xMove,yMove) && 
				game_board[location_y+yMove][location_x+xMove] != MONSTER)
			{
				//store last location to update gameboard after it moves
				int last_x = location_x, last_y = location_y;
				
				update_location(xMove,yMove);
				
				int new_spot = game_board[location_y][location_x];
				//update location on map so monsters don't overlap with each other
				if(new_spot != SHALL_NOT_PASS && new_spot != WINNER)
					game_board[location_y][location_x] = MONSTER;
				//update screen with environmental character after goose walks over it
				if(game_board[last_y][last_x] == SHALL_NOT_PASS)
					terminal_put(last_x,last_y,WALL_CHAR);
				else if (game_board[last_y][last_x] == WINNER)
					terminal_put(last_x,last_y,WIN_CHAR);
				else
					game_board[last_y][last_x] = EMPTY;	
			}
		}
	}
};

//moose bounces from wall to wall
class Bouncer : public Actor
{
  private:
	int move_sign;
	bool horizontal;

  public:
	Bouncer()
	{
		//give the moose a random direction to move in
		move_sign = rand()%2*2-1;
		actorChar = MOOSE_CHAR;
		horizontal = rand()%2;
	}
	
	Bouncer(int x0, int y0)
    {
    	move_sign = rand()%2*2-1;
        actorChar = MOOSE_CHAR;
		horizontal = rand()%2;
        update_location(x0,y0);
    }
    
    void bounce(int game_board[NUM_BOARD_Y][NUM_BOARD_X])
    {
    	int xMove = 0, yMove = 0;
    	
    	//move in assigned direction
    	if(horizontal)
    		xMove = move_sign;
		else
			yMove = move_sign;
		
		//if moose runs into wall or another monster reverse direction of movement
		if(game_board[location_y+yMove][location_x+xMove]!=EMPTY)
    	{
    		move_sign *= -1;
    		xMove *= -1;
			yMove *= -1;	
		}
		
		//update gameboard
		game_board[location_y][location_x] = EMPTY;
    	update_location(xMove,yMove);
    	game_board[location_y][location_x] = MONSTER;
	}
};

#endif

/*

------------------------------------
END OF gooseEscapeActors.hpp
------------------------------------

*/
