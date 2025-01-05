#pragma once

#include <iostream>
#include <stack> 
#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

const int BOARD_DIMENSIONS = 20; 
const char MISS_MARKER = 'O';
const char HIT_MARKER = 'X';
const char OCEAN = '~';
const char SHIP = '*'; 
const char OUT_OF_BOUNDS = char(177);

struct ship {
	string shipType; 
	int size; 
	bool placed; 
};

class Player {
	public: 
		string playerName;
		bool isCOM = false; 
		char board[BOARD_DIMENSIONS+2][BOARD_DIMENSIONS+2];
		ship carrier = { "Carrier", 6, false };
		ship battleship = {"Battleship", 5, false};
		ship cruiser = { "Crusier", 4 , false};
		ship destroyer = { "Destroyer", 3, false };
		ship submarine = { "Submarine", 3 , false};
		ship patrolBoat = { "Patrol Boat", 2, false }; 

		int hitCount = 0;
		int missCount = 0;
		int totalShots = 0;

		
	public: 
		Player();
		~Player(); 
		void printBoard(); 
		bool placeShip(int choiceShip, int choiceOrientation);
		bool placeBow(int choice, int choiceOrientation); //helper for placeShip
		bool completeShip(int choice, ship shipChoice, int row, int colConv); //helper for placeShip
		void placeShipRandom(); 
}; 
#endif

//could be put in CPP file 
Player::Player() {
	for (int i = 0; i < BOARD_DIMENSIONS+2; i++) {
		for (int j = 0; j < BOARD_DIMENSIONS+2; j++) {
			if (j == 0 || j == BOARD_DIMENSIONS + 1 || i == 0 || i == BOARD_DIMENSIONS + 1) {
				//cout << "i: " << i << "j " << j<<endl; 
				board[i][j] = OUT_OF_BOUNDS; 
			}
			else {
				board[i][j] = OCEAN;
			}
		}
	}
}
Player::~Player() {

}
void Player::printBoard() {
	int column = 65; //ASCII for A
	int row = 1; 
	//formatting 
	cout << "     ";
	//print column labels (letters) 
	for (int i = 0; i <= BOARD_DIMENSIONS - 1; i++) {
		cout << char(column) << " ";
		column++; 
	}
	cout << endl; 
	//print board 
	for (int i = 0; i <= BOARD_DIMENSIONS+1; i++) {
		//print row labels (numbers)
		if (i != 0 && i != BOARD_DIMENSIONS + 1) {
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
		//board 
		for (int j = 0; j <= BOARD_DIMENSIONS+1; j++) {
			if ( (i == 0 || i == BOARD_DIMENSIONS + 1) || (j == BOARD_DIMENSIONS+1 || j == 0) ) {
				cout << board[i][j] << board[i][j]; 
			}
			else {
				cout << board[i][j] << " ";
			}
		}
		cout << endl; 
	}
	cout << endl; 
}
bool Player::placeShip(int choiceShip, int choiceOrientation) {
	placeBow(choiceShip, choiceOrientation); 
	
	printBoard(); 
	return true; 
}
bool Player::placeBow(int choice, int choiceOrientation) {
	char column = ' ';
	int colConv = 0; //variable for conversion from user inputted char to corresponding column's int. Ex: D = 4 
	int row = 0;
	//placeholder variable for ship
	ship shipChoice = { "placeholder", 0, false };
	switch (choice) {
	case 1:
		shipChoice = carrier;
		break;
	case 2:
		shipChoice = battleship;
		break;
	case 3:
		shipChoice = cruiser;
		break;
	case 4:
		shipChoice = destroyer;
		break;
	case 5:
		shipChoice = submarine;
		break;
	case 6:
		shipChoice = patrolBoat;
		break;
	}
	cout << "Select coordinates for ship placement!" << endl;
	cout << "Enter Column: ";
	cin >> column;
	colConv = (int)column;
	if (column >= 97) {
		colConv = colConv - 32;
	}
	colConv = colConv - 64;
	cout << "Enter Row: ";
	cin >> row;
	cout << endl;
	if (colConv >= BOARD_DIMENSIONS + 1 || row >= BOARD_DIMENSIONS+1 || colConv <= 0 || row <= 0) {
		cout << "Those orders are not valid, the ship will leave the battlefield!" << endl;
		return false; 
	}
	//board[row][colConv] = SHIP;
	//complete placement of the ship
	if (!completeShip(choiceOrientation, shipChoice, row, colConv)) {
		return false; 
	}
	return true; 
}
bool Player::completeShip(int choice, ship shipChoice, int row, int colConv) {
	//stacks to store coordinates so if the placement is invalid for any part of the ship; 
	//the board is not changed and the user must place the ship again in a different spot
	stack<int> placementCoordsRow;
	stack<int> placementCoordsCol; 
	int placementCounter = 0;
	placementCoordsRow.push(row); //bow coord
	placementCoordsCol.push(colConv); //bow coord
	switch (choice) {
	//up
	case 1:
		placementCounter++;
		for (int i = 0; i < shipChoice.size-1; i++) {
			if (board[row - placementCounter][colConv] == SHIP) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false; 
			}
			if (board[row - placementCounter][colConv] == OUT_OF_BOUNDS) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false; 
			}
			placementCoordsRow.push(row-placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		//place the ship on the board
		for (int i = 0; i < shipChoice.size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()] = SHIP; 
			placementCoordsRow.pop(); 
			placementCoordsCol.pop(); 
		}
		break;
	//left
	case 2:
		placementCounter++;
		for (int i = 0; i < shipChoice.size-1; i++) {
			if (board[row][colConv - placementCounter] == SHIP) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row][colConv - placementCounter] == OUT_OF_BOUNDS) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv - placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice.size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()] = SHIP;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
	//right
	case 3:
		placementCounter++;
		for (int i = 0; i < shipChoice.size-1; i++) {
			if (board[row][colConv + placementCounter] == SHIP) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row][colConv + placementCounter] == OUT_OF_BOUNDS) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv + placementCounter);
			board[row][colConv + placementCounter] = SHIP;
			placementCounter++;
		}
		for (int i = 0; i < shipChoice.size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()] = SHIP;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
	//down
	case 4:
		placementCounter++;
		for (int i = 0; i < shipChoice.size-1; i++) {
			if (board[row + placementCounter][colConv] == SHIP) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (board[row + placementCounter][colConv] == OUT_OF_BOUNDS) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row + placementCounter);
			placementCoordsCol.push(colConv);
			board[row + placementCounter][colConv] = SHIP;
			placementCounter++;
		}
		for (int i = 0; i < shipChoice.size; i++) {
			board[placementCoordsRow.top()][placementCoordsCol.top()] = SHIP;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
	}
	return true; 
}
void Player::placeShipRandom() {


}