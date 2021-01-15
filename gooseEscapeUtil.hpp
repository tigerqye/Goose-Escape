/*
START OF gooseEscapeUtil.hpp
*/

//STUDENTS: Be very careful if you decide to change these values

#ifndef GOOSE_UTIL
#define GOOSE_UTIL

//Number values to put different entities on the map
const int EMPTY = 0;
const int SHALL_NOT_PASS = 1;
const int WINNER = 2;
const int MONSTER = 3;
const int BIRD_FEED = 4;
const int TIGER_ERIC = 5;

//Characters for entities
const int PLAYER_CHAR = int('i');
const int GOOSE_CHAR = int('G');
const int MOOSE_CHAR = int('M');
const int SQUIRREL_CHAR = int('S');
const int WALL_CHAR = 2588; //wall box character
const int WIN_CHAR = int('%'); 
const int BIRD_FEED_CHAR = int('*');
const int TE_CHAR = int('?'); //easter egg at level 10

// Screen layout
const int NUM_SCREEN_X = 80;
const int NUM_SCREEN_Y = 25;
const char SETUP_MESSAGE[] = "window: title='Escape the Goose', size=80x25";
const int MIN_SCREEN_X = 0;
const int MIN_SCREEN_Y = 0;
const int MAX_SCREEN_X = NUM_SCREEN_X - 1;
const int MAX_SCREEN_Y = NUM_SCREEN_Y - 1;

// Play area layout
const int NUM_BOARD_X = 80;  // needs to be <= NUM_SCREEN_X
const int NUM_BOARD_Y = 21;  // needs to be < NUM_SCREEN_Y
const int MIN_BOARD_X = 0;
const int MIN_BOARD_Y = 0;
const int MAX_BOARD_X = NUM_BOARD_X - 1;
const int MAX_BOARD_Y = NUM_BOARD_Y - 1;


// Console message area layout
const int NUM_CONSOLE_X = 79;  // needs to be <= NUM_SCREEN_X
const int NUM_CONSOLE_Y = NUM_SCREEN_Y - NUM_BOARD_Y;
const int MIN_CONSOLE_X = 1;
const int MIN_CONSOLE_Y = MAX_BOARD_Y + 1;
const int MAX_CONSOLE_X = MAX_SCREEN_X;
const int MAX_CONSOLE_Y = MAX_SCREEN_Y;

#endif

/*

------------------------------------
END OF gooseEscapeUtil.hpp
------------------------------------

*/
