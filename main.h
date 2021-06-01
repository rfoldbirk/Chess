#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "raylib.h"
#include "main.h"


typedef struct Notations {
	char x;
	int y;
} Notation;


typedef struct Pieces {
    char name;

    bool has_texture; // Bruges til at vide om en brik allerede har et texture :)
	bool alive;
    bool color;

	bool selected;

	// Move meta
    bool first_move;

    Texture2D texture;
	
	Notation notation;
} Piece;


typedef struct Boards {
	Piece pieces[64];
	int amount;

	bool reverse_board;

	// Til selektering af brikker
	Vector2 selected_pos;
	int selected_piece_id;

	bool selected_deb;
} Board;


typedef struct Beams {
	Vector2 dir;
	int length;
} Beam;





Board board_init(char fen[64], bool reverse_board);

Board add_piece(Board board, char piece, Vector2 pos, Notation npos);



void draw(Board board);
void draw_board(int size);
void draw_piece(Board board, Piece piece, int size);


Board check_click(Board board);
Board move_piece(Board board, int piece_id);

Board keypress(Board board);


int find_piece(Board board, char x, int y);



Vector2 calc_piece_pos(Board board, Notation npos);