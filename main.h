#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "raylib.h"
#include "main.h"



struct Piece {
    char name[2];
    int value; // Hvis værdien er 0, så skal den ikke tegnes
    bool color;

    Texture2D* texture;
	Vector2 pos;
	Vector2 selected_position;
};

struct Board {
	struct Piece* pieces[65];
	struct Piece* selected_piece;

	Vector2 selected;

	int amount_of_pieces;
	int size;
	
	char notation[100];

};

struct Smart_Texture {
    char name[2];
    Texture2D* texture;
};


struct Board* board_init();
int board_load_pieces(struct Board* board);
void board_draw(struct Board* board);
void board_check_click(struct Board* board);