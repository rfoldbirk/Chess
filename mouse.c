#include "main.h"

bool already_selected = false;

Board check_click(Board board) {
	if (IsMouseButtonDown(0)) {

		// Tjekker efter om spilleren trykker på en brik
		Vector2 mpos = GetMousePosition();
		board.selected_pos = mpos;
		int xpos = mpos.x / 64;
		int ypos = mpos.y / 64;

		// Hvis en brik allerede bliver rykket rundt på, skal vi ikke vælge en ny.
		if (!board.selected_deb) return board;

		// Hvis en brik allerede er valgt, skal man kunne trykke på stedet den skal rykkes hen til
		// for (int i = 0; i < 64; ++i) {
		// 	Piece P = board.pieces[i];
		// 	if (!P.alive) continue;
		// 	if (!P.selected) continue;

		// 	Notation target_pos = convert_pos_to_notation(board, mpos);

		// 	if (target_pos.x != P.notation.x && target_pos.y != P.notation.y) {
		// 		board = move_piece(board, i, target_pos);
		// 		return board;
		// 	}
		// }



		for (int i = 0; i < 64; ++i) {
			Piece P = board.pieces[i];

			Vector2 vpos = calc_piece_pos(board, P.notation);

			if (!P.alive) continue;
			if (vpos.x != xpos || vpos.y != ypos) continue;


			already_selected = P.selected;
			board = remove_selections(board); // Fjerner brikken som var valgt før, hvis den altså findes :)
			
			// Selekter brik
			board.selected_deb = false;
			P.selected = true;
			board.should_calc = true;

			board.selected_piece_id = i;

			board.pieces[i] = P;

			// Sætter alle træk i boarded til nul!
			for (int i = 0; i < 200; ++i)
				board.allowed_moves[i].x = '0';
		}
	}
	else if (!IsMouseButtonDown(0)) {
		if (!board.selected_deb) {
			int pid = board.selected_piece_id;


			board.selected_deb = true;

			// Find et nyt sted til vores brik
			Notation old_pos = board.pieces[ pid ].notation;
			Notation target_pos = convert_pos_to_notation(board, board.selected_pos);
			board = move_piece(board, board.selected_piece_id, target_pos);

			// Nulstiller selektions variablerne, så den snapper til den position den skal være på.
			if (old_pos.x == target_pos.x && old_pos.y == target_pos.y)
				board.pieces[ pid ].selected = !already_selected;
			else
				board.pieces[ pid ].selected = false;

		}
	}

	return board;
}