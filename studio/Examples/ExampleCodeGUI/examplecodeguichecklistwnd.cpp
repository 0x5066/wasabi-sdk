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
//      File:     ExampleCodeGUIChecklistWnd.cpp
//
//!##   Purpose:  This source module contains the functions for the 
//!##             construction of Lists, Checkboxes, and RadioGroups.
//
//      Requires: Please read ExampleCodeGUIWnd.cpp first.
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

#include "../../common/framewnd.h"
#include "../../common/styleposwnd.h"
#include "../../common/radiownd.h"
#include "../../common/radiogroup.h"
#include "../../common/textbar.h"
#include "../../common/xlatstr.h"

#include "../../bfc/notifmsg.h"

// ===========================================================================
//
//    *** Module Syllabus
//
//
//    Step 1:  Come up with a lame idea for something to use checks & radios

// ===========================================================================
//
//    *** Static Globals
//
const char *gayTitle = "Winamp3 can help your shopping!";
#define lazy(s) s##Str
#define attr(s) s##Attr
#define lazyDefine(s) const char *lazy(s) = #s
lazyDefine(bakery);
lazyDefine(bread);
lazyDefine(cake);
lazyDefine(donuts);
lazyDefine(dairy);
lazyDefine(milk);
lazyDefine(cheese);
lazyDefine(butter);

// ===========================================================================
//
//    *** ExampleCodeGUIWnd::createChecklistSheet()
//
//  This is the method in which we create the tabsheet for this module.  It is
//  our primary point of entry for instantiation and initialization.  Aren't 
//  you thrilled?
int ExampleCodeGUIWnd::createChecklistSheet() {
  // *** Here is where we create our user interface object hierarchy:

    StylePosWnd *container = new StylePosWnd;
    container->setName(_("Checklist"));

    TextBar *myTitleBar = new TextBar;
    myTitleBar->setDrawBox(1);
    myTitleBar->setCenter(1);
    myTitleBar->setName(gayTitle);
    container->addChild(StylePosWnd::ChildInfo(myTitleBar,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Top
      StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 5 pixels below the top border,
    //  set your right edge 5 pixels to the left of the right border,
    //  set your bottom edge to create a 25 pixel tall textbar."

    TextBar *centerpoint = new TextBar;
    centerpoint->setName("");
    // By doing this, we can setup 2 columns or have even-numbered multicentering.
    container->addChild(StylePosWnd::ChildInfo(centerpoint,
      StylePosWnd::ChildEdgeInfo(0, StylePosWnd::CENTER),     // Left
      StylePosWnd::ChildEdgeInfo(0, StylePosWnd::CENTER),     // Top
      StylePosWnd::ChildEdgeInfo(0, StylePosWnd::CENTER),     // Right
      StylePosWnd::ChildEdgeInfo(0, StylePosWnd::CENTER)));   // Bottom
    // "Place a zero sized object right in the middle of the client area."

    CheckWnd *bakery = new CheckWnd;
    bakery->setNotifyId(EXB_CHECKBAKERY);
    bakery->setNotifyWindow(this);
    container->addChild(StylePosWnd::ChildInfo(bakery,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOCK_BOTTOM, myTitleBar),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *bakeryText = new TextBar;
    bakeryText->setName("Bakery");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    bakeryText->setAutoToggleCheckWnd(bakery);
    container->addChild(StylePosWnd::ChildInfo(bakeryText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, bakery),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, bakery),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    CheckWnd *dairy = new CheckWnd;
    dairy->setNotifyId(EXB_CHECKDAIRY);
    dairy->setNotifyWindow(this);
    container->addChild(StylePosWnd::ChildInfo(dairy,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, centerpoint),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOCK_BOTTOM, myTitleBar),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the centerpoint,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *dairyText = new TextBar;
    dairyText->setName("Dairy");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    dairyText->setAutoToggleCheckWnd(dairy);
    container->addChild(StylePosWnd::ChildInfo(dairyText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, dairy),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, dairy),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    RadioWnd *bread = new RadioWnd;
    bread->setNotifyId(EXB_CHECKBREAD);
    // Don't set notify window -- do that on the RadioGroup
    container->addChild(StylePosWnd::ChildInfo(bread,
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BOTTOM, bakery),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *breadText = new TextBar;
    breadText->setName("Bread");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    breadText->setAutoToggleCheckWnd(bread);
    container->addChild(StylePosWnd::ChildInfo(breadText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, bread),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, bread),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    RadioWnd *cake = new RadioWnd;
    cake->setNotifyId(EXB_CHECKCAKE);
    // Don't set notify window -- do that on the RadioGroup
    container->addChild(StylePosWnd::ChildInfo(cake,
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BOTTOM, bread),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *cakeText = new TextBar;
    cakeText->setName("Cake");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    cakeText->setAutoToggleCheckWnd(cake);
    container->addChild(StylePosWnd::ChildInfo(cakeText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, cake),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, cake),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    RadioWnd *donuts = new RadioWnd;
    donuts->setNotifyId(EXB_CHECKDONUTS);
    // Don't set notify window -- do that on the RadioGroup
    container->addChild(StylePosWnd::ChildInfo(donuts,
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BOTTOM, cake),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *donutsText = new TextBar;
    donutsText->setName("Donuts");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    donutsText->setAutoToggleCheckWnd(donuts);
    container->addChild(StylePosWnd::ChildInfo(donutsText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, donuts),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, donuts),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    // Now we'll make a radio group to "virtually contain" them to provide
    // radio button functionality over all CheckWnd derived objects added to it.
    myBakeryGroup = new RadioGroup();
    myBakeryGroup->addChild(bread);
    myBakeryGroup->addChild(cake);
    myBakeryGroup->addChild(donuts);
    myBakeryGroup->setNotifyWindow(this);
    container->addChild(StylePosWnd::ChildInfo(myBakeryGroup,
      StylePosWnd::ChildEdgeInfo(),StylePosWnd::ChildEdgeInfo(),
      StylePosWnd::ChildEdgeInfo(),StylePosWnd::ChildEdgeInfo()));
    // "Set as an invisible object."
    // (This is done simply so we don't have to worry about deleting the pointer,
    // since RadioGroup objects never have a functional onPaint method.)
    
    RadioWnd *milk = new RadioWnd;
    milk->setNotifyId(EXB_CHECKMILK);
    // Don't set notify window -- do that on the RadioGroup
    container->addChild(StylePosWnd::ChildInfo(milk,
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_RIGHT, centerpoint),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BOTTOM, dairy),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *milkText = new TextBar;
    milkText->setName("Milk");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    milkText->setAutoToggleCheckWnd(milk);
    container->addChild(StylePosWnd::ChildInfo(milkText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, milk),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, milk),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    RadioWnd *cheese = new RadioWnd;
    cheese->setNotifyId(EXB_CHECKCHEESE);
    // Don't set notify window -- do that on the RadioGroup
    container->addChild(StylePosWnd::ChildInfo(cheese,
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_RIGHT, centerpoint),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BOTTOM, milk),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *cheeseText = new TextBar;
    cheeseText->setName("Cheese");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    cheeseText->setAutoToggleCheckWnd(cheese);
    container->addChild(StylePosWnd::ChildInfo(cheeseText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, cheese),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, cheese),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    RadioWnd *butter = new RadioWnd;
    butter->setNotifyId(EXB_CHECKBUTTER);
    // Don't set notify window -- do that on the RadioGroup
    container->addChild(StylePosWnd::ChildInfo(butter,
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_RIGHT, centerpoint),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOCK_BOTTOM, cheese),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_SIZE, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 25 pixels below the titlebar,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *butterText = new TextBar;
    butterText->setName("Butter");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    butterText->setAutoToggleCheckWnd(butter);
    container->addChild(StylePosWnd::ChildInfo(butterText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_RIGHT, butter),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_CHECK_OFFSET, StylePosWnd::LOCK_TOP, butter),     // Top
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_WIDTH, StylePosWnd::LOOSE),    // Right
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    // Now we'll make a radio group to "virtually contain" them to provide
    // radio button functionality over all CheckWnd derived objects added to it.
    myDairyGroup = new RadioGroup();
    myDairyGroup->addChild(milk);
    myDairyGroup->addChild(cheese);
    myDairyGroup->addChild(butter);
    myDairyGroup->setNotifyWindow(this);
    container->addChild(StylePosWnd::ChildInfo(myDairyGroup,
      StylePosWnd::ChildEdgeInfo(),StylePosWnd::ChildEdgeInfo(),
      StylePosWnd::ChildEdgeInfo(),StylePosWnd::ChildEdgeInfo()));
    // "Set as an invisible object."
    // (This is done simply so we don't have to worry about deleting the pointer,
    // since RadioGroup objects never have a functional onPaint method.)

    myChecklistText = new TextBar();
    myChecklistText->setDrawBox(1);
    myChecklistText->setCenter(1);
    myChecklistText->setName("");
    container->addChild(StylePosWnd::ChildInfo(myChecklistText,
      StylePosWnd::ChildEdgeInfo(EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(EXB_BUTTON_HEIGHT, StylePosWnd::LOOSE),       // Top
      StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER),    // Right
      StylePosWnd::ChildEdgeInfo(-EXB_MINE_EDGE, StylePosWnd::LOCK_BORDER)));  // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge to create a 25 pixel tall textbar,
    //  set your right edge 5 pixels to the left of the right border,
    //  set your bottom edge 5 pixels above the bottom border."

    addChild(container, _("This contains checky-type shietznite."));

  // For the nonce, return codes from and to the API should
  //  return "0" for failure and "1" for success.
  return 1;
}

int ExampleCodeGUIWnd::handleChecklistNotify(int msg, int objId, int param1, int param2) {
  String text;
  switch (msg) {
    
    case ChildNotify::CHECKWND_CLICK: {
      int checkboxState = param1; // just a clarification assignment

      text = "Check Button! ";
      switch (objId) {
        case EXB_CHECKBAKERY:
          text += lazy(bakery);
        break;
        case EXB_CHECKDAIRY:
          text += lazy(dairy);
        break;
      }
      switch (checkboxState) {
        case 0:
          text += " is off!";
        break;
        case 1:
          text += " is on!";
        break;
      }
    }
    break;
    case ChildNotify::RADIOGROUP_STATECHANGE: {
      int objectTurnedOn = param1; // just a clarification assignment

      text = "Radio Button! on: ";
      switch (objectTurnedOn) {
        case EXB_CHECKBREAD:
          text += lazy(bread);
        break;
        case EXB_CHECKCAKE:
          text += lazy(cake);
        break;
        case EXB_CHECKDONUTS:
          text += lazy(donuts);
        break;
        case EXB_CHECKMILK:
          text += lazy(milk);
        break;
        case EXB_CHECKCHEESE:
          text += lazy(cheese);
        break;
        case EXB_CHECKBUTTER:
          text += lazy(butter);
        break;
        case -1:
          text += "(nobody)";
        break;
        default:
          text += "(I don't write bugs!)";
        break;
      }

      text += " off: ";

      int objectTurnedOff = param2; // just a clarification assignment
      switch (objectTurnedOff) {
        case EXB_CHECKBREAD:
          text += lazy(bread);
        break;
        case EXB_CHECKCAKE:
          text += lazy(cake);
        break;
        case EXB_CHECKDONUTS:
          text += lazy(donuts);
        break;
        case EXB_CHECKMILK:
          text += lazy(milk);
        break;
        case EXB_CHECKCHEESE:
          text += lazy(cheese);
        break;
        case EXB_CHECKBUTTER:
          text += lazy(butter);
        break;
        case -1:
          text += "(nobody)";
        break;
        default:
          text += "(I don't write bugs!)";
        break;
      }
    }
    break;
  }
  displayChecklistText(text);
  return 1;
}

int ExampleCodeGUIWnd::displayChecklistText(const char *text) {
  myChecklistText->setName(text);
  return 1;
}

#if 0	// Reference Stuff
// ===========================================================================
//
//	EXAMPLECODEGUI: Reference Stuff
//
//    This section should (eventually) provide the reference materials for
//  the following SDK Objects:
//
//    CheckWnd
//    RadioWnd
//    RadioGroup
//
//    As always, if you have any specific questions about these objects,
//  please do feel free to also check the NSDN site, as well as the wasabi
//  developers' forum.




// ===========================================================================
//
//	EXAMPLECODEGUI: CheckWnd Reference
//
//  CheckWnd is inherited from ClickWnd
//


  // --------------------------------------------
  // Public Methods

  BOOL isChecked() const;
  void setChecked(BOOL is);
  // Get and set the checked status of this box

  void toggle();
  // Change the status to the other status

  void setBitmaps(const char *unchecked, const char *checked);
  // Set the two bitmaps to display for different states


  // --------------------------------------------
  // Virtual Methods

  virtual int onPaint(Canvas *c);
  // How to paint

  virtual void onLeftPush(int x, int y);
  // What to do on a mousepush

  virtual int onClick() { return 1; }
  // override this to catch clicks



// ===========================================================================
//
//	EXAMPLECODEGUI: RadioWnd Reference
//
//  RadioWnd is inherited from CheckWnd
//


  // --------------------------------------------
  // Public Methods

  "HAH!  I FOOLED YOU!  There are no public methods on RadioWnd."
  "The only thing that RadioWnd does is use different default art than CheckWnd!"



// ===========================================================================
//
//	EXAMPLECODEGUI: RadioGroup Reference
//
//  RadioGroup is inherited from VirtualWnd
//
//  NOTE:  Even though you "addChild" to this object, you must still add that
//  CheckWnd to the primary display list.  This object ONLY tracks the event
//  functionality of the objects grouped under it.  It will not display them.

//  If you want to track the event notifications of the CheckWnd objects you are
//  using in a RadioGroup, don't call setNotifyWindow on the CheckWnd objects.
//  This object wants to call setNotifyWindow to point to itself for all contained
//  objects.  However, if you call setNotifyWindow on the RadioGroup, the RadioGroup
//  object will pass the event notification method call along to its notification
//  target.  But if you change the CheckWnd target, you won't have Radio functionality.

//  All that makes sense, right?


  // --------------------------------------------
  // Public Methods

  void addChild(CheckWnd *child);
  // Track a checkwnd as part of this group.  The order entered into
  // the group is their index order for the next methods.

  CheckWnd *enumChild(int i);
  // Get back the pointer at that index.

  void setSelectedItem(int item);
  int getSelectedItem();
  // Poll and place the state of the radio buttons as a group.


  // --------------------------------------------
  // Virtual Methods

  virtual int childNotify(RootWnd *which, int msg, int param1, int param2);
  // Catch the event notifications of the tracked CheckWnd objects, set
  // the checked state of the objects based upon the rules of a RadioGroup.

#endif// Reference Stuff
