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
//      File:     ExampleAttribWnd.cpp
//
//!##   Purpose:  This module sets up yet another lame user interface to show off Attribs
//
//      Requires: Please read ExampleAttribWnd.h first.
//
//      Notes:    A note on the comments in this document:
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.
//

// Always begin with including std.h
#include "../../bfc/std.h"

#include "ExampleAttrib.h"
#include "ExampleAttribwnd.h"
#include "ExampleAttribHandler.h"
#include "resource.h"

#include "../../bfc/notifmsg.h" 
#include "../../common/textbar.h"
#include "../../common/nsguid.h"
#include "../../common/xlatstr.h"

#include "../../common/droplist.h"
#include "../../common/checkwnd.h"
#include "../../common/textbar.h"
#include "../../common/slider.h"
#include "../../common/editwndstring.h"


// ===========================================================================
//
//  ExampleAttrib: "ExampleAttribWnd::ExampleAttribWnd"
//
ExampleAttribWnd::ExampleAttribWnd( void ) {
  int myRetval = createChildWindows();

  ASSERTPR(attr != NULL,"WHY is your attrib null?  did you allocate it in the component's constructor?");
  // In order to get the info back from the configuration editor, we have to tell
  // the attribute handler our window pointer so we'll get called with the info.
  attr->bindCallbackObj(this);

  // Titlebar for the window shall be?
  setName("ExampleAttribWnd");
  // what a boring name.
}

// ===========================================================================
//
//  ExampleAttrib: "ExampleAttribWnd::ExampleAttribWnd"
//
ExampleAttribWnd::~ExampleAttribWnd() {
  // If we're ever deleted, we don't want the attr handler to call us anymore.
  attr->bindCallbackObj(NULL);
}

// =========================================================================
//
//  Methods required by Window Creation Services
//
const char *ExampleAttribWnd::getWindowTypeName() { return "Example Attrib Window"; }
GUID ExampleAttribWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExampleAttribWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}



// ===========================================================================
//
//  ExampleAttrib: "ExampleAttribWnd::createChildWindows"
//
int ExampleAttribWnd::createChildWindows( void ) {

  // So we need to think of 4 good user interface items to hook to attribs.

  //
  // Int - Droplist
    myDropList = new DropList;
    myDropList->setNotifyId(INT);
    myDropList->setNotifyWindow(this);
    addChild(StylePosWnd::ChildInfo(myDropList,
      StylePosWnd::ChildEdgeInfo(5, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(5, StylePosWnd::LOCK_BORDER),     // Top
      StylePosWnd::ChildEdgeInfo(-5, StylePosWnd::LOCK_BORDER),    // Right
      StylePosWnd::ChildEdgeInfo(25, StylePosWnd::LOOSE)));        // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 5 pixels below the top border,
    //  set your right edge 5 pixels to the left of the right border,
    //  set your bottom edge to create a 25 pixel tall bar."

  //
  // Bool - Checkbox
    myCheckWnd = new CheckWnd;
    myCheckWnd->setNotifyId(BOOL);
    myCheckWnd->setNotifyWindow(this);
    myCheckWnd->setChecked(attr->boolAttr); // Set our preliminary value from the attrib
    addChild(StylePosWnd::ChildInfo(myCheckWnd,
      StylePosWnd::ChildEdgeInfo(5, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(10, StylePosWnd::LOCK_BOTTOM, myDropList),// Top
      StylePosWnd::ChildEdgeInfo(15, StylePosWnd::LOOSE),          // Right
      StylePosWnd::ChildEdgeInfo(15, StylePosWnd::LOOSE)));        // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge 10 pixels below the droplist,
    //  set your right edge to create a 15 pixel wide button,
    //  set your bottom edge to create a 15 pixel tall button."

    TextBar *text = new TextBar;
    text->setName("CheckWnd");
    // When anyone clicks on the text bar, it will toggle that checkwnd.
    text->setAutoToggleCheckWnd(myCheckWnd);
    addChild(StylePosWnd::ChildInfo(text,
      StylePosWnd::ChildEdgeInfo(5, StylePosWnd::LOCK_RIGHT, myCheckWnd),// Left
      StylePosWnd::ChildEdgeInfo(-5, StylePosWnd::LOCK_TOP, myCheckWnd),// Top
      StylePosWnd::ChildEdgeInfo(-5, StylePosWnd::LOCK_BORDER),    // Right
      StylePosWnd::ChildEdgeInfo(25, StylePosWnd::LOOSE)));        // Bottom
    // "Set your left edge 5 pixels to the right of the checkbox,
    //  set your top edge 5 pixels above the checkbox,
    //  set your right edge 5 pixels to the left of the right border,
    //  set your bottom edge to create a 25 pixel tall object."

  //
  // Float - Slider
    mySlider = new SliderWnd;
    mySlider->setOrientation(0);  // Vertical is 1
    mySlider->setPosition((int)((double)SliderWnd::END * attr->floatAttr));  // Set our preliminary value from the attrib
    mySlider->setNotifyId(FLOAT);
    mySlider->setNotifyWindow(this);
    addChild(StylePosWnd::ChildInfo(mySlider,
      StylePosWnd::ChildEdgeInfo(5, StylePosWnd::LOCK_BORDER),     // Left
      StylePosWnd::ChildEdgeInfo(10, StylePosWnd::LOCK_BOTTOM, myCheckWnd),// Top
      StylePosWnd::ChildEdgeInfo(-5, StylePosWnd::LOCK_BORDER),    // Right
      StylePosWnd::ChildEdgeInfo(15, StylePosWnd::LOOSE)));        // Bottom
    // "Set your left edge 5 pixels to the right of the left border,
    //  set your top edge to create a 10 pixels above the ,
    //  set your right edge 5 pixels to the left of the right border,
    //  set your bottom edge 25 pixels above the bottom border."

  //
  // String - EditWndString
    myEditWnd = new EditWndString;
    myEditWnd->setName(_("This is a text edit field."));
    myEditWnd->setNotifyId(STRING);
    myEditWnd->setNotifyWindow(this);
    myEditWnd->setBuffer(String(_(attr->stringAttr)),2000);  // Set our preliminary value from the attrib
    addChild(StylePosWnd::ChildInfo(myEditWnd,
      StylePosWnd::ChildEdgeInfo(5, StylePosWnd::LOCK_BORDER),       // Left
      StylePosWnd::ChildEdgeInfo(10, StylePosWnd::LOCK_BOTTOM, mySlider),// Top
      StylePosWnd::ChildEdgeInfo(-5, StylePosWnd::LOCK_BORDER),      // Right
      StylePosWnd::ChildEdgeInfo(25, StylePosWnd::LOOSE)));          // Bottom
    // "Set your left edge to create a 15 pixel wide object,
    //  set your top edge to create a 120 pixel high object,
    //  set your right edge 10 pixels to the left of the right border,
    //  set your bottom edge 25 pixel tall object."

  return 1;
}

int ExampleAttribWnd::onInit() {
  int retval = EXAMPLEATTRIBWND_PARENT::onInit();
  // *** You cannot add items to DropList until after you call
  // the onInit parent virtual method.  So this method handles
  // that for us in examplebDroplist.cpp
  myDropList->addItem("0 - Zero");
  myDropList->addItem("1 - One");
  myDropList->addItem("2 - Two");
  myDropList->addItem("3 - Three");
  myDropList->addItem("4 - Four");
  myDropList->setCurSel(attr->intAttr);  // Set our preliminary value from the attrib

  return retval;
}

// ===========================================================================
//
//  ExampleAttrib: "ExampleAttribWnd::childNotify"
//
int ExampleAttribWnd::childNotify (RootWnd *which, int msg, int param1, int param2) {
  if (which != NULL) { // for an interesting reason, it is valid to be given NULL as a which pointer.
    handleChildNotify(msg, which->getNotifyId(), param1, param2);
  }
  return EXAMPLEATTRIBWND_PARENT::childNotify(which, msg, param1, param2);
}


// ===========================================================================
//
//  ExampleAttrib: "ExampleAttribWnd::handleChildNotify"
//
//  In our notify handler, we save the newly reported state of the UI objects.
//
int ExampleAttribWnd::handleChildNotify (int msg, int objId, int param1, int param2) {
  switch(msg)  {
    case ChildNotify::DROPLIST_SEL_CHANGED: {
      int Item = param1; // Just a clarification assignment.
      switch(objId) {
        case INT:
          attr->intAttr = Item;
        break;
      }
    }
    break;

    case ChildNotify::CHECKWND_CLICK: {
      int Active = param1; // Just a clarification assignment.
      switch(objId) {
        case BOOL:
          attr->boolAttr = Active;
        break;
      }
    }
    break;

    case ChildNotify::SLIDER_FINAL_POSITION:
    case ChildNotify::SLIDER_INTERIM_POSITION: {
      int SliderPos = param1; // Just a clarification assignment.
      switch (objId) {
        case FLOAT:
          attr->floatAttr = ((double)SliderPos / (double)SliderWnd::END);
        break;
      }
    }
    break;
    
    case ChildNotify::EDITWND_KEY_PRESSED: {
      int Key = param1; // Just a clarification assignment.
      switch(objId) {
        case STRING:
          // Interestingly enough, we don't care about the key, 
          // it's just a good event to trap to update the attrib.
          attr->stringAttr = myEditWnd->getBuffer(); 
        break;
      }
    }
    break;

    default:
      // Hrrmph.
      return 0;
  }

  return 1;
}

// ===========================================================================
//
//  ExampleAttrib: Attribute Change Notification Methods
//
void ExampleAttribWnd::onIntChange(int id, int value) {
  myDropList->setCurSel(attr->intAttr);
}
void ExampleAttribWnd::onBoolChange(int id, bool value) {
  myCheckWnd->setChecked(attr->boolAttr); 
}
void ExampleAttribWnd::onFloatChange(int id, double value) {
  mySlider->setPosition((int)((double)SliderWnd::END * attr->floatAttr));
}
void ExampleAttribWnd::onStringChange(int id, const char *value) {
  myEditWnd->setBuffer(String(_(attr->stringAttr)),2000);
}


#if 0 // Reference Stuff

#endif// Reference Stuff
