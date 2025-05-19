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
//      File:     ExampleHTTPXMLWnd.cpp
//
//!##   Purpose:  This is the source module that contains the top-level 
//!##             methods we'll be using for the construction of the ExampleHTTPXML
//!##             window object.
//
//      Requires: Please read ExampleBWnd.h first.
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

// Child Window Notification Messages
#include "../../bfc/notifmsg.h" 
// Button Wnd
#include "../../bfc/wnds/buttwnd.h"

#include "ExampleHTTPXML.h"
#include "ExampleHTTPXMLwnd.h"
#include "resource.h"

// Header for the _ object for language translation.
#include "../../common/xlatstr.h"


// Oh, buttons get included up in there somewhere, too....


// ===========================================================================
//
//  ExampleHTTPXML: Static Globals
//

// A simple little static to help us lay out buttons, etc.
static inline int PostIncReturn(int &counter, int amount) {
  // Return the given counter value and increment it by ``amount''
  int retval = counter;
  counter += amount;
  return retval;
}

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::ExampleHTTPXMLWnd"
//
//    *** We commit the tasks of 1) Instantiation and
//        2) Assembly in the CONSTRUCTOR of the containment
//        class.  The third primary task to occur before
//        normal framelooped behaviour, Initialization,
//        occurs AUTOMATICALLY for ALL contained objects by
//        the default baseclass onInit() event handler.
//
//    *** In addition, it is important to note that any GUI
//        objects that take pointers to other gui objects have
//        the responsibility for deleting their interred pointrs
//        upon the destruction of that object.
//
ExampleHTTPXMLWnd::ExampleHTTPXMLWnd( void ) :
  myXMLURLBuffer()  {  // Let our string member fully construct.
  // Blah blah blah... default initialization values... yadda yadda yadda
  myUrlGrab = NULL;
  myDownloadState = EXHTTPXML_DOWNLOAD_READY;
  currentUrl = "";

  //
  // In order to logically segregate the functions, we're
  // going to make a new method for our control class that
  // we call "createChildWindows" -- here's where we'll setup the
  // user interface objects that will exist in our sandbox.
  //
  //  (This lets me move that whole block of code around if I
  //   screw up and put it in the wrong place, or the API changes)
  int myRetval = createChildWindows();
  // and yes, we ignore the return code from the fxn.
  // what are you going to do about it, in a constructor?

  // Set the titlebar text for the window.
  setName("ExampleHTTPXMLWnd");
}

// =========================================================================
//
//  Methods required by Window Creation Services
//
const char *ExampleHTTPXMLWnd::getWindowTypeName() { return "Example HTTPXML Window"; }
GUID ExampleHTTPXMLWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExampleHTTPXMLWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::onInit()"
//
//  So, okay, the setting of a timer falls neatly under the heading of 
//	"initialization activities."  This is how we latch our initialization
//	event and perform our method overrides.  STANDARD C++ procedure, of
//	course, but I'm gonna be explicit about it, just so there's no problems:
int ExampleHTTPXMLWnd::onInit() {
  // By calling our parent class' onInit method, we aught to have all
  // of our children properly initialized as well.
  int retval = EXAMPLEHTTPXMLWND_PARENT::onInit();

  // Turns out we have nothing to do here, right now

  return retval;
}

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::timerCallback(int id)"
//
void ExampleHTTPXMLWnd::timerCallback(int id) {
	// Anyhow, so, we test the ID here to make sure we know
	// which timer fell out.  Since we only have one timer,
	// we didn't HAVE to switch on it, but it helps to setup
	// your frameworks like this anyhow.  Multiple if-chains
	// make things hard to read.
	switch (id)	{
		case EXHTTPXML_TIMER_ID:  
			// Periodically, handle URL downloading.
			pollDownloadState();
		break;
		default:
			// If, by some vague stretch of our crazed imaginations,
			// someone ELSE in our window derivation chain decided to
			// send up a timer, we must then pass it along up the chain.
			EXAMPLEHTTPXMLWND_PARENT::timerCallback(id);
			// NO, DO NOT QUESTION, JUST DO IT.  DO IT!	 DO IT NOW!!!
			//
			//			grr!
			//
		break;
	}
}

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::createChildWindows"
//
int ExampleHTTPXMLWnd::createChildWindows( void ) {
  // So?  What shall we do today?

  // Well, we're inherited from a tabsheet.  That means we can make lots of
  // little children windows that do their own thing, create them here, add
  // them into ourselves, and then have them be properly initialized and
  // functional automatically by the system.
  //
  // Slikschiznitz, eh?

  // SO OKAY, now that we know that we are a tabsheet, I guess we start 
  // stuffing kiddie windows into everything.

  //
  //  This one will let you punch in an url to an XML sheet that will download and
  //  display in a frame window assembly.
  createFramesSheet();  

  // Return "0" for failure and "1" for success.
  return 1;
}


// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::childNotify"
//

//
//  Okay, so, we're going to use our single derived object to properly dispatch
//  all the messaging from our children of myriad diversity.
//
//  This means I need to tell you about all the useful notification
//  messages that you might encounter in your day to day life with
//  our lovely little SDK:
//
//  *** BUTTON Notifications:
//
//    SO, firstly, we have buttons.  Stock ButtonWnd objects will send the
//      following Notification messages:
//
//        CHILD_NOTIFY_LEFTPUSH          
//        CHILD_NOTIFY_RIGHTPUSH         
//        CHILD_NOTIFY_LEFTDOUBLECLICK   
//        CHILD_NOTIFY_RIGHTDOUBLECLICK  
//
//    And the id that you assign to the button is sent as param1.
//                                       
int ExampleHTTPXMLWnd::childNotify (RootWnd *which, int msg, int param1, int param2) {
  StringPrintf text("Incoming rootWnd-childNotify: ptr:0x%08x msg:0x%03x p1:0x%08x p2:0x%08x \n",
        which, msg, param1, param2);
  OutputDebugString(text);
  //
  //  For the nonce, we're going to ignore the pointer to the object who is
  //  sending the notification event.  Pointers are icky, smelly, and if you
  //  touch them, you'll have to wash your hands with an aerosol oven cleaner
  //  for a week just to make sure you're not accidentally besmirched.
  //
  //  OBVIOUSLY, if any of the objects you're using require an acknowledgement
  //  or any other method to be called on them, you'll want to implement a
  //  schema for your code that keeps the "which" pointer around.  But we're
  //  not going to bother being that complicated here, kiddies.
  //
  //  Uncle mig's got a deadline, you know.
  //
  if (which != NULL) { // Apparently, it is VALID to receive NULL as your "which" pointer!  My mistake!
    handleChildNotify(msg, which->getNotifyId(), param1, param2);
  }

  return EXAMPLEHTTPXMLWND_PARENT::childNotify(which, msg, param1, param2);
}


// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::handleChildNotify"
//
//    Just to show off message handling without pointers.
//
int ExampleHTTPXMLWnd::handleChildNotify (int msg, int objId, int param1, int param2) {
  //
  // We're all familiar with nested switch for event handling.
  // There's nothing scary to be seen, here.
  switch(msg)  {
    // Messages from our buttons being pushed.
    case ChildNotify::BUTTON_LEFTPUSH:
    case ChildNotify::BUTTON_RIGHTPUSH:
    case ChildNotify::BUTTON_LEFTDOUBLECLICK:
    case ChildNotify::BUTTON_RIGHTDOUBLECLICK: {
      //
      // So, now, what we're going to do here is if ANY of those
      // messages came in, we just check which object ID shot that
      // message out in order to know what to do with it.
      //
      // Switch on the object's ID to implement the button functionality.
      switch(objId) {
        case EXHTTPXML_XML_GO:
          OutputDebugString(_("Button pushed corresponds to EXHTTPXML_XML_GO ButtonID\n"));
          // And you'll find this method there, as well.
          startURLDownload();
        break;
        case EXHTTPXML_XML_URL:
          OutputDebugString(_("Button pushed corresponds to EXHTTPXML_XML_URL ButtonID\n"));
          // And you'll find this method there, as well.
          startURLMinibrowser();
        break;
        default:
          // Hrrmph.
          return 0;
      }
    }
    break;
    default:
      // Hrrmph.
      return 0;
  }

  //
  // Yawn.  Do I care about my error condition, here?
  //
  //  Oh PROBABLY.
  //
  //            SOMEWHERE.
  //
  //  Maybe in North Dakota or something.
  //
  //      So, if you're a developer, and you're IN North Dakota, well,
  //      I feel so sorry for you that I'd suggest immediately changing
  //      the return value here from a 1 to a 0.  It won't make a big
  //      difference in the functionality here, but it will help bring
  //      unto you that most invaluable of gifts:
  //
  //                  Peace of mind.
  //
  //  See, aren't I a great pal?
  return 1;
}
