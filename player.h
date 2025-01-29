#pragma once
#include <iostream>
#include <stack> 
#include <windows.h>
#include "boardProperties.h"
#include "ship.h"
#ifndef PLAYER_H
#define PLAYER_H
using namespace std;

class Player {
	public: 
		string playerName;
		boardSquare board[BOARD_DIMENSIONS+2][BOARD_DIMENSIONS+2];
		ship carrier = { "Carrier", 6, 6, false};
		ship battleship = {"Battleship", 5, 5, false};
		ship cruiser = { "Crusier", 4, 4, false};
		ship destroyer = { "Destroyer", 3, 3, false};
		ship submarine = { "Submarine", 3, 3, false,};
		ship patrolBoat = { "Patrol Boat", 2, 2, false}; 
		bool isCom = false; 
		/*int hitCount = 0;
		int missCount = 0;
		int totalShots = 0;*/
	public: 
		Player();
		~Player(); 
		void printBoard();  
		bool placeShip(int choiceShip, int choiceOrientation);
		bool placeBow(int choice, int choiceOrientation); //helper for placeShip
		bool completeShip(int choice, ship* shipChoice, int row, int colConv); //helper for placeShip
		void placeShipCom(); //placement function for computer opponent 
}; 
#endif
//could be put in CPP file 
Player::Player() {
	for (int i = 0; i < BOARD_DIMENSIONS+2; i++) {
		for (int j = 0; j < BOARD_DIMENSIONS+2; j++) {
			board[i][j].isHit = false;
			board[i][j].isMiss = false;
			board[i][j].isShip = false;
			board[i][j].isHidden = false; 
			board[i][j].isWall = false; 
			board[i][j].isOcean = false; 
			board[i][j].isFound = false; 
			if (j == 0 || j == BOARD_DIMENSIONS + 1 || i == 0 || i == BOARD_DIMENSIONS + 1) {
				//cout << "i: " << i << "j " << j<<endl; 
				board[i][j].isWall = true; 
			}
			else {
				board[i][j].isOcean = true;
			}
			board[i][j].shipClass = NULL; 
		}
	}
}
Player::~Player() {

}
void Player::printBoard() {
	int column = 65; //ASCII for A
	int row = 1; 
	int ship = 0; //numerical ID of ship
	//formatting 
	cout << "     ";
	//print column labels (letters) 
	if (isCom == true) {
		SetConsoleTextAttribute(Console, ENEMY_TEXT);
	}
	else {
		SetConsoleTextAttribute(Console, PLAYER_TEXT);
	}
	for (int i = 0; i <= BOARD_DIMENSIONS - 1; i++) {
		cout << char(column) << " ";
		column++; 
	}
	cout << endl; 
	//print board 
	for (int i = 0; i <= BOARD_DIMENSIONS+1; i++) {
		//print row labels (numbers)
		if (i != 0 && i != BOARD_DIMENSIONS + 1) {
			if (isCom == true) {
				SetConsoleTextAttribute(Console, ENEMY_TEXT);
			}
			else {
				SetConsoleTextAttribute(Console, PLAYER_TEXT);
			}
			cout << row << " ";
			//formatting 
			if (row <= 9) {
				cout << " ";
			}
			row++;
		}
		else {
			cout << "   ";
		}
		//actual board 
		for (int j = 0; j <= BOARD_DIMENSIONS+1; j++) {
			if ( (i == 0 || i == BOARD_DIMENSIONS + 1) || (j == BOARD_DIMENSIONS+1 || j == 0) ) {
				//set wall color depending on whose board is being printed
				if (isCom == true) {
					SetConsoleTextAttribute(Console, ENEMY_TEXT);
				}
				else {
					SetConsoleTextAttribute(Console, PLAYER_TEXT);
				}
				cout << OUT_OF_BOUNDS << OUT_OF_BOUNDS; 
			}
			else {
				SetConsoleTextAttribute(Console, WATER_TEXT);
				if (board[i][j].isShip == true) {
					
					//if is opponent's ship and square hasn't been hit or missed 
					if (board[i][j].isHidden == true && board[i][j].isHit == false && board[i][j].isMiss == false) {
						cout << OCEAN << " "; //SHOULD BE OCEAN
					}
					else if (board[i][j].isHit == true) {
						//compute sunk status to essentially change the color of the hit marker if ship is sunk
						//completely unnecessary and I could have just used a different character, but it looks better 
						if (board[i][j].shipClass->health == 0) {
							SetConsoleTextAttribute(Console, SUNK_TEXT);
							cout << HIT_MARKER << " ";
							//SetConsoleTextAttribute(Console, WATER_TEXT);
						}
						//hit, not sunk 
						else {
							SetConsoleTextAttribute(Console, HIT_TEXT);
							cout << HIT_MARKER << " ";
						}
					}
					//else is user's board; so print ship 
					else {
						SetConsoleTextAttribute(Console, SHIP_TEXT);
						cout << SHIP << " ";
					}
				}
				else if (board[i][j].isShip == false && board[i][j].isMiss == true) {
					cout << MISS_MARKER << " "; 
				}
				//not hit, miss, or ship
				else {
					cout << OCEAN << " ";
				}
			}
		}
		cout << endl; 
	}
	//set for other player's turn 
	if (isCom == true) {
		SetConsoleTextAttribute(Console, PLAYER_TEXT);
	}
	else {
		SetConsoleTextAttribute(Console, ENEMY_TEXT);
	}
	cout << endl; 
}
//PLAYER 
bool Player::placeShip(int choiceShip, int choiceOrientation) {
	placeBow(choiceShip, choiceOrientation);

	//printBoard(); 
	return true;
}
bool Player::placeBow(int choice, int choiceOrientation) {
	char column = ' ';
	int colConv = 0; //variable for conversion from user inputted char to corresponding column's int. Ex: D = 4 
	int row = 0;
	//placeholder variable for ship
	ship* shipChoice = NULL;
	switch (choice) {
	case 1:
		shipChoice = &carrier;
		break;
	case 2:
		shipChoice = &battleship;
		break;
	case 3:
		shipChoice = &cruiser;
		break;
	case 4:
		shipChoice = &destroyer;
		break;
	case 5:
		shipChoice = &submarine;
		break;
	case 6:
		shipChoice = &patrolBoat;
		break;
	}
	if (shipChoice->placed == true) {
		cout << "Admiral, you have already given this ship orders." << endl;
		return false;
	}
	cout << "Select coordinates for ship placement!" << endl;
	cout << "Enter Column: ";
	cin >> column;
	colConv = (int)column;
	if (column >= 97) {
		colConv = colConv - 32;
	}
	colConv = colConv - 64;
	if (colConv < 1 || colConv > BOARD_DIMENSIONS) {
		cout << "Those orders are not recognized. Try again!" << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		return false;
	}
	cout << "Enter Row: ";
	cin >> row;
	if (!std::cin) {
		cout << "Those orders are not recognized. Try again!" << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		return false;
	}
	cout << endl;
	if (colConv >= BOARD_DIMENSIONS + 1 || row >= BOARD_DIMENSIONS + 1 || colConv <= 0 || row <= 0) {
		cout << "Those orders are not valid, the ship will leave the battlefield!" << endl;
		return false;
	}
	//complete placement of the ship
	if (!completeShip(choiceOrientation, shipChoice, row, colConv)) {
		return false;
	}
	return true;
}
bool Player::completeShip(int choice, ship* shipChoice, int row, int colConv) {
	//stacks to store coordinates so if the placement is invalid for any part of the ship; 
	//the board is not changed and the user must place the ship again in a different spot
	stack<int> placementCoordsRow;
	stack<int> placementCoordsCol;
	int placementCounter = 0;
	//get numerical representation of shipType 
	int shipType = 0;
	if (shipChoice->shipType == "Carrier") {
		shipType = 1;
	}
	if (shipChoice->shipType == "Battleship") {
		shipType = 2;
	}
	if (shipChoice->shipType == "Cruiser") {
		shipType = 3;
	}
	if (shipChoice->shipType == "Destoryer") {
		shipType = 4;
	}
	if (shipChoice->shipType == "Submarine") {
		shipType = 5;
	}
	if (shipChoice->shipType == "Patrol Boat") {
		shipType = 6;
	}
	placementCoordsRow.push(row); //bow coord
	placementCoordsCol.push(colConv); //bow coord
	switch (choice) {
		//up
	case 1:
		for (int i = 0; i < shipChoice->size; i++) {
			if (board[row - placementCounter][colConv].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row - placementCounter][colConv].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row - placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		//place the ship on the board
		for (int i = 0; i < shipChoice->size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			if (isCom == true) {
				board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
			}
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//left
	case 2:
		for (int i = 0; i < shipChoice->size; i++) {
			if (board[row][colConv - placementCounter].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row][colConv - placementCounter].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv - placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			if (isCom == true) {
				board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
			}
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//right
	case 3:
		for (int i = 0; i < shipChoice->size; i++) {
			if (board[row][colConv + placementCounter].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row][colConv + placementCounter].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv + placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			if (isCom == true) {
				board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
			}
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//down
	case 4:
		for (int i = 0; i < shipChoice->size; i++) {
			if (board[row + placementCounter][colConv].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row + placementCounter][colConv].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row + placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			if (isCom == true) {
				board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
			}
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
	}
	//ship now placed and should not be able to be placed again (unless edit option is chosen) 
	shipChoice->placed = true;
	return true;
}
void Player::placeShipCom() {
	bool allPlaced = false;
	int row = 0;
	int col = 0;
	int orientation = 0;
	//**make true random (based off of system clock) 
	srand((int)time(0));
	ship* shipChoice = &carrier;
	//helper variable for accessing array 
	int currShip = 0;

	//array for keeping track of successfully placed ships
	bool placementSuccessful[6] = {};

	while (allPlaced == false) {
		//**generate random x and y coordinates for placement
		col = rand() % (BOARD_DIMENSIONS + 1) + 1;
		row = rand() % (BOARD_DIMENSIONS + 1) + 1;
		orientation = rand() % 4 + 1;
		//could be replaced with an if chain checking the placed status of each ship, but it's going to be 
		//an if chain regardless and this makes more sense to me. So it will stay as this 
		if (placementSuccessful[0] == false) {
			shipChoice = &carrier;
		}
		else if (placementSuccessful[1] == false) {
			shipChoice = &battleship;
		}
		else if (placementSuccessful[2] == false) {
			shipChoice = &cruiser;
		}
		else if (placementSuccessful[3] == false) {
			shipChoice = &destroyer;
		}
		else if (placementSuccessful[4] == false) {
			shipChoice = &submarine;
		}
		else if (placementSuccessful[5] == false) {
			shipChoice = &patrolBoat;
		}
		else {
			allPlaced = true;
			break;
		}
		//complete placement of the ship
		if (completeShip(orientation, shipChoice, row, col)) {
			placementSuccessful[currShip] = true;
			if (currShip < 5) {
				currShip++;
			}
		}
	}
}