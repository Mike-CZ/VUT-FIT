/*
* Soubor:  Main.cpp
* Datum:   2017/11/17
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX 2
* Popis:   Sachy
*/


#include <iostream>
#include <gtk/gtk.h>

#include "Board.h"

Board board;
GtkWidget* window;
GtkWidget* window2;
int clickCount = 0;
int fromRow, fromCol = -1;
int playerOnTurn = 0;
bool isEnd = false;

void handleClick(int row, int col) {
	Piece *current = board.getPiece(row, col);

	if (clickCount == 0 && current != NULL) {
		if ((current->getColor() == 'w' && playerOnTurn % 2 == 0) || (current->getColor() == 'b' && playerOnTurn % 2 == 1)) {
			fromRow = row;
			fromCol = col;
			clickCount++;
			board.setLabel("Vyberte policko pro presun. Nebo pravym tlacitkem zruste vyber.");
		}
	}
	else if (clickCount == 1) {
		Piece *piece = board.getPiece(fromRow, fromCol);
		int moveResult = board.movePiece(piece, fromRow, fromCol, row, col);

		if (moveResult == 1) {
			clickCount = 0;
			playerOnTurn++;
			if (playerOnTurn % 2 == 0) {
				board.setLabel("Na tahu je bily hrac. Vyberte figurku.");
			}
			else {
				board.setLabel("Na tahu je cerny hrac. Vyberte figurku.");
			}
		}
		else if (moveResult == 2) {
			isEnd = true;
			if (playerOnTurn % 2 == 0) {
				board.setLabel("Vitezi bily hrac.");
			}
			else {
				board.setLabel("Vitezi cerny hrac.");
			}
			gtk_window_present(GTK_WINDOW(window2));
		}
		else {
			board.setLabel("Neplatny tah. Vyberte jine pole nebo pravym tlacitkem zruste vyber.");
		}
	}
}

gint click_event(GtkWidget *widget, GdkEventButton  *event, gpointer data) {
	int x, y, row, col;
	GdkModifierType state;

	gdk_window_get_pointer(widget->window, &x, &y, &state);

	// ignore border
	if (! (x >= 10 && y >= 30 && x <= 410 && y <= 430)) {
		return true;
	}
	
	// get col and row number
	col = (x - 10) / 50;
	row = (y - 30) / 50;

	if (!isEnd) {
		// right mouse button
		if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
			clickCount = 0;
			if (playerOnTurn % 2 == 0) {
				board.setLabel("Na tahu je bily hrac. Vyberte figurku.");
			}
			else {
				board.setLabel("Na tahu je cerny hrac. Vyberte figurku.");
			}
		}
		// left mouse button
		else if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
			// handle click event into board
			handleClick(row, col);
		}
		else {
			return true;
		}
		board.draw(window);
	}

	return true;
}

// start new game
void start_new_game(GtkWidget *widget, gpointer data){
	clickCount = 0;
	fromRow = 0;
	fromCol = 0;
	playerOnTurn = 0;
	isEnd = false;

	board.newGame();
	board.setLabel("Na tahu je bily hrac. Vyberte figurku.");
	board.draw(window);
	gtk_window_present(GTK_WINDOW(window));
}


int main(int argc, char **argv) {

	// init
	gtk_init(&argc, &argv);
	board.init();

	// set window properties
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 420, 440);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_title(GTK_WINDOW(window), "Sachy");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	// set signal callbacks
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_set_events(window, GDK_BUTTON_PRESS_MASK);
	g_signal_connect(window, "button_press_event", G_CALLBACK(click_event), NULL);
	
	board.setLabel("Na tahu je bily hrac. Vyberte figurku.");

	// draw window
	board.draw(window);

	// new window for menu
	GtkWidget *button;
	GtkWidget *table;

	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window2, 200, 150);
	gtk_window_set_title(GTK_WINDOW(window2), "Menu");
	gtk_window_set_resizable(GTK_WINDOW(window2), FALSE);
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window2), "Menu");
	gtk_container_set_border_width(GTK_CONTAINER(window2), 20);
	gtk_signal_connect(GTK_OBJECT(window2), "delete_event", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

	// table
	table = gtk_table_new(1, 1, TRUE);
	gtk_container_add(GTK_CONTAINER(window2), table);

	// new game
	button = gtk_button_new_with_label("Nova hra");
	gtk_signal_connect(GTK_OBJECT(button), "clicked",GTK_SIGNAL_FUNC(start_new_game), NULL);
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 3, 0, 1);
	gtk_widget_show(button);

	// quit button
	button = gtk_button_new_with_label("Konec");
	gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 3, 1, 2);
	gtk_widget_show(button);

	// show
	gtk_widget_show(table);
	gtk_widget_show(window2);
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}