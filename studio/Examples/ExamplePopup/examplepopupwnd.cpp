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
//      File:     ExamplePopupWnd.cpp
//
//!##   Purpose:  This module sets up yet another lame user interface to show off Attribs
//
//      Requires: Please read ExamplePopupWnd.h first.
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

#include "examplepopup.h"
#include "examplepopupwnd.h"
#include "resource.h"

#include "../../bfc/notifmsg.h" 

#include "../../common/popup.h"
#include "../../common/contextmenu.h"

// ===========================================================================
//
//  ExamplePopup: "ExamplePopupWnd::ExamplePopupWnd"
//
ExamplePopupWnd::ExamplePopupWnd( void ) {
  int myRetval = createChildWindows();
  // Set the titlebar text for the window.
  setName("ExamplePopupWnd");
}

// =========================================================================
//
//  Methods required by Window Creation Services
//
const char *ExamplePopupWnd::getWindowTypeName() { return "ExamplePopup Window"; }
GUID ExamplePopupWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExamplePopupWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}

// ===========================================================================
//
//  ExamplePopup: "ExamplePopupWnd::createChildWindows"
//
int ExamplePopupWnd::createChildWindows( void ) {
  // We work from a button, just because that's the easiest
  // way to get some events to toss up some popup menus on the screen.
  ButtonWnd * myButton = new ButtonWnd;
  myButton->setNotifyId(1);
  myButton->setNotifyWindow(this);
  myButton->setButtonText("Click me, bitch!",16);
  addChild(StylePosWnd::ChildInfo(myButton,
    StylePosWnd::ChildEdgeInfo(160, StylePosWnd::CENTER),     // Left
    StylePosWnd::ChildEdgeInfo(25, StylePosWnd::CENTER),      // Top
    StylePosWnd::ChildEdgeInfo(160, StylePosWnd::CENTER),     // Right
    StylePosWnd::ChildEdgeInfo(25, StylePosWnd::CENTER)));    // Bottom
  // "A 160x25 button always in the center."

  return 1;
}

int ExamplePopupWnd::onInit() {
  int retval = EXAMPLEPOPUPWND_PARENT::onInit();

  return retval;
}

// ===========================================================================
//
//  ExamplePopup: "ExamplePopupWnd::childNotify"
//
int ExamplePopupWnd::childNotify (RootWnd *which, int msg, int param1, int param2) {
  if (which != NULL) { // for an interesting reason, it is valid to be given NULL as a which pointer.
    handleChildNotify(msg, which->getNotifyId(), param1, param2);
  }
  return EXAMPLEPOPUPWND_PARENT::childNotify(which, msg, param1, param2);
}

// ===========================================================================
//
//  ExamplePopup: "ExamplePopupWnd::handleChildNotify"
//
//  In our notify handler, we save the newly reported state of the UI objects.
//
int ExamplePopupWnd::handleChildNotify (int msg, int objId, int param1, int param2) {
  switch(objId)  {
    case 1:
      // Probably a LITTLE naughty to just do this on ANY notify from the object,
      // so we check the message type, also.  Remember, the SDK may, in the future,
      // send all sorts of different unexpected messages to your objects.  If you
      // make assumptions like "oh, the engine will only send me these messages," you
      // are setting yourself up for your component to not work with later versions of
      // the system.  And then everyone would laugh at you.
      if (msg == ChildNotify::BUTTON_LEFTPUSH) {
        // Anyhow, you clicked?

        // PopupMenu is an object meant to be created and used within a single local scope.

        // You should realize that this means that the popup menu will BLOCK ALL PROCESSING.
        // (except for any processing going on in other threads.  see, aren't threads happy?)

        // Okay, first we have to make one.  Note that we give our this-pointer to the
        // popup menu.  Popups usually need to tie themselves to an OS-window, so by
        // passing 'this' to the constructor, the popup will be able to find your OS-window
        // easily through the engine to do its OS-Specific (someday crossplatform) magic.
        PopupMenu *topmenu = new PopupMenu(this);

        // Here we add items from the top down into the menu windows themselves.

        // Let's start with a piece of flat text.  This is done by adding a disabled command.
        topmenu->addCommand("static text", -1/*id*/, 0/*checked*/, 1/*diabled*/);

        // We can then add a super special separator
        topmenu->addSeparator();

        // So, popup menus have real commands in them.  Commands have text and a command identifier.
        topmenu->addCommand("command", MENUCOMMAND1);

        // We can then add a super special separator
        topmenu->addSeparator();

        // Popup menus also have submenus.  You can make your submenus inline, or you
        // can use a callback for them.  We'll just make them inline.
        PopupMenu *submenu = new PopupMenu(this);

        // Put a command in the submenu
        submenu->addCommand("sub-command", MENUCOMMAND2);
        
        // And then put the submenu into the topmenu
        topmenu->addSubMenu(submenu, "sub-menu");

        // We can then add a super special separator
        topmenu->addSeparator();

        // Another thing you can do is have a menu-checkbox for things like your _bool attributes.
        static checked = 0;
        topmenu->addCommand("menu-checkbox", MENUCOMMAND3, checked);

        // Once you're happy with what your menu looks like, 
        int command = topmenu->popAtMouse();

        switch (command) {
          case MENUCOMMAND1:
            // do whatever you'd do for MENUCOMMAND1
          break;
          case MENUCOMMAND2:
            // do whatever you'd do for MENUCOMMAND2
          break;
          case MENUCOMMAND3:
            // do whatever you'd do for MENUCOMMAND3
            checked = !checked;
          break;
        }
        
        return 0;
      }
      if (msg == ChildNotify::BUTTON_RIGHTPUSH) {
        // Anyhow, you rightclicked?

        // Maybe we'll do more here later.  For now, I'm gonna sano the sdk install.

        return 0;
      }

    default:
      // Hrrmph.
      return 0;
  }

  return 1;
}



#if 0 // Reference Stuff

#endif// Reference Stuff
