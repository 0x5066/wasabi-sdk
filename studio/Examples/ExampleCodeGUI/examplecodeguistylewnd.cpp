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
//      File:     ExampleCodeGUIStyleWnd.cpp
//
//!##   Purpose:  This source module contains the functions for the 
//!##             construction of a simple MineSweeper game and introduces
//!##             the StylePosWnd, FormWnd, and SliderWnd objects.
//
//      Requires: Please read ExampleCodeGUIFormWnd.cpp first.
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

// Always begin with including std.h
#include "../../bfc/std.h"

#include "examplecodeguiwnd.h"

#include "../../common/styleposwnd.h"
#include "../../common/textbar.h"
#include "../../common/xlatstr.h"
#include "../../common/slider.h"
#include "../../common/formwnd.h"
#include "../../bfc/wnds/buttwnd.h"
#include "../../bfc/memblock.h"

// Somehow, it is important to derive formcell?
class MineFormCell : public FormCell {
  public:
    MineFormCell() {
      // Now I can instantiate!
    }
};

// What shall we name the games that we play?
const char *gameTab = "Llama";
const char *gameTitle = "Llama Hunt";
const char *gameStart = "Start";
const char *gameOver = "Game Over";
const char *gameWin = "You Win!";

// What kinda stuff are we gonna have in this tabsheet?

// 1) Inside of a StylePosWnd that is the size of the client area of the sheet,
//    A) Place a title across the top
//    B) Place a "Start" button in the bottom left hand corner.
//    C) Place some sliders along the bottom and right hand edges.
//    D) Place some text boxes to display the cooked values for the sliders.
//    E) Place a formwnd in the center to hold the game.
//        i) Each cell of the formwnd will hold a blank button
//       ii) When the button is pressed, it will be replaced by a textbar.


// Everything on this tabsheet starts here.
int ExampleCodeGUIWnd::createMinesweeperSheet() {
  // First thing we need is the StylePosWnd to act as the bag for all our toys.
  myMineSheetWnd = new StylePosWnd;
  myMineSheetWnd->setName(gameTab);     // Remember, the name of the object is the
                                        // name on the tab in the stabsheet buttonbar.

  // Hmmm, we need a powerful yet catchy title for our window....
  myMineTitleBar = new TextBar;
  myMineTitleBar->setDrawBox(1);
  myMineTitleBar->setCenter(1);
  myMineTitleBar->setName(gameTitle);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineTitleBar,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
  // "Set your left edge 5 pixels to the right of the left border,
  //  set your top edge 5 pixels below the top border,
  //  set your right edge 5 pixels to the left of the right border,
  //  set your bottom edge to create a 25 pixel high object."

  // In the bottom left hand corner, we place our start button
  ButtonWnd *start = new ButtonWnd;
  start->setButtonText(gameStart, 16);
  start->setNotifyId(EXB_MINEGAMESTART);
  start->setNotifyWindow(this);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(start,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),        // Right
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER)));  // Bottom
  // "Set your left edge 5 pixels to the left of the left border,
  //  set your top edge to create a 25 pixel high object,
  //  set your right edge to create a 60 pixel wide object,
  //  set your bottom edge 5 pixels above the bottom border."

  // And we'll put our sliders along the sides of the bottom-right hand corner.
  myMineVertSlider = new SliderWnd;
  myMineVertSlider->setOrientation(1);  // Vertical is 1
  myMineVertSlider->setPosition(SliderWnd::START);
  myMineVertSlider->setNotifyId(EXB_MINEVERTSLIDER);
  myMineVertSlider->setNotifyWindow(this);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineVertSlider,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_SLIDERWD, StylePosWnd::LOOSE),       // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_SLIDERLN, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_THINEDGE, StylePosWnd::LOCK_BORDER),// Right
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_CORNER, StylePosWnd::LOCK_BORDER)));// Bottom
  // "Set your left edge to create a 15 pixel wide object,
  //  set your top edge to create a 120 pixel high object,
  //  set your right edge 10 pixels to the left of the right border,
  //  set your bottom edge 25 pixels above the bottom border."
  
  // And we'll put our sliders in the bottom-right hand corner (didn't I just say that?)
  myMineHorizSlider = new SliderWnd;
  myMineHorizSlider->setOrientation(0);  // Horizontal is 0
  myMineHorizSlider->setNotifyId(EXB_MINEHORIZSLIDER);
  myMineHorizSlider->setNotifyWindow(this);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineHorizSlider,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_SLIDERLN, StylePosWnd::LOOSE),       // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_SLIDERWD, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_CORNER, StylePosWnd::LOCK_BORDER),  // Right
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_THINEDGE, StylePosWnd::LOCK_BORDER)));// Bottom
  // "Set your left edge to create a 120 pixel wide object,
  //  set your top edge to create a 15 pixel high object,
  //  set your right edge 25 pixels to the left of the right border,
  //  set your bottom edge 10 pixels above the bottom border."

  // We'll also have some textbars to display what the sliders values are
  // in terms specific to our little application....
  myMineVertDisplay = new TextBar;
  myMineVertDisplay->setDrawBox(1);
  myMineVertDisplay->setCenter(1);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineVertDisplay,
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Left
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
    StylePosWnd::ChildEdgeInfo(-EXB_BUTTON_SPACER, StylePosWnd::LOCK_TOP, myMineVertSlider)));// Bottom
  // "Set your left edge to create a 25 pixel wide object,
  //  set your top edge to create a 25 pixel high object,
  //  set your right edge 5 pixels to the left of the right border,
  //  set your bottom edge 3 pixels above the vertical slider."

  // We'll also have some textbars to display what the sliders values are
  // in terms specific to our little application (I swear, there's an echo in here!)
  myMineHorizDisplay = new TextBar;
  myMineHorizDisplay->setDrawBox(1);
  myMineHorizDisplay->setCenter(1);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineHorizDisplay,
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Left
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(-EXB_BUTTON_SPACER, StylePosWnd::LOCK_LEFT, myMineHorizSlider),    // Right
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER)));  // Bottom
  // "Set your left edge to create a 25 pixel wide object,
  //  set your top edge to create a 25 pixel high object,
  //  set your right edge 3 pixels to the left of the horizontal slider,
  //  set your bottom edge 5 pixels above the bottom border."


  //
  //  This slider controls what percentage of cells will have mines in them.
  //  But the display shows the actual number of mines to place.
  //
  myMineMinesSlider = new SliderWnd;
  myMineMinesSlider->setOrientation(1);  // Vertical is 1
  myMineMinesSlider->setNotifyId(EXB_MINEMINESSLIDER);
  myMineMinesSlider->setPosition(SliderWnd::START);
  myMineMinesSlider->setNotifyWindow(this);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineMinesSlider,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_SLIDERWD, StylePosWnd::LOOSE),       // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_SLIDERLN, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_THINEDGE, StylePosWnd::LOCK_BORDER),// Right
    StylePosWnd::ChildEdgeInfo(-EXB_TOP_OFFSET, StylePosWnd::LOCK_TOP, myMineVertDisplay)));// Bottom
  // "Set your left edge to create a 15 pixel wide object,
  //  set your top edge to create a 120 pixel high object,
  //  set your right edge 10 pixels to the left of the right border,
  //  set your bottom edge 10 pixels above the vertical display object."
  
  myMineMinesDisplay = new TextBar;
  myMineMinesDisplay->setDrawBox(1);
  myMineMinesDisplay->setCenter(1);
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMineMinesDisplay,
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Left
    StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
    StylePosWnd::ChildEdgeInfo(-EXB_BUTTON_SPACER, StylePosWnd::LOCK_TOP, myMineMinesSlider)));// Bottom
  // "Set your left edge to create a 25 pixel wide object,
  //  set your top edge to create a 25 pixel high object,
  //  set your right edge 5 pixels to the left of the right border,
  //  set your bottom edge 3 pixels above the mines slider."

  // Setup the display values.
  displayVert(SliderWnd::START);
  displayHoriz(SliderWnd::START);

  // Initialize the FormWnd pointer to NULL
  myMinesGrid = NULL;
  // And create a game.
  newGame();  // Better do this AFTER we setup the display values, eh?

  // And then all the way down at the bottom, 
  // we send the window in as the new tabsheet.
  addChild(myMineSheetWnd, _("Hey look, tooltips work!"));

  return 1;
}

void ExampleCodeGUIWnd::newGame() {
  // Set everything up, and say that we did so.
  createMinesGrid();
  setupValues();
  gameInProgress = 1;
  myMineTitleBar->setName(gameTitle);
}

//
//  *** The UI for the cells in the game are handled by using button objects.
//  However, because we want to track the right mouse clicks, we have to
//  purposefully create a special subclass to not allow the engine to place
//  the main context window on top of our object.
class NoContextButtonWnd : public ButtonWnd {
  virtual int wantAutoContextMenu() { return 0; }
};

void ExampleCodeGUIWnd::createMinesGrid() {
  // If we already have a created grid,
  if (myMinesGrid != NULL) {
    // Delete all the childen,
    myMinesGridCells.deleteAll();
    // Forget about all the formcells,
    myMinesFormCells.removeAll();
    // And only then, delete it.
    myMineSheetWnd->deleteChild(myMinesGrid);
  }

  // Then we build
  myMinesGrid = new FormWnd;

  // And position ourselves
  myMineSheetWnd->addChild(StylePosWnd::ChildInfo(myMinesGrid,
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
    StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BOTTOM, myMineTitleBar),// Top
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_LEFT, myMineVertDisplay),// Right
    StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_TOP, myMineHorizDisplay)));// Bottom
  // "Set your left edge 5 pixels to the right of the left edge,
  //  set your top edge 5 pixels below the title bar,
  //  set your right edge 5 pixels to the left of the vertical display bar,
  //  set your bottom edge 5 pixels above the horizontal display bar."

  // And plan for our childrens' futures.
  int i,totalChildren = vertGrid * horizGrid;
  double propVert = 100.0 / (double)vertGrid;  // determine the proportional percentages to divide 
  double propHoriz = 100.0 / (double)horizGrid;// the form window into the proper number of cells

  for (i = 0; i < totalChildren; i++) {
    // Here's the new baby!
    FormCell *newCell = new MineFormCell;

    // See how big he is!
    newCell->setHeightProp(propVert);
    newCell->setWidthProp(propHoriz);

    // And here's the big spike we stuff into the baby's skull!
    ButtonWnd *cellCover = new NoContextButtonWnd;
    cellCover->setNotifyId(EXB_MINECELLBUTTON + i);
    cellCover->setNotifyWindow(this);

    // Insert object A into cell B
    newCell->setRootWndContent(cellCover);

    // Insert cell B into form C
    myMinesGrid->newCell(newCell);

    // And record the new big spike with the county registrar
    myMinesFormCells.addItem(newCell);
    myMinesGridCells.addItem(cellCover);

    // At the end of a row, tell the formwnd we're at the end of a row.
    if ((i % horizGrid) == (horizGrid - 1)) {
      myMinesGrid->nextRow();
    }

    // This was much easier with prop sizing based on floating point math.
  }

  // After all is said and done, do your initcheck on the formwnd.
  // When it is initialized, all of its children will be, as well.
  if (myMineSheetWnd->isInited()) {
    myMinesGrid->init(myMineSheetWnd);
  }
    
  // Yah, maybe someday we'll actually track where the mines and shit are.
}

// In raw slider units.
void ExampleCodeGUIWnd::displayVert(int rangeval) {
  // Calculate the value for the slider percentage between the min and max values.
  vertGrid = ((EXB_MINE_MAXGRID - EXB_MINE_MINGRID) * rangeval) / SliderWnd::END;
  vertGrid += EXB_MINE_MINGRID;
  myMineVertDisplay->setName(StringPrintf("%d",vertGrid));
  // The number of mines value changes based on the grid size.
  calcMines();
}

// In raw slider units.
void ExampleCodeGUIWnd::displayHoriz(int rangeval) {
  // Calculate the value for the slider percentage between the min and max values.
  horizGrid = ((EXB_MINE_MAXGRID - EXB_MINE_MINGRID) * rangeval) / SliderWnd::END;
  horizGrid += EXB_MINE_MINGRID;
  myMineHorizDisplay->setName(StringPrintf("%d",horizGrid));
  // The number of mines value changes based on the grid size.
  calcMines();
}

void ExampleCodeGUIWnd::calcMines() {
  // You can also find out about the position of the slider
  // via the getSliderPosition method.
  int rangeval = myMineMinesSlider->getSliderPosition();

  // Then we calculate the percentage of mines from the sliderpos
  int minesPct = ((EXB_MINE_MAXMINES - EXB_MINE_MINMINES) * rangeval) / SliderWnd::END;
  minesPct += EXB_MINE_MINMINES;

  // Then we multiply that through the number of cells in the
  // formwnd to get the unit number of mines to be placed.
  numMines = (horizGrid * vertGrid * minesPct) / 100;

  // And lastly we set the display
  myMineMinesDisplay->setName(StringPrintf("%d",numMines));
}

void ExampleCodeGUIWnd::cellHit(int buttonid) { // The _real_ button ID.
  // Don't click if game over
  if (gameInProgress) {
    // Get the linear index.
    int cellIdx = buttonid - EXB_MINECELLBUTTON;

    // If it is safe to click this button,
    if (myMinesSafeCells[cellIdx] == 0) {
      // Get the pointer for that cellbutton.
      BaseWnd *cellCover = myMinesGridCells[cellIdx];

      // Get the pointer for that formcell.
      FormCell *theCell = myMinesFormCells[cellIdx];

      // Get the new thing to schtikk in there
      TextBar *cellDisplay = new TextBar;
      cellDisplay->setDrawBox(1);
      cellDisplay->setCenter(1);
      int cellValue = getICell(cellIdx);
      if (cellValue == -1) {
        // OOOOOOOOPS!!!!!
        cellDisplay->setName("mine!");
        // Game over, man.... game over!@
        gameInProgress = 0;
        // You said it.
        myMineTitleBar->setName(gameOver);
        myMineTitleBar->invalidate();
      } else if (cellValue > 0) {
        cellDisplay->setInt(cellValue);
      }

      // Set the safe value of this cell to "viewed"
      myMinesSafeCells[cellIdx] = -1;

      // And then test to see if we win!@
      testWin();

      // Kill off the old
      cellCover->setVisible(FALSE);

      // Bring in the new
      myMinesGridCells.setItem(cellDisplay, cellIdx);
      theCell->setRootWndContent(cellDisplay);

      // And if we're lucky, the init call we have to make might actually work.
      theCell->init(myMinesGrid);

      // And the last thing we do is tell the formwnd to resize everyone.
      myMinesGrid->onResize();
    }
  }
}

void ExampleCodeGUIWnd::setSafe(int buttonid) { // The _real_ button ID.
  // Don't click if game over
  if (gameInProgress) {
    // Get the linear index.
    int cellIdx = buttonid - EXB_MINECELLBUTTON;

    // Get the pointer for that cellbutton.
    ButtonWnd *cellButton = static_cast<ButtonWnd*>(myMinesGridCells[cellIdx]);

    // Increment us to the next value state
    int newSafeVal = (myMinesSafeCells[cellIdx] + 1) % 3;

    // Set the button to display the icon for that state.
    switch (newSafeVal) {
      case 0:
        cellButton->setButtonText("", 16);
      break;
      case 1:
        cellButton->setButtonText("X", 16);
      break;
      case 2:
        cellButton->setButtonText("?", 16);
      break;
    }

    // Make the button update its picture.
    cellButton->invalidate();

    // And then set the new value into the variable.
    myMinesSafeCells[cellIdx] = newSafeVal;
  }
}

void ExampleCodeGUIWnd::testWin() {
  if (gameInProgress) {
    int i, total = 0, totalChildren = vertGrid * horizGrid;
    // Calculate the total number of unviewed cells.
    for (i = 0; i < totalChildren; i++ ) {
      total += (myMinesSafeCells[i] != -1);
    }
    // If that's equal to the number of mines,
    if (total == numMines) {
      // You win.
      gameInProgress = 0;
      myMineTitleBar->setName(gameWin);
      myMineTitleBar->invalidate();
    }
  }
}

void ExampleCodeGUIWnd::setupValues() {
  // Flush whatever is there.
  myMinesCellValues.deleteAll();

  // For every row, create a MemBlock
  int i, j, totalChildren = vertGrid * horizGrid;
  for (i = 0; i < horizGrid; i++) {
    MemBlock<int> *row = new MemBlock<int>;
    // And create one entry per column.
    row->setSize(vertGrid);
    // For every cell, set it to 0
    for (j = 0; j < vertGrid; j++) {
      row->getMemory()[j] = 0;     
    }
    // Add him to our list.
    myMinesCellValues.addItem(row);
  }

  // Place your mines.
  for (i = 0; i < numMines; i++) {
    int x,y;
    // Find an empty cell.
    do {
      x = Std::random() % horizGrid;
      y = Std::random() % vertGrid;
    } while (getXYCell(x, y) == -1);
    // Place the mine.
    setXYCell(x, y, -1);
  }

  // Then scan through the grid and calculate how many mines
  // are adjacent to each cell in 8 directions.
  for (i = 0; i < horizGrid; i++) {
    for (j = 0; j < vertGrid; j++) {
      // Because I forget silly things like this, I will remind myself...
      // i == x    j == y
      if (getXYCell(i, j) != -1) {
        int total = 0;
        // Now we will read up to 8 columns.
        if (i > 0) {
          if (j > 0) {
            total += (getXYCell(i-1, j-1) == -1);
          }
            total += (getXYCell(i-1, j-0) == -1);
          if (j < (vertGrid-1)) {
            total += (getXYCell(i-1, j+1) == -1);
          }
        }
        {
          if (j > 0) {
            total += (getXYCell(i-0, j-1) == -1);
          }
          if (j < (vertGrid-1)) {
            total += (getXYCell(i-0, j+1) == -1);
          }
        }
        if (i < (horizGrid-1)) {
          if (j > 0) {
            total += (getXYCell(i+1, j-1) == -1);
          }
            total += (getXYCell(i+1, j-0) == -1);
          if (j < (vertGrid-1)) {
            total += (getXYCell(i+1, j+1) == -1);
          }
        }
        // You are getting very sleepy......

        // Woo hoo!  now we can record our total!
        setXYCell(i, j, total);
      }
    }
  }

  myMinesSafeCells.setSize(totalChildren);
  for (i = 0; i < totalChildren; i++ ) {
    myMinesSafeCells[i] = 0;
  }
  // Et, voila!@
}

void ExampleCodeGUIWnd::setXYCell(int x, int y, int val) {
  myMinesCellValues[x]->getMemory()[y] = val;
}

int ExampleCodeGUIWnd::getXYCell(int x, int y) {
  return myMinesCellValues[x]->getMemory()[y];
}

void ExampleCodeGUIWnd::setICell(int linear_index, int val) {
  setXYCell(linear_index % horizGrid, linear_index / horizGrid /* vertGrid :) */, val);
}

int ExampleCodeGUIWnd::getICell(int linear_index) {
  return getXYCell(linear_index % horizGrid, linear_index / horizGrid /* vertGrid :) */);
}


#if 0	// Reference Stuff
// ===========================================================================
//
//	EXAMPLECODEGUI: Reference Stuff
//
//    This section should (eventually) provide the reference materials for
//  the following SDK Objects:
//
//      StylePosWnd
//      SliderWnd
//      FormWnd
//      FormCell
//
//    As always, if you have any specific questions about these objects,
//  please do feel free to also check the NSDN site, as well as the wasabi
//  developers' forum.



// ===========================================================================
//
//	EXAMPLECODEGUI: StylePosWnd Reference
//
//  StylePosWnd is inherited from LabelWnd
//


  // --------------------------------------------
  // Public Enumerations
  
  typedef enum {
    LOOSE,        // scalar value is size along axis. only one opposing edge may be LOOSE.
    FIXED,        // scalar value is fixed point in client space
    LOCK_BORDER,  // scalar value is added to border (ie: right hand, use neg values)
    CENTER,       // scalar value is size along axis.  both opposing edges must be CENTER to be valid.
    LOCK_LEFT,    // scalar value is added to the pos.left of the given BaseWnd object
    LOCK_TOP,     // scalar value is added to the pos.top of the given BaseWnd object
    LOCK_RIGHT,   // scalar value is added to the pos.right of the given BaseWnd object
    LOCK_BOTTOM,  // scalar value is added to the pos.bottom of the given BaseWnd object
    //
    NUM_CHILD_EDGE_STYLES
  } ChildEdgeStyle;
  // These different styles are used as parameters to the construction 
  // of the ChildEdgeInfo object in the addChild method.

  // Note how each style does something different with the scalar value.


  // --------------------------------------------
  // Public Subclasses - ChildEdgeInfo

  class ChildEdgeInfo {
  public:
    ChildEdgeInfo(int theScalar = 0, ChildEdgeStyle theStyle = FIXED, BaseWnd *theWho = NULL);
  };
  // This object holds the data to describe what to do with one edge of a rectangle.
  // It is a simple construct-and-carry container object.

  // The scalar parameter will have different meanings depending upon the ChildEdgeStyle
  // bound to it.  The default, FIXED, acts the same as a normal object addition.  Some
  // styles expect a BaseWnd pointer to get the position for this object-edge from the 
  // an edge of the given BaseWnd obejct.

  // *** IMPORTANT NOTE: Obviously, if you are referring to objects that are also going
  // to be contained in the StylePosWnd, you _MUST_ add those objects to the StylePosWnd
  // first.  Otherwise, your objects will perform unexpected tricks of positioning,
  // right before your wondering eyes.


  // --------------------------------------------
  // Public Subclasses - ChildInfo

  class ChildInfo {
  public:
    ChildInfo( BaseWnd *theChild, const ChildEdgeInfo &left, const ChildEdgeInfo &top,
      const ChildEdgeInfo &right, const ChildEdgeInfo &bottom, int doinvalidate_on_resize=1);
  };
  // Four perpendicular edges make a rectangle.
  // Four ChildEdgeInfo objects bound to a payload pointer make a ChildInfo object.
  // Again, this is just a simple construct-and-carry object for the StylePosWnd object.
  
  
  // --------------------------------------------
  // Public Methods

  StylePosWnd();
  virtual ~StylePosWnd();
  // Nothing special with constructors or destructors.
  // Destructor will delete all child items.  removeAll() if you don't like that.

  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  virtual int onResize();
  // These methods ensure that all the child objects are obeying the laws
  // set when they are added to this object.

  virtual int addChild(const ChildInfo &info);
  // A ChildInfo object should be constructed inline as the parameter to this object,
  // with 4 ChildEdgeInfo objects constructed inline as the parameters to the ChildInfo
  // object.

  int deleteChild(BaseWnd *child);
  int deleteAll();
  int removeChild(BaseWnd *child);  // Will remove a child without deleting it
  int removeAll();                  // Will remove all without deleting them.
  // Of course, you might want to actually change the contents of your user interface
  // on the fly, depending upon what you want to do, so you get these functions, too.


  // --------------------------------------------
  // Protected Methods

  virtual void updatePositions();
  // This is where most of the work gets done (and where most of the bugs will be found
  // if there are any left).  This properly repositions and resizes the child items based
  // upon the information specified upon addition.

  ChildInfo *findChild(BaseWnd *child);
  inline int numChildren() { return childinfo.getNumItems(); }
  inline ChildInfo *enumChildren(int child) { return childinfo[child]; }
  // This code is just helper code to the rest of the object and
  // isn't terribly important or interesting, so GET YOUR SLIMY HANDS OFF IT!



// ===========================================================================
//
//	EXAMPLECODEGUI: SliderWnd Reference
//
//  SliderWnd is inherited from VirtualWnd
//


  // --------------------------------------------
  // Public Enumerations

  enum {
    START = 0,
    END = 65535,
    FULL = END
  };
  // Some constants to describe the position of the thumb.


  // --------------------------------------------
  // Public Constructors

  SliderWnd();
  virtual ~SliderWnd();

  // --------------------------------------------
  // Public Methods

  virtual int onPaint(Canvas *canvas);
  virtual int onLeftButtonDown(int x, int y);
  virtual int onMouseMove(int x, int y); // only called when mouse captured
  virtual int onLeftButtonUp(int x, int y);
  virtual void onEnterArea();
  virtual void onLeaveArea();
  // These methods implement the functionality of a slider.  If you mess with them,
  // you'd best know what you're doing.

  virtual void setAlpha(int alpha);
  virtual int getAlpha(void);
  // These methods get and set the alpha rendering value for this object.

  virtual void setPosition(int newpos);
  int getSliderPosition();	// ranges from START .. END
  // These methods get and set the position of the thumb, in thumb coordinates.

  void setUseBaseTexture(int useit);
  void setBaseTexture(SkinBitmap *bmp, int x, int y);
  // This allows you to optionally set and use a base texture for your slider

  void setDrawOnBorders(int draw);
  // Include the edge borders as part of your drawing area.

  void setBitmaps(const char *thumbbmp, const char *thumbdownbmp, const char *thumbhighbmp, const char *leftbmp, const char *middlebmp, const char *rightbmp);
  void setLeftBmp(const char *name);
  void setMiddleBmp(const char *name);
  void setRightBmp(const char *name);
  void setThumbBmp(const char *name);
  void setThumbDownBmp(const char *name);
  void setThumbHiliteBmp(const char *name);
  SkinBitmap *getLeftBitmap();
  SkinBitmap *getRightBitmap();
  SkinBitmap *getMiddleBitmap();
  SkinBitmap *getThumbBitmap();
  SkinBitmap *getThumbDownBitmap();
  SkinBitmap *getThumbHiliteBitmap();
  // These set & get the 6 possible bitmaps used to paint a working slider object

  virtual int getHeight();
  virtual int getWidth();
  // Wanna guess what THESE do?  Actually, be very careful with these.
  // Only ONE of these functions is valid at any one time, based upon the state
  // of the vertical variable.  If the slider is vertical, getHeight is functional.
  // If the slider is horizontal, getWidth is functional.  If you call a method
  // when you shouldn't, you'll get an assertion.

  virtual void setEnable(int en);
  virtual int getEnable(void);
  // Methods to set and get whether or not the object should be enabled.

  virtual void setOrientation(int o); // 0=horizontal, 1=vertical
  // Do you want to be a vertical or a horizontal slider?

  virtual void setHotPosition(int h); // -1 to disable
  // This will set a "jump-to" position (like "center" for a balance slider).
  // The parameter is in thumb coordinates.


  // --------------------------------------------
  // Protected Methods

  virtual int onSetPosition();		// called constantly as mouse moves
  virtual int onSetFinalPosition();	// called once after move done
  // override these to handle position change notification in a subclass 
  // (or just call setNotifyWindow() on a SliderWnd to use notification messages)

  int getSeekStatus();	
  // returns 1 if user is sliding tab

  int thumbWidth();
  int thumbHeight();
  // the width or height of the thumb graphic (or default placeholder).


// ===========================================================================
//
//	EXAMPLECODEGUI: FormWnd Reference
//
//  FormWnd is inherited from LabelWnd
//
//  Its functionality is very similar to that of HTML tables.
//


  // --------------------------------------------
  // Public Methods

  FormWnd();
  virtual ~FormWnd();
  // Normal constructors and destructors.

  virtual int onInit();
  virtual int onResize();
  // Functionality implementation

  FormCell *newCell(FormCell *cell=NULL);
  // Pass a new cell into the form.  The cell understands its own sizing.

  void nextRow();
  // Make this call when you're done adding cells to the first horizontal row
  // and are ready to go to the next row value.

  void deleteAll();
  // Hmmm.... I give up!

  void recalcForm();
  // Forcibly recalculate the form sizes, etc.



// ===========================================================================
//
//	EXAMPLECODEGUI: FormCell Reference
//
//  FormCell is not inherited from anything
//


  // --------------------------------------------
  // Protected Constructor

  FormCell();
  // You are required to subclass this object to use it.


  // --------------------------------------------
  // Public Methods
  
  virtual ~FormCell();
  // Friendly Neighborhood Destructor

  void setWidthFixed(int w);
  void setHeightFixed(int h);
  void setWidthProp(double w);
  void setHeightProp(double h);
  // You may choose to set the dimensions of your cells to be a fixed pixel value
  // or a proportional percentage value (enumerated from 0-100 in percentiles).

  void setRootWndContent(RootWnd *rootwnd);
  // This is the payload object to display in the cell area.  This is the only
  // public method available to set the content of the cell area.

  virtual void init(FormWnd *parent);
  // Force initialization of the object to its parent window.  You'll need to
  // call this if you call setRootWndContent after the FormWnd is initialized.


  // --------------------------------------------
  // Protected Methods

  void resize(int x, int y, int w, int h);
  // This is called when the FormWnd is recalculating.

  void setContent(BaseWnd *basewnd);
  // This is a protected method, you can't call it.  This is for
  // if you subclass your own cell types.  See cells.h


#endif // Reference Stuff