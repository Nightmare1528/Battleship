#pragma once
#include <iostream>
#include <stack> 
#include <windows.h>
#include <cstdlib> 
#include <random> 
#include <ctime> 
#include "boardProperties.h"
#include "player.h" 
#ifndef COMPUTER_H
#define COMPUTER_H

using namespace std;

class Computer {
public:
	Player computer;

public: 
	void placeShipCom();
	bool completeShip(int choice, ship* shipChoice, int row, int colConv); //helper for placeShip
};

void Computer::placeShipCom() {
	bool allPlaced = false; 
	int row = 0; 
	int col = 0; 
	int orientation = 0; 
	//**make true random (based off of system clock) 
	srand((int)time(0));
	ship* shipChoice = &computer.carrier; 
	//helper variable for accessing array 
	int currShip = 0; 

	//array for keeping track of successfully placed ships
	bool placementSuccessful[6] = {};

	while (allPlaced == false) {
		//**generate random x and y coordinates for placement
		col = rand() % (BOARD_DIMENSIONS+1) + 1;
		row = rand() % (BOARD_DIMENSIONS+1) + 1;
	    orientation = rand() % 4 + 1;
		//could be replaced with an if chain checking the placed status of each ship, but it's going to be 
		//an if chain regardless and this makes more sense to me. So it will stay as this 
		if (placementSuccessful[0] == false) {
			shipChoice = &computer.carrier;
		}
		else if (placementSuccessful[1] == false) {
			shipChoice = &computer.battleship;
		}
		else if (placementSuccessful[2] == false) {
			shipChoice = &computer.cruiser;
		}
		else if (placementSuccessful[3] == false) {
			shipChoice = &computer.destroyer;
		}
		else if (placementSuccessful[4] == false) {
			shipChoice = &computer.submarine;
		}
		else if (placementSuccessful[5] == false) {
			shipChoice = &computer.patrolBoat;
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
bool Computer::completeShip(int choice, ship* shipChoice, int row, int colConv) {
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
		for (int i = 0; i < shipChoice->size; i++) {
			if (computer.board[row - placementCounter][colConv].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (computer.board[row - placementCounter][colConv].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row - placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		//place the ship on the board
		for (int i = 0; i < shipChoice->size; i++) {
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true; 
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//left
	case 2:
		for (int i = 0; i < shipChoice->size; i++) {
			if (computer.board[row][colConv - placementCounter].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (computer.board[row][colConv - placementCounter].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv - placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//right
	case 3:
		for (int i = 0; i < shipChoice->size; i++) {
			if (computer.board[row][colConv + placementCounter].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (computer.board[row][colConv + placementCounter].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row);
			placementCoordsCol.push(colConv + placementCounter);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
			placementCoordsRow.pop();
			placementCoordsCol.pop();
		}
		break;
		//down
	case 4:
		for (int i = 0; i < shipChoice->size; i++) {
			if (computer.board[row + placementCounter][colConv].isShip == true) {
				cout << "That ship cannot be placed there Admiral. There is already another ship occupying that space." << endl;
				return false;
			}
			if (computer.board[row + placementCounter][colConv].isWall == true) {
				cout << "Those orders are not valid Admiral, the ship will leave the battlefield!" << endl;
				return false;
			}
			placementCoordsRow.push(row + placementCounter);
			placementCoordsCol.push(colConv);
			placementCounter++;
		}
		for (int i = 0; i < shipChoice->size; i++) {
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isShip = true;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isOcean = false;
			computer.board[placementCoordsRow.top()][placementCoordsCol.top()].isHidden = true;
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