/*
START OF gooseEscapeMain.cpp
*/

#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
#include <time.h>
using namespace std;
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"


//set up the console.
Console out;

//function to count up the number of empty spaces in and around a tile
int empty_spaces(int game_board[NUM_BOARD_Y][NUM_BOARD_X], int x, int y)
{
	int empt = 0;
	if(x>0 && x < NUM_BOARD_X-1 && y>0 && y < NUM_BOARD_Y-1)
	{
		if(game_board[y][x] == EMPTY)
			empt++;
		if(game_board[y][x+1] == EMPTY)
			empt++;
		if(game_board[y][x-1] == EMPTY)
			empt++;
		if(game_board[y+1][x] == EMPTY)
			empt++;
		if(game_board[y-1][x] == EMPTY)
			empt++;
	}
	return empt;
}
	
int main()
{
	//Set up the window. 
    terminal_open();
  	terminal_set(SETUP_MESSAGE);
	
	//get random seed depending on time
	srand (time(NULL));
	/*
	All key presses start with "TK_" then the character.  So "TK_A" is the "a"
	key being pressed.
	*/
	int keyEntered = TK_A; 
	//level counter 
	int level = 0;
	//bird_feed can be used to stop monster movement for 3 turns
	int bird_feed = 0;
	//bool to check if game is lost yet
	bool lost = false;
	
	//after level 10 no extra additional monsters spawn
	const int MAX_LEVEL = 10;
	
	//most number of empty spaces for a particular tile
	const int MAX_EMPTY = 5;
	
	//easter egg for fun
	Actor tigeri(TE_CHAR, 0, 0);
	
	//loop to create a new level until game is lost or closed	
	do{
		//clear terminal to print new level
		terminal_clear();
		
		//increase level each time a level is cleared
		level++;
		
		int game_board[NUM_BOARD_Y][NUM_BOARD_X] = {0};
		
		//create monster arrays, maximum 10 geese and 10 "meese" (moose plural) on level
		Flier geese[MAX_LEVEL];
		Bouncer meese[MAX_LEVEL];
		
		//constants for calculating number of walls in level
		const int WALL_NUMBER_VARIATION = 10;
		const int MIN_WALL_NUMBER = 15;
		const int LEVEL_SCALING = 3;
		
		//random number of walls, minimum number of walls per level increases per level
		for(int wall = 0; wall < rand() % WALL_NUMBER_VARIATION + MIN_WALL_NUMBER
			 + LEVEL_SCALING*level; wall++)
		{
			//locations of walls
			int wall_y = 0;
			int wall_x = 0;
			
			//find random spots to start walls within the limits (max x and max y - 1)
			do{
				wall_y = rand() % (NUM_BOARD_Y-1);
				wall_x = rand() % (NUM_BOARD_X-1);
				//continue searching until a completely clear spot is found, ensures that 
				//there are no inaccessible spots in the map
			}while(empty_spaces(game_board,wall_x,wall_y) < MAX_EMPTY);
			
			//update gameboard with start point of new wall
			game_board[wall_y][wall_x] = SHALL_NOT_PASS;
			
			//constants for turns in wall
			const int WALL_TURN_VARIATION = 8;
			const int MIN_TURNS = 5;
			
			//random number of turns in the wall
			for(int wall_turns = 0; wall_turns < rand() % WALL_TURN_VARIATION + MIN_TURNS; 
				wall_turns++)
			{
				//assign random direction to the turn
				//either 1 or 0, indicates horizontal or vertical movement
				bool rand_direction = rand() % 2;
				//either -1 or 1, indicates if moves "left/right" or "up/down"
				int rand_way = (rand() % 2) * 2 -1;
				
				const int WALL_LENGTH_VARIATION = 5;
				const int MIN_LENGTH = 8;
				
				//random wall length after each turn
				for (int wall_length = 0; wall_length < rand() % WALL_LENGTH_VARIATION 
					 + MIN_LENGTH; wall_length++)
				{
					//checks if horizontal wall and if it cuts off areas of map, ensures 
					//that the wall is not cutting off locations of map
					if(rand_direction && empty_spaces(game_board, wall_x+2*rand_way, wall_y) 
					   == MAX_EMPTY)
					{
						//update gameboard
						wall_x += rand_way;
						game_board[wall_y][wall_x] = SHALL_NOT_PASS;
					}
					else if (empty_spaces(game_board, wall_x, wall_y+2*rand_way) 
							 == MAX_EMPTY)
					{
						wall_y += rand_way;
						game_board[wall_y][wall_x] = SHALL_NOT_PASS;
					}
					else
					{
						//arbitrary value greater than the max length of the wall to
						//leave loop if the wall cannot get longer
						wall_length = 100;
					}
				}
			}
		}
		
		//update game_board boarder and map
		for(int border_x = 0; border_x < NUM_BOARD_X; border_x++)
		{
			game_board[0][border_x] = SHALL_NOT_PASS;
			game_board[NUM_BOARD_Y-1][border_x] = SHALL_NOT_PASS;
		}
		for(int border_y = 0; border_y < NUM_BOARD_Y; border_y++)
		{
			game_board[border_y][0] = SHALL_NOT_PASS;
			game_board[border_y][NUM_BOARD_X-1] = SHALL_NOT_PASS;
		}
		
		int win_tile_y = 0;
		int win_tile_x = 0;
		
		//minimum openings to ensure a tile can be accessed from at least one other tile
		const int MIN_OPENINGS = 2;
		
		//random win tile location within the outside border of game_board
		do{
			win_tile_y = rand() % (NUM_BOARD_Y-2) + 1;
			win_tile_x = rand() % (NUM_BOARD_X-2) + 1;
			//ensure there is at least one opening to access the win tile
		}while(empty_spaces(game_board,win_tile_x,win_tile_y) < MIN_OPENINGS);
		
		game_board[win_tile_y][win_tile_x] = WINNER;
		
		//bird feed allows character to freeze time and move uninterrupted for 3 turns
		int bird_feed_y = 0;
		int bird_feed_x = 0;
		
		//Minimum of 1 bird feed per level, max of 3
		const int FEED_VARIATION = 3;
		const int MIN_FEED = 1;
		
		for(int amount = 0; amount < rand() % FEED_VARIATION + MIN_FEED; amount++)
		{
			do{
				//check locations within game border
				bird_feed_y = rand() % (NUM_BOARD_Y-2) + 1;
				bird_feed_x = rand() % (NUM_BOARD_X-2) + 1;
			}while(empty_spaces(game_board,bird_feed_x,bird_feed_y) < MIN_OPENINGS);
			
			game_board[bird_feed_y][bird_feed_x] = BIRD_FEED;
		}
		
		//turns to control movement of monsters and player
		int turn = 0;
		int time_stop = 0;
		
		//coordinates of actor spawn locations
		int actor_y = 0, actor_x = 0;
		
		//create player
		Protagonist player;
		
		const int MIN_Y_DIFF = 8;
		const int MIN_X_DIFF = 20;
		
		//find random location for player within boundary
		do{
			actor_y = rand() % (NUM_BOARD_Y-2) + 1;
			actor_x = rand() % (NUM_BOARD_X-2) + 1;
			//find new spot until player is far enough from win locaiton
		}while(game_board[actor_y][actor_x] != EMPTY 
			   || abs(actor_y - win_tile_y) < MIN_Y_DIFF 
			   || abs(actor_x - win_tile_x) < MIN_X_DIFF);
			   
		player.update_location(actor_x, actor_y);
		
		//spawn geese and meese accordingly to level (level 2 == 2 geese 2 meese)
		for (int spawn_level = 0; spawn_level < level; spawn_level++)
		{
			//find random locations for spawn
			do{
				actor_y = rand() % (NUM_BOARD_Y-2)+1;
				actor_x = rand() % (NUM_BOARD_X-2)+1;
			}while(game_board[actor_y][actor_x] != EMPTY);
			//spawn in appropriate goose from the geese array
			geese[spawn_level].update_location(actor_x, actor_y);
			
			do{
				actor_y = rand() % (NUM_BOARD_Y-2)+1;
				actor_x = rand() % (NUM_BOARD_X-2)+1;
			}while(game_board[actor_y][actor_x] != EMPTY);
			
			meese[spawn_level].update_location(actor_x, actor_y);
		}
		
		//secret easter egg if player reaches level 10
		if(level == MAX_LEVEL) 
		{
			int te_y = 0;
			int te_x = 0;
			do{
				te_y = rand() % (NUM_BOARD_Y-2) + 1;
				te_x = rand() % (NUM_BOARD_X-2) + 1;
			}while(empty_spaces(game_board,te_x,te_y) < MIN_OPENINGS 
				   && game_board[te_y][te_x] == EMPTY);
			game_board[te_y][te_x] = TIGER_ERIC;
			tigeri.update_location(te_x,te_y);
		}
		//end of easter egg code
		
		
		//print board
		printBoard(game_board);
		
		//print current level and startup messages;
		char buffer[80];
		itoa(level,buffer,10);
		out.writeLine("Escape the G/Moose! Level:" + string(buffer));
		out.writeLine("Use the arrow keys to move");
		out.writeLine("If a goose or moose catches you, you lose!");
		out.writeLine("Be careful! The goose can fly over walls!");
		
		//move when key is entered (that is not escape or close)
		while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
		  && !lost 
		  && game_board[player.get_y()][player.get_x()] != WINNER)
		{
	    // get player key press
	    keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
			//move the player
    	    movePlayer(keyEntered, player, game_board);
    	    
    	    //check if player collected bird feed
    	    if(game_board[player.get_y()][player.get_x()] == BIRD_FEED)
				{
					bird_feed++;
					out.writeLine("You found bird feed! Press (B) to use.");
					char buffer[80];
					itoa(bird_feed,buffer,10);
					out.writeLine("Bird Feed Amount: " + string(buffer));
					game_board[player.get_y()][player.get_x()] = EMPTY;
				}
				
			if(keyEntered == TK_B)
			{
				const int MAX_TIME_STOP = 3;
				if(bird_feed > 0)
				{
					//stop time and reduce bird feed amount
					time_stop = MAX_TIME_STOP;
					bird_feed -= 1;
					char buffer[80];
					itoa(bird_feed,buffer,10);
					out.writeLine("Bird Feed Amount: " + string(buffer));
				}
				else
					out.writeLine("You have no bird feed!");	
			}
			
			//if birdfeed is not in effect
			if(time_stop == 0)
			{
				//move through all the geese and meese moves
    	   		for(int move = 0; move < level; move++)
    	    	{
    	    		geese[move].chase(player, game_board, turn);
    	  	  		meese[move].bounce(game_board);
				}
			}
			
			/*if you move into the spot where a monster was last turn, the update location
			will delete the player char, so this is just reprinting it in case that
			happens*/
			player.update_location(0,0);
			
			//easter egg
			if(game_board[player.get_y()][player.get_x()] == TIGER_ERIC)
			{
				out.writeLine("Tiger: COME ON WHY IS IT NOT WORKING!");
				out.writeLine("Eric: Stupid moose.");
				out.writeLine("Tiger/Eric: Go away we are trying to finish our goose game!");
				game_board[player.get_y()][player.get_x()] = EMPTY;
			}
			//end of easter egg code
			
			terminal_refresh();
			turn++;	    
        }
        //check if player is captured
        lost = captured(player,game_board);
        
        //run down the timer for the time_stop if it is in effect
        if(time_stop > 0)
	        time_stop--;
  		}
  		//repeats loop generating new levels until player is defeated
	}while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE && !lost);
    

    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
		out.writeLine("GAME OVER");
		char buffer[80];
		itoa(level, buffer, 10);
		out.writeLine("You made it to level: " + string(buffer));
    	// Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }

	//game is done, close it  
    terminal_close();
}

/*

------------------------------------
END OF gooseEscapeMain.cpp
------------------------------------

*/
