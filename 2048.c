#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#define BOARD_SIZE 4
#define debug(M, ...) \
	do { fprintf(stderr, "[DEBUG] " M , ##__VA_ARGS__); fprintf(stderr, "\n"); } while(0)

int board[BOARD_SIZE][BOARD_SIZE];

void print_board();
bool is_full(int *empty);
void drop_random();
void move_up();
void move_down();
void move_right();
void move_left();
void do_resize(int dummy);

int main(int argc, char *argv[]) {
	initscr();
	curs_set(0);
	srand(time(NULL));
	memset(board, -1, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
	bool valid;
	
	while (!is_full(NULL)) {
		drop_random();
		print_board();

		valid = false;
		while (!valid) {
			char c = getchar();
			switch(c) {
				case 27:
					if (getchar() == 91) 
						switch (getchar()) {
							case 65: 
								move_up();
								valid = true;
								break;
							case 66: 
								move_down();
								valid = true;
								break;
							case 67: 
								move_right();
								valid = true;
								break;
							case 68:
								move_left();
								valid = true;
								break;
						}
					break;
				case 'a': 
					move_left();
					valid = true;
					break;
				case 'd': 
					move_right();
					valid = true;
					break;
				case 'w': 
					move_up();
					valid = true;
					break;
				case 's':
					move_down();
					valid = true;
					break;
				default:
					break;
			}
		}
	}

	endwin();

	return 0;
}

void move_up() {
	for (int j = 0; j < BOARD_SIZE; j++) {
		for (int i = 0; i < BOARD_SIZE; i++) { // all to the top
			if (board[i][j] == -1) continue;
			int k = i;
			while (k-1 >= 0 && board[k-1][j] == -1) {
				board[k-1][j] = board[k][j];
				board[k][j] = -1;
				k--;
			}
		}
		for (int i = 0; i < BOARD_SIZE-1; i++) { // combine one
			if (board[i][j] == -1) continue;
			if (board[i][j] == board[i+1][j]) {
				board[i][j] <<= 1;
				board[i+1][j] = -1;
			}
		}
		for (int i = 0; i < BOARD_SIZE; i++) { // all to the top
			if (board[i][j] == -1) continue;
			int k = i;
			while (k-1 >= 0 && board[k-1][j] == -1) {
				board[k-1][j] = board[k][j];
				board[k][j] = -1;
				k--;
			}
		}
	}
}

void move_left() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) { // all to the left
			if (board[i][j] == -1) continue;
			int k = j;
			while (k-1 >= 0 && board[i][k-1] == -1) {
				board[i][k-1] = board[i][k];
				board[i][k] = -1;
				k--;
			}
		}
		for (int j = 0; j < BOARD_SIZE-1; j++) { // combine one
			if (board[i][j] == -1) continue;
			if (board[i][j] == board[i][j+1]) {
				board[i][j] <<= 1;
				board[i][j+1] = -1;
			}
		}
		for (int j = 0; j < BOARD_SIZE; j++) { // all to the left
			if (board[i][j] == -1) continue;
			int k = j;
			while (k-1 >= 0 && board[i][k-1] == -1) {
				board[i][k-1] = board[i][k];
				board[i][k] = -1;
				k--;
			}
		}
	}
}

void move_down() {
	for (int j = 0; j < BOARD_SIZE; j++) {
		for (int i = BOARD_SIZE-1; i >= 0; i--) { // all to the bottom
			if (board[i][j] == -1) continue;
			int k = i;
			while (k+1 < BOARD_SIZE && board[k+1][j] == -1) {
				board[k+1][j] = board[k][j];
				board[k][j] = -1;
				k++;
			}
		}
		for (int i = BOARD_SIZE-1; i > 0; i--) { // combine one
			if (board[i][j] == -1) continue;
			if (board[i][j] == board[i-1][j]) {
				board[i][j] <<= 1;
				board[i-1][j] = -1;
			}
		}
		for (int i = BOARD_SIZE-1; i >= 0; i--) { // all to the bottom
			if (board[i][j] == -1) continue;
			int k = i;
			while (k+1 < BOARD_SIZE && board[k+1][j] == -1) {
				board[k+1][j] = board[k][j];
				board[k][j] = -1;
				k++;
			}
		}
	}
}

void move_right() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = BOARD_SIZE-1; j >= 0; j--) { // all to the right
			if (board[i][j] == -1) continue;
			int k = j;
			while (k+1 < BOARD_SIZE && board[i][k+1] == -1) {
				board[i][k+1] = board[i][k];
				board[i][k] = -1;
				k++;
			}
		}
		for (int j = BOARD_SIZE-1; j > 0; j--) { // combine one
			if (board[i][j] == -1) continue;
			if (board[i][j] == board[i][j-1]) {
				board[i][j] <<= 1;
				board[i][j-1] = -1;
			}
		}
		for (int j = BOARD_SIZE-1; j >= 0; j--) { // all to the right
			if (board[i][j] == -1) continue;
			int k = j;
			while (k+1 < BOARD_SIZE && board[i][k+1] == -1) {
				board[i][k+1] = board[i][k];
				board[i][k] = -1;
				k++;
			}
		}
	}
}

void drop_random() {
	int empty;
	is_full(&empty);
	int r = rand() % empty;
	int i = 0, j = 0;
	for (; (i < BOARD_SIZE) && (r > 0); i++) {
		for (j = 0; (j < BOARD_SIZE) && (r > 0); j++) {
			if (board[i][j] == -1) {
				r--;
				if (r == 0) {
					int x = (rand() % 2 == 0) ? 1 : 2;
					board[i][j] = x;
				}
			}
		}
	}
}

bool is_full(int *empty) {
	bool ret = true;
	if (empty != NULL) *empty = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == -1) {
				if (empty == NULL) return false;
				ret = false;
				(*empty)++;
			}
		}
	}
	return ret;
}

void do_resize(int dummy)
{
	clear();
	int row, col;
	getmaxyx(stdscr, row, col);
	char *mesg = "asddd";
	mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
	refresh();
}

void print_board() {
	clear();
	int row, col;
	getmaxyx(stdscr, row, col);
	char *mesg = "asddd";
	mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != -1) {
				//printfw(" %2d ", board[i][j]);
			} else {
				//printf(" [] ");
			}
		}
		//printf("\n");
	}
	
	refresh();
}
