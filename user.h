#pragma once
#include <iostream>
#include <stack> 
#include <windows.h>
#include "boardProperties.h"
#include "player.h" 
#ifndef USER_H
#define USER_H

using namespace std;

class User {
public: 
	Player user; 

public: 
	bool placeShip(int choiceShip, int choiceOrientation);
	bool placeBow(int choice, int choiceOrientation); //helper for placeShip
	bool completeShip(int choice, ship* shipChoice, int row, int colConv); //helper for placeShip

};

bool User::placeShip(int choiceShip, int choiceOrientation) {
	placeBow(choiceShip, choiceOrientation);

	//printBoard(); 
	return true;
}
bool User::placeBow(int choice, int choiceOrientation) {
	char column = ' ';
	int colConv = 0; //variable for conversion from user inputted char to corresponding column's int. Ex: D = 4 
	int row = 0;
	//placeholder variable for ship
	ship* shipChoice = NULL;
	switch (choice) {
	case 1:
		shipChoice = &user.carrier;
		break;
	case 2:
		shipChoice = &user.battleship;
		break;
	case 3:
		shipChoice = &user.cruiser;
		break;
	case 4:
		shipChoice = &user.destroyer;
		break;
	case 5:
		shipChoice = &user.submarine;
		break;
	case 6:
		shipChoice = &user.patrolBoat;
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
bool User::completeShip(int choice, ship* shipChoice, int row, int colConv) {
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
			if (user.board[row - placementCounter][colConv].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (user.board[row - placementCounter][colConv].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row - placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		//place the ship on the board
		for (int i = 0; i < shipChoice->size; i++) {
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//left
	case 2:
		for (int i = 0; i < shipChoice->size; i++) {
			if (user.board[row][colConv - placementCounter].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (user.board[row][colConv - placementCounter].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv - placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//right
	case 3:
		for (int i = 0; i < shipChoice->size; i++) {
			if (user.board[row][colConv + placementCounter].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (user.board[row][colConv + placementCounter].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv + placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//down
	case 4:
		for (int i = 0; i < shipChoice->size; i++) {
			if (user.board[row + placementCounter][colConv].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (user.board[row + placementCounter][colConv].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row + placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			user.board[placementCoordsRow.top()][placementCoordsCol.top()].shipClass = shipChoice;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
	}
	//ship now placed and should not be able to be placed again (unless edit option is chosen) 
	shipChoice->placed = true;
	return true;
}
#endif