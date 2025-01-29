#pragma once
#include <iostream>
#include "player.h"
#include "ship.h" 
#ifndef BOARDPROPERTIES_H
#define BOARDPROPERTIES_H
using namespace std; 

const int BOARD_DIMENSIONS = 20;
const char MISS_MARKER = 'O';
const char HIT_MARKER = 'X';
const char OCEAN = '~';
const char SHIP = '*';
const char OUT_OF_BOUNDS = char(177);
const int PLAYER_TEXT = 1;
const int ENEMY_TEXT = 4;
const int WATER_TEXT = 55;
const int SHIP_TEXT = 56;
const int HIT_TEXT = 52;
const int MISS_TEXT = 63;
const int HEADER_TEXT = 7;
const int SUNK_TEXT = 48; 
const HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);

struct boardSquare {
	bool isShip; 
	bool isHit;
	bool isMiss; 
	bool isOcean; 
	bool isWall; 
	bool isHidden; 	
	/*isFound helps the COM perform shot calculation after hitting a ship.
	  The COM scans the board for hit squares and if a ship is hit but not sunk; 
	  then the COM calculates the correct coordinate for the next shot by examining the squares around the hit peg.
	  As you would do when playing Battleship; upon hitting a ship, you fire around that single hit square until you 
	  discover another hit square, determining the "direction" the ship is in in the process. 
	  If you fire a line of hits and then get a miss (e.g. ~~XXX0~ for a cruiser) then you go to the 
	  other side of the line and place your next (and final) shot for that ship there, making ~XXXX0~ and sinking the cruiser. 
	  This is meant to help the COM achieve that exact process. 
	*/
	bool isFound; 
	ship *shipClass; 
};
#endif 