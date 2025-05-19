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
//      File:     ExampleCodeGUIFrameWnd.cpp
//
//!##   Purpose:  This source module contains the functions for the 
//!##             construction of Frames and Trees as well as use of 
//!##             a text entry field.
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
#include "../../common/contwnd.h"
#include "../../bfc/wnds/buttwnd.h"
#include "../../common/textbar.h"
#include "../../common/xlatstr.h"
#include "../../bfc/wnds/treewnd.h"
#include "../../common/editwndstring.h"

// ===========================================================================
//
//  Here's your syllabus for this module:

// ===========================================================================
//
//    *** Module Syllabus
//
//  In a tab of the parent tabsheet, we will create:
//  A Horizontal Frame Split.
//    In the top section, we will create:
//      A ContWnd containing:
//        A button and
//        A text edit field.
//    In the bottom section, we will create:
//      A Vertical Frame Split.
//        In the left section, we will create:
//          A TreeWnd Data Tree with:
//            10 TreeItem items.
//        In the right section, we will create:
//          A ContWnd containing:
//            Either a text bar or
//            A button,
//          Depending on certain conditions (see below).
//
//  The interface functionality that these items expose will be as follows:
//
//    1)  When a tree item is activated, 
//        Put a unique text into the right frame section as a button.
//    2)  When a button in the right frame section is pressed,
//        Place its text into the text edit field.
//    3)  When the text edit field received an enter keystroke, or
//        When the button in the top frame section is pressed,
//        Place the text of the text edit field into the right frame section as a textbar.
//
//  This should help you understand how to use these user interface objects properly.
//
//  If you're REALLY nice to me, I will put a reference section at the end of this
//  example module and tell you what all the methods on these objects do.
//

//    *** We're also going to start over, completely, from scratch.

// ===========================================================================
//
//    *** Local Interface Object Overrides
//
//  These objects are the derived classes we create from the stock objects in
//  the SDK to give specific functionality to our interfaces and events.

//
//  class ExampleCodeGUIEditWnd
//
//  The only method we will override for this example is the onEnter call.
//  The other event handling methods will be referenced in the reference section.
//
class ExampleCodeGUIEditWnd : public EditWndString {
public:
  // Construct.  Keep the pointer to our window class to be able to do stuff.
  ExampleCodeGUIEditWnd(ExampleCodeGUIWnd * theDisplay = NULL) :
      display(theDisplay), EditWndString() {}

  // On enter, place the editwnd's data in the textbar in the right frame.
  virtual int onEnter() {
    if (display != NULL) {
      display->displayText(getBuffer());
    }
    return 1;
  }

protected:
  ExampleCodeGUIWnd * display;
};

//
//  class ExampleCodeGUITreeItem
//
//  The only method we will override for this example is the onSelect call.
//  The other event handling methods will be referenced in the reference section.
//  Maybe.  This object is f'n huge, beeyotch.
class ExampleCodeGUITreeItem : public TreeItem {
public:
  // Construct
  ExampleCodeGUITreeItem(ExampleCodeGUIWnd * theDisplay = NULL, int theID = 0) :
      display(theDisplay), id(theID), TreeItem() {}

  // On select, place some unique data for your instance in a button in the right frame.
  virtual void onSelect() {
    if (display != NULL) {
      display->displayButton(getString());
    }
  }

  // Create a unique string identifier based on the ID#.
  virtual String getString() {
    return String(StringPrintf(_("ExampleCodeGUI Tree Item #%d"),id));
  }

protected:
  int id;
  ExampleCodeGUIWnd * display;
};

// ===========================================================================
//
//    *** ExampleCodeGUIWnd::createFramesSheet()
//
//  This is the method in which we create the tabsheet for this module.  It is
//  our primary point of entry for instantiation and initialization.  Aren't 
//  you thrilled?
int ExampleCodeGUIWnd::createFramesSheet() {

//  A Horizontal Frame Split.
//    In the top section, we will create:
//      A ContWnd containing:
//        A button and
//        A text edit field.
//    In the bottom section, we will create:
//      A Vertical Frame Split.
//        In the left section, we will create:
//          A TreeWnd Data Tree with:
//            10 TreeItem items.
//        In the right section, we will create:
//          A ContWnd containing:
//            Either a text bar or
//            A button,
//          Depending on certain conditions (see below).

  // *** Here is where we create our user interface object hierarchy:

  //  A Horizontal Frame Split.
  //  -----------------------------------------------
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
  //    In the top section, we will create:
  //      A ContWnd containing:
    ContWnd *container = new ContWnd();
  //        A button and
    ButtonWnd *go = new ButtonWnd();
    go->setButtonText(_("Go"), 16);
    go->setNotifyId(EXB_FRAMEBUTTON1);
    go->setNotifyWindow(this);
    container->addChild(go, EXB_LEFT_OFFSET, (EXB_TOP_OFFSET * 2),
      EXB_BUTTON_WIDTH, EXB_BUTTON_HEIGHT, 1);
  //        A text edit field.
    myFrameEditWnd = new ExampleCodeGUIEditWnd(this);
    myFrameEditWnd->setName(_("This is a text edit field."));
    container->addChild(myFrameEditWnd, 
      EXB_LEFT_OFFSET + EXB_BUTTON_WIDTH + EXB_BUTTON_SPACER,
      (EXB_TOP_OFFSET * 2), EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 1);
    myFrameEditWnd->setBuffer(String(_("This is a text edit field.")),EXB_EDIT_BUFFSIZE); 
  //    In the bottom section, we will create:
  //      A Vertical Frame Split.
    FrameWnd *vert = new FrameWnd();
    // set the frame options.
    vert->setDividerType(DIVIDER_VERTICAL);
    vert->setResizeable(TRUE);
    vert->setDividerPos(SDP_FROMLEFT, 50);
  //        In the left section, we will create:
  //          A TreeWnd Data Tree with:
    myFrameTreeWnd = new TreeWnd;
  //            10 TreeItem items.
    int i = 0;
    for ( i = 0; i < 10; i++ ) {
      // Create a tree item
      ExampleCodeGUITreeItem * newNode = new ExampleCodeGUITreeItem(this, i+1);
      // Give it a label -- if you don't give it a label, you'll ASSERT.
      newNode->setLabel(StringPrintf("Item %02d", i+1));
      // Attach our tree connections.
      myFrameTreeWnd->addTreeItem(newNode, NULL, FALSE, TRUE);
    }
  //        In the right section, we will create:
  //          A ContWnd containing:
    myFrameContWnd = new ContWnd;
  //            Either a text bar or
  //            A button,
  //          Depending on certain conditions (see below).

  //
  // And then we commit final assembly of our major pieces:
  //
    // the tree and the cont into the bottom frame
    vert->setChildren(myFrameTreeWnd, myFrameContWnd);
    // the editwnd/gobutton and the bottom frame in the main frame.
    horiz->setChildren(container, vert);
    // and then the main frame object into our tabsheet.
    addChild(horiz, _("This contains framey-type shietznite."));
  //
  // This method call sets up children for myFrameContWnd....
    displayText(_("This is the ``Frames`` tab."));
    // displayText() will make sure a text object exists in the myFrameContWnd
    // displayButton() will make sure a buttonobject exists in the myFrameContWnd
    // each will remove the contents of myFrameContWnd when they are executed.

  // For the nonce, return codes from and to the API should
  //  return "0" for failure and "1" for success.
  return 1;
}

// Make sure there's a text bar.  Get rid of the button if it's there.
void ExampleCodeGUIWnd::displayText(const char * text) {
  // Remove whatever's there, we're changing it.
  myFrameContWnd->deleteAll();
  myFrameTextBar = NULL;  
  myFrameButtWnd = NULL;

  // Make a text bar to display normal XML element data
  myFrameTextBar = new TextBar;
  myFrameTextBar->setName(text);

  // Add the object to the container with the specified boundaries.
  myFrameContWnd->addChild(myFrameTextBar, EXB_LEFT_OFFSET, 
    EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER,
    EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

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
void ExampleCodeGUIWnd::displayButton(const char * text) {
  // Remove whatever's there, we're changing it.
  myFrameContWnd->deleteAll();
  myFrameTextBar = NULL;  
  myFrameButtWnd = NULL;

  // Make a button to display the ability to launch an URL
  myFrameButtWnd = new ButtonWnd;
  myFrameButtWnd->setButtonText(text, 16);
  myFrameButtWnd->setNotifyId(EXB_FRAMEBUTTON2);
  myFrameButtWnd->setNotifyWindow(this);

  // Add the object to the container with the specified boundaries.
  myFrameContWnd->addChild(myFrameButtWnd, EXB_LEFT_OFFSET, 
    EXB_BUTTON_HEIGHT + EXB_BUTTON_SPACER,
    EXB_BUTTON_WIDTH * 6, EXB_BUTTON_HEIGHT, 0/*"Invalidate on Resize"*/);

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
}

// This method is called when our button marked "GO" is pressed.
// Calling our buttons "1" and "2" was probably a dumb idea -- you
// should be more descriptive in your naming -- but for something
// as simple as this, it should suffice.
void ExampleCodeGUIWnd::onFrameButton1() {
  // This button mimics what happens if you hit enter from within
  // the text edit field.  So we do a mild cheat and just call
  // "onEnter" on our object.  This vaguely breaks the rules of
  // clean OOP design, but, again, isn't that bad in this specific
  // instance.  Aren't I so nice to show you all the really
  // dumb, dangerous, ways to write your code?
  if (myFrameEditWnd != NULL) {
    myFrameEditWnd->onEnter();
  }
}

// This method is called when the button in the Right-Hand frame is pressed.
void ExampleCodeGUIWnd::onFrameButton2() {
  // Here, what we want to do is get the text out of the button display
  // and set it into the edit window.
  if ((myFrameButtWnd != NULL) && (myFrameEditWnd != NULL)) {
    myFrameEditWnd->setBuffer(String(myFrameButtWnd->getButtonText()),2000);
  }
}

#if 0	// Reference Stuff
// ===========================================================================
//
//	EXAMPLECODEGUI: Reference Stuff
//
//    This section should (eventually) provide the reference materials for
//  the following SDK Objects:
//
//      TreeWnd
//      TreeItem
//      EditWnd
//      EditWndString
//
//    As always, if you have any specific questions about these objects,
//  please do feel free to also check the NSDN site, as well as the wasabi
//  developers' forum.




// ===========================================================================
//
//	EXAMPLECODEGUI: TreeWnd Reference
//
//  TreeWnd is inherited from ScrlBkgWnd
//

  // --------------------------------------------
  // Basic Window Event Overrides.

  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  virtual int childNotify(RootWnd *child, int msg, int param1=0, int param2=0);
  virtual int onLeftButtonDown(int x, int y);
  virtual int onLeftButtonUp(int x, int y);
  virtual int onRightButtonUp(int x, int y);
  virtual int onMouseMove(int x, int y);
  virtual int onLeftButtonDblClk(int x, int y);
  virtual int onRightButtonDblClk(int x, int y);
  virtual int onMouseWheelUp(int clicked, int lines);
  virtual int onMouseWheelDown(int clicked, int lines);
  virtual int onContextMenu(int x, int y);
  virtual int onDeferredCallback(int param1, int param2);
  virtual int onKillFocus();
  virtual int onChar(char c);
  virtual int onKeyDown(int keycode);
  virtual void timerCallback(int c);
  // These overrides provide the bulk of the user input functionality for
  // the TreeWnd.  If you want to add more functionality to an object
  // derived from TreeWnd, you can further override these virtuals.

  // It should go without saying that you should call these explicitly
  // if you do override any of these virtual methods.


  // --------------------------------------------
  // Standard Drag-n-Drop Overrides.

  virtual int dragEnter(RootWnd *sourceWnd);
  // dragEnter    -- an item (possibly an SDK object) has entered this
  //              object's drag zone.  return 1 if you can accept it.

  virtual int dragOver(int x, int y, RootWnd *sourceWnd);
  // dragOver     -- this method is analogous to the "onMouseMove" method,
  //              except it is called while someone is dragging something.
  //              return 1 or 0 if to allow the drop at that point (or not).

  virtual int dragLeave(RootWnd *sourceWnd);
  // dragLeave    -- the object is now leaving your zone.  I'm not really
  //              sure what the return code should do here, but all our
  //              code seems to just return 1.

  virtual int dragDrop(RootWnd *sourceWnd, int x, int y);
  // dragDrop     -- the darn user just let go of the mouse button at that
  //              x/y point.  If you want to tell the user to stuff the
  //              darn object because you don't want it, return 0.  Otherwise,
  //              if you handle the drop, return 1.

  virtual int dragComplete(int success);
  // dragComplete -- the SDK will tell you if it thinks the drag and drop
  //              has succeeded.  Please use the return code to tell it
  //              if you agree or disagree.

  // The SDK Engine will call your object with the above DnD methods.

  // The default code in TreeWnd will just make the same calls down to
  // the items in the tree if the drag pointer is over tree items -- and
  // the default code will also call the following methods during normal
  // DnD processing:

  // don't need to override this: just calls thru to the treeitem
  virtual int onBeginDrag(TreeItem *treeitem);
  // these will be called if the pointer is not over a treeitem
  virtual int defaultDragOver(int x, int y, RootWnd *sourceWnd) { return 0; }
  virtual int defaultDragDrop(RootWnd *sourceWnd, int x, int y) { return 0; }
  // called with item that received a drop
  virtual void onItemRecvDrop(TreeItem *item) {}


  // --------------------------------------------
  // Public TreeItem Controls

  TreeItem *addTreeItem(TreeItem *item, TreeItem *par, int sorted=TRUE, int haschildtab=FALSE);
  // addTreeItem    -- You do want to add items to your tree, correct?
  // PARAMS:      TreeItem *item    -- The item to add
  //              TreeItem *par     -- The item to be the parent.
  //                                Use NULL if this is to be a root item.
  //              int sorted        -- Do you want this item to be sorted?  Do you want fries with that?
  //              int haschildtab   -- Do you want to be able to open this
  //                                item and look at its children?

  void moveTreeItem(TreeItem *item, TreeItem *newparent);
  // moveTreeItem   -- Give an item already in the tree a new parent.

  void deleteAllItems();
  // deleteAllItems -- Would you like to guess what this does?

  int expandItem(TreeItem *item);
  void expandItemDeferred(TreeItem *item);
  int collapseItem(TreeItem *item);
  void collapseItemDeferred(TreeItem *item);
  // These methods open or close the item hierarchy beneath the item provided

  void selectItem(TreeItem *item);	// selects.
  void selectItemDeferred(TreeItem *item);// selects. posted.
  // These methods cause the given item to become selected.

  void delItemDeferred(TreeItem *item);
  // This method will cause the item to be deleted.
  // *** The "Deferred" methods will cause the action to occur on the NEXT frame.  

  void hiliteItem(TreeItem *item);
  void unhiliteItem(TreeItem *item);
  // The hilite methods only control how the item is drawn.  Not its selected state.

  void setHilitedColor(const char *colorname);
  COLORREF getHilitedColor();
  // And these allow you to contol what color your item hilites use.
  
  TreeItem *getCurItem();
  // Get the pointer to the currently selected item.
  
  TreeItem *hitTest(POINT pos);
  TreeItem *hitTest(int x, int y);
  // Get the pointer to the item at the given client coordinates.
  
  virtual void jumpToNext(char c);
  // This method will hilite the next open item that begins with the
  // given ASCII character.
  
  void ensureItemVisible(TreeItem *item);
  // This method will expand whatever tree items needed to ensure that
  // the given item is displayed.
  
  TreeItem *getByLabel(TreeItem *parent, char *name);
  // This method will return the item with the given name that exists at
  // or below the given item in the item hierarchy.  Use a NULL parent
  // pointer to search all items.
  
  int getItemRect(TreeItem *item, RECT *r);
  // This method will fill the RECT struct with the info for the item
  // in question.

  int getNumRootItems();
  // How many root-level items are there?

  TreeItem *enumRootItem(int which);
  // Get the #'th root-level item.

  TreeItem *getSibling(TreeItem *item);
  // Get the next item after the given item.  If this is the last item,
  // you'll get a NULL for this.

  int getNumVisibleChildItems(TreeItem *c);
  int getNumVisibleItems();
  TreeItem *enumVisibleItems(int n);
  TreeItem *enumVisibleChildItems(TreeItem *c, int n);
  // Get how many visible items (or visible children) exist, and then
  // get the specifically indexed (the #'th) item.



  // just removes a TreeItem from the tree, doesn't delete it... this is for
  // ~TreeItem to call only.
  int removeTreeItem(TreeItem *item);
  // WIP: If TreeItem is already a friend object, shouldn't this be a
  // private method or a protected virtual?


  // --------------------------------------------
  // TreeItem Label Editing Methods

  void editItemLabel(TreeItem *item);
  // This method will allow the user to begin editing the label of an item.

  void cancelEditLabel(int destroyit=0);
  // This method will cancel the editing state.  If the param is TRUE, the
  // item currently being edited will be deleted.

  void setAutoEdit(int ae);
  int getAutoEdit();
  // The "AutoEdit" state of the TreeWnd will control whether or not the
  // default TreeWnd will automatically set the items into editing mode
  // through the default user interface.


  // --------------------------------------------
  // TreeItem Callback Methods

  virtual int compareItem(TreeItem *p1, TreeItem *p2);
  // This method is used to compare tree items in order to provide
  // proper sorting.  The default is to simply compare the labels
  // of the two items.  If you wish to override this method to
  // provide your own sorting rules, your returned values should be
  // analogous to the STRCMP function:
  //
  //  -1 means (*p1)  < (*p2)
  //   0 means (*p1) == (*p2)
  //   1 means (*p1)  > (*p2)
  //
  //  Just as a friendly note from your uncle mig, feel free to add
  //  int operator < and int operator == methods to your own TreeItem
  //  derivatives and create a new comparator method like this:
    //virtual int compareItem( MyTreeItem *p1, MyTreeItem *p2)
    //{
    //  return ( (*p1 < *p2) ? -1 : (*p1 == *p2) ? 0 : 1 );
    //}
  //  Or if you want to get really deep into the OOP stew, implement
  //  your item comparator with an int operator - method and return the
  //  following from an override:
    //{
    //  return *p1 - *p2;
    //}
  //  Obviously, you'll have to cast the params from the default
  //  compareItem method to your own objects -- but you'll likely have
  //  to do that anyhow in order to get your own info back from them.
  //
  //  Handling your comparisons within your tree items with your own
  //  virtual operators is often far less confusing for complex sorting
  //  logic, especially across multiple derived item classes.

  virtual void onLabelChange(TreeItem *item) {}
  //  When an item's label has been edited or changed, this method will be
  //  called with the pointer to the changed item.

  virtual void onItemSelected(TreeItem *item) {}
  //  When an item is selected, this method will be called with the pointer 
  //  to the selected item.

  virtual void onItemDeselected(TreeItem *item) {}
  //  When an item is deselected, this method will be called with the pointer 
  //  to the selected item.


  // --------------------------------------------
  // Public TreeWnd Controls

  virtual int getContentsWidth();
  virtual int getContentsHeight();
  // The rectangular area covered by the items display.

  virtual void setTip(const char *tip);
  virtual const char *getTip();
  // Set and get the "tool tip" hover text

  void setSorted(BOOL dosort);
  BOOL getSorted();
  // Set and get the sorted state of the tree.

  void sortTreeItems();
  // Force a sort.

  void setAutoCollapse(BOOL doautocollapse);
  // Set the autocollapse state of the tree.

  virtual int setFontSize(int newsize);
  int getFontSize();
  // Set and get the fontsize to use for the items.


  // --------------------------------------------
  // Stuff mig hasn't gotten around to figuring out yet
  int wantFocus() { return 1; }
  virtual void freeResources();
  virtual void reloadResources();
  void setRedraw(BOOL r);
  int ownerDraw();
  void hiliteDropItem(TreeItem *item);  
  void unhiliteDropItem(TreeItem *item);
  void invalidateMetrics();



// ===========================================================================
//
//	EXAMPLECODEGUI: TreeItem Reference
//
//  TreeItem is not inherited from any other objects.
//


  // --------------------------------------------
  // Basic Window Event Overrides.
  virtual void onTreeAdd() {}
  virtual void onTreeRemove() {}
  virtual void onSelect() {}
  virtual void onDeselect() {}
  virtual int onLeftDoubleClick() { return 0; }
  virtual int onRightDoubleClick() { return 0; }
  virtual int onContextMenu(int x, int y);
  virtual int onChar(UINT key) { return 0; }	// return 1 if you eat the key
  // For safety's sake, you should call the parent class
  // methods if you override these window events.
  

  // --------------------------------------------
  // Label Editing Overrides.
  void editLabel();
  virtual int onBeginLabelEdit();
  virtual int onEndLabelEdit(const char *newlabel);
  virtual void setLabel(const char *label);
  virtual const char *getLabel();
  // For safety's sake, you should call the parent class
  // methods if you override these window events.


  // --------------------------------------------
  // Drag & Drop Overrides.
  virtual int onBeginDrag(char *suggestedTitle) { return 0; }
  virtual int dragOver(RootWnd *sourceWnd) { return 0; }
  virtual int dragLeave(RootWnd *sourceWnd) { return 0; }
  virtual int dragDrop(RootWnd *sourceWnd) { return 0; }
  virtual int dragComplete(int success) { return 0; }
  // return 0 to refuse being dragged
  // else return 1 and install the droptype and dropitem


  // --------------------------------------------
  // Item Hierarchy Navigation
  
  TreeItem *getNthChild(int nth); 
  // enumerates children (zero based)

  TreeItem *getChild(); 
  // gets the 0th child

  TreeItem *getChildSibling(TreeItem *item);
  // get the next sibling of the given item

  TreeItem *getSibling();
  // get the next sibling of this item

  TreeItem *getParent();
  // get the parent of this item

  int getNumChildren();
  // get the number of children this item has

  BOOL hasSubItems();
  // return (getNumChildren() != 0);


  // --------------------------------------------
  // Public Item State Controls

  void ensureVisible();
  // Expand the hierarchy to make sure this object is visible.

  void setSorted(int issorted);
  BOOL isSorted();
  // Set and get the sorted state of the item.

  void setChildTab(int haschildtab);
  // Add or remove the drawing of the childtab.

  int collapse();
  BOOL isCollapsed();
  // Set and get the collapsed state of the item.

  int expand();
  BOOL isExpanded();
  // Set and get the expanded state of the item.
  // (AFAICT, this is just (!(isCollapsed())) )

  BOOL isSelected();
  // Is this the user-selected item?

  void setHilited(BOOL ishilited);
  BOOL isHilited();
  // Set and get the highlit state of the item.

  int getCurRect(RECT *r);
  // The rectangular size of the drawn item

  void invalidate();
  // Force the redraw of the rectangle holding the item.

  TreeWnd *getTree() const;
  // Get the tree that currently owns this item


  // --------------------------------------------
  // Protected Item State Controls

  void setHilitedDrop(BOOL ishilitedDrop);
  BOOL isHilitedDrop();
  // Set and get the hilited-for-drag-and-drop state.

  void linkTo(TreeItem *par);
  // Set this item to have a new parent.

  void setTree(TreeWnd *newtree);
  // Set the tree to currently own this item.

  void addSubItem(TreeItem *item);
  // Add an item as a child of this item.

  void setCurRect(int x1, int y1, int x2, int y2, int z);
  // Set the draw rectangle on this item.

  int getIndent();
  // Get the indentation level for this item.

  BOOL needTab();
  // Return whether or not we should display a childtab icon for this item.

  void sortItems(); 
  // Sorts the children of this item.
  
  void setEdition(BOOL isedited);
  BOOL getEdition();
  // Set and get the is-currently-being-edited state.


  // --------------------------------------------
  // Public Item Draw Controls

  virtual SkinBitmap *getIcon();
  virtual void setIcon(SkinBitmap *newicon);
  // Get and set the icon for this item.

  virtual int customDraw(Canvas *canvas, const POINT &pt, int defaultTxtHeight, int indentation, const RECT &clientRect); 
  // Override to draw by yourself. Return the width of what you've drawn


  // --------------------------------------------
  // Stuff mig hasn't gotten around to figuring out yet
  void setCurrent(BOOL tf); // No implementation?



// ===========================================================================
//
//	EXAMPLECODEGUI: EditWnd Reference
//
//  EditWnd is inherited from BaseWnd.
//


  // --------------------------------------------
  // Basic Window Event Overrides.

  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  virtual int onResize();
  // Make sure you call down into these methods if you override them.

  virtual LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  // Want your own wndProc?  Oh won't this be fun for porting to other OSs.


  // --------------------------------------------
  // Protected Event Overrides.
  
  virtual void onEditUpdate();
  virtual void onIdleEditUpdate();
  virtual int onEnter();	// user hit enter.. return 1 to close window
  virtual int onAbort();	// user hit escape.. return 1 to close window
  virtual int onLoseFocus();	// different from onKillFocus() from BaseWnd!
  virtual void timerCallback(int id);
  // Make sure you call down into these methods if you override them.

  
  // --------------------------------------------
  // Public Window State Methods.

  void setModal(int modal);//if modal, deletes self on enter
  void setBorder(int border);
  int getTextLength();
  void setAutoEnter(int a);


  // --------------------------------------------
  // Data Buffer Methods.

  virtual void setBuffer(char *buffer, int len);
  // You MUST call "setBuffer()" with a PERSISTANT DATA BUFFER!!

  virtual void getBuffer(char *outbuf, int outlen);
  // STRNCPY(outbuf,buffer,min(len,outlen)) -- got it?



// ===========================================================================
//
//	EXAMPLECODEGUI: EditWndString Reference
//
//  EditWnd is inherited from EditWnd.
//  It simplifies the use of EditWnd by handling its buffers internally.
//
//  The following alterations are made by EditWndString:

  // --------------------------------------------
  // Public Data Buffer Methods.
  virtual void setBuffer(char * buffer, int len);
  virtual const char * getBuffer(void); 
  // NOTE:  do NOT call the other available buffer calls provided in EditWnd!
  // If you do, don't blame me if bad things happen.

#endif// Reference Stuff
