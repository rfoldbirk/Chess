#include "main.h"


Board check_click(Board board) {
	if (IsMouseButtonDown(0)) {
		// Tjekker efter om spilleren trykker på en brik
		Vector2 mpos = GetMousePosition();
		board.selected_pos = mpos;
		int xpos = mpos.x / 64;
		int ypos = mpos.y / 64;

		// Hvis en brik allerede bliver rykket rundt på, skal vi ikke vælge en ny.
		if (!board.selected_deb) return board;

		for (int i = 0; i < 64; ++i) {
			Piece P = board.pieces[i];

			Vector2 vpos = calc_piece_pos(board, P.notation);

			if (!P.alive) continue;
			if (vpos.x != xpos || vpos.y != ypos) continue;

			board.selected_deb = false;
			P.selected = true;

			board.selected_piece_id = i;

			board.pieces[i] = P;
		}
	}
	else if (!IsMouseButtonDown(0)) {
		if (!board.selected_deb) {
			board.selected_deb = true;

			// Find et nyt sted til vores brik
			board = move_piece(board, board.selected_piece_id);

			// Nulstiller selektions variablerne, så den snapper til den position den skal være på.
			board.pieces[board.selected_piece_id].selected = false;
		}
	}

	return board;
}