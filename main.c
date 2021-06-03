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
	static char fen[64] = "1nbqkbnr/p1pppppp/8/8/8/8/PBPPPPPP/1N1QKBNR";
	Board board = board_init(fen, false);

	// Tilføjer en brik
	Notation npos = { 'D', 3 };
	board = add_piece(board, 'r', npos);


	// Slår first_move fra på brik G2
	int pid = find_piece(board, 'G', 2);
	board.pieces[pid].first_move = false;


	// Selekter brik
	pid = find_piece(board, 'E', 2);
	board.pieces[pid].selected = !board.pieces[pid].selected;


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
