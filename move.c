#include "main.h"


Beam check_direction(Board board, Piece piece, Vector2 dir, int limit) {
	Beam beam = { dir, 0, false };

	Notation on = piece.notation;



	while (true) {

		int reverse = (piece.color) ? 1:-1;

		on.x += dir.x;
		on.y += dir.y * reverse;

		// Tjekker om brikken er nået grænsen
		if (beam.length >= limit && limit != -1) break;
		if (on.x < 'A' || on.x > 'H') break;
		if (on.y < 1 || on.y > 8) break;


		int pid = find_piece(board, on.x, on.y);

		if (board.pieces[pid].color != piece.color && pid != -1 && limit != 2) {
			beam.length += 1;
			beam.last_is_enemy = true;
			break;
		}
		else {
			// break;
		}

		if (pid != -1) break;

		beam.length += 1;
	}

	return beam;
}


Board move_piece(Board board, int piece_id) {
	return board;
}


Board add_possible_move(Board board, Notation move) {
	for (int i = 0; i < 200; ++i) {
		if (board.allowed_moves[i].x == '0') continue;

		board.allowed_moves[i] = move;
	}

	return board;
}


Board calc_moves(Board board, Piece piece) {

	// 1. Lav nogle beams
	// 2. Oversæt beams til træk.


	// Laver otte beams
	Beam beams[8];
	for (int i = 0; i < 8; ++i)
		beams[i].length = 0;



	if (piece.name == 'p') {
		board.allowed_moves[0].x = 'D';
		board.allowed_moves[0].y = 3;
	}


	return board;
}