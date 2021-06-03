#include "main.h"

Vector2 n_dir = { 0, 1 };
Vector2 nw_dir = { -1, 1 };
Vector2 ne_dir = { 1, 1 };
Vector2 s_dir = { 0, -1 };
Vector2 sw_dir = { -1, -1 };
Vector2 se_dir = { 1, -1 };
Vector2 w_dir = { 1, 0 };
Vector2 e_dir = { -1, 0 };




Board board_init(char fen[64], bool reverse_board) {
	Board board; // Laver et bræt.

	// Board booleans som er vigtige!
	board.reverse_board = reverse_board;
	board.selected_deb = true;

	// Sætter alle beams i boarded til nul!
	for (int i = 0; i < 8; ++i)
		board.beam_collection[i].length = 0;

	for (int i = 0; i < 200; ++i)
		board.allowed_moves[i].x = '0';


	// Bruges til at positionere brikkerne.
	Vector2 pos = { 0, 0 };
	Notation npos = { 'A', 8 };

	// Lav nogle stykker
	int amount = sizeof(board.pieces) / sizeof(board.pieces[0]); // For længden af FEN koden.
	for (int i = 0; i < amount; ++i) {

		// Indstiller bools på brikken
		board.pieces[i].has_texture = false;
		board.pieces[i].alive = false;
		board.pieces[i].selected = false;
		board.pieces[i].first_move = true;


		// Sørger for at y bliver øget.
		Notation original = npos;
		npos = increment(npos, fen[i]);
		if (fen[i] < 'A') continue;

		board = add_piece(board, fen[i], original);

		pos.x += 1;
	}

	return board;
}


Board add_piece(Board board, char piece, Notation npos) {

	// ---------------- Find et nyt index ---------------- //
	int index = -1;
	for (int o = 0; o < 64; ++o) {
		if (!board.pieces[o].alive) {
			index = o;
			break;
		}
	}
	if (index == -1 || index > 63) return board;

	// --------------------- Position -------------------- //
	board.pieces[index].notation = npos;

	// ------------------- Navn & Farve ------------------ //
	board.pieces[index].alive = true;
	board.pieces[index].has_texture = true;
	board.pieces[index].name = tolower(piece);
	board.pieces[index].color = (piece < 'a') ? 1:0;


	// --------------------- Texture --------------------- //


	// Tjek om texture allerede findes...
	int already_exists_id = -1;

	for (int i = 0; i < 64; ++i) {
		Piece _piece = board.pieces[i];

		if (i == index) continue;
		if (!board.pieces[i].has_texture) break;

		if (_piece.name == board.pieces[index].name) {
			if (_piece.color == board.pieces[index].color) {
				already_exists_id = i;
			}
		}
	}


	// Valg af texture

	if (already_exists_id != -1) {
		board.pieces[index].texture = board.pieces[already_exists_id].texture;
	}
	else {
		static char src[13] = "pieces/__.png";
		src[7] = (piece < 'a') ? 'w':'b';
		src[8] = tolower(piece);
		
		Image img = LoadImage(src);
		Texture2D texture = LoadTextureFromImage(img);

		board.pieces[index].texture = texture;
	}


	return board;
}




Board draw(Board board) {
	draw_board(64);

	int index_render_last = -1;


	for (int i = 0; i < 64; ++i) {

		if (!board.pieces[i].alive) continue;

		// Hvis brikken er selekteret skal den vises til sidst
		if (board.pieces[i].selected) {
			index_render_last = i;
			continue;
		}

		board = draw_piece(board, board.pieces[i], 64);
	}


	// Tegner det øverste lag til sidst.
	if (index_render_last != -1) {
		board = draw_piece(board, board.pieces[index_render_last], 64);
	}

	return board;
}




Board draw_piece(Board board, Piece piece, int size) {
	Vector2 vpos = calc_piece_pos(board, piece.notation);
	Vector2 pos = { vpos.x*size, vpos.y*size };


	if (piece.selected) {
		Color col = { 255, 100, 255, 50 };
		Vector2 vsize = { 64, 64 };
		DrawRectangleV(pos, vsize, col);
	}

	// Hvis brikken er valgt skal positionen være lidt anderledes
	if (piece.selected) {

		// Tegner alle mulige træk for brikken.
		board = draw_moves(board, piece);


		if (!board.selected_deb) {
			// Sætter brikkens position til musens position.
			pos.x = board.selected_pos.x - size/2;
			pos.y = board.selected_pos.y - size/2;
		}

	}

	// Tegner brik
	DrawTextureV(piece.texture, pos, WHITE);

	return board;
}






Board draw_moves(Board board, Piece piece) {
	int SIZE = 64;

	// Beam beams[8];

	// // Sætter alle beams til nul, så de ikke bliver tegnet hvis de ikke er i brug.
	// for (int i = 0; i < 8; ++i) {
	// 	beams[i].length = 0;
	// 	board.beam_collection[i].length = 0;
	// }



	// if (piece.name == 'p') {
	// 	board.beam_collection[0] = check_direction(board, piece, n_dir, (piece.first_move ? 2:1));
	// 	board.beam_collection[1] = check_direction(board, piece, nw_dir, 1);
	// 	board.beam_collection[2] = check_direction(board, piece, ne_dir, 1);

	// 	if (!board.beam_collection[1].last_is_enemy) board.beam_collection[1].length = 0;
	// 	if (!board.beam_collection[2].last_is_enemy) board.beam_collection[2].length = 0;




	// }
	// else if (piece.name == 'b') {
	// 	board.beam_collection[0] = check_direction(board, piece, nw_dir, -1);
	// 	board.beam_collection[1] = check_direction(board, piece, sw_dir, -1);
	// 	board.beam_collection[2] = check_direction(board, piece, ne_dir, -1);
	// 	board.beam_collection[3] = check_direction(board, piece, se_dir, -1);
	// }
	// else if (piece.name == 'r') {
	// 	board.beam_collection[0] = check_direction(board, piece, n_dir, -1);
	// 	board.beam_collection[1] = check_direction(board, piece, s_dir, -1);
	// 	board.beam_collection[2] = check_direction(board, piece, w_dir, -1);
	// 	board.beam_collection[3] = check_direction(board, piece, e_dir, -1);
	// }
	// else if (piece.name == 'q' || piece.name == 'k') {
	// 	int length = (piece.name == 'q') ? -1:1;

	// 	board.beam_collection[0] = check_direction(board, piece, n_dir, length);
	// 	board.beam_collection[1] = check_direction(board, piece, s_dir, length);
	// 	board.beam_collection[2] = check_direction(board, piece, w_dir, length);
	// 	board.beam_collection[3] = check_direction(board, piece, e_dir, length);
	// 	board.beam_collection[4] = check_direction(board, piece, nw_dir, length);
	// 	board.beam_collection[5] = check_direction(board, piece, sw_dir, length);
	// 	board.beam_collection[6] = check_direction(board, piece, ne_dir, length);
	// 	board.beam_collection[7] = check_direction(board, piece, se_dir, length);
	// }
	// else if (piece.name == 'n') {

	// }
	// else {
	// 	return board;
	// }




	// Brikkens position

	// Farve
	Color transparent_gray = { 50, 50, 50, 50 };


	// Tegn beam markeringer
	// for (int i = 0; i < 8; ++i) {

	// 	Beam cb = board.beam_collection[i]; // En kopi af den aktuelle beam
	// 	Notation on = piece.notation; // Original notation


	// 	// Tegner alle markeringer i en beam.
	// 	for (int o = 0; o < cb.length; ++o) {
	// 		int reverse = (piece.color) ? 1:-1;

	// 		on.x += board.beam_collection[i].dir.x;
	// 		on.y += board.beam_collection[i].dir.y * reverse;

	// 		Vector2 vpos = calc_piece_pos(board, on);
	// 		Vector2 pos = { vpos.x*SIZE + SIZE/2, vpos.y*SIZE + SIZE/2 };

	// 		if (o == cb.length-1 && cb.last_is_enemy)
	// 			for (int z = 0; z < 5; ++z)
	// 				DrawCircleLines(pos.x, pos.y, SIZE/2 - z, transparent_gray);
	// 		else
	// 			DrawCircle(pos.x, pos.y, SIZE/5, transparent_gray);

	// 	}
	// }

	board = calc_moves(board, piece);


	for (int i = 0; i < 1; ++i) {
		Notation note = board.allowed_moves[i];

		if (note.x == '0') continue;


		Vector2 vpos = calc_piece_pos(board, note);
		Vector2 pos = { vpos.x*SIZE + SIZE/2, vpos.y*SIZE + SIZE/2 };

		DrawCircle(pos.x, pos.y, SIZE/5, transparent_gray);
	}


	return board;
}



void draw_board(int size) {
	// Mønsteret på brættet
	bool switch_color = false;
	
	// Laver et 8x8 bræt
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; ++y) {
			Color tile_color = (switch_color) ? BROWN:BEIGE;	
			switch_color = !switch_color;

			DrawRectangle(x * size, y * size, size, size, tile_color);
		}
		switch_color = !switch_color;
	}
}




// Returnerer id'et på et stykke.
int find_piece(Board board, char x, int y) {
	for (int i = 0; i < 64; ++i) {
		Piece piece = board.pieces[i];

		// Tjekker om den er i live.
		if (!piece.alive) continue;

		// Hvis position IKKE passer!
		if (piece.notation.x != x || piece.notation.y != y) continue;

		return i;
	}

	return -1;
}



Board remove_selections(Board board) {
	for (int i = 0; i < 64; ++i) {
		if (!board.pieces[i].alive) continue;
		if (!board.pieces[i].selected) continue;
		
		board.pieces[i].selected = false;
	}

	return board;
}


// Konvertere en notation til den korrekte vektor position.
Vector2 calc_piece_pos(Board board, Notation npos) {
	Vector2 vpos;

	if (board.reverse_board) {
		vpos.x = 7-(npos.x - 'A');
		vpos.y = npos.y - 1;
	}
	else {
		vpos.x = npos.x - 'A';
		vpos.y = 8 - npos.y;
	}


	return vpos;
}


// Bruges til at parse FEN notationen.
Notation increment(Notation npos, char fen_char) {
	switch (fen_char) {
		case '/':
			npos.x = 'A';
			npos.y -= 1;
			break;

		case '8':
			npos.x += 8;
			break;

		case '1':
			npos.x += 1;
			break;

		default:
			npos.x += 1;

			if (npos.x > 'H') {
				npos.x = 'A';
				// npos.y -= 1;
			}
	}

	return npos;
}