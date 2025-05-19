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
//      File:     ExampleHTTPXMLWnd.h
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


#ifndef _EXAMPLEHTTPXMLWND_H
#define _EXAMPLEHTTPXMLWND_H

//
// This is our parent class: TabSheet from "../../common/tabsheet.h"
// If you are including more than just the header to your parent class
// in your headers, you need to think carefully about why you are doing so.
#include "../../bfc/wnds/tabsheet.h"

//
//  Forward References
class EditWnd;
class TreeWnd;
class ContWnd;
class UrlGrab;
class TextBar;
class ButtWnd;

//
//  Class Definition
#define EXAMPLEHTTPXMLWND_PARENT TabSheet
class ExampleHTTPXMLWnd : public EXAMPLEHTTPXMLWND_PARENT {
public:

  //
  // These are the ID values that we give to our
  // buttons in order to know what the hell got
  // pushed when we get told something got pushed.
  typedef enum {
    EXHTTPXML_PREVIOUS      = 0,
    EXHTTPXML_PLAY          = 1,
    EXHTTPXML_PAUSE         = 2,
    EXHTTPXML_STOP          = 3,
    EXHTTPXML_NEXT          = 4,
    EXHTTPXML_XML_GO        = 5,
    EXHTTPXML_XML_URL       = 6,
    //
    NUM_EXHTTPXML_NOTIFY
  } ExampleHTTPXMLWindowNotifyIDs;

  //
  // These constants control the number of pixels to offset the UI Objects
  // from the top-left of their container window (see the .CPP file)
  typedef enum {
    EXHTTPXML_LEFT_OFFSET   = 11,
    EXHTTPXML_TOP_OFFSET    = 10,
    EXHTTPXML_BUTTON_WIDTH  = 60,
    EXHTTPXML_BUTTON_HEIGHT = 25,
    EXHTTPXML_BUTTON_SPACER = 3,
    EXHTTPXML_XML_BUFFSIZE  = 2000,
    EXHTTPXML_TIMER_ID      = 0x0D0D0D0D,
    EXHTTPXML_TIMER_DUR     = 200,
    //
    NUM_EXHTTPXML_VALUES
  } ExampleHTTPXMLValues;

  //
  // These constants describe the different states of downloading a file
  // using UrlGrab and are used to control the state of the display.
  typedef enum {
    EXHTTPXML_DOWNLOAD_WAIT   = 0,
    EXHTTPXML_DOWNLOAD_READY  = 1,
    EXHTTPXML_DOWNLOAD_ERROR  = 2,
    //
    NUM_EXHTTPXML_DOWNLOAD_STATES
  } ExampleHTTPXMLDownloadStates;

public:

  // We commit the tasks of 1) Instantiation and 2) Assembly in the 
  // CONSTRUCTOR of the containment class.  The third primary task to occur 
  // before normal framelooped behaviour, Initialization, occurs AUTOMATICALLY
  // for ALL underived contained objects by the baseclass onInit() handler.
  ExampleHTTPXMLWnd();
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
  //  Methods used primarily by ExampleHTTPXMLWnd.cpp
  //
  // Go create all the windows into the tabsheet.
  int createChildWindows();     

  // =========================================================================
  //
  //  Methods used primarily by ExampleHTTPXMLFrameWnd.cpp
  //
  // Make some frames and process some XML.  Send urls to the minibrowser.
  int createFramesSheet();  
  // Use the string contained in the edit field as an url to get an XML document.
  void startURLDownload();
  // Use the string contained in currentUrl to launch the minibrowser.
  void startURLMinibrowser();
  // Fetch the string from the edit field in a more friendly manner than buffer copies.
  const String getXMLURL();
  // Process the state of our XML download.
  void pollDownloadState();
  // Put up the error box into the display region.
  void displayDownloadError();
  // Put up the 'Please wait' box into the display region
  void displayDownloadWait();
  // Parse the file.  Setup the display.
  void displayXMLData();
  // Set the text in the display field.
  void displayText(const char * text);
  // Set the text in a button that can be clicked (instead of the display field)
  void displayButton(const char * text);


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
  //  Data values used primarily by ExampleHTTPXMLFrameWnd.cpp
  //
  // Oh goody.  We get to carry around a fixed size buffer!  Joy!   GRUMP!
  char        myXMLURLBuffer[ EXHTTPXML_XML_BUFFSIZE ];
  // We need to keep the pointers to some objects in order to more easily
  // interact with them in a positive and supportive manner.
  EditWnd *   xmlurledit;
  TreeWnd *   myFrameTreeWnd;
  ContWnd *   myFrameContWnd;
  TextBar *   myFrameTextBar;
  ButtonWnd *   myFrameButtWnd;
  UrlGrab *   myUrlGrab;
  String      currentUrl;
  // Since we're downloading a file and then processing it, we set up a
  // cooperatively asynchronous state machine to handle the different
  // state transitions of the concept of a file download via HTTP.
  // This is the variable that holds the current state of the machine.
  ExampleHTTPXMLDownloadStates    myDownloadState;

};

#endif //_EXAMPLEHTTPXMLWND_H
