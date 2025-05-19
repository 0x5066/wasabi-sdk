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
//      File:     ExampleItemSequencerWnd.cpp
//
//!##   Purpose:  This is the source module that contains the top-level 
//!##             methods we'll be using for the construction of the ExampleItemSequencer
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

#include "ExampleItemSequencer.h"
#include "ExampleItemSequencerwnd.h"
#include "resource.h"

// Child Window Notification Messages
#include "../../bfc/notifmsg.h" 
// Header for button windows!
#include "../../bfc/wnds/buttwnd.h"
// Header for the _ object for language translation.
#include "../../common/xlatstr.h"
// Header for CharPtrSequencer for deregistration
#include "../../studio/charptrsequence.h"


// Oh, buttons get included up in there somewhere, too....


ExampleItemSequencerWnd::ExampleItemSequencerWnd() {
  // Blah blah blah... default initialization values... yadda yadda yadda
  myUrlGrab = NULL;
  myDownloadState = EXITEMSEQ_DOWNLOAD_READY;
  currentUrl = "";

  // All sortsa null pointers.
  urledit = NULL;
  extedit = NULL;
  myFrameTreeWnd = NULL;
  myTopStylePosWnd = NULL;
  myRightStylePosWnd = NULL;
  myFrameTextBar = NULL;
  myFrameButtWnd = NULL;
  myEnqueueAllButton = NULL;
  
  // All of these items we must delete ourselves.
  mySequencer = NULL;

  int myRetval = createChildWindows();

  // Set the titlebar text for the window.
  setName("ExampleItemSequencerWnd");
}

ExampleItemSequencerWnd::~ExampleItemSequencerWnd() {
  if (mySequencer != NULL) {
    api->core_deregisterSequencer(NULL, mySequencer);
    delete mySequencer;
  }
}


// =========================================================================
//
//  Methods required by Window Creation Services
//
const char *ExampleItemSequencerWnd::getWindowTypeName() { return "ExampleItemSequencer Window"; }
GUID ExampleItemSequencerWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExampleItemSequencerWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}




// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::onInit()"
//
//  So, okay, the setting of a timer falls neatly under the heading of 
//	"initialization activities."  This is how we latch our initialization
//	event and perform our method overrides.  STANDARD C++ procedure, of
//	course, but I'm gonna be explicit about it, just so there's no problems:
int ExampleItemSequencerWnd::onInit() {
  // By calling our parent class' onInit method, we aught to have all
  // of our children properly initialized as well.
  int retval = EXAMPLEITEMSEQUENCERWND_PARENT::onInit();

  // Funny, there's no longer anything to do, here.

	return retval;
}

// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::timerCallback(int id)"
//
void ExampleItemSequencerWnd::timerCallback(int id) {
	// Anyhow, so, we test the ID here to make sure we know
	// which timer fell out.  Since we only have one timer,
	// we didn't HAVE to switch on it, but it helps to setup
	// your frameworks like this anyhow.  Multiple if-chains
	// make things hard to read.
	switch (id)	{
		case EXITEMSEQ_TIMER_ID:  
			// Periodically, handle URL downloading.
			pollDownloadState();
		break;
		default:
			// If, by some vague stretch of our crazed imaginations,
			// someone ELSE in our window derivation chain decided to
			// send up a timer, we must then pass it along up the chain.
			EXAMPLEITEMSEQUENCERWND_PARENT::timerCallback(id);
			// NO, DO NOT QUESTION, JUST DO IT.  DO IT!	 DO IT NOW!!!
			//
			//			grr!
			//
		break;
	}
}

// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::createChildWindows"
//
int ExampleItemSequencerWnd::createChildWindows( void ) {
  createFramesSheet();  
  return 1;
}


// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::childNotify"
//

int ExampleItemSequencerWnd::childNotify (RootWnd *which, int msg, int param1, int param2) {
  if (which != NULL) { // for an interesting reason, it is valid to be given NULL as a which pointer.
    handleChildNotify(msg, which->getNotifyId(), param1, param2);
  }

  return EXAMPLEITEMSEQUENCERWND_PARENT::childNotify(which, msg, param1, param2);
}


// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::handleChildNotify"
//
//    Just to show off message handling without pointers.
//
int ExampleItemSequencerWnd::handleChildNotify (int msg, int objId, int param1, int param2) {
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
        case EXITEMSEQ_URL_GO:
          OutputDebugString(_("Button pushed corresponds to EXITEMSEQ_URL_GO ButtonID\n"));
          startURLDownload();
        break;
        case EXITEMSEQ_ENQUEUE_ALL:
          OutputDebugString(_("Button pushed corresponds to EXITEMSEQ_ENQUEUE_ALL ButtonID\n"));
          enqueueAll();
        break;
        case EXITEMSEQ_ENQUEUE:
          OutputDebugString(_("Button pushed corresponds to EXITEMSEQ_ENQUEUE ButtonID\n"));
        break;
        case EXITEMSEQ_DOWNLOAD:
          OutputDebugString(_("Button pushed corresponds to EXITEMSEQ_DOWNLOAD ButtonID\n"));
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
