/*

  Nullsoft WASABI Source File License

  Copyright 1999-2001 Nullsoft, Inc.

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.


  Brennan Underwood
  brennan@nullsoft.com

*/

// ===========================================================================
//
//    NULLSOFT WASABI SDK EXAMPLE PROJECTS
//
//      File:     ExampleCodeGUIWnd.h
//
//!##   Purpose:  Define the Component's main window class for our example
//
//      Requires: Please read Example1 first to understand how to make a component
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.
//


#ifndef _EXAMPLECODEGUIWND_H
#define _EXAMPLECODEGUIWND_H

//
// This is our parent class: TabSheet from "../../common/tabsheet.h"
// If you are including more than just the header to your parent class
// in your headers, you need to think carefully about why you are doing so.
#include "../../bfc/wnds/tabsheet.h"

//
//  Forward References
class ExampleCodeGUIEditWnd;
class TreeWnd;
class ContWnd;
class UrlGrab;
class TextBar;
class ButtWnd;
class StylePosWnd;
class SliderWnd;
class FormWnd;
class FormCell;
class RadioGroup;
template<class T> // forward referencing a template :)
class MemBlock;
class DropList;
class ListWnd;


//
//  Class Definition
#define EXAMPLECODEGUIWND_PARENT TabSheet
class ExampleCodeGUIWnd : public EXAMPLECODEGUIWND_PARENT {
public:

  //
  // These are the ID values that we give to our
  // buttons in order to know what the hell got
  // pushed when we get told something got pushed.
  typedef enum {
    // Core Buttons Tabsheet IDs
    EXB_PREVIOUS = 0,
    EXB_PLAY = 1,
    EXB_PAUSE = 2,
    EXB_STOP = 3,
    EXB_NEXT = 4,
    // Frames Tabsheet IDs
    EXB_FRAMEBUTTON1 = 5,
    EXB_FRAMEBUTTON2 = 6,
    // Minesweeper Tabsheet IDs
    EXB_MINEGAMESTART = 7,
    EXB_MINEVERTSLIDER = 8,
    EXB_MINEHORIZSLIDER = 9,
    EXB_MINEMINESSLIDER = 10,
    EXB_MINECELLBUTTON = 100,  // Offset the cell index by this value.
    // Checklist Tabsheet IDs
    EXB_CHECKBAKERY = 20,
    EXB_CHECKBREAD = 21,
    EXB_CHECKCAKE = 22,
    EXB_CHECKDONUTS = 23,
    EXB_CHECKDAIRY = 24,
    EXB_CHECKMILK = 25,
    EXB_CHECKCHEESE = 26,
    EXB_CHECKBUTTER = 27,
		// Droplist Tabsheet IDs
		EXB_DROPLIST = 30,
    //
    NUM_EXB_NOTIFY
  } ExampleCodeGUIWindowNotifyIDs;

  //
  // These constants control the number of pixels to offset the UI Objects
  // from the various places they might be offset (see the .CPP file) as well
  // as other basic constants required by various user interface objects.
  typedef enum {
    // Core Buttons and Frames Tabsheets
    EXB_LEFT_OFFSET = 11,
    EXB_TOP_OFFSET = 10,
    EXB_BUTTON_WIDTH  = 60,
    EXB_BUTTON_HEIGHT = 25,
    EXB_BUTTON_SPACER = 3,
    EXB_EDIT_BUFFSIZE = 2000,
    // Minesweeper Tabsheet
    EXB_MINE_SLIDERWD = 15,
    EXB_MINE_SLIDERLN = 120,
    EXB_MINE_EDGE = 5,
    EXB_MINE_THINEDGE = 10,
    EXB_MINE_CORNER = 25,
    EXB_MINE_MINGRID = 5,
    EXB_MINE_MAXGRID = 15,
    EXB_MINE_MINMINES = 20,
    EXB_MINE_MAXMINES = 40,
    // Checklist Tabsheet
    EXB_CHECK_SIZE = 15,
    EXB_CHECK_OFFSET = -5,
    //
    NUM_EXB_VALUES
  } ExampleCodeGUIValues;

public:

  // We commit the tasks of 1) Instantiation and 2) Assembly in the 
  // CONSTRUCTOR of the containment class.  The third primary task to occur 
  // before normal framelooped behaviour, Initialization, occurs AUTOMATICALLY
  // for ALL underived contained objects by the baseclass onInit() handler.
  ExampleCodeGUIWnd();
  // So, obviously, if you override your onInit() method, you'd best be 
  // calling your parentclass too, unless you've got a damn good reason not to

  // =========================================================================
  //
  //  Methods required by Window Creation Services
  //
  static const char *getWindowTypeName();
  static GUID getWindowTypeGuid();
  static void setIconBitmaps(ButtonWnd *button);

  // =========================================================================
  //
  //  Methods used primarily by ExampleCodeGUIWnd.cpp
  //
  // Go create all the windows into the tabsheet.
  int createChildWindows();     
  // Make some fun buttons that link to the core controls.
  int createCoreButtonsSheet(); 


  // =========================================================================
  //
  //  Methods used primarily by ExampleCodeGUIFrameWnd.cpp
  //
  // Make some frames and a tree, maybe an editbox, too.
  int createFramesSheet();  
  // Set the text in the display field.
  void displayText(const char * text);
  // Set the text in a button that can be clicked (instead of the display field)
  void displayButton(const char * text);
  // Handle the buttons in the UI being pushed.
  void onFrameButton1();
  void onFrameButton2();


  // =========================================================================
  //
  //  Methods used primarily by ExampleCodeGUIStyleWnd.cpp
  //
  // Make some SliderWnds, and a FormWnd, stick'm all in a StylePosWnd
  int createMinesweeperSheet();
  void newGame();
  void createMinesGrid();
  void displayVert(int rangeval); // In raw slider units.
  void displayHoriz(int rangeval); // In raw slider units.
  void calcMines();
  void cellHit(int buttonid); // The _real_ button ID.
  void setSafe(int buttonid); // The _real_ button ID.
  void testWin();
  void setupValues();
  void setXYCell(int x, int y, int val); // Helpers for the cell values
  int getXYCell(int x, int y);
  void setICell(int linear_index, int val);
  int getICell(int linear_index);

  // =========================================================================
  //
  //  Methods used primarily by ExampleCodeGUIChecklistWnd.cpp
  //
  // Checkboxes and RadioGroups
  int createChecklistSheet();
  int handleChecklistNotify(int msg, int objId, int param1, int param2);
  int displayChecklistText(const char *text);

  // =========================================================================
  //
  //  Methods used primarily by ExampleCodeGUIChecklistWnd.cpp
  //
  // Lists and Droplists
	int createDroplistSheet();
	int postInitDroplist();		// can't add items to these things until initted.
	int handleDroplistChanged(int msg, int objId, int param1, int param2);
	void populateListWnd(const char *newLameText);

  // =========================================================================
  //
  //  Methods used by all primary modules in this project
  //
  // A centralized notification handler, that strictly uses the
  // msg and param values, not pointers.
  int handleChildNotify (int msg, int objId, int param1, int param2);


  // =========================================================================
  //
  //  Virtual methods from parent derived for this object
  //
  virtual int childNotify(RootWnd *which, int msg, int param1, int param2);
	virtual int onInit();

protected:

  // =========================================================================
  //
  //  Data values used primarily by ExampleCodeGUIFrameWnd.cpp
  //
  // We need to keep the pointers to some objects in order to more easily
  // interact with them in a positive and supportive manner.
  TreeWnd *myFrameTreeWnd;
  ContWnd *myFrameContWnd;
  TextBar *myFrameTextBar;
  ButtonWnd *myFrameButtWnd;
  ExampleCodeGUIEditWnd *myFrameEditWnd;

  // =========================================================================
  //
  //  Data values used primarily by ExampleCodeGUIStyleWnd.cpp
  //
  StylePosWnd *myMineSheetWnd;
  FormWnd *myMinesGrid;
  PtrList<BaseWnd> myMinesGridCells;
  PtrList<BaseWnd> myMinesDeletedCells;
  PtrList<FormCell> myMinesFormCells;
  PtrList< MemBlock<int> > myMinesCellValues;
  MemBlock<int> myMinesSafeCells;
  SliderWnd *myMineVertSlider;
  SliderWnd *myMineHorizSlider;
  SliderWnd *myMineMinesSlider;
  TextBar *myMineTitleBar;
  TextBar *myMineVertDisplay;
  TextBar *myMineHorizDisplay;
  TextBar *myMineMinesDisplay;
  int vertGrid;
  int horizGrid;
  int numMines;
  int gameInProgress;

  // =========================================================================
  //
  //  Data values used primarily by ExampleCodeGUIChecklistWnd.cpp
  //
  TextBar *myChecklistText;
  RadioGroup *myBakeryGroup;
  RadioGroup *myDairyGroup;

  // =========================================================================
  //
  //  Data values used primarily by ExampleCodeGUIDroplistWnd.cpp
  //
	DropList *myDropList;
	ListWnd *myListWnd;
};

#endif _EXAMPLECODEGUIWND_H
