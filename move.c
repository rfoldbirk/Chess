#include "main.h"


Vector2 n_dir = { 0, 1 };
Vector2 nw_dir = { -1, 1 };
Vector2 ne_dir = { 1, 1 };
Vector2 s_dir = { 0, -1 };
Vector2 sw_dir = { -1, -1 };
Vector2 se_dir = { 1, -1 };
Vector2 w_dir = { 1, 0 };
Vector2 e_dir = { -1, 0 };



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

		// Når den finder en fjendlig brik.
		if (board.pieces[pid].color != piece.color && pid != -1) {
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


Notation convert_pos_to_notation(Board board, Vector2 pos) {

	char x = (board.reverse_board) ? 'H':'A';
	int y;

	int add_to_x = floor( board.selected_pos.x / board.size );
	int add_to_y = floor( board.selected_pos.y / board.size );

	x = (board.reverse_board) ? x-add_to_x : x+add_to_x;
	y = (board.reverse_board) ? add_to_y+1 : 8-add_to_y;

	Notation note = { x, y };

	return note;
}

Board remove_just_moved(Board board) {
	for (int i = 0; i < 64; ++i) {
		if (!board.pieces[i].alive) continue;

		board.pieces[i].just_enpassant = false;
	}

	return board;
}


Board move_piece(Board board, int piece_id, Notation target_pos) {
	// Konverter position til notation
	// Tjek om notation er i allowed_moves
	// Fjern potentiel modstander brik.

	Piece piece = board.pieces[piece_id];

	bool valid_target = false;

	for (int i = 0; i < 200; ++i) {
		Notation a_move = board.allowed_moves[i];
		if (a_move.x == '0') continue;

		if (a_move.x == target_pos.x && a_move.y == target_pos.y) {
			valid_target = true;
			break;
		}
	}

	if (valid_target) {

		int pid = find_piece(board, target_pos.x, target_pos.y);
		if (pid != -1) {
			if (board.pieces[pid].color != board.pieces[piece_id].color) {
				board.pieces[pid].alive = false;
			}
		}

		// Fjerner alle just_moved
		board = remove_just_moved(board);
		board.turn = !board.turn;

		piece.notation = target_pos;
		piece.first_move = false;
		board = remove_selections(board);
	}

	board.pieces[piece_id] = piece;
	return board;
}


Board add_possible_move(Board board, Notation move) {
	for (int i = 0; i < 200; ++i) {
		if (board.allowed_moves[i].x != '0') continue;

		board.allowed_moves[i] = move;
		break;
	}

	return board;
}


Board calc_moves(Board board, Piece piece) {

	if (piece.color == board.turn) return board;

	if (!board.should_calc) return board;
	board.should_calc = false;

	printf("%c\n", piece.notation.x);

	// Sætter alle træk i boarded til nul!
	for (int i = 0; i < 200; ++i)
		board.allowed_moves[i].x = '0';

	// 1. Lav nogle beams
	// 2. Oversæt beams til træk.


	// Laver otte beams
	Beam beams[8];
	for (int i = 0; i < 8; ++i)
		beams[i].length = 0;



	if (piece.name == 'p') {
		beams[0] = check_direction(board, piece, n_dir, (piece.first_move ? 2:1));
		beams[1] = check_direction(board, piece, nw_dir, 1);
		beams[2] = check_direction(board, piece, ne_dir, 1);

		if (beams[0].last_is_enemy) beams[0].length = 0;
		if (!beams[1].last_is_enemy) beams[1].length = 0;
		if (!beams[2].last_is_enemy) beams[2].length = 0;
	}
	else if (piece.name == 'b') {
		beams[0] = check_direction(board, piece, nw_dir, -1);
		beams[1] = check_direction(board, piece, sw_dir, -1);
		beams[2] = check_direction(board, piece, ne_dir, -1);
		beams[3] = check_direction(board, piece, se_dir, -1);
	}
	else if (piece.name == 'r') {
		beams[0] = check_direction(board, piece, n_dir, -1);
		beams[1] = check_direction(board, piece, s_dir, -1);
		beams[2] = check_direction(board, piece, w_dir, -1);
		beams[3] = check_direction(board, piece, e_dir, -1);
	}
	else if (piece.name == 'q' || piece.name == 'k') {
		int length = (piece.name == 'q') ? -1:1;

		beams[0] = check_direction(board, piece, n_dir, length);
		beams[1] = check_direction(board, piece, s_dir, length);
		beams[2] = check_direction(board, piece, w_dir, length);
		beams[3] = check_direction(board, piece, e_dir, length);
		beams[4] = check_direction(board, piece, nw_dir, length);
		beams[5] = check_direction(board, piece, sw_dir, length);
		beams[6] = check_direction(board, piece, ne_dir, length);
		beams[7] = check_direction(board, piece, se_dir, length);
	}






	// Konvertere beams til træk
	for (int i = 0; i < 8; ++i) {

		Beam cb = beams[i]; // En kopi af den aktuelle beam
		Notation on = piece.notation; // Original notation


		// Tegner alle markeringer i en beam.
		for (int o = 0; o < cb.length; ++o) {
			int reverse = (piece.color) ? 1:-1;

			on.x += cb.dir.x;
			on.y += cb.dir.y * reverse;

			board = add_possible_move(board, on);
		}
	}


	return board;
}