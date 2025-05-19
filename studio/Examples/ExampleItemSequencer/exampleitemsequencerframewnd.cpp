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
//      File:     ExampleItemSequencerFrameWnd.cpp
//
//!##   Purpose:  This source module contains the functions for the 
//!##             construction of the URL Document Display sheet of 
//!##             the ExampleItemSequencer project.
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

#include "ExampleItemSequencerWnd.h"

#include "../../common/framewnd.h"
#include "../../common/styleposwnd.h"
#include "../../common/textbar.h"
#include "../../common/editwndstring.h"
#include "../../common/xlatstr.h"
#include "../../common/urlgrab.h"
#include "../../common/corehandle.h"

#include "../../bfc/wnds/treewnd.h"
#include "../../bfc/wnds/buttwnd.h"

#include "../../studio/xmlstrparams.h"
#include "../../studio/charptrsequence.h"


//
// Globals and Constants
const int maxFileParseSize = 300000; // Files that are too big will take forever.
                                     // A file this big will take forever to parse, too.
                                     // Why?  Because I wrote a stupid parser, on purpose.
                                     // Keep reading.  It'll make painful sense, soon.
//
// Compilation-control #defines
#define USE_LOCAL_FILE      0

//
// Constants and Globals.
#if     USE_LOCAL_FILE
// If we're using a local file, this is the path to it.
const char * urlTempFilename = "I don't want to use a local file, anymore.";
#else //USE_LOCAL_FILE
const char * urlTempFilename = "exb-temp.url";
#endif//USE_LOCAL_FILE

// ===========================================================================
//
//  ExampleItemSequencer: class ExampleItemSequencerTreeItem
//
class ExampleItemSequencerTreeItem : public TreeItem {
public:
  ExampleItemSequencerTreeItem(TreeWnd * theTree, ExampleItemSequencerWnd * theDisplay, const String &theTag, const String &theLabel) : 
      tree(theTree), display(theDisplay), tag(theTag), data(theLabel), params(), TreeItem() {
    parseStrParams();
  }

  //
  //  Here's a tree item method we override
  virtual void onSelect() {
    // ***  This is where you process the act of someone selecting
    //      this item in the tree.  We should pass our info to the
    //      display wnd to spit out in graphical form in the other
    //      part of the frame.
    
    // All caps.  But only because we called toupper() on the tags as we
    // added them to the params object, below.
    String link = getLinkString();  
    
    display->displayLink(link, data);
    display->playSequencedString(link);

    // it works without this.  that doesn't mean it shouldn't be here.
    TreeItem::onSelect();
  }

  void parseStrParams() {
    // The link that we want to pull from the tag is in a parameter
    // form and uses the keyword "HREF" as its key with the link as
    // its value.  Because the href could be any parameter of the
    // tag, and because I'm supposed to be showing you how to use our
    // objects, I'm going to parse the whole thing into an XmlStrParams
    // object and then just query that object in the code above.
    
    // The first thing we're going to do is make a cheater's string.
    // This string contains all the ASCII character values for whitespace.
    char whitespace[] = {
      ' ',   // space (0x20)
      0x09,  // tab (Ctrl-I)
      0x0a,  // linefeed (Ctrl-J)
      0x0d,  // carriage return (Ctrl-M)
      0 // null terminate the string.
    };
    // We can then use this string with the SpliceChar method to
    // efficiently seperate out sections of the string that may
    // have one-or-more whitespace characters between them.
    
    // It's because of dumb, simplistic code like this that you don't want
    // to have spaces in your urls.  Right?  Remember, we're not writing
    // a real HTML parser, we're just showing off our object methods.
    do {
      // Anyhow, we will splice on whitespace until we pull more than 1 char.
      String &value = tag.lSpliceChar(whitespace);
      // I like to call this "whitesplicing" because it's a halfrhyme and because
      // making up words as phrases is true to the Germanic heritage of my language.

      // If we have 2 or more whitespace characters in a row, value.len() will be 0
      // as our splice method finds and removes each one.  When value.len() is
      // greater than 0, we know we have a real chunk of data to examine.
      if (value.len() > 0) {

        // We are expecting all of the strings we whitesplice to have at least
        // one '=' char, the leftmost acts as the separator between key and value.
        // And we're not doing much work checking for whether or not that assumption
        // is valid. We just use the splice method to split key from value
        // and get rid of that infernal '=' char.
        String &key = value.lSpliceChar('=');
        // Note that the way splice works, if there is no '=' to be found, value
        // becomes empty and key contains the entire string.

        // We don't want our key searches to be case sensitive.
        // Therefore, we set all the key strings to uppercase and
        // use only uppercase queries on them. 
        key.toupper();

        //
        // And if there are quotes, remove them.  They annoy us.
        int end = value.len();
        // So, if we begin with a doublequote and are at least 2 chars long,
        if ((value.getValue()[0] == '\"') && (end > 1)) {
          // then we get rid of the ending quote
          // You'd better hope there's an ending quote.  :)
          value.getNonConstVal()[end-1] = 0;
          // Get rid of the beginning quote by passing the next
          // char into the params method.
          params.addItem(key.getValue(), value.getValue() + 1);          
        } else {
          params.addItem(key, value);
        }
      }
      // And we keep splicing whitespace until our tag string is empty.
    } while (tag.len());
  }

  String getLinkString() {
    // Here's the href parameter.
    String retval = params.getItemValue("HREF");

    // This is probably the wrong way to properly test if
    // the href is a local or global url.
    String temp = retval;
    String &protocol = temp.lSplitChar(':');

    // If there was a protocol string,
    if (temp.len() != 0) {
      // then it's probably a global url.
    } else {
      // otherwise we need to assemble a full url.

      // first, get the url from the text edit field
      String url = display->getUrl();

      // then determine if the item is local to the url or to the server
      // by checking to see if the first character is a foreslash or backslash
      if ((retval[0] == '/') || (retval[0] == '\\')) {
        // local to the server.
        String &protocol = url.lSpliceChar("/\\");
        String &slash = url.lSpliceChar("/\\");
        String &server = url.lSpliceChar("/\\");

        // assemble the real link string.
        retval = protocol + "//" + server + retval;
      } else {
        // local to the url

        // remove anything to the right of the rightmost slash (and the slash, too).
        url.rSplitChar("/\\");

        // assemble the real link string.
        retval = url + "/" + retval;
      }
    }
    return retval;
  }

public:
  TreeWnd * tree;
  ExampleItemSequencerWnd * display;
  String data;
  String tag;
  XmlStrParams params;
};

// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::createFramesSheet"
//
int ExampleItemSequencerWnd::createFramesSheet() {  
  // Make some frames and process some HTML, this time.

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
  horiz->setDividerPos(SDP_FROMTOP, EXITEMSEQ_TOP_FRAME);

  //////////////////////////////////////////////
  // *** Setup the TOP frame to contain the GUI 
  //     to specify an url to an URL document.
  //////////////////////////////////////////////
  
    // First, we'll make a styled positioning window and stuff it on in.

    // Hey look... we have a new UIO.  The StylePosWnd.  It's kinda tricky.
    // *** PLEASE read "../../common/styleposwnd.h" to better understand how to
    // use this object.  It's rather complicated but rather useful.
    myTopStylePosWnd = new StylePosWnd();
    // In it, we'll put:

    //
    // 1) A 'Go' button.
    //
    ButtonWnd *go = new ButtonWnd();
    go->setButtonText(_("Go"), 16);
    go->setNotifyId(EXITEMSEQ_URL_GO);
    go->setNotifyWindow(this);
    myTopStylePosWnd->addChild(StylePosWnd::ChildInfo(go,
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_LEFT_OFFSET, StylePosWnd::LOCK_BORDER),   // Left
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::CENTER),      // Top
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_WIDTH, StylePosWnd::LOOSE),        // Right
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::CENTER)));    // Bottom
    // "Set your left edge 11 pixels to the left of the left border,
    //  set your top edge to create the y-axis centering of a 25 pixel high object,
    //  set your right edge to whatever is needed to make the object be 60 pixels wide,
    //  set your bottom edge to create the y-axis centering of a 25 pixel high object."

    //
    // 2) A text entry field.
    //
    urledit = new EditWndString();
    urledit->setName(_("This is an url edit field."));
    myTopStylePosWnd->addChild(StylePosWnd::ChildInfo(urledit,
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_SPACER, StylePosWnd::LOCK_RIGHT, go),// Left
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::LOOSE),      // Top
      StylePosWnd::ChildEdgeInfo(-EXITEMSEQ_LEFT_OFFSET-EXITEMSEQ_BUTTON_WIDTH-EXITEMSEQ_BUTTON_SPACER, StylePosWnd::LOCK_BORDER),  // Right
      StylePosWnd::ChildEdgeInfo(((EXITEMSEQ_BUTTON_HEIGHT/2)-(EXITEMSEQ_BUTTON_SPACER/2)), StylePosWnd::LOCK_TOP, go)));// Bottom
    // "Set your left edge 3 pixels to the left of the right edge of the go button,
    //  set your top edge loose to be a 25 pixel high object,
    //  set your right edge to the right of the right border with enough space for another button,
    //  set your bottom edge to be a few pixels below the top of the go button :)"

    //
    // 3) Another text entry field.
    //
    extedit = new EditWndString();
    extedit->setName(_("This is an ext edit field."));
    myTopStylePosWnd->addChild(StylePosWnd::ChildInfo(extedit,
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_SPACER, StylePosWnd::LOCK_RIGHT, go),// Left
      StylePosWnd::ChildEdgeInfo(-((EXITEMSEQ_BUTTON_HEIGHT/2)-(EXITEMSEQ_BUTTON_SPACER/2)), StylePosWnd::LOCK_BOTTOM, go),// Bottom
      StylePosWnd::ChildEdgeInfo(-EXITEMSEQ_LEFT_OFFSET-EXITEMSEQ_BUTTON_WIDTH-EXITEMSEQ_BUTTON_SPACER, StylePosWnd::LOCK_BORDER),  // Right
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
    // "Set your left edge 3 pixels to the left of the right edge of the go button,
    //  set your top edge to be a few pixels above the bottom of the go button,
    //  set your right edge to the right of the right border with enough space for another button,
    //  set your bottom edge loose to be a 25 pixel high object."

    // Pay attention to what happens to these objects when you change the dimensions
    // of the rectangular area defined by the top-frame.  You'll like it, I think.
    // StylePosWnd essentially allows you to not have to use 0,0 as the ONLY reference
    // point for the placement of user interface objects within it.  We'll see more of
    // this object later in this example and I'm sure I'll have it co-starring under a
    // tab in ExampleB someday soon.

    //
    //  Now, after we add the text entry field, we fill it in with interesting data.
    urledit->setBuffer("http://download.nullsoft.com/pub/music/", EXITEMSEQ_URL_BUFFSIZE);
    extedit->setBuffer(".mp3;.ogg;.wma;.pls;.m3u;I'm sure I forgot something!", EXITEMSEQ_URL_BUFFSIZE);


  //////////////////////////////////////////////
  // *** Setup the BOTTOM frame to have a 
  //     vertical frame with URL data display
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
    vert->setDividerPos(SDP_FROMLEFT, EXITEMSEQ_LEFT_FRAME);

    //
    //  So now we make the child windows in prep for adding them to mr vert:

    // Make our tree.
    myFrameTreeWnd = new TreeWnd;

    myRightStylePosWnd = new StylePosWnd;

    //
    // And we add them, or set them, or something like that.
    vert->setChildren(myFrameTreeWnd, myRightStylePosWnd);

  //////////////////////////////////////////////
  // *** Set the two children into the main obj
  //////////////////////////////////////////////

    horiz->setChildren(myTopStylePosWnd, vert);
    addChild(horiz, _("This contains framey-type shietznite."));

  // For the nonce, return codes from and to the API should
  //  return "0" for failure and "1" for success.
  return 1;
}

// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::startURLDownload"
//
void ExampleItemSequencerWnd::startURLDownload() {
  // So.  Anyhow.  We've got an url.  Start downloading it.
  // *** UrlGrab is an interface to an asynchronous HTTP downloader.

  // Clear the current tree.
  myFrameTreeWnd->deleteAllItems();

  // Remove the 'enqueue all' button.
  removeEnqueueAll();

  // Parse the second edit field for the extensions to filter.
  parseExtensionsString();

  // If we're currently downloading anything, knock that shit off right now.
  if (myUrlGrab != NULL) {
    // yes, deleting terminates the download.
    // what, do you think we're morons or something?
    delete myUrlGrab; 
    myUrlGrab = NULL;
    // And stop the timer!
    killTimer(EXITEMSEQ_TIMER_ID);
  }
  // don't forget to delete the tempfile.
  api->fileRemove(urlTempFilename);
  // okay, so, urlgrab is off...
  myUrlGrab = new UrlGrab(urledit->getBuffer(), urlTempFilename );
  // ...and running.
  pollDownloadState();  // "mommy are we THERE yet?"
  // So start the timer.
  setTimer(EXITEMSEQ_TIMER_ID, EXITEMSEQ_TIMER_DUR);
}

// ===========================================================================
//
//  ExampleItemSequencer: "ExampleItemSequencerWnd::pollDownloadState"
//
void ExampleItemSequencerWnd::pollDownloadState() {
  // not testing for this would be "bad," right?
  if (myUrlGrab != NULL) {

    int urlgrabstate = myUrlGrab->run();  
    //
    ExampleItemSequencerDownloadStates newDownloadState;
    // While this is a lame way to use the return value from run()
    // as the current state of our download machine, it is an excellent
    // means of showing what each return value of run() represents.
    switch (urlgrabstate) {
      case -1:
        newDownloadState = EXITEMSEQ_DOWNLOAD_ERROR;
      break;
      case  0:
        newDownloadState = EXITEMSEQ_DOWNLOAD_WAIT;
      break;
      case  1:
        newDownloadState = EXITEMSEQ_DOWNLOAD_READY;
      break;
      default:
        // Since we already have the internal concept for an error,
        // it makes it easy to handle other things... like unexpected
        // return codes from functions.
        newDownloadState = EXITEMSEQ_DOWNLOAD_ERROR;
      break;
    }
    // or something.  :P
    
    // If our status has changed, reflect that fact.
    if (myDownloadState != newDownloadState) {
      // Wow!  TWO switch statements in ONE function!  I never seen THAT before, man!
      switch (newDownloadState) {
        case EXITEMSEQ_DOWNLOAD_ERROR:
          // Delete the current UrlGrabber.
          delete myUrlGrab;
          myUrlGrab = NULL;
          // And stop the timer!
          killTimer(EXITEMSEQ_TIMER_ID);
          // Put up the error box into the display region.
          displayDownloadError();
        break;
        case EXITEMSEQ_DOWNLOAD_WAIT:
          // Put up the 'Please wait' box into the display region
          displayDownloadWait();
        break;
        case EXITEMSEQ_DOWNLOAD_READY:
          // Delete the current UrlGrabber.
          delete myUrlGrab;
          myUrlGrab = NULL;
          // And stop the timer!
          killTimer(EXITEMSEQ_TIMER_ID);
          // Parse the file.  Setup the display.
          parseHTMLLinks();
          // Add the 'enqueue all' button.
          addEnqueueAll();
        break;
      }
      myDownloadState = newDownloadState;
    } else if (myDownloadState == EXITEMSEQ_DOWNLOAD_WAIT) {
      // Update the display of how much we've downloaded.
      displayDownloadWait(myUrlGrab->filesize());
    }
  }
}

// Put up the error box into the display region.
void ExampleItemSequencerWnd::displayDownloadError() {
  displayText( _("Error downloading file from url.  You bozo.") );
}

// Put up the 'Please wait' box into the display region
void ExampleItemSequencerWnd::displayDownloadWait(int amount_downloaded) {
  StringPrintf downloadWait("Please wait while the file is downloaded.  [%d kb]", amount_downloaded / 1024 );
  displayText( _(downloadWait) );
}

// Parse the file.  Setup the display.
void ExampleItemSequencerWnd::parseHTMLLinks() {
  // Clear the display
  displayText("");

  // I guess we'll just read the entire file into a tempbuffer
  // and try to parse it.  (shrug)  What else can one do?

  //
  //  Okay, this is a candyass way to write this... if you try to load
  //  a binary file it's going to go waste a great deal of your time
  //  downloading a file before it tells you that you suck for giving 
  //  it a binary file.
  //
  //  It is left as an exercise to the reader to tighten this function
  //  up nice and prettylike.  This is just Q&D to get the job done and
  //  actually show off the stuff in our SDK.
  //
  FILE *htmlFile = FOPEN(urlTempFilename,"rb");
  FSEEK(htmlFile, 0, SEEK_END);
  int fileSize = FTELL(htmlFile);

  if (fileSize > maxFileParseSize) {
    displayText( _("Your file is too big!  Don't download binaries.") );
    FCLOSE(htmlFile);
    return;
  }

  char *fileBuffer = new char[fileSize+1];
  FSEEK(htmlFile, 0, SEEK_SET);
  int sizeRead = FREAD(fileBuffer,fileSize,1,htmlFile);
  fileBuffer[fileSize] = 0; // Null terminate.
  String fileString(fileBuffer); // Entire human readable file as a string.
  delete[] fileBuffer;
  FCLOSE(htmlFile);

  //
  // Okay, SO, now that we've got our happy-hippy file entirely stuffed into
  // a string, we're going to use our string parsing methods to burn through
  // a stupid quick and dirty parse of the information as if it were an
  // HTML document.
  //
  // So, yes, you're correct if you tell me this is a less-than-optimal way
  // to implement this functionality.  However, it works very nicely to introduce
  // some of the String-based parsing methods.
  //
  // Am I obviously defensive enough, yet, about peoples' comments about my
  // coding in the forums?  No?  Well, okay, I'll try harder to be more paranoid.
  //

  //
  //  *** For more info about these parsing methods, please read "bfc/string.h"
  //      and "teststring.dsw"
  //

  // Split the string, counting from the left, at the first '<'
  // and discard the information.
  fileString.lSplitChar('<');
  // This goes above the while loop to provide for the first test
  // against len (if there are no '<' characters, fileString is emptied).

  // Looped parsing ends when we run out of string.
  while (fileString.len()) {
    // Okay if we get here, we got a new tag.  Move the contents into
    // our local string to be tested to see if it is a link.
    //
    // lSplice will cause the closing '>' to be discarded.
    String &tag = fileString.lSpliceChar('>');
    // now tag contains all the characters between '<' and '>'

    // We use a reference to save a copy and preserve the string to the
    // end of the reference's scope.

    // If the tag tests true as being of type "a"
    // (ie: <a href="www.migvideo.com">)
    if (testValidATag(tag)) {
      // Okay, so, if we ARE a link-tag, slurp out the link label to the next
      // actual tag.
      String &label = fileString.lSpliceChar('<');
      // And, yes, if your target link has other tags in its label, 
      // this will cause something of a problem, right?  Oh well.
      // Again, feel free to make this perfect on your own time. :)
      
      // It's JUST example code, sirrah.  And don't you forget it.

      // If we know we SHOULD have a label and, yet, our label string
      // is empty, give it a default value so we can still see it in
      // the tree display.
      if (label.len() == 0) {
        label = "(empty)";
      }

      // Anyhow, now that we finally have a tag and a label, we can make
      // a tree item for us.
      addLinkToTree(tag, label);
    } else {
      // If it wasn't an 'A' tag, discard everything to the next < char
      fileString.lSplitChar('<');
    }
  }
}

// Check the given tag to see if it is a valid "a" element (ie: <a href="www.migvideo.com">)
int ExampleItemSequencerWnd::testValidATag(const String &tag) {
  // This is a function call because I might decide
  // to get more complicated.  but probably not.
  return (tag.getValue()[0] == 'A') || (tag.getValue()[0] == 'a');  
}

// If it passes the filters, add the link info to the tree.
void ExampleItemSequencerWnd::addLinkToTree(const String &tag, const String &label) {
  // First we make it, and it knows how to parse itself.
  ExampleItemSequencerTreeItem * newNode = new ExampleItemSequencerTreeItem(myFrameTreeWnd,this,tag,label);

  // Then we ask it to tell us its link, and check if that link passes the filter.
  if (testValidExtension(newNode->getLinkString()) == 0) {
    delete newNode;
    return;
  }

  // Record the pointer in our local list for easy access.
  myTreeItemList.addItem(newNode);

  // Attach our tree connections.
  myFrameTreeWnd->addTreeItem(newNode, NULL, FALSE, TRUE);
  newNode->setLabel(label);
}

// Make sure there's a text bar.  Get rid of the button if it's there.
void ExampleItemSequencerWnd::displayText(const char * text) {
  // Remove whatever's there, we're changing it.
  myRightStylePosWnd->deleteAll();
  myFrameTextBar = NULL;  
  myFrameButtWnd = NULL;

  // Make a text bar to display the text we wanna display.
  myFrameTextBar = new TextBar;
  myFrameTextBar->setName(text);

  // Add the object to the container with the specified boundaries.

  // Hey look... we have a new UIO.  The StylePosWnd.  It's kinda tricky.
  // *** PLEASE also read exampleBstylewnd.cpp to better understand how to
  // use this object.  It's rather complicated but rather useful.
  myRightStylePosWnd->addChild(StylePosWnd::ChildInfo(myFrameTextBar,
    StylePosWnd::ChildEdgeInfo(EXITEMSEQ_LEFT_OFFSET, StylePosWnd::LOCK_BORDER),   // Left
    StylePosWnd::ChildEdgeInfo(EXITEMSEQ_TOP_OFFSET, StylePosWnd::LOCK_BORDER),    // Top
    StylePosWnd::ChildEdgeInfo(-EXITEMSEQ_LEFT_OFFSET, StylePosWnd::LOCK_BORDER),  // Right
    StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
  // "Set your left edge 11 pixels inside of the left border,
  //  set your top edge 10 pixels below the top border,
  //  set your right edge 11 pixels inside of the right border,
  //  set your bottom edge to whatever is needed to make the object be 25 pixels high."

  // *** IF YOUR PARENT-OBJECT IN THE CONSTRUCTION HIERARCHY HAS ALREADY
  // BEEN INITIALIZED, YOU ***MUST*** MANUALLY INITIALIZE ANY OBJECTS
  // YOU ADD TO IT LATER.
  if (myRightStylePosWnd->isInited()) {
    myFrameTextBar->init(myRightStylePosWnd);
  }
    
  // You don't actually have to invalidate the contwnd after adding an item
  // to it, but it's a good habit to invalidate all UI objects if you make
  // changes to them any time after they have been initialized.
  myRightStylePosWnd->invalidate();
  // If you find that something isn't displaying properly, try invalidating
  // it.  If that doesn't work, and it's in an unmodified UI object
  // (ie: our code, not your code), it may be a bug.  Feel free to report it.
}


void ExampleItemSequencerWnd::displayLink(String &link, String &label) {
  // Remove whatever's there, we're changing it.
  myRightStylePosWnd->deleteAll();
  myFrameTextBar = NULL;  
  myFrameButtWnd = NULL;

  // So, what kinda stuff do we wanna do for our link?

  // I suppose we should first, at least, display the url....

  // Make a text bar to display the url, yah, okay.  We coulda just called
  // displayText() for this, Einstein.
  myFrameTextBar = new TextBar;
  myFrameTextBar->setName(link);
  myRightStylePosWnd->addChild(StylePosWnd::ChildInfo(myFrameTextBar,
    StylePosWnd::ChildEdgeInfo(EXITEMSEQ_LEFT_OFFSET, StylePosWnd::LOCK_BORDER),   // Left
    StylePosWnd::ChildEdgeInfo(EXITEMSEQ_TOP_OFFSET, StylePosWnd::LOCK_BORDER),    // Top
    StylePosWnd::ChildEdgeInfo(-EXITEMSEQ_LEFT_OFFSET, StylePosWnd::LOCK_BORDER),  // Right
    StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::LOOSE)));     // Bottom
  // "Set your left edge 11 pixels inside of the left border,
  //  set your top edge 10 pixels below the top border,
  //  set your right edge 11 pixels inside of the right border,
  //  set your bottom edge to whatever is needed to make the object be 25 pixels high."

  if (myRightStylePosWnd->isInited()) {
    myFrameTextBar->init(myRightStylePosWnd);
  }

  myRightStylePosWnd->invalidate();
}

// Hey, our button just got hit.  If there actually is a string, use it.
void ExampleItemSequencerWnd::startURLMinibrowser() {
  if ((currentUrl.getValue() != NULL) && (STRCMP(currentUrl,"") != 0)) {
    api->navigateUrl(currentUrl);
  }
}

const String ExampleItemSequencerWnd::getUrl() {
  return urledit->getBuffer();
}

// Add the "enqueue all" button.
void ExampleItemSequencerWnd::addEnqueueAll() {
  if (myEnqueueAllButton == NULL) {
    myEnqueueAllButton = new ButtonWnd();
    myEnqueueAllButton->setButtonText(_("EnQ All"), 16);
    myEnqueueAllButton->setNotifyId(EXITEMSEQ_ENQUEUE_ALL);
    myEnqueueAllButton->setNotifyWindow(this);
    myTopStylePosWnd->addChild(StylePosWnd::ChildInfo(myEnqueueAllButton,
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_WIDTH, StylePosWnd::LOOSE),        // Left
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::CENTER),      // Top
      StylePosWnd::ChildEdgeInfo(-EXITEMSEQ_LEFT_OFFSET, StylePosWnd::LOCK_BORDER),  // Right
      StylePosWnd::ChildEdgeInfo(EXITEMSEQ_BUTTON_HEIGHT, StylePosWnd::CENTER)));    // Bottom
    // "Set your left edge to whatever is needed to make the object be 60 pixels wide,
    //  set your top edge to create the y-axis centering of a 25 pixel high object,
    //  set your right edge 11 pixels inside of the right border,
    //  set your bottom edge to create the y-axis centering of a 25 pixel high object."
    if (myTopStylePosWnd->isInited()) {
      myEnqueueAllButton->init(myTopStylePosWnd);
    }
    myTopStylePosWnd->invalidate();
  }
}

// Remove the "enqueue all" button.
void ExampleItemSequencerWnd::removeEnqueueAll() {
  if (myEnqueueAllButton != NULL) {
    myTopStylePosWnd->deleteChild(myEnqueueAllButton);
    myEnqueueAllButton = NULL;
  }
}

// Actually do the enqueueing of all.
void ExampleItemSequencerWnd::enqueueAll() {
  // So here's our sequencer to play with.

  if (mySequencer == NULL) {
    // If we don't have one, make one
    mySequencer = new CharPtrSequencer<1/*looping*/>;
  } else {
    // Otherwise, just empty the one we've got.
    mySequencer->deleteAll();
  }

  // For every item in the tree,
  int i,num = myTreeItemList.getNumItems();
  for (i = 0; i < num; i++) {
    // add it to the sequencer
    mySequencer->addItem(myTreeItemList[i]->getLinkString());
  }

  // Okay, now that we've filled our sequencer, what do we do with it?

  // We register it as the current sequencer.
  api->core_registerSequencer(NULL, mySequencer);

  // And indicate that something might have happened.
  if (num) {
    displayText(_("Now hit play, eh?"));
  } else {
    displayText(_("Nothing to enqueue?"));
  }
}

// Parse the extensions string
void ExampleItemSequencerWnd::parseExtensionsString() {
  // Empty the extensions strings already there
  extList.deleteAll();

  // Splice the extensions into new strings to keep in a pointer list.
  String *ext,extStr = extedit->getBuffer();
  while (extStr.len()) {
    ext = new String(extStr.lSpliceChar(';')); // remember, splice removes the character.
    ext->toupper(); // non-case-sensitive compares.
    extList.addItem(ext);
  }
}

// Test an url to have a valid extension
int ExampleItemSequencerWnd::testValidExtension(String link) {
  // Note that this function takes a copy of a string, not a
  // reference to a string.  That's because we're going to split
  // it, and it would be bad to split the original.

  // Split the given link at the leftmost period.
  String &linkext = link.rSplitChar('.'); // remember, split preserves the character.
  linkext.toupper(); // non-case-sensitive compares.

  int i,num = extList.getNumItems();
  for (i = 0; i < num; i++ ) {
    if (*(extList.enumItem(i)) == linkext) {
      return 1;
    }
  }
  return 0;
}

//
// *** Note how this method handles two very different cases:
//
//  How to play an index from a sequencer and
//  How to play a single url through the core.
//

int ExampleItemSequencerWnd::playSequencedString(String link) {
  // If we've already loaded our sequencer,
  if (mySequencer != NULL) {
    // Figure out which index that is.
    int index = mySequencer->findItem(link);
    // If it actually is someone,
    if (index != -1) {
      // Set that to be current
      mySequencer->setCurrent(index);

      // And make it play as the next item.
      CoreHandle newHandle;
      newHandle.play();

      // That wasn't so hard, was it?
      return 1;
    }
  } else {
    // Just play the url itself
    CoreHandle newHandle;
    newHandle.setNextFile(link);
    newHandle.play();
  }
  return 0;
}

#if 0	// Reference Stuff
// ===========================================================================
//
//	EXAMPLEITEMSEQUENCER: Reference Stuff
//
//    This section should (eventually) provide the reference materials for
//  the following SDK Objects:
//
//      String
//      ItemSequencerI
//      ListSequencer
//      CharPtrSequencer
//
//    As always, if you have any specific questions about these objects,
//  please do feel free to also check the NSDN site, as well as the wasabi
//  developers' forum.



// ===========================================================================
//
//	EXAMPLEITEMSEQUENCER: String Reference
//
//  String is not inherited from YourMomWnd
//


  // --------------------------------------------
  // Public Methods

  String(const char *initial_val=NULL);
  String(const String &s);
  ~String();
  // Constructors and destructors.  Nothing fancy.

  const char *getValue() const;
  operator const char *() const;
  // Data accessors.  Note the implicit cast method to const char *

  char *getNonConstVal();
  // Use this if you need a non const char *, but if you're modifying it,
  // be careful of space considerations.

  const char *setValue(const char *newval);
  const char *operator =(const char *newval);
  String& operator =(const String &s) 
  // Change the string to hold a new string of characters

  const char *operator +=(const char *addval)
  const char *operator +=(char addval);
  const char *operator +=(int value);
  // Concatenation operators.  The integer method converts the int to string.

  inline int operator ==(const char *val) const 
  inline int operator <(const char *val) const 
  inline int operator !=(const char *val) const
  inline int operator >(const char *val) const 
  inline int operator ==(const String &val) const
  inline int operator <(const String &val) const 
  inline int operator !=(const String &val) const
  inline int operator >(const String &val) const 
  // Arithmetic Comparator Operators

  inline String operator + (const char * val)
  inline String operator + (const String &val) 
  inline String operator + (const char val) 
  // Arithmetic Copy-Concatenation Operators

  int len() const;
  // how long is the string?

  int isempty() const;
  // is the string empty?

  void toupper();	
  // converts entire string to uppercase
  
  void tolower();	
  // converts entire string to lowercase
  
  int isequal(const char *val) const;	
  // basically !strcmp
  
  int islessthan(const char *val) const;	
  // basically strcmp < 0

  void changeChar(int from, int to);
  // Switch these two characters.

  const char *printf(const char *format, ...);
  // Standard printf formatting, replaces string with new value.

  const char *cat(const char *value);
  // Adds to the end

  const char *ncpy(const char *newstr, int numchars);
  // Replaces string with n chars of val or length of val, whichever is less.

  // --------------------------------------------
  // Character based find-n-splice methods --
  // "l" and "r" prefixes specify to begin at
  // front or back of string:

  // Returns index of first found, -1 if not found.
  int lFindChar(char findval);  
  int lFindChar(const char * findval);  // a list of chars to search for
  int rFindChar(char findval);  
  int rFindChar(const char * findval);  // a list of chars to search for
  
  // Splits string at findval.  Characters passed by search, including the 
  // found character, are MOVED to the returned string.  If there is no char
  // to be found, the entire string is returned and the called instance is
  // left empty.  (Makes looped splits very easy).
  String lSplit(int idxval);  
  String lSplitChar(char findval);  
  String lSplitChar(const char *findval);  
  String rSplit(int idxval);  
  String rSplitChar(char findval);  
  String rSplitChar(const char *findval);  

  // Same as split, except the find char is cut completely.
  String lSpliceChar(char findval);
  String lSpliceChar(const char * findval);
  String rSpliceChar(char findval);  
  String rSpliceChar(const char * findval);

  // You can find out more about using the splicing and arithmetic methods in
  // the teststring project in the SDK.



// ===========================================================================
//
//	EXAMPLEITEMSEQUENCER: ItemSequencerI Reference
//
//  ItemSequencerI is inherited from ItemSequencer (which simply
//  implements the dispatch table)
//
//  Basically, the way item sequencer works is that it tells you everything
//  which is going on with the core playback system and then will ask the
//  object for what piece of media to play next.


  // --------------------------------------------
  // Public Defines for Notification

  #define SEQNOTIFY_ONREGISTER	10
  #define SEQNOTIFY_ONDEREGISTER	20
  #define SEQNOTIFY_ONNEXTFILE	30
  #define SEQNOTIFY_ONTITLECHANGE	40
  #define SEQNOTIFY_ONSTARTED	50
  #define SEQNOTIFY_ONSTOPPED	60
  #define SEQNOTIFY_ONPAUSED	70
  #define SEQNOTIFY_ONUNPAUSED	80
  #define SEQNOTIFY_ONREWIND	90
  #define SEQNOTIFY_ONFORWARD	100


  // --------------------------------------------
  // Virtual Methods to be Overriden  

  virtual int rewind();
  // The rewind button was hit.

  virtual int forward();
  // The forward button was hit.

  virtual const char *getNextPlayItem();
  // The core wants to know what to play next.

  virtual int getCurrentPlaybackNumber();
  // If you are using internal numbering, you can return it here
  // or you can return -1.


  // --------------------------------------------
  // Virtual Methods to optionally be Overriden  

  virtual int onRegister() { return 0; };
  virtual int onDeregister() { return 0; };
  virtual int onNextFile() { return 0; }// on transition
  virtual int onTitleChange() { return 0; }
  virtual int onStarted() { return 0; }
  virtual int onStopped() { return 0; }
  virtual int onPaused() { return 0; }
  virtual int onUnpaused() { return 0; }
  virtual int onRewind() { return 0; }
  virtual int onForward() { return 0; }
  // Override any of these methods to handle different aspects of playback

  virtual int onNotify(int msg, int param1, int param2);
  // Or override this to handle SEQNOTIFY messages directly,
  // but this function simply functions as a switchtable into
  // the above block of virtual methods.



// ===========================================================================
//
//	EXAMPLEITEMSEQUENCER: ListSequencer Reference
//
//  ListSequencer is inherited from ItemSequencerI
//
//  ListSequencer sets up the infrastructure for sequencing items through a
//  simple numbered list.  "Just override the protected stuff."


  // --------------------------------------------
  // Virtual Methods to be Overriden  

  virtual int getNumEntries()=0;
  // return how many items are in your list.

  virtual const char *enumItem(int pos)=0;
  // return the indexed item from your list.

  virtual int getCurrent()=0;
  // return the current value index for your list.

  virtual int setCurrent(int cur)=0;
  // set the current value index for your list.


  // --------------------------------------------
  // Virtual Methods to optionally be Overriden  

  virtual int loop() { return 0; }	
  // override as necessary, return 1 if you want to automatically loop.



// ===========================================================================
//
//	EXAMPLEITEMSEQUENCER: CharPtrSequencer Reference
//
//  CharPtrSequencer is inherited from ListSequencer
//
//  CharPtrSequencer is a LITTLE bit weird, because it uses a default
//  template -- essentially this object is setup to be used without
//  requiring you to subclass anything, but the optional "loop" method
//  of the ListSequence is very useful and I still wanted to expose that
//  functionality.
//
//  So.  If you say new CharPtrSequencer, you'll get a nonlooping sequencer.
//
//  If you say new CharPtrSequencer<1>, you'll get a looping sequencer.
//  This is why there is the typedef to LoopCharSequencer at the end of 
//  charptrsequence.h
//
//  Think of this object as a pointer list with sequencer powers (because it is).


  // --------------------------------------------
  // Sequence Methods -- these methods will be called by the engine code

  virtual int getNumEntries();
  // How many entries do you have to play?

  virtual const char *enumItem(int pos);
  // Get the nth item entry.

  virtual int getCurrent();
  // Get the current playing index

  virtual int setCurrent(int cur);
  // Set the current playing index

  virtual int loop();
  // Should we loop or not?

  
  // --------------------------------------------
  //  Pointer List Methods - you call these methods to fill the list

  virtual const char *addItem(const char *item);
  // Add an item to the end of the list

  virtual void deleteAll();
  // Empty the list.

  virtual int findItem(const char *item);
  // Get the index of an item in the list (linear search)

  virtual void deleteItem(const char *item);
  // Delete an item in the list (linear search)

  virtual void deleteItem(int item);
  // Delete an index in the list.

  virtual const char *getFirst();
  // Get the first value of the list

  virtual const char *getLast();
  // Get the last value of the list

  virtual int getNumItems();
  // Get the number of items in the list.

#endif	// Reference Stuff
