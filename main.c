#include "main.h"


int main(void)
{
	const int screenWidth = 512;
	const int screenHeight = 512;

	const int size = 64;
		
	InitWindow(screenWidth, screenHeight, "Chess");
	SetTargetFPS(60);

	SetWindowPosition((1920/2-512/2) - 20, 900/2-512/2);



	// ---------------------------- Opsætning af bræt ---------------------------- //


	// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
	static char fen[64] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	Board board = board_init(fen, false);

	Vector2 pos = { 3, 5 };
	Notation npos = { 'E', 4 };
	// board = add_piece(board, 'q', pos, npos);


	while (!WindowShouldClose())
	{
		// Update
		board = check_click(board);

		board = keypress(board);

		// ----------------------- Draw ----------------------- //
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// ---------- Tegner brættet og alle stykker ---------- //
		draw(board);

		EndDrawing();
	}


	CloseWindow();
	return 0;
}
