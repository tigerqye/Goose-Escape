/*
START OF gooseEscapeGamePlay.hpp
*/

#ifndef GOOSE_ESCAPE_GAMEPLAY
#define GOOSE_ESCAPE_GAMEPLAY
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"



// print the game board function
void printBoard(int game_board[NUM_BOARD_Y][NUM_BOARD_X]);

//check if player captured function
bool captured(Actor const & player, int game_board[NUM_BOARD_Y][NUM_BOARD_X]);

void movePlayer(int key, Actor & player, int game_board[NUM_BOARD_Y][NUM_BOARD_X]);

#endif

/*

------------------------------------
END OF gooseEscapeGamePlay.hpp
------------------------------------

*/
