#include "main.h"


int main(void)
{
	const int screenWidth = 512*2;
	const int screenHeight = 512*2;

	const int size = 64*2;
		
	InitWindow(screenWidth, screenHeight, "Chess");
	SetTargetFPS(60);

	SetWindowPosition((1920-512) - 20, 35);



	// Opsætning af bræt
	struct Board* board = board_init();

	printf("Successfully loaded: (%d) pieces!\n", board->amount_of_pieces);

	while (!WindowShouldClose())
	{
		// Update
		board_check_click(board);

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Tegner brættet og alle stykker
		board_draw(board);

		EndDrawing();
	}


	CloseWindow();
	return 0;
}
