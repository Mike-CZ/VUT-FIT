/*
* Soubor:  Board.cpp
* Datum:   2017/11/17
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX 2
* Popis:   Sachy
*/


#include "Board.h"

// get piece on coords
Piece *Board::getPiece(int row, int col) {
	if (row >= NUMBER_OF_ROWS || col > NUMBER_OF_COLS || row < 0 || col < 0) {
		return NULL;
	}

	return _board[row][col];
}


// move piece on board
int Board::movePiece(Piece *piece, int fromRow, int fromCol, int toRow, int toCol) {
	bool isEnd = false;
	if (fromRow >= NUMBER_OF_ROWS || fromCol >= NUMBER_OF_COLS || toRow >= NUMBER_OF_ROWS || toCol >= NUMBER_OF_COLS ||
		fromRow < 0 || fromCol < 0 || toRow < 0 || toCol < 0) {
		return -1;
	}
	else if (_board[fromRow][fromCol] == NULL) {
		return -1;
	}

	int validMove = piece->isValidMove(fromRow, fromCol, toRow, toCol, _board);
	if (validMove == -1) {
		return -1;
	}

	Piece *target = _board[toRow][toCol];
	if (target != NULL) {
		if (target->getColor() == piece->getColor()) {
			return -1;
		}
		else {
			if (target->getPiece() == 'K') {
				isEnd = true;
			}
			delete _board[toRow][toCol];
			_board[toRow][toCol] = NULL;
		}
	}

	// transform pawn into queen when reaches opposite side of desk
	if (piece->getPiece() == 'P' && piece->getColor() == 'w') {
		if (toRow == 0) {
			delete piece;
			piece = (Piece *)new Queen('w');
		}
	}
	else if (piece->getPiece() == 'P' && piece->getColor() == 'b') {
		if (toRow == 7) {
			delete piece;
			piece = (Piece *)new Queen('b');
		}
	}

	_board[fromRow][fromCol] = NULL;
	_board[toRow][toCol] = piece;
	return isEnd ? 2 : 1;
}

// draw board
void Board::draw(GtkWidget* window) {
	GtkWidget *icon;
	std::string suffix;

	// free container
	if (_container != NULL) {
		gtk_container_remove(GTK_CONTAINER(window), _container);
	}

	// create new table
	_table = gtk_table_new(9, 8, FALSE);
	gtk_widget_set_size_request(_table, 400, 420);

	icon = NULL;
	for (int x = 0; x < NUMBER_OF_ROWS; x++) {
		for (int y = 0; y < NUMBER_OF_COLS; y++) {

			Piece *ptr = (Piece *)_board[x][y];
			suffix = (_board_colors[x][y] == 'd' ? "_dark" : "_light");

			if (ptr == NULL) {
				suffix = "./icons/empty" + suffix + ".jpg";
				icon = gtk_image_new_from_file(suffix.c_str());
			}
			else {
				suffix = "./icons/" + ptr->getTextureName() + suffix + ".jpg";
				icon = gtk_image_new_from_file(suffix.c_str());
			}
			gtk_table_attach(GTK_TABLE(_table), icon, y, y + 1, x + 1, x + 2, GTK_FILL, GTK_FILL, 0, 0);
		}
	}

	// set label
	_label = gtk_label_new(_labelText.c_str());
	gtk_widget_set_size_request(_label, 400, 20);
	gtk_table_attach(GTK_TABLE(_table), _label, 0, 8, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	// show window
	_container = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(_container), _table, 0, 0);
	gtk_container_add(GTK_CONTAINER(window), _container);
	gtk_widget_show_all(window);
}

// set label text
void Board::setLabel(std::string str) {
	_labelText = str;
}

// start new game
void Board::newGame() {
	for (int x = 0; x < NUMBER_OF_ROWS; x++) {
		for (int y = 0; y < NUMBER_OF_COLS; y++) {
			if (_board[x][y] != NULL) {
				delete _board[x][y];
				_board[x][y] = NULL;
			}
		}
	}
	init();
}


// init game board
void Board::init() {

	// ---------- BLACK -------------
	for (int y = 0; y < NUMBER_OF_COLS; y++) {
		_board[1][y] = (Piece *)new Pawn('b');
	}

	_board[0][0] = (Piece *)new Rook('b');
	_board[0][NUMBER_OF_COLS - 1] = (Piece *)new Rook('b');

	_board[0][1] = (Piece *)new Knight('b');
	_board[0][NUMBER_OF_COLS - 2] = (Piece *)new Knight('b');

	_board[0][2] = (Piece *)new Bishop('b');
	_board[0][NUMBER_OF_COLS - 3] = (Piece *)new Bishop('b');

	_board[0][3] = (Piece *)new Queen('b');
	_board[0][NUMBER_OF_COLS - 4] = (Piece *)new King('b');


	// ---------- WHITE -------------
	for (int y = 0; y < NUMBER_OF_COLS; y++) {
		_board[NUMBER_OF_ROWS - 2][y] = (Piece *)new Pawn('w');
	}

	_board[NUMBER_OF_ROWS - 1][0] = (Piece *)new Rook('w');
	_board[NUMBER_OF_ROWS - 1][NUMBER_OF_COLS - 1] = (Piece *)new Rook('w');

	_board[NUMBER_OF_ROWS - 1][1] = (Piece *)new Knight('w');
	_board[NUMBER_OF_ROWS - 1][NUMBER_OF_COLS - 2] = (Piece *)new Knight('w');

	_board[NUMBER_OF_ROWS - 1][2] = (Piece *)new Bishop('w');
	_board[NUMBER_OF_ROWS - 1][NUMBER_OF_COLS - 3] = (Piece *)new Bishop('w');

	_board[NUMBER_OF_ROWS - 1][3] = (Piece *)new Queen('w');
	_board[NUMBER_OF_ROWS - 1][NUMBER_OF_COLS - 4] = (Piece *)new King('w');
}

// Board constructor
Board::Board() {
	_table = NULL;
	_container = NULL;
	_label = NULL;
	_labelText = "Chess game";

	// init game board
	for (int x = 0; x < NUMBER_OF_ROWS; x++) {
		for (int y = 0; y < NUMBER_OF_COLS; y++) {
			_board[x][y] = NULL;

			if (x % 2 == 0) {
				if (y % 2 == 0) {
					_board_colors[x][y] = 'l';
				}
				else {
					_board_colors[x][y] = 'd';
				}
			}
			else {
				if (y % 2 == 0) {
					_board_colors[x][y] = 'd';
				}
				else {
					_board_colors[x][y] = 'l';
				}
			}
		}
	}
}

// Board destructor
Board::~Board() {
	for (int x = 0; x < NUMBER_OF_ROWS; x++) {
		for (int y = 0; y < NUMBER_OF_COLS; y++) {
			if (_board[x][y] != NULL) {
				delete _board[x][y];
				_board[x][y] = NULL;
			}
		}
	}
}

// print board content into console
void Board::print() {
	for (int x = 0; x < NUMBER_OF_ROWS; x++) {
		for (int y = 0; y < NUMBER_OF_COLS; y++) {
			if (_board[x][y] == NULL) {
				std::cout << '*';
			}
			else {
				std::cout << _board[x][y]->getPiece();
			}
		}
		std::cout << std::endl;
	}
}