#include <iostream>
#include <algorithm>
using namespace std;

const char PLAYER1 = 'X';
const char PLAYER2 = 'O';
const char BLANK = '-';
const int SIZE = 8;

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

bool placePiece(char board[][SIZE], int& row, int& col, const int& turn) {
	for (int n = SIZE-1; n >= 0; n--) {
		if (board[n][col] == BLANK) {
			board[n][col] = (turn % 2 == 0) ? PLAYER1 : PLAYER2;
			row = n;
			return true;
		}
	}
	return false;
}

void copyBoard(char board[][SIZE], char temp[][SIZE]) {
	for (int j=0; j < SIZE; j++) {
		for (int k=0; k < SIZE; k++) {
			temp[j][k] = board[j][k];
		}
	}
}

int score(int p, int a) {
	if (a == 4)
		return 50;
	if (p == 4)
		return -50;
	if ((a == 0 && p == 0) || (a > 0 && p > 0))
		return 0;
	else if (a > 0 && p == 0)
		return 1;
	else if (p > 0 && a == 0)
		return -1;
	else
		return 0;
}

int findFour(char temp[][SIZE], int r, int c, int dir) {
	int p1 = 0;
	int ai = 0;
	
	//horizontal
	if (dir == 0) {
		for (int n=0; n < 4; n++) {
			if (temp[r][c+n] == PLAYER1)
				p1++;
			if (temp[r][c+n] == PLAYER2)
				ai++;
		}
		return score(p1, ai);
	}

	//vertical
	if (dir == 1) {
		for (int n=0; n < 4; n++) {
			if (temp[r+n][c] == PLAYER1)
				p1++;
			if (temp[r+n][c] == PLAYER2)
				ai++;
		}
		return score(p1, ai);
	}

	//right diagonal
	if (dir == 2) {
		for (int n=0; n < 4; n++) {
			if (temp[r+n][c-n] == PLAYER1)
				p1++;
			if (temp[r+n][c-n] == PLAYER2)
				ai++;
		}
		return score(p1, ai);
	}

	//left diagonal
	if (dir == 3) {
		for (int n=0; n < 4; n++) {
			if (temp[r+n][c+n] == PLAYER1)
				p1++;
			if (temp[r+n][c+n] == PLAYER2)
				ai++;
		}
		return score(p1, ai);
	}
	return 0;
}

int evalScore(char temp[][SIZE]) {
	int score = 0;

	//horizontal evaluation
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE-3; j++) {
			score += findFour(temp, i, j, 0);
		}
	}

	//vertical evaluation
	for (int i=0; i < SIZE-3; i++) {
		for (int j=0; j < SIZE; j++) {
			score += findFour(temp, i, j, 1);
		}
	}

	//right diagonal evaluation
	for (int i=0; i < SIZE-3; i++) {
		for (int j=SIZE-1; j > 2; j--) {
			score += findFour(temp, i, j, 2);
		}
	}

	//left diagonal evaluation
	for (int i=0; i < SIZE-3; i++) {
		for (int j=0; j < SIZE-3; j++) {
			score += findFour(temp, i, j, 3);
		}
	}

	return score;
}

//AI implemented using minimax algorithm up to 2 levels of lookahead
void playAI(char board[][SIZE]) {
	char t[SIZE][SIZE];
	int scores1[SIZE] = {-100, -100, -100, -100, -100, -100, -100, -100};
	int scores2[SIZE] = {100, 100, 100, 100, 100, 100, 100, 100};
	int row, bestCol;
	for (int j=0; j < SIZE; j++) {
		copyBoard(board, t);
		if (placePiece(t, row, j, 1)) {
			for (int k=0; k < SIZE; k++) {
				if (placePiece(t, row, k, 0)) {
					scores2[k] = evalScore(t);
					scores1[j] = evalScore(t);
				}
			}
		}
		//find minimum score of opponent
		scores1[j] = *(min_element(scores2, scores2+SIZE));
	}
	//find maximum score of AI
	bestCol = max_element(scores1, scores1+SIZE) - scores1;

	//if AI won
	if (placePiece(board, row, bestCol, 1)) {
		if (gameOver(board, board[row][bestCol], row, bestCol)) {
			cout << "AI won!" << endl;
			displayBoard(board);
			system("PAUSE");
			exit(0);
		}
	}
}

void play(char board[][SIZE], int& ans) {
	int row, col;
	//1 player
	if (ans == 1) {
		for (int moves = 0; moves < SIZE * SIZE; moves++) {
			displayBoard(board);

			cout << "Player1, enter a column to place piece: ";
			cin >> col;
			while (col < 0 || col > SIZE-1) {
				cout << "Please enter a valid column number: ";
				cin >> col;
			}
			cout << endl;

			while (!placePiece(board, row, col, 0)) {
				cout << "Column is full, enter a different column: ";
				cin >> col;
			}

			if (gameOver(board, board[row][col], row, col)) {
				cout << "Player 1 won!" << endl;
				displayBoard(board);
				system("PAUSE");
				exit(0);
			}

			playAI(board);
		}
	}
	//2 players
	else {
		for (int moves = 0; moves < SIZE * SIZE; moves++) {
			displayBoard(board);

			cout << "Player" << (moves%2)+1 << ", enter a column to place piece: ";
			cin >> col;
			while (col < 0 || col > SIZE-1) {
				cout << "Please enter a valid column number: ";
				cin >> col;
			}
			cout << endl;

			while (!placePiece(board, row, col, moves)) {
				cout << "Column is full, enter a different column: ";
				cin >> col;
			}

			if (gameOver(board, board[row][col], row, col)) {
				if (moves % 2 == 0)
					cout << "Player 1 won!";
				else 
					cout << "Player 2 won!";
				cout << endl;
				displayBoard(board);
				system("PAUSE");
				exit(0);
			}
		}
	}
}

int main() {
	char board[SIZE][SIZE];

	//initialize empty board
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			board[i][j] = BLANK;
		}
	}

	int ans;
	cout << "1 player or 2 players? (Enter 1 or 2): ";
	cin >> ans;
	while (ans != 1 && ans != 2) {
		cout << "Please enter a valid answer: ";
		cin >> ans;
	}
	cout << endl;

	play(board, ans);

	cout << endl;
	cout << "It's a draw!";
	displayBoard(board);
}
