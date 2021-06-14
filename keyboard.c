#include "main.h"


char combo[2] = "00";
const int timer_max = 101; // Skal være ullige!
int timer = timer_max;

char whitelist[] = "abcdefgh12345678";

int choosen_piece = -1;


Board keypress(Board board) {

	char cp = toupper( GetKeyPressed() );

	if (cp >= 'A' && cp <= 'H') {
		board = remove_selections(board);
		int pid = find_piece(board, cp, 2);

		board.pieces[pid].selected = !board.pieces[pid].selected;
	}


	if (IsKeyPressed('M')) {
		int pid = find_piece(board, 'G', 2);
		Notation new_pos = { 'G', 3 };
		move_piece(board, pid, new_pos);
	}


	if (IsKeyPressed('X')) {
		board.reverse_board = !board.reverse_board;
	}
	
	return board;
}
