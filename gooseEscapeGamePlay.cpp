/*
START OF gooseEscapeGamePlay.cpp
*/

#include <iostream>
#include <cmath>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

extern Console out;

// print the game board function
void printBoard(int game_board[NUM_BOARD_Y][NUM_BOARD_X])
{
	for (int y_location_on_board = 0; y_location_on_board < NUM_BOARD_Y; 
		 y_location_on_board++)
	{
		for (int x_location_on_board = 0; x_location_on_board < NUM_BOARD_X; 
			 x_location_on_board++)
		{
			if(game_board[y_location_on_board][x_location_on_board] == WINNER)
				terminal_put(x_location_on_board,y_location_on_board,WIN_CHAR);
			else if(game_board[y_location_on_board][x_location_on_board] == SHALL_NOT_PASS)
				terminal_put(x_location_on_board,y_location_on_board,WALL_CHAR);	
			else if(game_board[y_location_on_board][x_location_on_board] == BIRD_FEED)
				terminal_put(x_location_on_board,y_location_on_board,BIRD_FEED_CHAR);	
		}
	}
}

//check if player on same tile as monster after each turn
bool captured(Actor const & player, int game_board[NUM_BOARD_Y][NUM_BOARD_X])
{
    return (game_board[player.get_y()][player.get_x()] == MONSTER);
}


void movePlayer(int key, Actor & player, int game_board[NUM_BOARD_Y][NUM_BOARD_X])
{
    int yMove = 0, xMove = 0;
    
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
        
    if (player.can_move(xMove, yMove) && 
		game_board[player.get_y()+yMove][player.get_x()+xMove]!= SHALL_NOT_PASS)
	{
		player.update_location(xMove, yMove);
	}
}


/*

------------------------------------
END OF gooseEscapeGamePlay.cpp
------------------------------------

*/
