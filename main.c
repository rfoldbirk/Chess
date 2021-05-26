#include "main.h"


int main(void)
{
	const int screenWidth = 512;
	const int screenHeight = 512;

	const int size = 64;
		
	InitWindow(screenWidth, screenHeight, "Chess");
	SetTargetFPS(60);

	SetWindowPosition((1440/2-512/2) - 20, 900/2-512/2);



	// ---------------------------- Opsætning af bræt ---------------------------- //


	// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	static char fen[64] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	struct Board board = board_init(fen);

	Vector2 pos = { 3, 5 };
	board = board_add_piece(board, 'q', pos);


	while (!WindowShouldClose())
	{
		// Update
		board = board_check_click(board);


		// ----------------------- Draw ----------------------- //
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// ---------- Tegner brættet og alle stykker ---------- //
		board_draw(board);

		EndDrawing();
	}


	CloseWindow();
	return 0;
}
