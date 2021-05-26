#include "main.h"



struct Board board_init(char fen[64]) {
	struct Board board;

	board.selected_deb = true;
	Vector2 pos = { 0, 0 };

	// Lav nogle stykker
	int amount = sizeof(board.pieces) / sizeof(board.pieces[0]);
	for (int i = 0; i < amount; ++i) {
		board.pieces[i].in_use = false;
		board.pieces[i].alive = false;
		board.pieces[i].selected = false;

		board.pieces[i].first = true;
		board.pieces[i].just_moved = false;

		if (fen[i] == '/') {
			pos.x = 0;
			pos.y += 1;
			continue;
		}
		else {
			int num = fen[i] - '0';
			if (num > 0 && num <= 9) {
				pos.x += num;
				continue;
			}
		}

		if (fen[i] < 'A') continue;

		board = board_add_piece(board, fen[i], pos);

		pos.x += 1;
	}

	return board;
}


struct Board board_add_piece(struct Board board, char piece, Vector2 pos) {

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
	board.pieces[index].pos = pos;

	// ------------------- Navn & Farve ------------------ //
	board.pieces[index].alive = true;
	board.pieces[index].in_use = true;
	board.pieces[index].name = tolower(piece);
	board.pieces[index].color = (piece < 'a') ? 1:0;


	// --------------------- Texture --------------------- //


	// Tjek om texture allerede findes...
	int already_exists_id = -1;

	for (int i = 0; i < 64; ++i) {
		struct Piece _piece = board.pieces[i];

		if (i == index) continue;
		if (!board.pieces[i].in_use) break;

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
		src[8] = piece;
		
		Image img = LoadImage(src);
		Texture2D texture = LoadTextureFromImage(img);

		board.pieces[index].texture = texture;
	}


	return board;
}


void board_draw(struct Board board) {
	board_draw_board(64);

	int index_render_last = -1;

	for (int i = 0; i < 64; ++i) {
		if (!board.pieces[i].alive) continue;

		// Hvis brikken er selekteret skal den vises til sidst
		if (board.pieces[i].selected) {
			index_render_last = i;
			continue;
		}

		board_draw_piece(board, board.pieces[i], 64);
	}

	if (index_render_last != -1) {
		board_draw_piece(board, board.pieces[index_render_last], 64);
	}
}



void board_draw_piece(struct Board board, struct Piece piece, int size) {
	Vector2 pos = { piece.pos.x * size, piece.pos.y * size };

	// Hvis brikken er valgt skal positionen være lidt anderledes
	if (piece.selected) {
		pos.x = board.selected_pos.x - size/2;
		pos.y = board.selected_pos.y - size/2;
	}

	DrawTextureV(piece.texture, pos, WHITE);
}


void board_draw_board(int size) {
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





struct Board board_check_click(struct Board board) {
	if (IsMouseButtonDown(0)) {
		// Tjekker efter om spilleren trykker på en brik
		Vector2 mpos = GetMousePosition();
		board.selected_pos = mpos;
		int xpos = mpos.x / 64;
		int ypos = mpos.y / 64;

		// Hvis en brik allerede bliver rykket rundt på, skal vi ikke vælge en ny.
		if (!board.selected_deb) return board;

		for (int i = 0; i < 64; ++i) {
			struct Piece P = board.pieces[i];

			if (!P.alive) continue;

			if (P.pos.x == xpos && P.pos.y == ypos) {
				board.selected_deb = false;
				P.selected = true;

				board.selected_piece_id = i;
			}


			board.pieces[i] = P;
		}
	}
	else if (!IsMouseButtonDown(0)) {
		if (!board.selected_deb) {
			board.selected_deb = true;
			// Find et nyt sted til vores brik

			board = board_move_piece(board, board.selected_piece_id);

			board.pieces[board.selected_piece_id].selected = false;
		}
	}

	return board;
}



int board_load_pieces(struct Board* board) {
	return 1;
}




