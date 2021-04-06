#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "raylib.h"
#include "main.h"

struct Highlight {
	Vector2 pos;
	Color color;
	bool activated;
};

struct Piece {
    char name[2];
    int value; // Hvis værdien er 0, så skal den ikke tegnes
    bool color;
	bool alive;

    Texture2D* texture;
	Vector2 pos;
	Vector2 selected_position; // En alternativ position som udelukkende bliver brugt når man rykker på brikken.
};

struct Board {
	struct Piece* pieces[65];
	struct Piece* selected_piece;

	Vector2 selected;
	struct Highlight highlights[64];

	int amount_of_pieces;
	int size;
	
	char notation[100];
	char history[200];
};

struct Smart_Texture {
    char name[2];
    Texture2D* texture;
};


struct Board* board_init();
int board_load_pieces(struct Board* board);
void board_draw(struct Board* board);
void board_check_click(struct Board* board);
void board_move_piece(struct Board* board, Vector2 from, Vector2 to);
