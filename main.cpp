#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cctype>
#include "player.h"
#include "boardProperties.h"
/*
checklist:
	 main functionality: 
		player ship placement ^
		input validation for above  ^ 
		elimination of all infinite loops ^ 
		enemy ship placement ^
		hit and miss calculation ^
		enemy COM intelligence regarding where to place shot ^ 
	cosmetic: 
		player and enemy text color ^
		turn screen white on surrender X 
		animate text so it prints gradually instead of all at once X 
	New things I've learned: 
		changing the color of console text
		making true "random" using the system clock as a seed 
	Most difficult part: 
		programming the COM's "intelligence" regarding shot placement 

*/
using namespace std; 
//functions 
void printMenu(int menu);
//this function needs to be able to access both boards 
bool fireShot(Player *playerOne, Player *playerTwo); 
bool fireShotCom(Player* playerOne, Player* playerTwo);
void calculateNextShot(Player* playerOne, Player* playerTwo, int* x, int* y);
bool gameOver(Player* playerOne, Player* playerTwo); 
//global variables 
int GAMESTATE = 0; 
//global vars used in com shot calculation 
//these ints log the coordinate of the first hit the COM got on a player's ship 
int firstHitX = 0;
int firstHitY = 0;
bool reverseDir = false; 
bool hitLastShot = false; 

int main() {
	//HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	/*for (int i = 1; i < 257; i++) {
		SetConsoleTextAttribute(Console, i);
		cout << "I: "<<i<<"In Rainbows" << endl; 
	}*/

	//main() variables 
	Player user;
	Player computer; 
	computer.isCom = true; 
	GAMESTATE = 0;
	/*
	GAMESTATE 0 = initial state
	GAMESTATE 1 = ship selection state 
	GAMESTATE 2 = ship orientation selection state 
	GAMESTATE 3 = user's turn state 
	GAMESTATE 4 = COM's turn state 
	GAMESTATE 5 = battle over state 
	*/
	int input = 0;
	int shipChoice = 0;
	int battleChoice = 0; 
	int orientationChoice = 0;
	int whoseTurn = 0; 
	bool allPlaced = false;
	bool shotSuccessfulPlayer = false; 
	bool shotSuccessfulCom = false; 	
	computer.playerName = "COM";

	//main() start 
	SetConsoleTextAttribute(Console, PLAYER_TEXT);//set text to blue (player text) 
	cout << "Playing as Admiral ..." << endl;
	std::cin >> user.playerName;
	SetConsoleTextAttribute(Console, HEADER_TEXT);//set text to white (headers) 
	cout << "------------BATTLESHIP------------" << endl;
	SetConsoleTextAttribute(Console, PLAYER_TEXT);
	cout << "Admiral " << user.playerName << ", we have detected an enemy fleet approaching!" << endl;
	cout << "Send orders to the fleet to take attack formation." << endl << endl;
	while (GAMESTATE == 0) {
		printMenu(GAMESTATE);
		std::cin >> input; 
		//** stops infinite loop when letter is entered
		if (!std::cin) {
			//cout << "Those orders are not recognized. Try again!" << endl; 
			cin.clear(); 
			cin.ignore(10000, '\n'); 
		}
		//place ship
		if (input == 1) {
			while (allPlaced == false || shipChoice == 7) {
				//ship selection state (for placement)
				GAMESTATE = 1;
				user.printBoard();
				printMenu(GAMESTATE);
				std::cin >> shipChoice;
				if (!std::cin) {
					cout << "Those orders are not recognized. Try again!" << endl;
					cin.clear();
					cin.ignore(10000, '\n');
					GAMESTATE = 0; 
					break; 
				}
				//if user chose "back" 
				if (shipChoice == 7) {
					GAMESTATE = 0;
					break; 
				}
				//input validation
				if (shipChoice <= 0 || shipChoice > 7) {
					GAMESTATE = 0; 
					cout << "Those orders are not recognized. Try again!" << endl; 
					break; 
				}
				//orientation state (for placement continued) 
				GAMESTATE = 2;
				printMenu(GAMESTATE);
				std::cin >> orientationChoice;
				user.placeShip(shipChoice, orientationChoice);

				if (user.carrier.placed == true && user.battleship.placed == true && user.cruiser.placed == true
					&& user.destroyer.placed == true && user.submarine.placed == true && user.patrolBoat.placed == true) {
					cout << "All ships have been placed! We are ready for battle." << endl; 
					allPlaced = true; 
					GAMESTATE = 0; 

				}
			}
			//user.placeShipCom(); 
		}
		//edit ship placement 
		if (input == 2) {
			cout << "2"; 
		}
		//to battle 
		if (input == 3) {
			GAMESTATE = 3; 
			computer.placeShipCom(); 
			computer.printBoard(); //temp 
			
			while (GAMESTATE != 5) {         //6 = battle over
				printMenu(GAMESTATE);
				cin >> battleChoice; 
				if (!std::cin) {
					cin.clear();
					cin.ignore(10000, '\n');
					GAMESTATE = 3;
				}
				if (battleChoice == 1) {
					shotSuccessfulPlayer = false;
					while (shotSuccessfulPlayer == false) {
						if (fireShot(&user, &computer)) {
							shotSuccessfulPlayer = true;
						}
					}
					GAMESTATE = 4; //COM's turn
					shotSuccessfulCom = false; 
					while (shotSuccessfulCom == false) {
						if (fireShotCom(&user, &computer)) {
							shotSuccessfulCom = true; 
						}
					}
					GAMESTATE = 3; 
				}
				else if (battleChoice == 2) {
					cout << "Surrendered" << endl; 
					break; 
				}
				else {
					cout << "Those orders are not recognized. Try again!" << endl;
				}
				if (gameOver(&user, &computer)) {
					GAMESTATE = 5; 
					break; 
				}
			}
		}
		//exit
		if (input == 4) {
			break; 
		}
		//input validation
		if (input <= 0 || input > 4) {
			cout << "Those orders are not recognized. Try again!" << endl; 
		}
	}
}

void printMenu(int menu) {
	switch (menu){
		case 0: 
			SetConsoleTextAttribute(Console, HEADER_TEXT);
			cout << "------------FLEET ORDERS------------" << endl;
			SetConsoleTextAttribute(Console, PLAYER_TEXT);
			cout << "1. Place Ship" << endl
				<< "2. Edit Ship Placement" << endl
				<< "3. To Battle! " << endl
				<< "4. Surrender" << endl << endl; 
			break; 
		case 1: 
			SetConsoleTextAttribute(Console, HEADER_TEXT);
			cout << "------------SHIP SELECTION------------" << endl;
			SetConsoleTextAttribute(Console, PLAYER_TEXT);
			cout << "1. Carrier" << endl
				<< "2. Battleship" << endl
				<< "3. Cruiser" << endl
				<< "4. Destoryer" << endl
				<< "5. Submarine" << endl
				<< "6. Patrol Boat" << endl
				<< "7. <--- Back" << endl << endl; 
			break; 
		case 2: 
			SetConsoleTextAttribute(Console, HEADER_TEXT);
			cout << "------------SHIP ORIENTATION------------" << endl;
			SetConsoleTextAttribute(Console, PLAYER_TEXT);
			cout << "1. Up" << endl
				<< "2. Left" << endl
				<< "3. Right" << endl
				<< "4. Down "<< endl; 
			break; 
		case 3: 
			SetConsoleTextAttribute(Console, HEADER_TEXT);
			cout << "------------BATTLE OPTIONS------------" << endl;
			SetConsoleTextAttribute(Console, PLAYER_TEXT);
			cout << "1. Fire Shot " << endl
				<< "2. Surrender " << endl<<endl;
			break; 
	}
}

bool fireShot(Player *playerOne, Player *playerTwo) {
	char column = ' ';
	int colConv = 0; //variable for conversion from user inputted char to corresponding column's int. Ex: D = 4 
	int row = 0;
	cout << "What coordinates are we firing this shot at?" << endl;
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
		cout << "Those orders are not valid, the shot is outside of the battlefield!" << endl;
		return false;
	}
	if (playerTwo->board[row][colConv].isHit == true || playerTwo->board[row][colConv].isMiss == true) {
		cout << "Admiral, you already fired at that sector. Choose different coordinates!" << endl; 
		return false; 
	}
	if (playerTwo->board[row][colConv].isShip == true) {
		playerTwo->board[row][colConv].isHit = true; 
		playerTwo->board[row][colConv].shipClass->health--; 
		cout << playerOne->playerName << " fires a shot at " << column << row << endl;
		cout << playerOne->playerName << " scores a hit on " << playerTwo->playerName << "'s " <<
			playerTwo->board[row][colConv].shipClass->shipType << "! " << endl;
		if (playerTwo->board[row][colConv].shipClass->health == 0) {
			cout << playerOne->playerName << " sunk " << playerTwo->playerName << "'s " <<
				playerTwo->board[row][colConv].shipClass->shipType << "! " << endl;
		}
	}
	else {
		playerTwo->board[row][colConv].isMiss = true;
		cout << playerOne->playerName << " fires a shot at " << column << row << endl;
		cout << playerOne->playerName << "'s shot misses! " << endl;
	}
	playerTwo->printBoard();
	return true;
}

bool fireShotCom(Player* playerOne, Player* playerTwo) {
	SetConsoleTextAttribute(Console, ENEMY_TEXT);
	//random generation 
	int row = 0;
	int col = 0;
	bool found = false; 
	char colConv; 
	//scan board for "unfound" hit square (as in, a ship has been discovered by COM but it is not sunk yet)
	//COM should mimic an opponent player "looking" for the rest of the ship
	for (int i = 0; i < BOARD_DIMENSIONS + 2; i++) {
		for (int j = 0; j < BOARD_DIMENSIONS + 2; j++) {
			if (playerOne->board[i][j].isHit == true && playerOne->board[i][j].shipClass->health != 0 && playerOne->board[i][j].isFound == false){
				row = i; 
				col = j; 
				playerOne->board[i][j].isFound = true;
				found = true; 
				break; 
			}
		}
		if (found == true) {
			break; 
		}
	}
	//if no "discovered" ships are present, then shoot randomly 
	if (found == false) {
		srand((int)time(0));
		col = rand() % (BOARD_DIMENSIONS) + 1;
		row = rand() % (BOARD_DIMENSIONS) + 1;
	}
	//else compute where next shot should go. 
	else {
		calculateNextShot(playerOne, playerTwo, &row, &col); 
	}
	//if square randomly chosen has already been fired at previously, select new coordinates 
	if (playerOne->board[row][col].isHit == true || playerOne->board[row][col].isMiss == true ) {
		return false;
	}
	int tempRow = 0; 
	int tempCol = 0; 
	colConv = col + 64;
	if (playerOne->board[row][col].isShip == true) {
		playerOne->board[row][col].isHit = true;
		playerOne->board[row][col].shipClass->health--;
		cout << playerTwo->playerName << " fires a shot at " << colConv << row << endl;
		cout << playerTwo->playerName << " scores a hit on Admiral " << playerOne->playerName << "'s " <<
			playerOne->board[row][col].shipClass->shipType << "! " << endl; 
		if (playerOne->board[row][col].shipClass->health == 0) {
			cout << playerTwo->playerName << " sunk " << playerOne->playerName << "'s " <<
				playerOne->board[row][col].shipClass->shipType << "! " << endl;
			//reset shot calculation variables after sinkage 
			firstHitX = 0; 
			firstHitY = 0; 
			reverseDir = false; 
		}
	}
	else {
		playerOne->board[row][col].isMiss = true;
		cout << playerTwo->playerName << " fires a shot at " << colConv<<row<<endl;
		cout << playerTwo->playerName << "'s shot misses! " << endl;
	}
	playerOne->printBoard();
	return true;
}

void calculateNextShot(Player* playerOne, Player* playerTwo, int* x, int* y) {
	int rChoice = 0; 
	bool valid = false; 
	//if an adjacent square is also marked HIT, then place shot in opposite direction
	//AND the hit marker belongs to the same ship as the current hit marker 
	//right
	if (playerOne->board[*x][*y + 1].isHit == true && playerOne->board[*x][*y + 1].shipClass->shipType == playerOne->board[*x][*y].shipClass->shipType) {
		//niche edge case where shot opposite of an adjacent HIT marker happens to be a wall. 
		//in this event, shot should be placed two squares from current hit square in same direction as adjacent HIT
		//square 
		if (playerOne->board[*x][*y - 1].isWall == true) {
			*y = *y + 2;
		}
		else {
			*y = *y - 1;
			//if the square it will shoot at next will be a miss, then 
			//rest of ship must be in other direction. As in, "reverse" the pattern/direction 
			//the COM is firing shots in 
			if (playerOne->board[*x][*y - 1].isShip == false) {
				reverseDir = true;
				playerOne->board[firstHitX][firstHitY].isFound = false;
			}
		}
	}
	//bottom
	else if (playerOne->board[*x + 1][*y].isHit == true && playerOne->board[*x + 1][*y].shipClass->shipType == playerOne->board[*x][*y].shipClass->shipType) {
		if (playerOne->board[*x - 1][*y].isWall == true) {
			*x = *x + 2;
		}
		else {
			*x = *x - 1;
			if (playerOne->board[*x - 1][*y].isShip == false) {
				reverseDir = true;
				playerOne->board[firstHitX][firstHitY].isFound = false;
			}
		}
	}
	//left
	else if (playerOne->board[*x][*y - 1].isHit == true && playerOne->board[*x][*y - 1].shipClass->shipType == playerOne->board[*x][*y].shipClass->shipType) {
		if (playerOne->board[*x][*y + 1].isWall == true) {
			*y = *y - 2;
		}
		else {
			*y = *y + 1;
			if (playerOne->board[*x][*y + 1].isShip == false) {
				reverseDir = true;
				playerOne->board[firstHitX][firstHitY].isFound = false;
			}
		}
	}
	//upper
	else if (playerOne->board[*x - 1][*y].isHit == true && playerOne->board[*x - 1][*y].shipClass->shipType == playerOne->board[*x][*y].shipClass->shipType) {
		if (playerOne->board[*x + 1][*y].isWall == true) {
			*x = *x - 2;
		}
		else {
			*x = *x + 1;
			if (playerOne->board[*x + 1][*y].isShip == false) {
				reverseDir = true;
				playerOne->board[firstHitX][firstHitY].isFound = false;
			}
		}
	}
	//else current square has no neighboring hit squares (is an 'X' by itself) 
	else {
		//cooridnates of the first peg the COM hit. Helps when "reversing" shot pattern is needed 
		firstHitX = *x; 
		firstHitY = *y; 
		//place next shot at a random adjacent square 
		while (valid == false) { 
			srand((int)time(0));
			rChoice = rand() % (4) + 1;
			switch (rChoice) {
			case 1:
				if ((playerOne->board[*x][*y + 1].isHit == false && playerOne->board[*x][*y + 1].isMiss == false && 
					playerOne->board[*x][*y + 1].isWall == false)) {
					//if the random square chosen by COM is not a ship, then the current hit marker needs to "be rediscovered"
					// as in; the program needs to stay at the current hit square and simply pick a different adjacent square 
					//for the next shot

					//or if the chosen square is a ship, but a different ship than the current square, then set the current square's found status to false
					//so the COM can return to that ship after it finishes this new ship
					if (playerOne->board[*x][*y + 1].isShip == false || playerOne->board[*x][*y + 1].shipClass->shipType != playerOne->board[*x][*y].shipClass->shipType) {
						playerOne->board[*x][*y].isFound = false; 
					}
					*y = *y + 1;
					valid = true; 
				}
				break;
			case 2:
				if ((playerOne->board[*x - 1][*y].isHit == false && playerOne->board[*x - 1][*y].isMiss == false && 
					playerOne->board[*x - 1][*y].isWall == false)) {
					if (playerOne->board[*x - 1][*y].isShip == false || playerOne->board[*x - 1][*y].shipClass->shipType != playerOne->board[*x][*y].shipClass->shipType) {
						playerOne->board[*x][*y].isFound = false;
					}
					*x = *x - 1;
					valid = true;
				}
				break;
			case 3:
				if ((playerOne->board[*x][*y - 1].isHit == false && playerOne->board[*x][*y - 1].isMiss == false && 
					playerOne->board[*x][*y - 1].isWall == false)) {
					if (playerOne->board[*x][*y - 1].isShip == false || playerOne->board[*x][*y - 1].shipClass->shipType != playerOne->board[*x][*y].shipClass->shipType) {
						playerOne->board[*x][*y].isFound = false;
					}
					*y = *y - 1;
					valid = true;
				}
				break;
			case 4:
				if ((playerOne->board[*x + 1][*y].isHit == false && playerOne->board[*x + 1][*y].isMiss == false && 
					playerOne->board[*x + 1][*y].isWall == false)) {
					if (playerOne->board[*x + 1][*y].isShip == false || playerOne->board[*x + 1][*y].shipClass->shipType != playerOne->board[*x][*y].shipClass->shipType) {
						playerOne->board[*x][*y].isFound = false;
					}
					*x = *x + 1;
					valid = true;
				}
				break;
			}
		}
	}
} 

bool gameOver(Player* playerOne, Player* playerTwo) {
	if (playerOne->carrier.health == 0 && playerOne->battleship.health == 0 && playerOne->cruiser.health == 0 &&
		playerOne->destroyer.health == 0 && playerOne->submarine.health == 0 && playerOne->patrolBoat.health == 0) {
		cout << "All of Admiral " << playerOne->playerName << "'s ships have been sunk! " << endl; 
		return true; 
	}
	else if (playerTwo->carrier.health == 0 && playerTwo->battleship.health == 0 && playerTwo->cruiser.health == 0 &&
		playerTwo->destroyer.health == 0 && playerTwo->submarine.health == 0 && playerTwo->patrolBoat.health == 0) {
		cout << "All of " << playerTwo->playerName << "'s ships have been sunk! " << endl;
		return true;
	}
	else {
		return false; 
	}
}