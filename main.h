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


typedef struct Beams {
	Vector2 dir;
	int length;
	bool last_is_enemy;
} Beam;


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

	Beam beam_collection[8]; // 0: n ne nw - s se sw - e w
	Notation allowed_moves[200];
} Board;



// board.c
Board board_init(char fen[64], bool reverse_board);
Board add_piece(Board board, char piece, Notation npos);

Board draw(Board board);
void draw_board(int size);
Board draw_piece(Board board, Piece piece, int size);
Board draw_moves(Board board, Piece piece);

int find_piece(Board board, char x, int y);
Board remove_selections(Board board);
Vector2 calc_piece_pos(Board board, Notation npos);
Notation increment(Notation npos, char fen_char);


// move.c
Board calc_moves(Board board, Piece piece);
Board move_piece(Board board, int piece_id);

Beam check_direction(Board board, Piece piece, Vector2 dir, int limit);


// keyboard.c
Board keypress(Board board);


// mouse.c
Board check_click(Board board);