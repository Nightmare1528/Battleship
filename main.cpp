#include <iostream>
#include <iomanip>
#include "player.h"
using namespace std; 

void printMenu(int menu); 

int GAMESTATE = 0; 
int main() {
	Player user;
	cout << "Playing as Admiral ..." << endl; 
	cin >> user.playerName; 
	user.isCOM = false; 
	Player computer; 
	computer.playerName = "COM"; 
	computer.isCOM = true; 

	cout << "------------BATTLESHIP------------" << endl; 
	
	cout << "Admiral " << user.playerName << ", we have detected an enemy fleet approaching!" << endl; 
	cout << "Send orders to the fleet to take attack formation." << endl<<endl;
	GAMESTATE = 0; 
	int input = 0; 
	printMenu(GAMESTATE);
	int shipChoice = 0; 
	int orientationChoice = 0; 
	while (GAMESTATE == 0) {
		cin >> input; 
		//place ship
		if (input == 1) {
			//ship selection state (for placement)
			GAMESTATE = 1; 
			user.printBoard(); 
			printMenu(GAMESTATE);
			cin >> shipChoice; 
			GAMESTATE = 2; 
			printMenu(GAMESTATE); 
			cin >> orientationChoice; 
			user.placeShip(shipChoice, orientationChoice); 

		}
		//edit ship placement 
		if (input == 2) {

		}
		//to battle 
		if (input == 3) {

		}
	}
}

void printMenu(int menu) {
	switch (menu){
		case 0: 
			cout << "------------FLEET ORDERS------------" << endl; 
			cout << "1. Place Ship" << endl
				<< "2. Edit Ship Placement" << endl
				<< "3. To Battle! " << endl<<endl;
			break; 
		case 1: 
			cout << "------------SHIP SELECTION------------" << endl;
			cout << "1. Carrier" << endl
				<< "2. Battleship" << endl
				<< "3. Cruiser" << endl
				<< "4. Destoryer" << endl
				<< "5. Submarine" << endl
				<< "6. Patrol Boat" << endl << endl; 
			break; 
		case 2: 
			cout << "------------SHIP ORIENTATION------------" << endl; 
			cout << "1. Up" << endl
				<< "2. Left" << endl
				<< "3. Right" << endl
				<< "4. Down "<< endl; 
			break; 
		case 3: 
			cout << "------------BATTLE OPTIONS------------" << endl;
			cout << "1. Fire Shot " << endl
				<< "2. Surrender " << endl<<endl;
			break; 
	}
}