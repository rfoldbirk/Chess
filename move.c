#include "main.h"



Board move_piece(Board board, int piece_id) {
	Vector2 dpos = board.selected_pos; // Desired position
	Piece P = board.pieces[piece_id];

	Beam beam[8];

	for (int i = 0; i < 8; ++i) {
		Vector2 vec = { 0, 0 };

		switch (i) {
			case 0: // nord
				vec.y = -1;
				break;
			case 1: // nord øst
				vec.x = 1;
				vec.y = -1;
				break;
			case 2: // øst
				vec.x = 1;
				break;
			case 3: // syd øst
				vec.x = 1;
				vec.y = 1;
				break;
			case 4: // syd
				vec.y = 1;
				break;
			case 5: // syd vest
				vec.x = -1;
				vec.y = 1;
				break;
			case 6: // vest
				vec.x = -1;
				break;
			case 7: // nord vest
				vec.x = -1;
				vec.y = -1;
				break;
		}

		beam[i].dir = vec;
		beam[i].length = 0;
	}


	// Hvis det er en bonde!
	if (P.name == 'p') {
		int dir = (P.color) ? -1:1;

		beam[0].length = (P.first_move) ? 2:1;

		// Find en brik til siderne

		Vector2 pos = calc_piece_pos(board, P.notation);

		Vector2 vecr = { pos.x+1, pos.y + dir };
		Vector2 vecl = { pos.x-1, pos.y + dir };
		int pri = find_piece(board, 'B', 2);
		int pli = find_piece(board, 'B', 2);

	
	}


	board.pieces[piece_id] = P;
	return board;
}