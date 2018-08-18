/*
* Soubor:  window.h
* Datum:   2017/9/30
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX, Jednoduchy graficky editor
* Popis:   Jednoduchy graficky editor.
*/

#include <stdlib.h>
#include <stdio.h>

#include <Xm/Xm.h>		
#include <Xm/PushB.h>	
#include <Xm/BulletinB.h>
#include <Xm/DrawingA.h>
#include <Xm/MainW.h>
#include <Xm/Protocols.h>
#include <Xm/ComboBox.h>
#include <Xm/CascadeB.h>
#include <X11/keysym.h>
#include <Xm/MenuShell.h>
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>	
#include <Xm/MessageB.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>

#include <iostream>
#include <string>

#define WIDTH 800
#define HEIGHT 440

/*
* Drawing objects
*/
typedef enum {
	POINT = 0,
	LINE = 1,
	RECTANGLE = 2,
	FILLED_RECTANGLE = 3,
	ELIPSE = 4,
	FILLED_ELIPSE = 5
}DrawingType;
#define DRAWING_TYPE_COUNT 6
const String StrShapes[DRAWING_TYPE_COUNT] = { String("Point"), String("Line"), String("Rectangle"), String("Filled Rectangle"), String("Elipse"), String("Filled Elipse") };


/*
* Line thickness
*/
typedef enum {
	NONE = 0,
	MEDIUM = 1,
	BIG = 2
}Thickness;
#define THICKNESS_COUNT 3
const String StrThickness[THICKNESS_COUNT] = { String("0px"), String("3px"), String("8px") };
const int thickness_list[] = { 0, 3, 8 };

/*
* Line type
*/
typedef enum {
	FULL = 0,
	DASHED = 1
}LineType;
#define LINE_TYPE_COUNT 2
const String StrLineType[LINE_TYPE_COUNT] = { String("Full"), String("Dashed") };

/*
* Colors
*/
typedef enum {
	BLACK = 0,
	WHITE = 1,
	RED = 2,
	BLUE = 3
}Color;
#define COLORS_COUNT 4
const String StrColors[COLORS_COUNT] = { String("Black"), String("White"), String("Red"), String("Blue") };

/*
* Settings structure to store global settings
*/
typedef struct {
	DrawingType drawingType;
	int thickness;
	int lineType;
	Pixel lineFGColor;
	Pixel lineBGColor;
	Pixel fillFGColor;
	Pixel fillBGColor;
}Settings;

/*
* Structure for single drawing
*/
typedef struct {
	DrawingType drawingType;
	int thickness;
	int lineType;
	Pixel lineFGColor;
	Pixel lineBGColor;
	Pixel fillFGColor;
	Pixel fillBGColor;
	int x1, x2;
	int y1, y2;
}Drawing;

/*
* Main window class
*/
class MainWindow {
public:
	MainWindow() { }
	void start(int argc, char **argv);

private:
	XtAppContext _context;
	Widget _topLevel;
	Widget _box;
	Widget _menuBar;
	Widget _leftMenu;

	void colorsInit();
	void topLevelInit(int argc, char **argv);
	void boxInit();
	void menuBarInit();
	void leftMenuInit();
	void drawingAreaInit();
};

/*
* Callback functions
*/
void drawCallback(Widget w, XtPointer client_data, XtPointer call_data);
void motionMaskHandler(Widget w, XtPointer client_data, XEvent *event, Boolean *cont);
void exposeCallback(Widget w, XtPointer client_data, XtPointer call_data);
void clearCallback(Widget w, XtPointer client_data, XtPointer call_data);
void controlCallback(Widget w, XtPointer client_data, XtPointer call_data);
void exitCallback(Widget w, XtPointer client_data, XtPointer call_data);
void dialogCallback(Widget w, XtPointer client_data, XtPointer call_data);
void setDrawingType(Widget w, XtPointer client_data, XtPointer call_data);
void setThickness(Widget w, XtPointer client_data, XtPointer call_data);
void setLineType(Widget w, XtPointer client_data, XtPointer call_data);
void setLineFGColor(Widget w, XtPointer client_data, XtPointer call_data);
void setLineBGColor(Widget w, XtPointer client_data, XtPointer call_data);
void setFillFGColor(Widget w, XtPointer client_data, XtPointer call_data);
void setFillBGColor(Widget w, XtPointer client_data, XtPointer call_data);