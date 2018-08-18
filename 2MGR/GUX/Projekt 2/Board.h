/*
* Soubor:  Board.h
* Datum:   2017/11/17
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX 2
* Popis:   Sachy
*/


#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <gtk/gtk.h>


#include "Piece.h"

#define NUMBER_OF_COLS 8
#define NUMBER_OF_ROWS 8

/*
* Class representing game board
*/
class Board {
public:
	void init();
	void print();
	void draw(GtkWidget* window);
	Board();
	~Board();
	Piece *getPiece(int row, int col);
	int movePiece(Piece *piece, int fromRow, int fromCol, int toRow, int toCol);
	void setLabel(std::string str);
	void newGame();

private:
	Piece *_board[NUMBER_OF_COLS][NUMBER_OF_ROWS];
	char _board_colors[NUMBER_OF_COLS][NUMBER_OF_ROWS];
	GtkWidget* _table;
	GtkWidget* _container;
	GtkWidget* _label;
	std::string _labelText;
};