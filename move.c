#include "main.h"
int get_piece(struct Board board, Vector2 pos);




struct Board board_move_piece(struct Board board, int piece_id) {
	struct Piece P = board.pieces[piece_id];

	struct Beam beam[8];

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

	if (P.name == 'p') {
		int dir = (P.color) ? -1:1;

		beam[0].length = (P.first) ? 2:1;

		// Find en brik til siderne

		Vector2 vecr = { P.pos.x+1, P.pos.y + dir };
		Vector2 vecl = { P.pos.x-1, P.pos.y + dir };
		int pri = get_piece(board, vecr);
		int pli = get_piece(board, vecl);

		printf("\n%i\n", pri);
		printf("\n%i\n", pli);

		if (pri != -1) {
			printf("%c\n", board.pieces[pri].name);
		}

		if (pli != -1) {
			printf("%c\n", board.pieces[pli].name);
		}
	}


	board.pieces[piece_id] = P;
	return board;
}


int get_piece(struct Board board, Vector2 pos) {
	for (int i = 0; i < 64; ++i) {
		Vector2 p = board.pieces[i].pos;

		if (p.x == pos.x && p.y == pos.y) {
			return i;
		}
	}

	return -1;
}