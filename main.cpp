#include <iostream>
using namespace std;

const char PLAYER1 = 'X';
const char PLAYER2 = 'O';
const char BLANK = '-';
const int SIZE = 7;

void displayBoard(char board[][SIZE]) {
	for (int j = 0; j < SIZE; j++) {
		for (int k = 0; k < SIZE; k++)
			cout << board[j][k];
		cout << endl;
	}
	cout << endl;
}

bool gameOver(char board[][SIZE], const char player, int row, int col) {
	//horizonal win
	int inARow = 0;
	for (int i = 0; i < SIZE; i++) {
		if (board[row][i] == player) {
			inARow++;
			if (inARow > 3)
				return true;
		}
		else
			inARow = 0;
	}
	
	//vertical win
	inARow = 0;
	for (int j = col; j < SIZE; j++) {
		if (board[j][col] == player)
			inARow++;
		else
			inARow = 0;
	}
	if (inARow > 3)
		return true;

	//right-handed diagonal win
	inARow = 0;
	int k = row;
	int m = col;
	while (k < SIZE && m >= 0) {
		if (board[k][m] == player)
			inARow++;
		else
			break;
		k++; m--;
	}
	if (inARow > 3)
		return true;
	k = row-1;
	m = col+1;
	while (k >= 0 && m < SIZE) {
		if (board[k][m] == player)
			inARow++;
		else
			break;
		k--; m++;
	}
	if (inARow > 3)
		return true;

	//left-handed diagonal win
	inARow = 0;
	k = row;
	m = col;
	while (k < SIZE && m < SIZE) {
		if (board[k][m] == player)
			inARow++;
		else
			break;
		k++; m++;
	}
	if (inARow > 3)
		return true;
	k = row-1;
	m = col-1;
	while (k >= 0 && m >= 0) {
		if (board[k][m] == player)
			inARow++;
		else
			break;
		k--; m--;
	}
	if (inARow > 3)
		return true;
	return false;
}

bool placePiece(char board[][SIZE], int& col, const int& turn) {
	for (int n = SIZE-1; n >= 0; n--) {
		if (board[n][col] == BLANK) {
			board[n][col] = (turn % 2 == 0) ? PLAYER1 : PLAYER2;
			if (gameOver(board, board[n][col], n, col)) {
				if (turn % 2 == 0)
					cout << "Player 1 won!";
				else 
					cout << "Player 2 won!";
				cout << endl;
				displayBoard(board);
				system("PAUSE");
				exit(0);
			}
			return true;
		}
	}
	return false;
}

int main() {
	char board[SIZE][SIZE];

	//initialize empty board
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			board[i][j] = BLANK;
		}
	}

	int col;
	for (int moves = 0; moves < SIZE * SIZE; moves++) {
		displayBoard(board);

		cout << "Player" << (moves%2)+1 << ", enter a column to place piece: ";
		cin >> col;

		while (!placePiece(board, col, moves)) {
			cout << "Column is full, enter a different column: ";
			cin >> col;
		}
	}

	cout << endl;
	cout << "It's a draw!";
	displayBoard(board);
}
