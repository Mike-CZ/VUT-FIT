NAME=sachy
CFLAGS=-g -Wall -std=c++11 -o $(NAME)
GTKFLAGS=`pkg-config --cflags --libs gtk+-2.0`
SRCS=Piece.cpp Board.cpp Main.cpp
CC=g++

all: main
 
main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(GTKFLAGS)
	unzip icons.zip
 
clean:
	rm -f $(NAME)
	rm -rf icons