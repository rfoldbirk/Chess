#include "main.h"


int board_init(struct Piece* pieces[], char notation[64]) {
	
	int* index = malloc(sizeof(int));
	(*index) = 0;

	int* posx = malloc(sizeof(int));
	(*posx) = 0;

	// char* captured = malloc(sizeof(char)*12);
	// strcpy(captured, "");

	struct Smart_Texture* captured[12];
	int* captured_amount = malloc(sizeof(int));
	(*captured_amount) = 0;


	for (int i = 0; i < strlen(notation); ++i)
	{	
		if (notation[i] == '-') continue;

		char* n = malloc(sizeof(char));
		n[0] = notation[i];

		// Allokerer hukommelse til brikken
		pieces[(*index)] = malloc(sizeof(struct Piece));
		strcpy(pieces[(*index)]->name, "??"); // Default navn

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
			pieces[(*index)]->pos.x = (*posx)-(*mx)*8;
			pieces[(*index)]->pos.y = (*mx);
			(*posx) ++;

			// Kilde
			strcpy(src, "");
			strcat(src, "pieces/");
			strcat(src, name);
			strcat(src, ".png");
			
			strcpy(pieces[(*index)]->name, name);

			// printf("\nPiece: (%d): %s - %s\n", (*index), name, src);
			

			// Image & Texture

			// Prøver at finde et passende texture i hukommelsen
			pieces[(*index)]->texture = NULL;

			for (int c = 0; c < (*captured_amount); ++c)
			{
				if (strstr(captured[c]->name, name)) {
					pieces[(*index)]->texture = captured[c]->texture;
				}
			}

			
			if (pieces[(*index)]->texture == NULL) {
				// Allokering
				Image* img = malloc(sizeof(Image));
				Texture2D* texture = malloc(sizeof(Texture2D));

				// Lav resourcer
				(*img) = LoadImage(src);
				(*texture) = LoadTextureFromImage((*img));

				// Tildeling
				pieces[(*index)]->texture = texture;

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



void board_draw(int size) {
	bool* switch_color = malloc(sizeof(bool));
	(*switch_color) = 0;
	
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; ++y) {
			Color tc = ((*switch_color)) ? BROWN:BEIGE;
			// (*switch_color) = !(*switch_color);
			switch_color[0] = !switch_color[0];
			DrawRectangle(x*size, y*size, size, size, tc);
		}
		(*switch_color) = !(*switch_color);
	}

	free(switch_color);
}
