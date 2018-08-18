/*
* Soubor:  window.cpp
* Datum:   2017/9/30
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: GUX, Jednoduchy graficky editor
* Popis:   Jednoduchy graficky editor.
*/

#include "window.h"

// exit dialog
Widget _quitQuestion;

// drawing area
Widget _drawingArea;

// colors
XColor _colors[COLORS_COUNT];

// settings
Settings settings;

// check if button is pressed
int isButtonPressed = 0;

// list of drawings
Drawing *drawings = NULL;
int drawingsCount = 0;

// drawing
Drawing drawing;

String fbr[] = {
	String("*bar.control.labelString: Control"),
	String("*bar.control.mnemonic: C"),
	String("*controlMenu.clear.labelString: Clear"),
	String("*controlMenu.clear.mnemonic: C"),
	String("*controlMenu.clear.acceleratorText: Ctrl-C"),
	String("*controlMenu.clear.accelerator: Ctrl<Key>C"),
	String("*controlMenu.exit.labelString: Exit"),
	String("*controlMenu.exit.mnemonic: E"),
	String("*controlMenu.exit.acceleratorText: Ctrl-Q"),
	String("*controlMenu.exit.accelerator: Ctrl<Key>Q"),
	String("*quitQuestion.dialogTitle: Exit"),
	String("*quitQuestion.messageString: Exit application?"),
	String("*quitQuestion.okLabelString: Yes"),
	String("*quitQuestion.cancelLabelString: No"),
	String("*quitQuestion.messageAlignment: XmALIGNMENT_CENTER"),
	String("*.shapeLabel.labelString: Shape:"),
	String("*.thicknessLabel.labelString: Thickness:"),
	String("*.lineTypeLabel.labelString: Line Type:"),
	String("*.lineFGColorLabel.labelString: Line Foreground Color:"),
	String("*.lineBGColorLabel.labelString: Line Background Color:"),
	String("*.fillFGColorLabel.labelString: Fill Foreground Color:"),
	String("*.fillBGColorLabel.labelString: Fill Background Color:"),
	NULL
};

/*
* Start application
*/
void MainWindow::start(int argc, char **argv) {
	XtSetLanguageProc(NULL, NULL, NULL);

	topLevelInit(argc, argv);
	colorsInit();
	boxInit();
	menuBarInit();
	leftMenuInit();
	drawingAreaInit();

	Atom wm = XInternAtom(XtDisplay(_topLevel), "WM_DELETE_WINDOW", False);
	XmAddWMProtocolCallback(_topLevel, wm, exitCallback, NULL);
	XmActivateWMProtocol(_topLevel, wm);

	XtManageChild(_box);
	XtManageChild(_menuBar);
	XtManageChild(_drawingArea);

	XtRealizeWidget(_topLevel);
	XtAppMainLoop(_context);
}

/*
* Top level initialization
*/
void MainWindow::topLevelInit(int argc, char **argv) {
	_topLevel = XtVaAppInitialize(
		&_context,
		"Draw",
		NULL, 0,
		&argc, argv,
		fbr,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL
	);

	_quitQuestion = XmCreateQuestionDialog(_topLevel, String("quitQuestion"), NULL, 0);
	XtVaSetValues(_quitQuestion, XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL, NULL);
	XtUnmanageChild(XmMessageBoxGetChild(_quitQuestion, XmDIALOG_HELP_BUTTON));
	XtAddCallback(_quitQuestion, XmNcancelCallback, dialogCallback, (XtPointer)0);
	XtAddCallback(_quitQuestion, XmNokCallback, dialogCallback, (XtPointer)1);

	settings.drawingType = LINE;
	settings.thickness = MEDIUM;
	settings.lineType = FULL;
}

/*
* Drawing area initialization
*/
void MainWindow::drawingAreaInit() {
	_drawingArea = XtVaCreateManagedWidget(
		"drawingArea",			
		xmDrawingAreaWidgetClass,		
		_leftMenu,
		XmNbackground, _colors[WHITE].pixel,
		NULL);

	XtAddEventHandler(_drawingArea, ButtonMotionMask, False, motionMaskHandler, NULL);
	XtAddCallback(_drawingArea, XmNexposeCallback, exposeCallback, _drawingArea);
	XtAddCallback(_drawingArea, XmNinputCallback, drawCallback, _drawingArea);
}

/*
* Box initialization
*/
void MainWindow::boxInit() {
	_box =  XtVaCreateManagedWidget(
		"box", 		
		xmMainWindowWidgetClass,
		_topLevel,
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		XmNwidth, WIDTH,
		XmNheight, HEIGHT,
		NULL);	
}

/*
* Menu bar initialization
*/
void MainWindow::menuBarInit() {
	Widget controlShell, controlMenu, controlButton, clearButton, exitButton;

	_menuBar = XtVaCreateManagedWidget("bar",
		xmRowColumnWidgetClass, _box,
		XmNrowColumnType, XmMENU_BAR,
		NULL);

	controlShell = XtVaCreatePopupShell("controlShell",
		xmMenuShellWidgetClass, _menuBar,
		XmNwidth, 1,
		XmNheight, 1,
		XmNallowShellResize, True,
		XmNoverrideRedirect, True,
		NULL);

	controlMenu = XtVaCreateWidget("controlMenu",
		xmRowColumnWidgetClass, controlShell,
		XmNrowColumnType, XmMENU_PULLDOWN,
		NULL);

	controlButton = XtVaCreateManagedWidget("control",
		xmCascadeButtonWidgetClass, _menuBar,
		XmNsubMenuId, controlMenu,
		NULL);

	clearButton = XtVaCreateManagedWidget("clear",
		xmPushButtonWidgetClass,
		controlMenu,
		NULL);
	XtAddCallback(clearButton, XmNactivateCallback, controlCallback, (XtPointer)1);

	exitButton = XtVaCreateManagedWidget("exit",
		xmPushButtonWidgetClass,
		controlMenu,
		NULL);
	XtAddCallback(exitButton, XmNactivateCallback, controlCallback, (XtPointer)2);
}

/*
* Left menu bar initialization
*/
void MainWindow::leftMenuInit() {
	Widget row, shapes, thicknes, lineType, lineFGColor, lineBGColor, fillFGColor, fillBGColor;

	_leftMenu = XtVaCreateManagedWidget(
		"leftMenu",				
		xmFrameWidgetClass,		
		_box,				
		NULL);	

	row = XtVaCreateManagedWidget(
		"row",
		xmRowColumnWidgetClass,
		_leftMenu,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmVERTICAL,
		XmNpacking, XmPACK_TIGHT,
		NULL);

	// SHAPES
	XtVaCreateManagedWidget(
		"shapeLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	XmStringTable list = (XmStringTable) XtMalloc(DRAWING_TYPE_COUNT * sizeof(XmString));

	for (int x = 0; x < DRAWING_TYPE_COUNT; x++) {
		list[x] = XmStringCreateLocalized(StrShapes[x]);
	}

	shapes = XtVaCreateManagedWidget(
		"shapeSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, DRAWING_TYPE_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		XmNselectedPosition, LINE,
		NULL);

	XtAddCallback(shapes, XmNselectionCallback, setDrawingType, 0);

	// LINE THICKNESS
	XtVaCreateManagedWidget(
		"thicknessLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	list = (XmStringTable)XtMalloc(THICKNESS_COUNT*sizeof(XmString));

	for (int x = 0; x < THICKNESS_COUNT; x++) {
		list[x] = XmStringCreateLocalized(StrThickness[x]);
	}

	thicknes = XtVaCreateManagedWidget(
		"thicknessSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, THICKNESS_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		NULL);

	XtAddCallback(thicknes, XmNselectionCallback, setThickness, 0);

	// LINE TYPE
	XtVaCreateManagedWidget(
		"lineTypeLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	list = (XmStringTable)XtMalloc(LINE_TYPE_COUNT*sizeof(XmString));

	for (int x = 0; x < LINE_TYPE_COUNT; x++) {
		list[x] = XmStringCreateLocalized(StrLineType[x]);
	}

	lineType = XtVaCreateManagedWidget(
		"thicknessSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, LINE_TYPE_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		NULL);

	XtAddCallback(lineType, XmNselectionCallback, setLineType, 0);

	// LINE FOREGROUND COLOR
	XtVaCreateManagedWidget(
		"lineFGColorLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	list = (XmStringTable)XtMalloc(COLORS_COUNT*sizeof(XmString));

	for (int x = 0; x < COLORS_COUNT; x++) {
		list[x] = XmStringCreateLocalized(StrColors[x]);
	}

	lineFGColor = XtVaCreateManagedWidget(
		"lineFGSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COLORS_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		XmNselectedPosition, BLACK,
		NULL); 

	XtAddCallback(lineFGColor, XmNselectionCallback, setLineFGColor, 0);

	// LINE BACKGROUND COLOR
	XtVaCreateManagedWidget(
		"lineBGColorLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	lineBGColor = XtVaCreateManagedWidget(
		"lineBGSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COLORS_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		XmNselectedPosition, WHITE,
		NULL);

	XtAddCallback(lineBGColor, XmNselectionCallback, setLineBGColor, 0);

	// FILL FOREGROUND COLOR
	XtVaCreateManagedWidget(
		"fillFGColorLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	fillFGColor = XtVaCreateManagedWidget(
		"fillFGColorSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COLORS_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		XmNselectedPosition, BLACK,
		NULL);

	XtAddCallback(fillFGColor, XmNselectionCallback, setFillFGColor, 0);

	// FILL BACKGROUND COLOR
	XtVaCreateManagedWidget(
		"fillBGColorLabel",
		xmLabelWidgetClass,
		row,
		NULL);

	fillBGColor = XtVaCreateManagedWidget(
		"fillBGColorSelect",
		xmComboBoxWidgetClass,
		row,
		XmNcomboBoxType, XmDROP_DOWN_LIST,
		XmNitemCount, COLORS_COUNT,
		XmNitems, list,
		XmNpositionMode, XmZERO_BASED,
		XmNselectedPosition, WHITE,
		NULL);

	XtAddCallback(fillBGColor, XmNselectionCallback, setFillBGColor, 0);
}

/*
* Colors initialization
*/
void MainWindow::colorsInit() {
	Display* display = XOpenDisplay(NULL);
	int defaultScreen = DefaultScreen(display);
	Colormap colorMap = DefaultColormap(display, defaultScreen);

	_colors[BLACK].red = 0; _colors[BLACK].green = 0; _colors[BLACK].blue = 0;
	_colors[WHITE].red = 65535; _colors[WHITE].green = 65535; _colors[WHITE].blue = 65535;
	_colors[RED].red = 65535; _colors[RED].green = 0; _colors[RED].blue = 0;
	_colors[BLUE].red = 0; _colors[BLUE].green = 0; _colors[BLUE].blue = 65535;

	for (int x = 0; x < COLORS_COUNT; x++) {
		_colors[x].flags = DoRed | DoGreen | DoBlue;
		XAllocColor(display, colorMap, &_colors[x]);
	}

	settings.lineFGColor = _colors[BLACK].pixel;
	settings.lineBGColor = _colors[WHITE].pixel;
	settings.fillFGColor = _colors[BLACK].pixel;
	settings.fillBGColor = _colors[WHITE].pixel;
}

void render(Widget w, GC gc, Drawing drawing) {
	XGCValues values;
	int x1 = drawing.x1;
	int y1 = drawing.y1;
	int x2 = drawing.x2;
	int y2 = drawing.y2;
	int width = drawing.thickness;
	values.foreground = drawing.lineFGColor;
	values.background = drawing.lineBGColor;
	values.line_width = drawing.thickness;
	values.line_style = drawing.lineType;

	XChangeGC(XtDisplay(w), gc, GCLineStyle | GCLineWidth | GCForeground | GCBackground, &values);

	switch (drawing.drawingType) {
	case POINT:
		if (width > 0)
			XFillArc(XtDisplay(w), XtWindow(w), gc, (int)(x2 - width / 2.0), (int)(y2 - width / 2.0), width, width, 0, 360 * 64);
		else
			XDrawPoint(XtDisplay(w), XtWindow(w), gc, x2, y2);
		break;
	case LINE:
		XDrawLine(XtDisplay(w), XtWindow(w), gc, x1, y1, x2, y2);
		break;
	case RECTANGLE:
		XDrawRectangle(XtDisplay(w), XtWindow(w), gc, x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
		break;
	case ELIPSE:
		XDrawArc(XtDisplay(w), XtWindow(w), gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
		break;
	case FILLED_RECTANGLE:
		XSetForeground(XtDisplay(w), gc, drawing.fillFGColor);
		XFillRectangle(XtDisplay(w), XtWindow(w), gc, x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
		XSetForeground(XtDisplay(w), gc, drawing.lineFGColor);
		XDrawRectangle(XtDisplay(w), XtWindow(w), gc, x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2, abs(x2 - x1), abs(y2 - y1));
		break;
	case FILLED_ELIPSE:
		XSetForeground(XtDisplay(w), gc, drawing.fillFGColor);
		XFillArc(XtDisplay(w), XtWindow(w), gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
		XSetForeground(XtDisplay(w), gc, drawing.lineFGColor);
		XDrawArc(XtDisplay(w), XtWindow(w), gc, x1 - abs(x2 - x1), y1 - abs(y2 - y1), abs(x2 - x1) * 2, abs(y2 - y1) * 2, 0, 360 * 64);
		break;
	default:
		break;
	}
}

/*
* Motion mask handler
*/
void motionMaskHandler(Widget w, XtPointer client_data, XEvent *event, Boolean *cont) {
	static GC gc = 0;
	Pixel foreground;
	Pixel background;

	if (isButtonPressed) {
		if (!gc) {
			gc = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
			XtVaGetValues(w, XmNforeground, &foreground, XmNbackground, &background, NULL);
			XSetForeground(XtDisplay(w), gc, foreground ^ background);
			XSetFunction(XtDisplay(w), gc, GXxor);
			XSetPlaneMask(XtDisplay(w), gc, ~0);
		}

		if (isButtonPressed > 1) {
			render(w, gc, drawing);
		}
		else {
			isButtonPressed = 2;
		}

		drawing.x2 = event->xmotion.x;
		drawing.y2 = event->xmotion.y;
		render(w, gc, drawing);
	}
}
/*
* Expose callback
*/
void exposeCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	static GC gc = 0;
	
	if (drawingsCount > 0) {
		if (!gc) {
			gc = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
		}
		for (int x = 0; x < drawingsCount; x++) {
			render(w, gc, drawings[x]);
		}
	}
}

/*
* Draw callback
*/
void drawCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	XmDrawingAreaCallbackStruct *cbs = (XmDrawingAreaCallbackStruct *)call_data;

	if (cbs->event->type == ButtonPress) {
		if (cbs->event->xbutton.button == Button1) {
			isButtonPressed = 1;
			drawing.drawingType = settings.drawingType;
			drawing.thickness = settings.thickness;
			drawing.lineType = settings.lineType;
			drawing.lineFGColor = settings.lineFGColor ^ _colors[WHITE].pixel;
			drawing.lineBGColor = settings.lineBGColor ^ _colors[WHITE].pixel;
			drawing.fillFGColor = settings.fillFGColor ^ _colors[WHITE].pixel;
			drawing.fillBGColor = settings.fillBGColor ^ _colors[WHITE].pixel;
			drawing.x1 = cbs->event->xbutton.x;
			drawing.y1 = cbs->event->xbutton.y;
		}
	}
	else if (cbs->event->type == ButtonRelease) {
		if (cbs->event->xbutton.button == Button1) {
			isButtonPressed = 0;
			drawing.x2 = cbs->event->xbutton.x;
			drawing.y2 = cbs->event->xbutton.y;
			drawing.lineFGColor = settings.lineFGColor ^ _colors[BLACK].pixel;
			drawing.lineBGColor = settings.lineBGColor ^ _colors[BLACK].pixel;
			drawing.fillFGColor = settings.fillFGColor ^ _colors[BLACK].pixel;
			drawing.fillBGColor = settings.fillBGColor ^ _colors[BLACK].pixel;

			drawingsCount++;
			drawings = (Drawing *)realloc(drawings, drawingsCount * sizeof(Drawing));
			memcpy(&drawings[drawingsCount - 1], &drawing, sizeof(Drawing));

			XClearArea(XtDisplay(w), XtWindow(w), 0, 0, 0, 0, True);
		}
	}
}

/*
* Clear callback
*/
void clearCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	free(drawings);
	drawingsCount = 0;
	drawings = NULL;
	XClearWindow(XtDisplay(_drawingArea), XtWindow(_drawingArea));
}

/*
* Control callback
*/
void controlCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	int data = (long)client_data;
	if (data == 1) {
		clearCallback(w, client_data, call_data);
	}
	else if (data == 2) {
		exitCallback(w, client_data, call_data);
	}
}

/*
* Exit callback
*/
void exitCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	XtManageChild(_quitQuestion);
}

/*
* Dialog callback
*/
void dialogCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	int data = (long)client_data;
	if (data == 1) {
		exit(0);
	}
}

/*
* Set drawing type callback
*/
void setDrawingType(Widget w, XtPointer client_data, XtPointer call_data) {
	settings.drawingType = (DrawingType)((XmComboBoxCallbackStruct *)call_data)->item_position;
}

/*
* Set thickness callback
*/
void setThickness(Widget w, XtPointer client_data, XtPointer call_data) {
	settings.thickness = thickness_list[((XmComboBoxCallbackStruct *)call_data)->item_position];
}

/*
* Set drawing type callback
*/
void setLineType(Widget w, XtPointer client_data, XtPointer call_data) {
	switch (((XmComboBoxCallbackStruct *)call_data)->item_position)	{
	case DASHED:
		settings.lineType = LineDoubleDash;
		break;
	default:
		settings.lineType = LineSolid;
		break;
	}
}

/*
* Set line foreground color
*/
void setLineFGColor(Widget w, XtPointer client_data, XtPointer call_data) {
	settings.lineFGColor = _colors[((XmComboBoxCallbackStruct *)call_data)->item_position].pixel;
}

/*
* Set line background color
*/
void setLineBGColor(Widget w, XtPointer client_data, XtPointer call_data) {
	settings.lineBGColor = _colors[((XmComboBoxCallbackStruct *)call_data)->item_position].pixel;
}

/*
* Set fill foreground color
*/
void setFillFGColor(Widget w, XtPointer client_data, XtPointer call_data) {
	settings.fillFGColor = _colors[((XmComboBoxCallbackStruct *)call_data)->item_position].pixel;
}

/*
* Set fill background color
*/
void setFillBGColor(Widget w, XtPointer client_data, XtPointer call_data) {
	settings.fillBGColor = _colors[((XmComboBoxCallbackStruct *)call_data)->item_position].pixel;
}