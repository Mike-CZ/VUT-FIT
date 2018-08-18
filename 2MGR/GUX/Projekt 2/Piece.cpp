/*
* Soubor:  Piece.cpp
* Datum:   2017/11/17
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX 2
* Popis:   Sachy
*/


#include "Piece.h"

// get type of piece
char Pawn::getPiece() {
	return 'P';
}

// get texture name
std::string Pawn::getTextureName() {
	return  std::string(_color == 'b' ? "black" : "white") + "_pawn";
}

// check valid move
int Pawn::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) {
	if (board[fromRow][fromCol]->getColor() == 'w' && toRow > fromRow) {
		return -1;
	}
	else if (board[fromRow][fromCol]->getColor() == 'b' && toRow < fromRow) {
		return -1;
	}

	if (fromRow == 1 && fromCol == toCol && toRow == fromRow + 2) {
		if (board[fromRow + 1][fromCol] != NULL) {
			return -1;
		}
		return 1;
	}
	else if (fromRow == 6 && fromCol == toCol && toRow == fromRow - 2) {
		if (board[fromRow - 1][fromCol] != NULL) {
			return -1;
		}
		return 1;
	}
	else if (toRow == fromRow + 1 && fromCol == toCol) {
		if (board[fromRow +1][fromCol] != NULL) {
			return -1;
		}
		return 1;
	}
	else if (toRow == fromRow - 1 && fromCol == toCol) {
		if (board[fromRow - 1][fromCol] != NULL) {
			return -1;
		}
		return 1;
	}

	if (toRow == fromRow - 1 && (fromCol == toCol - 1 || fromCol == toCol + 1)) {
		if (board[toRow][toCol] != NULL) {
			return 1;
		}
		return -1;
	}
	else if (toRow == fromRow + 1 && (fromCol == toCol - 1 || fromCol == toCol + 1)) {
		if (board[toRow][toCol] != NULL) {
			return 1;
		}
		return -1;
	}

	return -1;
}

// get type of piece
char King::getPiece() {
	return 'K';
}

// get texture name
std::string King::getTextureName() {
	return  std::string(_color == 'b' ? "black" : "white") + "_king";
}

// check valid move
int King::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) {
	if (toRow == fromRow && (toCol == fromCol - 1 || toCol == fromCol + 1)) {
		return 1;
	}
	else if (fromCol == toCol && (toRow == fromRow - 1 || toRow == fromRow + 1)) {
		return 1;
	}
	else if (toCol == fromCol - 1 && (toRow == fromRow - 1 || toRow == fromRow + 1)) {
		return 1;
	}
	else if (toCol == fromCol + 1 && (toRow == fromRow - 1 || toRow == fromRow + 1)) {
		return 1;
	}

	return -1;
}

// get type of piece
char Queen::getPiece() {
	return 'Q';
}

// get texture name
std::string Queen::getTextureName() {
	return  std::string(_color == 'b' ? "black" : "white") + "_queen";
}

// check valid move
int Queen::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) {
	if (fromCol == toCol) {
		if (fromRow > toRow) {
			for (int x = fromRow - 1; x > toRow; x--) {
				if (board[x][toCol] != NULL) {
					return -1;
				}
			}
			return 1;
		}
		else if (fromRow < toRow) {
			for (int x = fromRow + 1; x < toRow; x++) {
				if (board[x][toCol] != NULL) {
					return -1;
				}
			}
			return 1;
		}
	}	
	else if (fromRow == toRow) {
		if (fromCol > toCol) {
			for (int y = fromCol - 1; y > toCol; y--) {
				if (board[toRow][y] != NULL) {
					return -1;
				}
			}
			return 1;
		}
		else if (fromCol < toCol) {
			for (int y = fromCol + 1; y < toCol; y++) {
				if (board[toRow][y] != NULL) {
					return -1;
				}
			}
			return 1;
		}
	}
	else {
		if (fromRow > toRow) {
			if (fromCol > toCol) {
				int x = 1;
				while (true) {
					if (fromRow - x == toRow && fromCol - x == toCol) {
						return 1;
					}
					else if (fromRow - x < 0 || fromCol - x < 0) {
						return -1;
					}
					else if (board[fromRow - x][fromCol - x] != NULL) {
						return -1;
					}
					x++;
				}
			}
			else if (fromCol < toCol) {
				int x = 1;
				while (true) {
					if (fromRow - x == toRow && fromCol + x == toCol) {
						return 1;
					}
					else if (fromRow - x < 0 || fromCol + x > NUMBER_OF_COLS) {
						return -1;
					}
					else if (board[fromRow - x][fromCol + x] != NULL) {
						return -1;
					}
					x++;
				}
			}
		}
		else if (fromRow < toRow) {
			if (fromCol > toCol) {
				int x = 1;
				while (true) {
					if (fromRow + x == toRow && fromCol - x == toCol) {
						return 1;
					}
					else if (fromRow + x > NUMBER_OF_ROWS || fromCol - x < 0) {
						return -1;
					}
					else if (board[fromRow + x][fromCol - x] != NULL) {
						return -1;
					}
					x++;
				}
			}
			else if (fromCol < toCol) {
				int x = 1;
				while (true) {
					if (fromRow + x == toRow && fromCol + x == toCol) {
						return 1;
					}
					else if (fromRow + x > NUMBER_OF_ROWS || fromCol + x > NUMBER_OF_COLS) {
						return -1;
					}
					else if (board[fromRow + x][fromCol + x] != NULL) {
						return -1;
					}
					x++;
				}
			}
		}
	}

	return -1;
}

// get type of piece
char Rook::getPiece() {
	return 'R';
}

// get texture name
std::string Rook::getTextureName() {
	return  std::string(_color == 'b' ? "black" : "white") + "_rook";
}

// check valid move
int Rook::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) {
	if (fromCol == toCol) {
		if (fromRow > toRow) {
			for (int x = fromRow - 1; x > toRow; x--) {
				if (board[x][toCol] != NULL) {
					return -1;
				}
			}
			return 1;
		}
		else if (fromRow < toRow) {
			for (int x = fromRow + 1; x < toRow; x++) {
				if (board[x][toCol] != NULL) {
					return -1;
				}
			}
			return 1;
		}
	}
	else if (fromRow == toRow) {
		if (fromCol > toCol) {
			for (int y = fromCol - 1; y > toCol; y--) {
				if (board[toRow][y] != NULL) {
					return -1;
				}
			}
			return 1;
		}
		else if (fromCol < toCol) {
			for (int y = fromCol + 1; y < toCol; y++) {
				if (board[toRow][y] != NULL) {
					return -1;
				}
			}
			return 1;
		}
	}
	return -1;
}

// get type of piece
char Bishop::getPiece() {
	return 'B';
}

// get texture name
std::string Bishop::getTextureName() {
	return  std::string(_color == 'b' ? "black" : "white") + "_bishop";
}

// check valid move
int Bishop::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) {
	if (fromRow > toRow) {
		if (fromCol > toCol) {
			int x = 1;
			while (true) {
				if (fromRow - x == toRow && fromCol - x == toCol) {
					return 1;
				}
				else if (fromRow - x < 0 || fromCol - x < 0) {
					return -1;
				}
				else if (board[fromRow - x][fromCol - x] != NULL) {
					return -1;
				}
				x++;
			}
		}
		else if (fromCol < toCol) {
			int x = 1;
			while (true) {
				if (fromRow - x == toRow && fromCol + x == toCol) {
					return 1;
				}
				else if (fromRow - x < 0 || fromCol + x > NUMBER_OF_COLS) {
					return -1;
				}
				else if (board[fromRow - x][fromCol + x] != NULL) {
					return -1;
				}
				x++;
			}
		}
	}
	else if (fromRow < toRow) {
		if (fromCol > toCol) {
			int x = 1;
			while (true) {
				if (fromRow + x == toRow && fromCol - x == toCol) {
					return 1;
				}
				else if (fromRow + x > NUMBER_OF_ROWS || fromCol - x < 0) {
					return -1;
				}
				else if (board[fromRow + x][fromCol - x] != NULL) {
					return -1;
				}
				x++;
			}
		}
		else if (fromCol < toCol) {
			int x = 1;
			while (true) {
				if (fromRow + x == toRow && fromCol + x == toCol) {
					return 1;
				}
				else if (fromRow + x > NUMBER_OF_ROWS || fromCol + x > NUMBER_OF_COLS) {
					return -1;
				}
				else if (board[fromRow + x][fromCol + x] != NULL) {
					return -1;
				}
				x++;
			}
		}
	}
	return -1;
}

// get type of piece
char Knight::getPiece() {
	return 'N';
}

// get texture name
std::string Knight::getTextureName() {
	return  std::string(_color == 'b' ? "black" : "white") + "_knight";
}

// check valid move
int Knight::isValidMove(int fromRow, int fromCol, int toRow, int toCol, Piece *board[NUMBER_OF_COLS][NUMBER_OF_ROWS]) {
	if (toCol == fromCol - 2 || toCol == fromCol + 2) {
		if (toRow == fromRow - 1 || toRow == fromRow + 1) {
			return 1;
		}
	}
	else if (toRow == fromRow - 2 || toRow == fromRow + 2) {
		if (toCol == fromCol - 1 || toCol == fromCol + 1) {
			return 1;
		}
	}
	return -1;
}

// piece contructor
Piece::Piece(char color) {
	_color = color;
}

// get color
char Piece::getColor() {
	return _color;
}