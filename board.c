#include "main.h"


struct Board* board_init() {
	struct Board* nboard = malloc(sizeof(struct Board));

	strcpy(nboard->notation, "rnbqkbnr-pppppppp-8888-PPPPPPPP-RNBQKBNR");

	nboard->amount_of_pieces = board_load_pieces(nboard);
	nboard->size = 64;
	nboard->selected_piece = NULL;

	return nboard;
}


void board_check_click(struct Board* board) {
	Vector2 reset_pos = { -1, -1 };
	Vector2 mpos = GetMousePosition();
	int xpos = mpos.x/board->size;
	int ypos = mpos.y/board->size;

	if (IsMouseButtonPressed(1)) {
		board->selected.x = xpos;
		board->selected.y = ypos;
	}
	else if (IsMouseButtonDown(0)) {
		board->selected = reset_pos;

		// Tjekker efter om spilleren trykker på en brik
		for (int i = 0; i < board->amount_of_pieces; ++i) {
			if (board->selected_piece != NULL) continue;

			if (board->pieces[i]->pos.x == xpos && board->pieces[i]->pos.y == ypos) {
				board->selected_piece = board->pieces[i];
				
				printf("SELECTED: %s\n", board->pieces[i]->name);
			}
		}
	} else {
		if (board->selected_piece != NULL) {
			
			board->selected_piece->selected_position = reset_pos;
			board->selected_piece = NULL;
		}
	}

	if (board->selected_piece != NULL) {
		Vector2 cmpos = { mpos.x-board->size/2, mpos.y-board->size/2 };
		board->selected_piece->selected_position = cmpos;
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


	for (int i = 0; i < strlen(board->notation); ++i)
	{	
		if (board->notation[i] == '-') continue;

		char* n = malloc(sizeof(char));
		n[0] = board->notation[i];

		// Allokerer hukommelse til brikken
		board->pieces[(*index)] = malloc(sizeof(struct Piece));
		strcpy(board->pieces[(*index)]->name, "??"); // Default navn

		if (n[0] >= 'A') {
			char* name = malloc(sizeof(char)*3);
			char* src = malloc(sizeof(char)*14);
			strcpy(name, "");

			char* small_letter = malloc(sizeof(char));
			(*small_letter) = tolower(n[0]);

			// Navn
			strcpy(name, (n[0] < 'a') ? "w":"b");
			strncat(name, &small_letter[0], 1);
	
			// Position
			int* mx = malloc(sizeof(int));
			(*mx) = (*posx)/8;
			board->pieces[(*index)]->pos.x = (*posx)-(*mx)*8;
			board->pieces[(*index)]->pos.y = (*mx);
			(*posx) ++;

			// Selected position
			board->pieces[(*index)]->selected_position.x = -1;
			board->pieces[(*index)]->selected_position.y = -1;

			// Kilde
			strcpy(src, "");
			strcat(src, "pieces/");
			strcat(src, name);
			strcat(src, ".png");

			if (strstr(name, "wu")) {
				printf("\nPiece: (%d): %s - %s\n", (*index), name, src);
			}
			
			strcpy(board->pieces[(*index)]->name, name);

			// printf("\nPiece: (%d): %s - %s\n", (*index), name, src);
			

			// Image & Texture

			// Prøver at finde et passende texture i hukommelsen
			board->pieces[(*index)]->texture = NULL;

			for (int c = 0; c < (*captured_amount); ++c)
			{
				if (strstr(captured[c]->name, name)) {
					board->pieces[(*index)]->texture = captured[c]->texture;
				}
			}

			
			if (board->pieces[(*index)]->texture == NULL) {
				// Allokering
				Image* img = malloc(sizeof(Image));
				Texture2D* texture = malloc(sizeof(Texture2D));

				// Lav resourcer
				(*img) = LoadImage(src);
				(*texture) = LoadTextureFromImage((*img));

				// Tildeling
				board->pieces[(*index)]->texture = texture;

				free(img); // Hvis vi fjerner texture forsvinder de

				// Gem og genbrug textures

				captured[(*captured_amount)] = malloc(sizeof(struct Smart_Texture));

				strcpy(captured[(*captured_amount)]->name, name);
				captured[(*captured_amount)]->texture = texture;
				
				(*captured_amount) ++;
			}




			// Ting der skal frigøres
			/*
			captured
			index

			name
			src
			small_letter
			*/

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



void board_draw(struct Board* board) {
	// Mønsteret på brættet
	bool* switch_color = malloc(sizeof(bool));
	(*switch_color) = 0;
	
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; ++y) {
			Color tc = ((*switch_color)) ? BROWN:BEIGE;
			tc = (board->selected.x == x && board->selected.y == y) ? DARKBLUE:tc;
			// (*switch_color) = !(*switch_color);
			switch_color[0] = !switch_color[0];
			DrawRectangle(x*board->size, y*board->size, board->size, board->size, tc);
		}
		(*switch_color) = !(*switch_color);
	}
	free(switch_color);



	// Brikker
	for (int i = 0; i < board->amount_of_pieces; ++i) {
		if (board->pieces[i]->texture != NULL) {
			// Tegner kun hvis texture findes
			Vector2 correct_pos = { board->pieces[i]->pos.x * board->size, board->pieces[i]->pos.y * board->size };
			
			correct_pos = (board->pieces[i]->selected_position.x != -1) ? board->pieces[i]->selected_position:correct_pos;

			DrawTextureV((*board->pieces[i]->texture), correct_pos, WHITE);
		}
	}
}