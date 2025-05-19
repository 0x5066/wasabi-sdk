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
//      File:     ExampleCodeGUIWnd.cpp
//
//!##   Purpose:  This is the source module that contains the top-level 
//!##             methods we'll be using for the construction of the ExampleCodeGUI
//!##             window object.
//
//      Requires: Please read ExampleCodeGUIWnd.h first.
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

#include "examplecodegui.h"
#include "examplecodeguiwnd.h"
#include "resource.h"

// To be able to have ExampleCanvasWnd as a Child Window
#include "../examplecanvas/examplecanvaswnd.h"  
// Core Handle for control of mp3 playback.
#include "../../common/corehandle.h"
// Header to allow us to throw a textbar in there.
#include "../../common/textbar.h"
// Header for generic guid mangling.
#include "../../common/nsguid.h"
// Header for the _ object for language translation.
#include "../../common/xlatstr.h"
// Header for ContWnd
#include "../../common/contwnd.h"
// Child Window Notification Messages
#include "../../bfc/notifmsg.h" 


// Oh, buttons get included up in there somewhere, too....


// ===========================================================================
//
//  EXAMPLECODEGUI: Static Globals
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
//  EXAMPLECODEGUI: "ExampleCodeGUIWnd::ExampleCodeGUIWnd"
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
ExampleCodeGUIWnd::ExampleCodeGUIWnd( void ) {
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
  setName("ExampleCodeGUIWnd");
}


// =========================================================================
//
//  Methods required by Window Creation Services
//
const char *ExampleCodeGUIWnd::getWindowTypeName() { return "ExampleCodeGUI Window"; }
GUID ExampleCodeGUIWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExampleCodeGUIWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}


// ===========================================================================
//
//  EXAMPLECODEGUI: "ExampleCodeGUIWnd::onInit()"
//
int ExampleCodeGUIWnd::onInit() {
  // By calling our parent class' onInit method, we aught to have all
  // of our children properly initialized as well.
  int retval = EXAMPLECODEGUIWND_PARENT::onInit();

	// SEEE?!?  You didn't believe me?  Now there is a reason for this method.

	// *** You cannot add items to DropList until after you call
	// the onInit parent virtual method.  So this method handles
	// that for us in examplebDroplist.cpp
	postInitDroplist();

	return retval;
}

// ===========================================================================
//
//  EXAMPLECODEGUI: "ExampleCodeGUIWnd::createChildWindows"
//
int ExampleCodeGUIWnd::createChildWindows( void ) {
  // So?  What shall we do today?

  // Well, we're inherited from a tabsheet.  That means we can make lots of
  // little children windows that do their own thing, create them here, add
  // them into ourselves, and then have them be properly initialized and
  // functional automatically by the system.
  //
  // Slikschiznitz, eh?

  // SO OKAY, now that we know that we are a tabsheet, I guess we start 
  // stuffing kiddie windows into everything.

  // First we'll use the custom-blitting window code we created in Example1.  That's
  // a good way to start.  So, follow along here:

  // *** For a child of a tabsheet, the name of the window becomes the name
  //     listed on the tab.  So, the instructions here are:
  //
  //    1) Instantiate your child window object.
  ExampleCanvasWnd *myExampleCanvasWnd = new ExampleCanvasWnd(ExampleCanvasWnd::BORING);
  //    2) Give it a name (altho you might have your own custom window
  //        give itself its own name).  For Tabsheet objects, the name
  //        of the child object becomes the name in the tag.
  myExampleCanvasWnd->setName(_("Boring"));
  //    3) Add it as a simple child to yourself.
  //        NOTE: The "Add Child" method for TabSheet allows the specification
  //        of a "TIP" string. 
  addChild(myExampleCanvasWnd,_("This is a boring version of ``hello world''"));
  // WIP: AFAICT, the tip string is currently ignored.  But that won't crush
  // us, and it won't stop us from hoping that, one day, all the tip strings
  // in all the world can stand together hand in hand in the light of day without
  // fear of hatred or reprisals or silly little api bugs that prevent them
  // from attaining their full and complete and proper place in our society.

  //
  // *** Note the _("...") object.  The _ object will feed your const string
  //     through the localization and translation code to allow your strings
  //     to be translated to other languages.

  //
  // Here's another copy of the blitter window, in a second tab.
  ExampleCanvasWnd *myExampleCanvasWnd2 = new ExampleCanvasWnd(ExampleCanvasWnd::EXCITING);
  myExampleCanvasWnd2->setName(_("Exciting"));
  addChild(myExampleCanvasWnd2,_("This is a fun version of ``hello world''"));

  //
  //  Now what we're going to do is have an individual method for the instantiation
  //  and assembly of each tabsheet window.  First up will be the example sheet to
  //  display the proper care and feeding of a gaggle of buttons whose button
  //  functionality is tied to the core MP3 functionality:
  createCoreButtonsSheet(); 

  //
  //  This one shows how to use frames, text edit fields, and trees.
  createFramesSheet();  

  //
  //  This one shows styleposwnd, formwnd, and sliderwnd.
  createMinesweeperSheet();

  //
  //  This one shows checkboxes and, one hopes, radio groups.
  createChecklistSheet();

	//
	//	How about droplists and listwnds?
	createDroplistSheet();

  // Return "0" for failure and "1" for success.
  return 1;

}

// ===========================================================================
//
//  EXAMPLECODEGUI: "ExampleCodeGUIWnd::createCoreButtonsSheet"
//
int ExampleCodeGUIWnd::createCoreButtonsSheet() { 
  // Make some fun buttons that link to the core controls.

  // First thing we do is make a container to hold our buttons.
  ContWnd * myButtonContainer = new ContWnd();
  // Name him, for the tab.
  myButtonContainer->setName(_("Buttons"));
  // Add him to ourselves to make a new tab sheet.
  addChild( myButtonContainer, _("This contains buttons that control core playback.") );

  //
  //  *** Some things to know about ContWnd:
  //
  //      ContWnd is made to contain objects with fixed sizes in a fixed layout.
  //      The addChild() method explicitly defines and sets the rectangular size
  //      of the children being added.
  //

  //
  // Now we start making the children objects for the button container.
  //

  // First thing we want to do is make up an inline counting system
  // to allow us to more easily lay out our buttons.  The static inline
  // method used here is defined at the top of the module.
  int y = EXB_TOP_OFFSET; // used by PostIncReturn
  // By creating this, I can be lazy and cut and paste my buttons without
  // worrying about re-twiddling every pixel position by hand.

  // A play button would be nice.  So we make a button.
  ButtonWnd *play = new ButtonWnd();
  // Then we give it some text to display.
  play->setButtonText("Play", 16);
  // And assign a notification ID to it.
  play->setNotifyId(EXB_PLAY);
  // And assign our object to handle all notifications from this button
  play->setNotifyWindow(this);
  // And finally add it to the container window.
  myButtonContainer->addChild(play, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize*/);

  //
  // NOTE:  The "Notify" stuff is where it all happens, but it's mildly complex.
  //        checkout ExampleCodeGUIWnd::childNotify() for where the magic happens.
  //

  // How about a stop button?
  ButtonWnd *stop = new ButtonWnd();
  stop->setButtonText(_("Stop"), 16);
  stop->setNotifyId(EXB_STOP);
  stop->setNotifyWindow(this);
  myButtonContainer->addChild(stop, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // How about a pause button?
  ButtonWnd *pause = new ButtonWnd();
  pause->setButtonText(_("Pause"), 16);
  pause->setNotifyId(EXB_PAUSE);
  pause->setNotifyWindow(this);
  myButtonContainer->addChild(pause, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // Space down by 2 yspacers.
  PostIncReturn(y, EXB_BUTTON_SPACER + EXB_BUTTON_SPACER);

  // "Previous" button....
  ButtonWnd *prev = new ButtonWnd();
  prev->setButtonText(_("Previous"), 16);
  prev->setNotifyId(EXB_PREVIOUS);
  prev->setNotifyWindow(this);
  myButtonContainer->addChild(prev, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // "Next" button....
  ButtonWnd *next = new ButtonWnd();
  next->setButtonText(_("Next"), 16);
  next->setNotifyId(EXB_NEXT);
  next->setNotifyWindow(this);
  myButtonContainer->addChild(next, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // Space down by 4 yspacers.
  PostIncReturn(y, EXB_BUTTON_SPACER * 4);

  // And create some text items.
  TextBar *textbar = new TextBar();
  textbar->setName(_("This is my text bar to say that the next text bars are my guid."));
  textbar->setDrawBox(1); // This one will draw a box around him.
  textbar->setCenter(1);  // and be centered in his box.
  myButtonContainer->addChild(textbar, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // Want to display your GUID?
  TextBar *guidbar = new TextBar();
  char text[200];
  // I'm being a dick and connecting to my guid as a global variable.
  // that's lame and sloppy programming, kids.  never do that.  :)
  extern GUID exb_guid;
  guidbar->setName(nsGUID::toChar(exb_guid, text));
  // Oh yah, and DON'T be a goddamn hypocrite, either!
  // (inspect the "nsGUID::toChar" function, however)
  guidbar->setTextOutlined(1);  // This one will outline his font
  guidbar->setCenter(1);        // And draw centered in his bounding rect.
  myButtonContainer->addChild(guidbar, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // Want to test that guids are being properly translated?
  GUID testGUID = nsGUID::fromChar(text);

  // Want to display your crosstranslated GUID?
  TextBar *testguidbar = new TextBar();
  char testtext[200];
  testguidbar->setName(nsGUID::toChar(testGUID, testtext));
  testguidbar->setTextShadowed(1); // This one will shadow his font
  testguidbar->setCenter(0);       // And draw left-justified
  myButtonContainer->addChild(testguidbar, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // And one more text item for good measure, eh?
  TextBar *textbar2 = new TextBar();
  textbar2->setName(_("Are they equal?  Can you read this?"));
  textbar2->setTextShadowed(0);  // This one might not be readable!
  myButtonContainer->addChild(textbar2, EXB_LEFT_OFFSET, 
    PostIncReturn(y, EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER),
    EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);


  // For the nonce, return codes from and to the API should
  //  return "0" for failure and "1" for success.
  return 1;
}

// ===========================================================================
//
//  EXAMPLECODEGUI: "ExampleCodeGUIWnd::childNotify"
//

//
//  Okay, so, we're going to use our single derived object to properly dispatch
//  all the messaging from our children of myriad diversity.
//
//  All objects derived from BaseWnd (which is, as far as we're concerned right now,
//  everything) allow their notification to be identified with an integer id tag
//  using the setNotifyId(int) method.  This integer id tag can be read from the
//  RootWnd *which parameter of the childNotify method by the getNotifyId() method.
//
//  In the same way, all objects derived from BaseWnd can also redirect their
//  notification to any other BaseWnd pointer, using the setNotifyWindow(BaseWnd*)
//  method.  Please note the use of these method calls in this module.
//
//  So, ANYWAY (yah, sometimes I start to bore myself, too...)
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
//
//  *** SLIDER Notifications:
//
//    Just like bro, the sliders use these notification messages:
//
//        SLIDER_INTERIM_POSITION
//        SLIDER_FINAL_POSITION
//                                       
//  *** CHECKWND Notifications:
//
//        CHECKWND_CLICK
//
//  *** RADIOGROUP Notifications:
//
//        RADIOGROUP_STATECHANGE

int ExampleCodeGUIWnd::childNotify (RootWnd *which, int msg, int param1, int param2) {
  StringPrintf text("Incoming childNotify: ptr:0x%08x msg:0x%04x p1:0x%08x p2:0x%08x \n",
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

  if (which != NULL) { // for an interesting reason, it is valid to be given NULL as a which pointer.
    if (handleChildNotify(msg, which->getNotifyId(), param1, param2) == -1) {
      return 0;
      // We do this because someone might delete the "which" object, and that
      // is no longer safe to pass along.  However, you shoule ONLY override 
      // the call to one's parent if you are ABSOLUTELY SURE that it is safe.
      
      // In this case, we know it is safe, because I make it return the magic
      // value of -1 when I am handling the deleting of an object that I know
      // my parent object isn't paying any attention to.
    }
  }


  return EXAMPLECODEGUIWND_PARENT::childNotify(which, msg, param1, param2);
}


// ===========================================================================
//
//  EXAMPLECODEGUI: "ExampleCodeGUIWnd::handleChildNotify"
//
//    Just to show off message handling without pointers.
//
int ExampleCodeGUIWnd::handleChildNotify (int msg, int objId, int param1, int param2) {
  //
  // We're all familiar with nested switch for event handling.
  // There's nothing scary to be seen, here.
  switch(msg)  {
    // Messages from our buttons being pushed.
    case ChildNotify::BUTTON_LEFTPUSH: {
      //
      // So, now, what we're going to do here is if ANY of those
      // messages came in, we just check which object ID shot that
      // message out in order to know what to do with it.
      //
      // Switch on the ID to know what to do.
      switch(objId) {
        case EXB_PREVIOUS: {
          OutputDebugString(_("Button pushed corresponds to EXB_PREVIOUS ButtonID\n"));
          CoreHandle newHandle;
          newHandle.prev();
        }
        break;
        case EXB_PLAY: {
          OutputDebugString(_("Button pushed corresponds to EXB_PLAY ButtonID\n"));
          CoreHandle newHandle;
          newHandle.play();
        }
        break;
        case EXB_PAUSE: {
          OutputDebugString(_("Button pushed corresponds to EXB_PAUSE ButtonID\n"));
          CoreHandle newHandle;
          newHandle.pause();
        }
        break;
        case EXB_STOP: {
          OutputDebugString(_("Button pushed corresponds to EXB_STOP ButtonID\n"));
          CoreHandle newHandle;
          newHandle.stop();
        }
        break;
        case EXB_NEXT: {
          OutputDebugString(_("Button pushed corresponds to EXB_NEXT ButtonID\n"));
          CoreHandle newHandle;
          newHandle.next();
        }
        break;
        //
        // These two buttons are created and handled in examplebframewnd.cpp
        case EXB_FRAMEBUTTON1:
          OutputDebugString(_("Button pushed corresponds to EXB_FRAMEBUTTON1 ButtonID\n"));
          onFrameButton1();
        break;
        //
        // These two buttons are created and handled in examplebframewnd.cpp
        case EXB_FRAMEBUTTON2:
          OutputDebugString(_("Button pushed corresponds to EXB_FRAMEBUTTON2 ButtonID\n"));
          onFrameButton2();
        break;
        //
        // This button is created and handled in examplebstylewnd.cpp
        case EXB_MINEGAMESTART:
          OutputDebugString(_("Button pushed corresponds to EXB_MINEGAMESTART ButtonID\n"));
          newGame();
        break;
        default:
          // The minesweeper game makes LOTS of buttons.
          if (objId >= EXB_MINECELLBUTTON) {
            cellHit(objId);
            return -1;  // this means we shouldn't pass the message to our parent!
            // We do this because cellHit deletes the object up there, and that
            // is no longer safe to pass along.  However, not passing this particular
            // message along _is_ safe.  You shoule ONLY override the call to one's
            // parent if you are ABSOLUTELY SURE that it is safe to do so.
          }
          // Hrrmph.
          return 0;
      }
    }
    break;

    case ChildNotify::BUTTON_RIGHTPUSH: {
      //
      // So, HEY!  Here's where we'll handle our RMB handling, okay?
      //

      // The minesweeper game makes LOTS of buttons.
      if (objId >= EXB_MINECELLBUTTON) {
        setSafe(objId);
      } else {
        return 0;
      }
    }
    break;

    case ChildNotify::BUTTON_LEFTDOUBLECLICK:
    case ChildNotify::BUTTON_RIGHTDOUBLECLICK:
      // We don't really do anything with these messages.
    break;


    //
    // In addition to buttons, we can also be notified about the movement of
    // sliders in our UI.  In this case, we're notified both _as_ the object
    // moves, and we're notified that it is done moving.
    //
    //    SLIDER_INTERIM_POSITION - The slider is moving and this is where it
    //                              currently resides.
    //    SLIDER_FINAL_POSITION   - The slider is done moving and this is 
    //                              where it has stopped.
    //
    // I'm going to be a bit redundant in how I split out these notifications,
    // but it will hopefully give you a clearer picture of how these two work.
    //
    //  So, here's where we track the sliders as they move...
    case ChildNotify::SLIDER_INTERIM_POSITION: {
      int SliderPos = param1; // Just a clarification assignment.
      switch (objId) {
        case EXB_MINEVERTSLIDER:
          OutputDebugString(_("The vertical size slider is moving.\n"));
          displayVert(SliderPos); // In raw slider units.
        break;
        case EXB_MINEHORIZSLIDER:
          OutputDebugString(_("The horizontal size slider is moving.\n"));
          displayHoriz(SliderPos); // In raw slider units.
        break;
        case EXB_MINEMINESSLIDER:
          OutputDebugString(_("The number of mines slider is moving.\n"));
          calcMines();
        break;
      }
    }
    break;
    //  So, here's where we track the sliders when they complete their movement...
    case ChildNotify::SLIDER_FINAL_POSITION: {
      int SliderPos = param1; // Just a clarification assignment.
      switch (objId) {
        case EXB_MINEVERTSLIDER:
          OutputDebugString(_("The vertical size slider has stopped moving.\n"));
          displayVert(SliderPos); // In raw slider units.
        break;
        case EXB_MINEHORIZSLIDER:
          OutputDebugString(_("The horizontal size slider has stopped moving.\n"));
          displayHoriz(SliderPos); // In raw slider units.
        break;
        case EXB_MINEMINESSLIDER:
          OutputDebugString(_("The number of mines slider has stopped moving.\n"));
          calcMines();
        break;
      }
    }
    break;
    case ChildNotify::CHECKWND_CLICK:
    case ChildNotify::RADIOGROUP_STATECHANGE:
      // To attempt to prevent confusion, we will handle these two event types
      // in depth in ExampleCodeGUIChecklistWnd.cpp --
      return handleChecklistNotify(msg, objId, param1, param2);
    break;
    case ChildNotify::DROPLIST_SEL_CHANGED:
			return handleDroplistChanged(msg, objId, param1, param2);
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


#if 0	// Reference Stuff
// ===========================================================================
//
//	EXAMPLECODEGUI: Reference Stuff
//
//    This section should (eventually) provide the reference materials for
//  the following SDK Objects:
//
//      ButtonWnd
//      TextBar
//
//    As always, if you have any specific questions about these objects,
//  please do feel free to also check the NSDN site, as well as the wasabi
//  developers' forum.



// ===========================================================================
//
//	EXAMPLECODEGUI: ButtonWnd Reference
//
//  ButtonWnd is inherited from ClickWnd
//
//  ButtonWnd uses the BaseWnd notification ID methods for ID tracking.


  // --------------------------------------------
  // Public Methods

  int setBitmaps(const char *normal, const char *pushed=NULL, const char *hilited=NULL, const char *activated=NULL);
  int setBitmaps(HINSTANCE hInst, int normal, int pushed, int hilited, int activated);
  int setRightBitmap(char *bitmap);
  int setBitmapCenter(int centerit);
  void setUseBaseTexture(int useit);
  void setBaseTexture(SkinBitmap *bmp, int x, int y, int tile=0);
  // methods to modify button looks

  int setButtonText(const char *text, int size=DEFAULT_BUTTON_TEXT_SIZE);
  const char * getButtonText();
  // specify a text string to be drawn centered on the button.

  void setTextJustification(ButtonJustify jus);
  // change the justification methods

  void setWantFocus(int want) { iwantfocus = !!want; }
  virtual int wantFocus() const { return iwantfocus; }
  // specify if you will accept keyfocus

  void enableButton(int enabled);	
  // can be pushed

  int getWidth();	
  int getHeight();
  // our preferred width and height (from bitmaps)

  void setCheckBitmap(char *checkbm);
  // set a bitmap to display as a "checked" mode (default is none)
  void setChecked(int c) { checked=c; }; // <0=nocheck, 0=none, >0=checked
  int  getChecked() const { return checked; }
  // set and get the checked modes

  void setHilite(int h);
  int getHilite();
  // set and get the hilite modes
  void setPushed(int p);
  int getPushed() const;
  // used by menus to simulate pushing
  void setAutoDim(int ad) { autodim=!!ad; }
  int getAutoDim() const { return autodim; }
  // nonzero makes it dim if there's no hilite bitmap
  void setActivatedButton(int a);
  int getActivatedButton() const;
  // set and get the activated modes

  void setBorders(int b) { borders=!!b; }
  int getBorders() const { return borders; }
  // set true to have borders around your buttons.

  void setFolderStyle(int f) { folderstyle = !!f; }
  // display as a "folder style" button (right angles on the bottom corners)

  void setInactiveAlpha(int a) { inactivealpha=a; }
  void setActiveAlpha(int a) { activealpha=a; }
  // set alpha values for active and inactive states

  void setColors(const char *text="studio.button.text", const char *hilite="studio.button.hiliteText", const char *dimmed="studio.button.dimmedText");
  // set the various colors for the button to use
  

  // --------------------------------------------
  // Virtual Methods - If you override them, call the parent method.

  virtual int onPaint(Canvas *canvas);
  // how to draw a button

  virtual void onLeftPush(int x, int y);
  virtual void onRightPush(int x, int y);
  virtual void onLeftDoubleClick(int x, int y);
  virtual void onRightDoubleClick(int x, int y);
  virtual void onLeaveArea();
  virtual int onMouseMove(int x, int y);	// need to catch region changes
  // mouse events
  
  virtual int onGetFocus();
  virtual int onKillFocus();
  // focus events

  virtual int onChar(char c);
  // typing events

  virtual int onResize();
  virtual int onEnable(int is);
  // system events

  virtual int onActivateButton(int active);
  virtual int onActivate();
  virtual int onDeactivate();
  // activate events

  virtual void setAlpha(int a);
  virtual int getAlpha(void) const;
  // get and set the alpha value for drawing

  virtual int getEnabled() const;
  // is the button enabled?

  virtual int getPreferences(int what);
  // respods to the following preference IDs:  SUGGESTED_W, SUGGESTED_H

  virtual int userDown() { return userdown; }
  // is the button being held down by the user?

  virtual int wantClicks() { return getEnabled(); }
  // will we accept mouse clicks?

  virtual void freeResources();
  virtual void reloadResources();
  // resource management.  hands off.

  virtual BOOL mouseInRegion(int x, int y);
  // return true if the mouse is over you.



// ===========================================================================
//
//	EXAMPLECODEGUI: TextBar Reference
//
//  TextBar is inherited from VirtualWnd
//
//  TextBar uses the BaseWnd name field of the object as the text to be displayed.


  // --------------------------------------------
  // Public Methods

  int setInt(int i);
  // set the displayed text (and the name) to an ascii numeric value
  
  int setTextSize(int newsize);
  // set the point size of the text to be displayed

  int getTextWidth();
  int getTextHeight();
  // get the pixel extents for the current displayed text

  void setUseBaseTexture(int u);
  // use the base texture as your background (default: off)

  void setCenter(int center);
  BOOL getCentered();
  // centers the text in the bounding rect (otherwise, left justifies)

  // the following three options have ascending overriding priority --

  void setTextShadowed(int settextshadowed);
  int getTextShadowed();
  // shadows the text with the "bgcolor" value.

  void setTextOutlined(int settextoutlined);
  int getTextOutlined();
  // outlines the text with the "bgcolor" value.

  void setDrawBox(int setdrawbox);
  int getDrawBox();
  // draws a box of bgcolor that fills the bounding rect (otherwise, transparent)

  
  void setAutoToggleCheckWnd(CheckWnd *target);
  // links a tetxbar to a checkbox window
  

  // --------------------------------------------
  // Virtual Methods - If you override them, call the parent method.

  virtual int onPaint(Canvas *canvas);
  // how to draw some text?

  virtual void onSetName();
  // your text has been changed

  virtual int onLeftButtonDown(int x, int y);
  // used by the AutoToggleCheckWnd to allow you to let the clicking
  // on this text item cause a checkbox to turn on and off.




#endif// Reference Stuff
