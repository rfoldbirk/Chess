#include "main.h"



Notation increment(Notation npos, char fen_char) {


	switch (fen_char) {
		case '/':
			npos.x = 'A';
			npos.y -= 1;
			break;

		case '8':
			npos.x += fen_char;
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


Board board_init(char fen[64], bool reverse_board) {
	Board board; // Laver et bræt.

	// Board booleans som er vigtige!
	board.reverse_board = reverse_board;
	board.selected_deb = true;


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

		board = add_piece(board, fen[i], pos, original);

		pos.x += 1;
	}

	return board;
}


Board add_piece(Board board, char piece, Vector2 pos, Notation npos) {

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




void draw(Board board) {
	draw_board(64);

	int index_render_last = -1;


	for (int i = 0; i < 64; ++i) {

		if (!board.pieces[i].alive) continue;

		// Hvis brikken er selekteret skal den vises til sidst
		if (board.pieces[i].selected) {
			index_render_last = i;
			continue;
		}

		draw_piece(board, board.pieces[i], 64);
	}


	// Tegner det øverste lag til sidst.
	if (index_render_last != -1) {
		draw_piece(board, board.pieces[index_render_last], 64);
	}
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


void draw_piece(Board board, Piece piece, int size) {
	Vector2 vpos = calc_piece_pos(board, piece.notation);
	Vector2 pos = { vpos.x*size, vpos.y*size };


	if (piece.selected) {
		Color col = { 255, 100, 255, 50 };
		Vector2 vsize = { 64, 64 };
		DrawRectangleV(pos, vsize, col);
	}

	// Hvis brikken er valgt skal positionen være lidt anderledes
	if (piece.selected) {
		pos.x = board.selected_pos.x - size/2;
		pos.y = board.selected_pos.y - size/2;
	}

	// Tegner brik
	DrawTextureV(piece.texture, pos, WHITE);
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
		if (piece.notation.x != x || piece.notation.y == y) continue;

		return i;
	}

	return -1;
}



