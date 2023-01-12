#include <iostream>;
#include <vector>;
#include <string>;
#include <time.h>
#include <regex>;

//Note : No exception handling has been implemented

using namespace std;

// Main functions
vector<vector<string>> createGrid(int size);
void printGrid(vector<vector<string>> grid);
bool validateChoice(vector<vector<string>> grid, string choice);
int checkVictoryOrDraw(vector<vector<string>> grid);
string randomChoice(vector<vector<string>> grid);

// Helper functions for visuals only.
int countDigits(int number);
string printValueWithSpaces(string value, int spaces);
string printDashes(int spaces);

int main() {
	bool playing = true;
	int size = 0;
	int currPlayer = 1;
	int victoryOrDraw;
	int numberOfPlayers = 0;
	string choice = "";
	string error = "";

	while (size < 3 || size > 20) {
		system("cls");
		cout << "Welcome to Tic Tac Toe extreme!!\n\n";
		cout << "To begin with : What size would you like your grid to be?\n";
		// We limit the size to 20 for the visuals when printing. The function should work with any input so long as its contents don't overflow the max int possible.
		cout << "(Min = 3 and Max = 20)\n\nChoose :  "; 
		cin >> size;
	}
	while (numberOfPlayers < 1 || numberOfPlayers > 2) {
		system("cls");
		cout << "Now, are you playing against the computer or with a friend?\n\n";
		cout << "- Against the computer. (1)\n";
		cout << "- With a friend. (2)\n\n";
		cout << "Choose : ";
		cin >> numberOfPlayers;
	}
	vector<vector<string>> grid = createGrid(size);

	system("cls");

	while (playing) {
		cout << "Tic Tac Toe - Extreme\n\n";
		if (numberOfPlayers == 2) cout << "Player 1 (X) - Player 2 (O)\n\n";
		else cout << "Player 1 (X) - Ordinateur (O)\n\n";
		printGrid(grid);
		if (error != "") cout << error;

		// The computer will be taking over player 2 when only one player is selected at the begining.
		if (currPlayer == 1 || (currPlayer == 2 && numberOfPlayers == 2)) {
			cout << "Player " << currPlayer << ", enter a number : ";
			cin >> choice;
			if (validateChoice(grid, choice)) {
				error = "";
				grid[(stoi(choice) - 1) / size][(stoi(choice) - 1) % size] = (currPlayer == 1) ? "X" : "O";;
				currPlayer = (currPlayer == 1) ? 2 : 1;
			}
			else {
				error = "*** Invalid choice, pick again ***\n\n";
			}
		}
		else {
			choice = randomChoice(grid);
			grid[(stoi(choice) - 1) / size][(stoi(choice) - 1) % size] = "O";
			currPlayer = 1;
		}

		victoryOrDraw = checkVictoryOrDraw(grid);
		if (victoryOrDraw == 1 || victoryOrDraw == 2) {
			playing = false;
		}
		system("cls");
	}

	// Final print with results.
	currPlayer = (currPlayer == 1) ? 2 : 1;
	cout << "Tic Tac Toe - Extreme\n\n";
	printGrid(grid);
	if (victoryOrDraw == 2) cout << "==> The match is a draw. Nobody wins.\n\n";
	else {
		if (numberOfPlayers == 1) {
			if (currPlayer == 1) cout << "==> Bravo, you win!\n\n";
			else cout << "==> Better luck next time. The computer wins!\n\n";
		}
		else {
			cout << "==> Player " << currPlayer << " wins!\n\n";
		}
	}
	return 0;
}

/// According to the size chosen, a square matrix will be created and each cell will be assigned a value starting from 1.
vector<vector<string>> createGrid(int size) {
	vector<vector<string>> grid = {};
	int temp = 1;
	for (int i = 0; i < size; i++) {
		vector<string> row = {};
		grid.push_back(row);
		for (int j = 0; j < size; j++) {
			grid.at(i).push_back(to_string(temp));
			temp++;
		}
	}
	return grid;
}

void printGrid(vector<vector<string>> grid) {
	int size = grid.size();
	int printSize = grid.size() * 2 - 1;
	int spaces = countDigits(size * size);// For visuals only. We count how many digits the last cell number has so we can fill the lower cells with spaces.
	for (int i = 0; i < printSize; i++) {
		for (int j = 0; j < printSize; j++) {
			if (i % 2 == 0) {
				if (j % 2 == 0) cout << printValueWithSpaces(grid.at(i / 2).at(j / 2), spaces);
				else cout << "|";
			}
			else {
				if (j % 2 == 0) cout << printDashes(spaces);
				else cout << " ";
			}
			if (j == printSize - 1) cout << "\n";
		}
	}
	cout << "\n";
	return;
}

bool validateChoice(vector<vector<string>> grid, string choice) {
	int size = grid.size();
	// First we validate that the input is not out of bounds.
	if (stoi(choice) < 0 || stoi(choice) > size * size) return false;
	// Then we validate if the cell is not already taken.
	else {
		if (grid[(stoi(choice) - 1) / size][(stoi(choice) - 1) % size] == "X" || grid[(stoi(choice) - 1) / size][(stoi(choice) - 1) % size] == "O") {
			return false;
		}
		else {
			return true;
		}
	}
}

/// return values meaning:
/// 0 = keep playing
/// 1 = victory
/// 2 = draw
int checkVictoryOrDraw(vector<vector<string>> grid) {
	/*
	*	Win logic : Every element is the same in just one row, one column or one diagonal.
	*	Draw logic : At least one 'X' and one 'O' in every row, column and diagonal. No need to keep playing.
	*/

	int size = grid.size();
	int invalidRows = 0, invalidCols = 0, invalidDiag = 0;
	int countX = 0, countO = 0;

	// Rows
	for (int i = 0; i < size; i++) {
		countX = 0;
		countO = 0;
		for (int j = 0; j < size; j++) {
			if (grid[i][j] == "X") countX++;
			if (grid[i][j] == "O") countO++;
		}
		if (countX == size) return 1;
		if (countO == size) return 1;
		if (countX > 0 && countO > 0) invalidRows++;
	}
	// Columns
	for (int j = 0; j < size; j++) {
		countX = 0;
		countO = 0;
		for (int i = 0; i < size; i++) {
			if (grid[i][j] == "X") countX++;
			if (grid[i][j] == "O") countO++;
		}
		if (countX == size) return 1;
		if (countO == size) return 1;
		if (countX > 0 && countO > 0) invalidCols++;
	}
	// Diagonal from top left to to bottom right
	countX = 0;
	countO = 0;
	for (int i = 0; i < size; i++) {
		if (grid[i][i] == "X") countX++;
		if (grid[i][i] == "O") countO++;
	}
	if (countX == size) return 1;
	if (countO == size) return 1;
	if (countX > 0 && countO > 0) invalidDiag++;

	// Diagonal from top right to to bottom left
	countX = 0;
	countO = 0;
	for (int i = 0; i < size; i++) {
		if (grid[i][size - 1 - i] == "X") countX++;
		if (grid[i][size - 1 - i] == "O") countO++;
	}
	if (countX == size) return 1;
	if (countO == size) return 1;
	if (countX > 0 && countO > 0) invalidDiag++;

	if (invalidRows == size && invalidCols == size && invalidDiag == 2) return 2;

	return 0;
}

string randomChoice(vector<vector<string>> grid) {
	/*
	* We go through every element of the grid and store the available cells in a new vector from which we will choose one at random
	*/

	vector<int> validOptions = {};
	int currentCell = 1;
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid.size(); j++) {
			if (grid[i][j] != "X" && grid[i][j] != "O") {
				validOptions.push_back(currentCell);
			}
			currentCell++;
		}
	}
	srand(time(NULL));
	return to_string(validOptions[(rand() % validOptions.size())]);
}

// Helper functions for printing the grid. Only for visuals.

string printValueWithSpaces(string value, int spaces) {
	string newString = "";
	while (newString.length() != spaces - value.length()) {
		newString = newString + " ";
	}
	newString = newString + value;
	return " " + newString + " ";
}

string printDashes(int spaces) {
	string newString = "";
	for (int i = 0; i < spaces; i++) {
		newString = newString + "-";
	}
	return "-" + newString + "-";
}

int countDigits(int number) {
	int count = 0;
	while (number != 0) {
		number = number / 10;
		count++;
	}
	return count;
}
