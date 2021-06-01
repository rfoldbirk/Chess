#include "main.h"


char combo[2] = "00";
const int timer_max = 101; // Skal være ullige!
int timer = timer_max;

char whitelist[] = "abcdefgh12345678";

int choosen_piece = -1;


Board keypress(Board board) {


	if (IsKeyPressed('G')) {
		int pid = find_piece(board, 'A', 2);

		board.pieces[pid].selected = !board.pieces[pid].selected;

		printf("%s\n", "Wuhu!");
	}


	if (IsKeyPressed('X')) {
		board.reverse_board = !board.reverse_board;
	}
	
	return board;
}


Board remove_selection(Board board, int pid) {
	
	return board;
}