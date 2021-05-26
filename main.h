#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "raylib.h"
#include "main.h"


struct Piece {
    char name;

    bool in_use;
    bool color;
	bool alive;
	bool selected;

	// Move meta
    bool first;
    bool just_moved;

    Texture2D texture;
	
	Vector2 pos;
};


struct Board {
	struct Piece pieces[64];
	int amount;

	// Til selektering af brikker
	Vector2 selected_pos;
	int selected_piece_id;

	bool selected_deb;
};


struct Beam {
	Vector2 dir;
	int length;
};


struct Board board_init(char fen[64]);
struct Board board_add_piece(struct Board board, char piece, Vector2 pos);
// int board_load_pieces(struct Board* board);
void board_draw(struct Board board);
void board_draw_board(int size);
void board_draw_piece(struct Board board, struct Piece piece, int size);

struct Board board_check_click(struct Board board);
struct Board board_move_piece(struct Board board, int piece_id);
