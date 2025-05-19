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
//      File:     ExampleHTTPXMLFrameWnd.cpp
//
//!##   Purpose:  This source module contains the functions for the 
//!##             construction of the XML Document Display sheet of 
//!##             the ExampleHTTPXML project.
//
//      Requires: Please read ExampleBWnd.cpp first.
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

#include "ExampleHTTPXMLWnd.h"

#include "../../common/framewnd.h"
#include "../../common/contwnd.h"
#include "../../common/textbar.h"
#include "../../common/editwnd.h"
#include "../../common/xlatstr.h"
#include "../../bfc/wnds/treewnd.h"
#include "../../common/urlgrab.h"

#include "../../bfc/wnds/buttwnd.h"

#include "../../studio/xmlreader.h"
#include "../../studio/xmlmanager.h"
#include "../../studio/xmlmanagedobj.h"

//
// Compilation-control #defines
#define USE_LOCAL_FILE      0
// I've had to implement a great deal of this code without access to the
// internet.  So, for my testing (and your own edification), you can flip
// the above switch on and it will use the local file in the path below.

//
// Constants and Globals.
#if     USE_LOCAL_FILE
// If we're using a local file, this is the path to it.
const char * xmlTempFilename = "wacs\\xml\\ExampleHTTPXML\\slashdot.xml";
#else //USE_LOCAL_FILE
const char * xmlTempFilename = "exb-temp.xml";
#endif//USE_LOCAL_FILE

// ===========================================================================
//
//  ExampleHTTPXML: class ExampleHTTPXMLFrameXmlBase
//
// *** This object shows how to extend the TreeItem class to be fully and 
// automatically constructed to replicate the data hierarchy of an XML file.
//
// Neat, eh?
//
// Now do you understand why we used a template for XmlManager?
//
// Can you think of other GUI items that would be fun to extend to be created
// fully formed from an XML file?
class ExampleHTTPXMLFrameXmlBase : public TreeItem {
public:
  ExampleHTTPXMLFrameXmlBase(TreeWnd * theTree, ExampleHTTPXMLWnd * theDisplay) : 
      tree(theTree), display(theDisplay), TreeItem() {}

  //
  //  Here's a tree item method we override
  // 
  virtual void onSelect() {
    //
    // ***  This is where you process the act of someone selecting
    //      this item in the tree.  It is left as an exercize to the
    //      reader to determine if this is a node covered by the DTD
    //      document of the XML file and to process it as an actual
    //      document.  I just cheat by checking to see if the tag name
    //      is "url"
    if (isURL) {
      display->displayButton(data);
    } else {
      display->displayText(data);
    }
    // it works without this.  that doesn't mean it shouldn't be here.
    TreeItem::onSelect();
  }

  //
  //  And the rest of these are just method extensions to be able
  //  to provide element-node matching using the XML manager.
  //

  // It is important that xmlTestPath create the proper new object.
  virtual ExampleHTTPXMLFrameXmlBase * xmlTestPath(const char * xmlpath, const char * xmltag) {
    //
    // ***  For our example, we're only using a single xml managed class
    //      (which, for those of you not paying attention, is this class).
    //      I would like to take the time, however, to note that this
    //      function is where the parent xml element node can determine
    //      what type of object is to be created for the child xml element,
    //      based off of the xmlpath and xmltag information.
    //
    //      For instance, I could just as easily have implemented another
    //      object class specifically to handle elements named "url" rather
    //      than make it an option of this class.  If you have numerous
    //      disparate objects to be created from your own xml file, or
    //      you're out to handle the DTD doc of my file, you'd create
    //      your different objects here.
    
    // Here, we create a new version of this subclass and pass it back.
    ExampleHTTPXMLFrameXmlBase * newNode = new ExampleHTTPXMLFrameXmlBase(tree,display);
    // Attach our tree connections.
    tree->addTreeItem(newNode, this, FALSE, TRUE);
    // Make it the new active element.
    return newNode;
  }

  virtual void xmlSetTag(const char * xmlpath, const char * xmltag) {
    // Set its label
    setLabel(xmltag);
    if  (
          ( STRCMP(xmltag,"url") == 0 ) ||
          ( STRCMP(xmltag,"link") == 0 )
        ) {                             // so, just to be lame, let's 
      isURL = 1;                        // do something different if the
    } else {                            // tag name is "url" or "link"
      isURL = 0;
    }
  }

  virtual void xmlSetData(const char * xmlpath, const char * xmltag, const char * xmldata) {
    data = xmldata;
  }

  virtual void xmlSetParams(const char * xmlpath, const char * xmltag, XmlReaderParams * xmlparams) {
    params = *xmlparams; // note that we're assigning from one class type to another.  
  }

  virtual void xmlComplete(const char * xmlpath, const char * xmltag) {
    // We do nothing!  We don't care!  Hedonism, baby... pure hedonism.
  }
public:
  TreeWnd *     tree;
  ExampleHTTPXMLWnd * display;
  String        data;
  XmlStrParams  params;

  int           isURL;
};

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::createFramesSheet"
//
int ExampleHTTPXMLWnd::createFramesSheet() {  
  // Make some frames and process some XML.

  // Okay, so, frames are kewl, right?
  // Hellyar!  Frames are groovy!
  //      Let's start cooking on them!

  // Instantiate our top level window
  FrameWnd * horiz = new FrameWnd();
  horiz->setName(_("Frames"));  // Remember, the name of the object is the
                                // name on the tab in the stabsheet buttonbar.
  //
  //  *** Important Options for FrameWnd.
  //
  // Set some preinitialization parameters, like which way to divide,
  horiz->setDividerType(DIVIDER_HORIZONTAL);
  // Setting the object resizable makes a divider get created that's moveable
  horiz->setResizeable(TRUE);
  // And this simply allows us to set the divider position to whatever we'd like it to be.
  horiz->setDividerPos(SDP_FROMTOP, 50);

  //////////////////////////////////////////////
  // *** Setup the TOP frame to contain the GUI 
  //     to specify an url to an XML document.
  //////////////////////////////////////////////
  
    // First, we'll make a container window and stuff it on in.
    ContWnd *container = new ContWnd();
    // In it, we'll put:

    //
    // 1) A 'Go' button.
    //
    ButtonWnd *go = new ButtonWnd();
    go->setButtonText(_("Go"), 16);
    go->setNotifyId(EXHTTPXML_XML_GO);
    go->setNotifyWindow(this);
    container->addChild(go, EXHTTPXML_LEFT_OFFSET, (EXHTTPXML_TOP_OFFSET * 2),
      EXHTTPXML_BUTTON_WIDTH, EXHTTPXML_BUTTON_HEIGHT, 1);

    //
    // 2) A text entry field.
    //
    xmlurledit = new EditWnd();
    xmlurledit->setName(_("This is now a text edit field."));
    container->addChild(xmlurledit, 
      EXHTTPXML_LEFT_OFFSET + EXHTTPXML_BUTTON_WIDTH + EXHTTPXML_BUTTON_SPACER,
      (EXHTTPXML_TOP_OFFSET * 2), EXHTTPXML_BUTTON_WIDTH * 6, EXHTTPXML_BUTTON_HEIGHT, 1);

    //
    //  Now, after we add the text entry field, we fill it in with interesting data.
    STRNCPY(myXMLURLBuffer,"http://www.slashdot.org/slashdot.xml", EXHTTPXML_XML_BUFFSIZE); // In this case, slashdot's XML headlines file.
    xmlurledit->setBuffer(myXMLURLBuffer, EXHTTPXML_XML_BUFFSIZE);
    //
    // *** ExampleB now uses the new EditWndString object I created to show
    // how to extend our user interface objects.  You might want to give it
    // a peek:  "../../common/editwndstring.cpp"

  //////////////////////////////////////////////
  // *** Setup the BOTTOM frame to have a 
  //     vertical frame with XML data display
  //////////////////////////////////////////////

    // Instantiate
    FrameWnd *vert = new FrameWnd();
    //
    //  Again, a framewind always has two children.  
    //  One on each side of the frame.          (duh!)
    //

    //
    // And set the frame options.
    vert->setDividerType(DIVIDER_VERTICAL);
    vert->setResizeable(TRUE);
    vert->setDividerPos(SDP_FROMLEFT, 50);

    //
    //  So now we make the child windows in prep for adding them to mr vert:

    // Make our tree.
    myFrameTreeWnd = new TreeWnd;

    myFrameContWnd = new ContWnd;

    // The initialization text for the display area.
    displayText(_("Press the 'GO' button to begin"));
    // displayText() will make sure a text object exists in the myFrameContWnd
    // displayButton() will make sure a buttonobject exists in the myFrameContWnd
    // each will remove the contents of myFrameContWnd when they are executed.

    //
    // And we add them, or set them, or something like that.
    vert->setChildren(myFrameTreeWnd, myFrameContWnd);

  //////////////////////////////////////////////
  // *** Set the two children into the main obj
  //////////////////////////////////////////////

    horiz->setChildren(container, vert);
    addChild(horiz, _("This contains framey-type shietznite."));

  // For the nonce, return codes from and to the API should
  //  return "0" for failure and "1" for success.
  return 1;
}

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::startURLDownload"
//
void ExampleHTTPXMLWnd::startURLDownload() {
  // In this function, we are loading the url given to us in the edit field and
  // hoping that the XML parser can make SOME sort of sense out of whatever document
  // we can find.
  OutputDebugString(_("\n\n*************************************\n"));
  OutputDebugString(_("Now I am refreshing the XML URL....\n"));

  xmlurledit->getBuffer(myXMLURLBuffer, EXHTTPXML_XML_BUFFSIZE);

  OutputDebugString(_("\n And this is the url I'm going to try to be getting: "));
  OutputDebugString(myXMLURLBuffer);

#if     USE_LOCAL_FILE
  displayXMLData();
#else //USE_LOCAL_FILE
  // So.  Anyhow.  We've got an url.  Start downloading it.
  // *** UrlGrab is an interface to an asynchronous HTTP downloader.

  // First, if we're currently downloading anything, knock that shit off right now.
  if (myUrlGrab != NULL) {
    // yes, deleting terminates the download.
    // what, do you think we're morons or something?
    delete myUrlGrab; 
    myUrlGrab = NULL;
    // Kill the timer (even tho we set it below, always balance set and kill calls)
    killTimer(EXHTTPXML_TIMER_ID);
  }
  // don't forget to delete the tempfile.
  api->fileRemove(xmlTempFilename);
  // okay, so, urlgrab is off...
  myUrlGrab = new UrlGrab(myXMLURLBuffer, xmlTempFilename );
  // ...and running.
  pollDownloadState();  // "mommy are we THERE yet?"
  // So set the timer to start polling.
  setTimer(EXHTTPXML_TIMER_ID, EXHTTPXML_TIMER_DUR);
#endif//USE_LOCAL_FILE

  //
  // Okay, anyhow, enough talking.  Let's do it!  XML AHOY!
  //
  OutputDebugString(_("\n*** DONE refreshing the XML URL....\n"));
}

// ===========================================================================
//
//  ExampleHTTPXML: "ExampleHTTPXMLWnd::pollDownloadState"
//
void ExampleHTTPXMLWnd::pollDownloadState() {
  // not testing for this would be "bad," right?
  if (myUrlGrab != NULL) {
    int urlgrabstate = myUrlGrab->run();  
    //
    ExampleHTTPXMLDownloadStates newDownloadState;
    // While this is a lame way to use the return value from run()
    // as the current state of our download machine, it is an excellent
    // means of showing what each return value of run() represents.
    switch (urlgrabstate) {
      case -1:
        newDownloadState = EXHTTPXML_DOWNLOAD_ERROR;
      break;
      case  0:
        newDownloadState = EXHTTPXML_DOWNLOAD_WAIT;
      break;
      case  1:
        newDownloadState = EXHTTPXML_DOWNLOAD_READY;
      break;
      default:
        // Since we already have the internal concept for an error,
        // it makes it easy to handle other things... like unexpected
        // return codes from functions.
        newDownloadState = EXHTTPXML_DOWNLOAD_ERROR;
      break;
    }
    // or something.  :P
    
    // If our status has changed, reflect that fact.
    if (myDownloadState != newDownloadState) {
      // Wow!  TWO switch statements in ONE function!  I never seen THAT before, man!
      switch (newDownloadState) {
        case EXHTTPXML_DOWNLOAD_ERROR:
          // Delete the current UrlGrabber.
          delete myUrlGrab;
          myUrlGrab = NULL;
          // Kill the timer 
          killTimer(EXHTTPXML_TIMER_ID);
          // Put up the error box into the display region.
          displayDownloadError();
        break;
        case EXHTTPXML_DOWNLOAD_WAIT:
          // Put up the 'Please wait' box into the display region
          displayDownloadWait();
        break;
        case EXHTTPXML_DOWNLOAD_READY:
          // Delete the current UrlGrabber.
          delete myUrlGrab;
          myUrlGrab = NULL;
          // Kill the timer 
          killTimer(EXHTTPXML_TIMER_ID);
          // Parse the file.  Setup the display.
          displayXMLData();
        break;
      }
      myDownloadState = newDownloadState;
    }
  }
}

// Put up the error box into the display region.
void ExampleHTTPXMLWnd::displayDownloadError() {
  displayText( _("Error downloading file from url.  You bozo.") );
}
// Put up the 'Please wait' box into the display region
void ExampleHTTPXMLWnd::displayDownloadWait() {
  displayText( _("Please wait while the file is downloaded.....") );
}
// Parse the file.  Setup the display.
void ExampleHTTPXMLWnd::displayXMLData() {
  // Clear the display
  displayText("");
  //
  // Now we setup tree items with the magic of OOP-XML!
  //
  // *** Read these, damn you.
  //
  // Firstly, clear whatever was in the tree, previously.
  myFrameTreeWnd->deleteAllItems();
  // Create a new single root node which will hold the constructed node-tree.
  ExampleHTTPXMLFrameXmlBase * rootNode = new ExampleHTTPXMLFrameXmlBase(myFrameTreeWnd, this);
  // Give it a label -- we'll use the downloaded url.
  rootNode->setLabel(myXMLURLBuffer);
  // Add it into the treeWnd
  myFrameTreeWnd->addTreeItem(rootNode, NULL, FALSE, TRUE);
  // Then instantiate and construct a new XmlManager to control the parsing.
  XmlManager<ExampleHTTPXMLFrameXmlBase> myXmlManager(rootNode);
  // Then, simply enough, we register the callback and read the file.
  XmlReader::registerCallback("*",&myXmlManager);
  // How can anything in the world be easier than this?    :P  God, I love OOP.
  XmlReader::loadFile(xmlTempFilename);
}


// Make sure there's a text bar.  Get rid of the button if it's there.
void ExampleHTTPXMLWnd::displayText(const char * text) {
  // Remove whatever's there, we're changing it.
  myFrameContWnd->deleteAll();
  myFrameTextBar = NULL;  
  myFrameButtWnd = NULL;

  // Make a text bar to display normal XML element data
  myFrameTextBar = new TextBar;
  myFrameTextBar->setName(text);

  // Add the object to the container with the specified boundaries.
  myFrameContWnd->addChild(myFrameTextBar, EXHTTPXML_LEFT_OFFSET, 
    EXHTTPXML_BUTTON_HEIGHT + EXHTTPXML_BUTTON_SPACER,
    EXHTTPXML_BUTTON_WIDTH * 6, EXHTTPXML_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // *** IF YOUR PARENT-OBJECT IN THE CONSTRUCTION HIERARCHY HAS ALREADY
  // BEEN INITIALIZED, YOU ***MUST*** MANUALLY INITIALIZE ANY OBJECTS
  // YOU ADD TO IT LATER.
  if (myFrameContWnd->isInited()) {
    myFrameTextBar->init(myFrameContWnd);
  }
    
  // You don't actually have to invalidate the contwnd after adding an item
  // to it, but it's a good habit to invalidate all UI objects if you make
  // changes to them any time after they have been initialized.
  myFrameContWnd->invalidate();
  // If you find that something isn't displaying properly, try invalidating
  // it.  If that doesn't work, and it's in an unmodified UI object
  // (ie: our code, not your code), it may be a bug.  Feel free to report it.
}

// Make sure there's a button.  Get rid of the text bar if it's there.
void ExampleHTTPXMLWnd::displayButton(const char * text) {
  // Remove whatever's there, we're changing it.
  myFrameContWnd->deleteAll();
  myFrameTextBar = NULL;  
  myFrameButtWnd = NULL;

  // Make a button to display the ability to launch an URL
  myFrameButtWnd = new ButtonWnd;
  myFrameButtWnd->setButtonText(text, 16);
  myFrameButtWnd->setNotifyId(EXHTTPXML_XML_URL);
  myFrameButtWnd->setNotifyWindow(this);

  // Add the object to the container with the specified boundaries.
  myFrameContWnd->addChild(myFrameButtWnd, EXHTTPXML_LEFT_OFFSET, 
    EXHTTPXML_BUTTON_HEIGHT + EXHTTPXML_BUTTON_SPACER,
    EXHTTPXML_BUTTON_WIDTH * 6, EXHTTPXML_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

  // *** IF YOUR PARENT-OBJECT IN THE CONSTRUCTION HIERARCHY HAS ALREADY
  // BEEN INITIALIZED, YOU ***MUST*** MANUALLY INITIALIZE ANY OBJECTS
  // YOU ADD TO IT LATER.
  if (myFrameContWnd->isInited()) {
    myFrameButtWnd->init(myFrameContWnd);
  }
  
  // You don't actually have to invalidate the contwnd after adding an item
  // to it, but it's a good habit to invalidate all UI objects if you make
  // changes to them any time after they have been initialized.
  myFrameContWnd->invalidate();
  // If you find that something isn't displaying properly, try invalidating
  // it.  If that doesn't work, and it's in an unmodified UI object
  // (ie: our code, not your code), it may be a bug.  Feel free to report it.

  // Lastly, this is the url we're going to launch if the button is pushed.
  currentUrl = text;
}

// Hey, our button just got hit.  If there actually is a string, use it.
void ExampleHTTPXMLWnd::startURLMinibrowser() {
  if ((currentUrl.getValue() != NULL) && (STRCMP(currentUrl,"") != 0)){
    api->navigateUrl(currentUrl);
  }
}

//
// But, but...  This file just looks so empty without a comment at the end