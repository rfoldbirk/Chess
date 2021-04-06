#include "main.h"


struct Board* board_init() {
	struct Board* nboard = malloc(sizeof(struct Board));

	strcpy(nboard -> notation, "rnbqkbnr-pppppppp-8888-PPPPPPPP-RNBQKBNR");

	nboard -> amount_of_pieces = board_load_pieces(nboard);

	nboard -> size = 64*2;

	nboard -> selected_piece = malloc(sizeof(struct Piece));
	nboard -> selected_piece -> pos.x = -1;
	nboard -> selected_piece -> pos.y = -1;
	nboard -> selected.x = -1;
	nboard -> selected.y = -1;



	nboard -> highlights;
	for (int i = 0; i < 64; i++) {
		nboard -> highlights[i].pos.x = -1;
		nboard -> highlights[i].pos.y = -1;
		
		Color blue = { 0, 121, 241, 100 };
		nboard -> highlights[i].color = blue;
		nboard -> highlights[i].activated = false;
	}

	strcpy(nboard -> history, "");

	return nboard;
}



void board_draw(struct Board* board) {
	// Mønsteret på brættet
	bool switch_color = false;
		
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; ++y) {
			Color tile_color = (switch_color) ? BROWN:BEIGE;	
			switch_color = !switch_color;
			DrawRectangle(x*board -> size, y*board -> size, board -> size, board -> size, tile_color);

			for (int i = 0; i < 64; i++) {
				if (board -> highlights[i].pos.x == x && board -> highlights[i].pos.y == y) {
					if (board -> highlights[i].activated) continue;
					DrawRectangle(x*board -> size, y*board -> size, board -> size, board -> size, board -> highlights[i].color);
				}
			}

		}
		switch_color = !switch_color;
	}



	// Brikker
	for (int i = 0; i < board -> amount_of_pieces; ++i) {
		if (!board -> pieces[i] -> alive) continue;
		if (board -> pieces[i] -> texture != NULL) {
			// Tegner kun hvis texture findes
			Vector2 correct_pos = { board -> pieces[i] -> pos.x * board -> size, board -> pieces[i] -> pos.y * board -> size };
			correct_pos = (board -> pieces[i] -> selected_position.x != -1) ? board -> pieces[i] -> selected_position:correct_pos;

			DrawTextureV((*board -> pieces[i] -> texture), correct_pos, WHITE);
		}
	}
}



void board_move_piece(struct Board* board, Vector2 from, Vector2 to) {
	
	// Find brik
	// Ryk brik

	bool found[2] = { false, false };
	struct Piece* select;
	struct Piece* target;


	for (int i = 0; i < board -> amount_of_pieces; i++) {
		if (!board -> pieces[i] -> alive) continue;
		if (board -> pieces[i] -> pos.x == from.x && board -> pieces[i] -> pos.y == from.y) {
			select = malloc(sizeof(struct Piece));
			select = board -> pieces[i];
			found[0] = true;
		}
		else if (board -> pieces[i] -> pos.x == to.x && board -> pieces[i] -> pos.y == to.y) {
			target = malloc(sizeof(struct Piece));
			target = board -> pieces[i];
			found[1] = true;
		}
	}

	if (found[0] && found[1]) {
		// Hvis man prøver at trække brikken over på en anden brik.
		printf("colors: %d, %d\n", select -> color, target -> color);

		if (select -> color == target -> color) {
			return;
		}
	}

	// Tjek for lovlige træk
	Vector2 possible_moves[50];

	
	

	// Tjek for skak
	if (found[0]) select -> pos = to;
	if (found[1]) target -> alive = false;
}




void board_check_click(struct Board* board) {
	Vector2 from = { 0, 6 };
	Vector2 to = { 0, 5 };

	if (IsKeyPressed('A')) board_move_piece(board, from, to);

	Vector2 reset_pos = { -1, -1 };
	Vector2 mpos = GetMousePosition();
	int xpos = mpos.x/board -> size;
	int ypos = mpos.y/board -> size;

	if (IsMouseButtonPressed(1)) {
		int index = -1;
		for (int i = 0; i < 64; i++) {
			if (board -> highlights[i].pos.x != xpos && board -> highlights[i].pos.y != ypos) {
				index = i;
				break;
			}
		}

		if (index != -1) {
			Vector2 np; // = { xpos, ypos };
			np.x = xpos;
			np.y = ypos;

			board -> highlights[index].pos = np;
			board -> highlights[index].activated;
		}
	}
	else if (IsMouseButtonDown(0)) {
		// Fjerner markerede felter
		for (int i = 0; i < 64; i++)
			board -> highlights[i].activated = false;

		// Tjekker efter om spilleren trykker på en brik
		for (int i = 0; i < board -> amount_of_pieces; ++i) {
			if (!board -> pieces[i] -> alive) continue;
			if (board -> selected_piece != NULL) continue;

			if (board -> pieces[i] -> pos.x == xpos && board -> pieces[i] -> pos.y == ypos) {
				board -> selected_piece = board -> pieces[i];
				
				printf("SELECTED: %s\n", board -> pieces[i] -> name);
			}
		}
	} else {
		// Affyres når musen slippes
		if (board -> selected_piece != NULL) {
			Vector2 mposv = { xpos, ypos };
			board_move_piece(board, board -> selected_piece -> pos, mposv);

			board -> selected_piece -> selected_position = reset_pos;
			board -> selected_piece = NULL;
		}
	}

	// Opdaterer brikken så den positioneres ud fra musen, mens man trækker rundt på den.
	if (board -> selected_piece != NULL) {
		Vector2 cmpos = { mpos.x-board -> size/2, mpos.y-board -> size/2 };
		board -> selected_piece -> selected_position = cmpos;
	}
}



int board_load_pieces(struct Board* board) {
	
	int* index = malloc(sizeof(int));
	(*index) = 0;

	int* posx = malloc(sizeof(int));
	(*posx) = 0;

	// char* captured = malloc(sizeof(char)*12);
	// strcpy(captured, "");

	struct Smart_Texture* captured[12];
	int* captured_amount = malloc(sizeof(int));
	(*captured_amount) = 0;


	for (int i = 0; i < strlen(board -> notation); ++i)
	{	
		if (board -> notation[i] == '-') continue;

		char* n = malloc(sizeof(char));
		n[0] = board -> notation[i];

		// Allokerer hukommelse til brikken
		board -> pieces[(*index)] = malloc(sizeof(struct Piece));
		strcpy(board -> pieces[(*index)] -> name, "??"); // Default navn
		board -> pieces[(*index)] -> alive = false;

		if (n[0] >= 'A') {
			char* name = malloc(sizeof(char)*3);
			char* src = malloc(sizeof(char)*14);
			strcpy(name, "");

			char* small_letter = malloc(sizeof(char));
			(*small_letter) = tolower(n[0]);

			// Navn
			strcpy(name, (n[0] < 'a') ? "w":"b");
			strncat(name, &small_letter[0], 1);

			// Farve
			board -> pieces[(*index)] -> color = (n[0] < 'a') ? true:false;
			board -> pieces[(*index)] -> alive = true;
	
			// Position
			int* mx = malloc(sizeof(int));
			(*mx) = (*posx)/8;
			board -> pieces[(*index)] -> pos.x = (*posx)-(*mx)*8;
			board -> pieces[(*index)] -> pos.y = (*mx);
			(*posx) ++;

			// Selected position
			board -> pieces[(*index)] -> selected_position.x = -1;
			board -> pieces[(*index)] -> selected_position.y = -1;

			// Kilde
			strcpy(src, "");
			strcat(src, "pieces/");
			strcat(src, name);
			strcat(src, ".png");

			if (strstr(name, "wu")) {
				printf("\nPiece: (%d): %s - %s\n", (*index), name, src);
			}
			
			strcpy(board -> pieces[(*index)] -> name, name);

			// printf("\nPiece: (%d): %s - %s\n", (*index), name, src);
			

			// Image & Texture

			// Prøver at finde et passende texture i hukommelsen
			board -> pieces[(*index)] -> texture = NULL;

			for (int c = 0; c < (*captured_amount); ++c)
			{
				if (strstr(captured[c] -> name, name)) {
					board -> pieces[(*index)] -> texture = captured[c] -> texture;
				}
			}

			
			if (board -> pieces[(*index)] -> texture == NULL) {
				// Allokering
				Image* img = malloc(sizeof(Image));
				Texture2D* texture = malloc(sizeof(Texture2D));

				// Lav resourcer
				(*img) = LoadImage(src);
				(*texture) = LoadTextureFromImage((*img));

				// Tildeling
				board -> pieces[(*index)] -> texture = texture;

				free(img); // Hvis vi fjerner texture forsvinder de

				// Gem og genbrug textures

				captured[(*captured_amount)] = malloc(sizeof(struct Smart_Texture));

				strcpy(captured[(*captured_amount)] -> name, name);
				captured[(*captured_amount)] -> texture = texture;
				
				(*captured_amount) ++;
			}




			// Ting der skal frigøres
			free(small_letter);
			free(name);
			free(src);
		} else {
			(*posx) += (*n)-48;
			(*index) --;
		}

		(*index) ++;
		free(n);
	}
	
	free(posx);
	free(captured_amount);
	
	int cpy_index = (*index);
	free(index);
	return cpy_index;
}




