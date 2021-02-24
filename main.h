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
};

struct Board {
	struct Piece* pieces[65];
	char notation[65];

	struct Piece* selected_piece;
};

struct Smart_Texture {
    char name[2];
    Texture2D* texture;
};


struct Board* board_m();
int board_init(struct Piece* pieces[], char notation[64]);
void board_draw(int size);