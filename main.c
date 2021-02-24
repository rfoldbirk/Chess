#include "main.h"


int main(void)
{
	const int screenWidth = 512;
	const int screenHeight = 512;

	const int size = 64;
		
	InitWindow(screenWidth, screenHeight, "Chess");
	SetTargetFPS(60);

	SetWindowPosition((1920-512)/2 - 100, (1080-512)/2);


	// Opsætning af bræt
	const int amount = 65;
	struct Piece* pieces[amount];
	
	char notation[amount];
	strcpy(notation, "rnbqkbnr-pppppppp--8888--PPPPPPPP-RNBQKBNR");


	int AMOUNT = board_init(pieces, notation);

	printf("Successfully loaded: (%d) pieces!\n", AMOUNT);

	while (!WindowShouldClose())
	{

		if (IsMouseButtonPressed(0)) {
			Vector2 mpos = GetMousePosition();

			printf("%f %f\n", mpos.x, mpos.y);

			for (int i = 0; i < AMOUNT; ++i) {
				if (mpos.x >= pieces[i]->pos.x*size && mpos.x <= pieces[i]->pos.x*size+size) {
					if (mpos.y >= pieces[i]->pos.y*size && mpos.y <= pieces[i]->pos.y*size+size) {
						printf("%s\n", pieces[i]->name);
					}
				}
			}
		}


		BeginDrawing();
		ClearBackground(RAYWHITE);


		board_draw(size);


		// Pieces
		for (int i = 0; i < AMOUNT; ++i) {
			if (pieces[i] == NULL) continue;
			if (pieces[i]->texture != NULL) {
				// Tegner kun hvis texture findes
				DrawTexture((*pieces[i]->texture), pieces[i]->pos.x*size, pieces[i]->pos.y*size, WHITE);
			}
		}


		EndDrawing();
	}


	CloseWindow();
	return 0;
}
