/*
* Soubor:  Piece.h
* Datum:   2017/11/17
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX 2
* Popis:   Sachy
*/


#pragma once

#define NUMBER_OF_COLS 8
#define NUMBER_OF_ROWS 8

#include <string>


/*
* Abstract class representing a piece
*/
class Piece {
public:
	virtual char getPiece() = 0;
	virtual std::string getTextureName() = 0;
	virtual int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) = 0;
	char getColor();
	Piece(char color);
	virtual ~Piece() {};

protected:
	char _color;
};

/*
* Class representing pawn
*/
class Pawn : Piece {
public:
	char getPiece();
	std::string getTextureName();
	int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]);
	Pawn(char color) : Piece(color) { }
	~Pawn() { }
};

/*
* Class representing king
*/
class King : Piece {
public:
	char getPiece();
	std::string getTextureName();
	int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]);
	King(char color) : Piece(color) { }
	~King() { }
};

/*
* Class representing queen
*/
class Queen : Piece {
public:
	char getPiece();
	std::string getTextureName();
	int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]);
	Queen(char color) : Piece(color) { }
	~Queen() { }
};

/*
* Class representing rook
*/
class Rook : Piece {
public:
	char getPiece();
	std::string getTextureName();
	int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]);
	Rook(char color) : Piece(color) { }
	~Rook() { }
};

/*
* Class representing bishop
*/
class Bishop : Piece {
public:
	char getPiece();
	std::string getTextureName();
	int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]);
	Bishop(char color) : Piece(color) { }
	~Bishop() { }
};

/*
* Class representing knight
*/
class Knight : Piece {
public:
	char getPiece();
	std::string getTextureName();
	int isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]);
	Knight(char color) : Piece(color) { }
	~Knight() { }
};

