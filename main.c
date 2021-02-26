#include "main.h"


int main(void)
{
	const int screenWidth = 512;
	const int screenHeight = 512;

	const int size = 64;
		
	InitWindow(screenWidth, screenHeight, "Chess");
	SetTargetFPS(60);

	SetWindowPosition((1920-512) - 20, 35);


	// Opsætning af bræt

	struct Board* board = board_init();


	// const int amount = 65;
	// struct Piece* pieces[amount];
	
	// char notation[amount];
	// strcpy(notation, "rnbqkbnr-pppppppp--8888--PPPPPPPP-RNBQKBNR");


	// int AMOUNT = board_init(pieces, notation);

	printf("Successfully loaded: (%d) pieces!\n", board->amount_of_pieces);

	while (!WindowShouldClose())
	{
		// Update
		board_check_click(board);

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);


		board_draw(board);

		EndDrawing();
	}


	CloseWindow();
	return 0;
}
