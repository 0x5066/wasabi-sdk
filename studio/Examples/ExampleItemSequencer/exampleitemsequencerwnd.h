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
//      File:     ExampleItemSequencerWnd.h
//
//!##   Purpose:  Define the Component's main window class for our example
//
//      Requires: Please read ExampleBwnd.h first.
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


#ifndef _EXAMPLEITEMSEQUENCERWND_H
#define _EXAMPLEITEMSEQUENCERWND_H

//
// This is our parent class: TabSheet from "../../common/tabsheet.h"
// If you are including more than just the header to your parent class
// in your headers, you need to think carefully about why you are doing so.
#include "../../bfc/wnds/tabsheet.h"

// (sigh) unfortunately, you can't forward reference a typedef.
#include "../../studio/charptrsequence.h"

//
//  Forward References
class EditWndString;
class TreeWnd;
class StylePosWnd;
class UrlGrab;
class TextBar;
class ButtWnd;
class XmlStrParams;
class ExampleItemSequencerTreeItem;

//
//  Class Definition
#define EXAMPLEITEMSEQUENCERWND_PARENT TabSheet
class ExampleItemSequencerWnd : public EXAMPLEITEMSEQUENCERWND_PARENT {
public:

  //
  // These are the ID values that we give to our
  // buttons in order to know what the hell got
  // pushed when we get told something got pushed.
  typedef enum {
    EXITEMSEQ_URL_GO        = 1,
    EXITEMSEQ_ENQUEUE_ALL   = 2,
    EXITEMSEQ_ENQUEUE       = 3,
    EXITEMSEQ_DOWNLOAD      = 4,
    //
    NUM_EXITEMSEQ_NOTIFY
  } ExampleItemSequencerWindowNotifyIDs;

  //
  // These constants control the number of pixels to offset the UI Objects
  // from the top-left of their container window (see the .CPP file)
  typedef enum {
    EXITEMSEQ_LEFT_OFFSET   = 11,
    EXITEMSEQ_TOP_OFFSET    = 10,
    EXITEMSEQ_BUTTON_WIDTH  = 60,
    EXITEMSEQ_BUTTON_HEIGHT = 25,
    EXITEMSEQ_BUTTON_SPACER = 3,
    EXITEMSEQ_TOP_FRAME     = 75,
    EXITEMSEQ_LEFT_FRAME    = 125,
    EXITEMSEQ_URL_BUFFSIZE  = 2000,
    EXITEMSEQ_TIMER_ID      = 0x0E0E0E0E,
    EXITEMSEQ_TIMER_DUR     = 200,          // This is how long to delay while polling urlGrab.
                                          // If you make this value too small, you may starve
                                          // music playback.  If you make it too big, your
                                          // downloads will go slow.  Your choice, pal.
    //
    NUM_EXITEMSEQ_VALUES
  } ExampleItemSequencerValues;

  //
  // These constants describe the different states of downloading a file
  // using UrlGrab and are used to control the state of the display.
  typedef enum {
    EXITEMSEQ_DOWNLOAD_WAIT   = 0,
    EXITEMSEQ_DOWNLOAD_READY  = 1,
    EXITEMSEQ_DOWNLOAD_ERROR  = 2,
    //
    NUM_EXITEMSEQ_DOWNLOAD_STATES
  } ExampleItemSequencerDownloadStates;

public:

  ExampleItemSequencerWnd();
  ~ExampleItemSequencerWnd();

  // =========================================================================
  //
  //  Methods required by Window Creation Services
  //
  static const char *getWindowTypeName();
  static GUID getWindowTypeGuid();
  static void setIconBitmaps(ButtonWnd *button);

  // =========================================================================
  //
  //  Methods used primarily by ExampleItemSequencerWnd.cpp
  //
  // Go create all the windows into the tabsheet.
  int createChildWindows();     

  // =========================================================================
  //
  //  Methods used primarily by ExampleItemSequencerFrameWnd.cpp
  //
  // Make some frames and process some URL.  Send urls to the minibrowser.
  int createFramesSheet();  
  // Use the string contained in the edit field as an url to get an URL document.
  void startURLDownload();
  // Use the string contained in currentUrl to launch the minibrowser.
  void startURLMinibrowser();
  // Fetch the string from the edit field in a more friendly manner than buffer copies.
  const String getUrl();
  // Process the state of our URL download.
  void pollDownloadState();
  // Put up the error box into the display region.
  void displayDownloadError();
  // Put up the 'Please wait' box into the display region
  void displayDownloadWait(int amount_downloaded = 0);
  // Parse the file.  Setup the display.
  void parseHTMLLinks();
  // Set a simple line of text in the display field.
  void displayText(const char * text);
  // Set the HREF info in the display field.
  void displayLink(String &link, String &label);
  // Check the given tag to see if it is a valid "a" element (ie: <a href="www.winamp.com">)
  int testValidATag(const String &tag);
  // Add the link info to the tree.
  void addLinkToTree(const String &tag, const String &label);
  // Add the "enqueue all" button.
  void addEnqueueAll();
  // Remove the "enqueue all" button.
  void removeEnqueueAll();
  // Actually do the enqueueing of all.
  void enqueueAll();
  // Parse the extensions string
  void parseExtensionsString();
  // Test an url to have a valid extension
  int testValidExtension(String link);
  // Play the given string through the sequencer
  int playSequencedString(String link);

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
	virtual void timerCallback(int id);
	virtual int onInit();

protected:

  // =========================================================================
  //
  //  Data values used primarily by ExampleItemSequencerFrameWnd.cpp
  //
  // We need to keep the pointers to some objects in order to more easily
  // interact with them in a positive and supportive manner.
  EditWndString * urledit;
  EditWndString * extedit;
  TreeWnd * myFrameTreeWnd;
  StylePosWnd * myTopStylePosWnd;
  StylePosWnd * myRightStylePosWnd;
  TextBar * myFrameTextBar;
  ButtonWnd * myFrameButtWnd;
  UrlGrab * myUrlGrab;
  String currentUrl;
  PtrList< String > extList;
  ButtonWnd * myEnqueueAllButton;
  CharPtrSequencer<1> *mySequencer;
  PtrList< ExampleItemSequencerTreeItem > myTreeItemList;

  // Since we're downloading a file and then processing it, we set up a
  // cooperatively multitasked state machine to handle the different
  // state transitions of the concept of a file download via HTTP.
  // This is the variable that holds the current state of the machine.
  ExampleItemSequencerDownloadStates    myDownloadState;
};

#endif //_EXAMPLEITEMSEQUENCERWND_H
