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

//=====================================
//  Project Includes
//=====================================
#include "exampledragdropwnd.h" //ExampleDragDropWnd, DropWnd, DropWndItem class.

//=====================================
//  Wasabi Includes
//=====================================
#include "../../common/framewnd.h"  //FrameWnd class.
#include "../../common/filename.h"  //Filename, FilenameI classes.

//=====================================
// Globals
//=====================================

// {94AD934A-830F-4c22-A8A7-CFEEAFD6A5C8} //ExampleDragDropWnd's GUID.
static const GUID exampledragdropwnd_guid = 
{ 0x94ad934a, 0x830f, 0x4c22, { 0xa8, 0xa7, 0xcf, 0xee, 0xaf, 0xd6, 0xa5, 0xc8 } };

// {8949CEBB-6E75-4dfb-931F-395C89D35025} //DropWnd's GUID.
static const GUID dropwnd_guid = 
{ 0x8949cebb, 0x6e75, 0x4dfb, { 0x93, 0x1f, 0x39, 0x5c, 0x89, 0xd3, 0x50, 0x25 } };

//=======================================
//  ExampleDragDropWnd Class
//=======================================
ExampleDragDropWnd::ExampleDragDropWnd() {
  setName("Example Drag And Drop Window");  //Set our window name (it becomes the window title caption).

  leftPane = new DropWnd;   //Create our left pane window.
  rightPane = new DropWnd;  //Create our right pane window.

  setSlideMode(FRAMEWND_SQUISH);    //Sets our slider mode for our framewnd to "squish".
  setDividerType(DIVIDER_VERTICAL); //Vertical divider for our framewnd.
  setDividerPos(SDP_FROMLEFT, 250); //Divider moves from the left, with a base position of 250 from left.
  setResizeable(TRUE);              //Frames are resizeable.

  setChildren(leftPane, rightPane); //Set our left and right pane.


}

ExampleDragDropWnd::~ExampleDragDropWnd() {
  
}

int ExampleDragDropWnd::onPostOnInit() {
  int retval = EXAMPLEDRAGDROPWND_PARENT::onPostOnInit(); //Call up the our parent.
  /*
    Here is where we could fill our DropWnd's with content.
  */
  return retval;
}

GUID ExampleDragDropWnd::getWindowTypeGuid() {
  return exampledragdropwnd_guid; //Return this window type's GUID.
}

//=======================================
//  DropWnd Class
//=======================================
DropWnd::DropWnd() {

}

DropWnd::~DropWnd() {

}

int DropWnd::onInit() {
  int retval = DROPWND_PARENT::onInit();  //Call up to our parent.
  /*
    Here is where we could do supplementary initialisation
    of our DropWnd if we needed to.
  */
  return retval;
}

GUID DropWnd::getWindowTypeGuid() {
  return dropwnd_guid;  //Return this window type's GUID.
}

int DropWnd::dragOver(int x, int y, RootWnd *sourceWnd) {
  DI *di = new DI(sourceWnd); //Get the drag interface of the source window.

  DROPWND_PARENT::dragOver(x, y, sourceWnd);  //Call up to our parent.

  if(di->dragCheckData(DD_FILENAME) != -1) return 1;  //Check if the data is of type DD_FILENAME.
  if(di->dragCheckData(DD_DWITEM) != -1) return 1;    //Check if the data is of type DD_DWITEM.
  
  return 0; //The type of data does not match anything we handle! return 0.
}

int DropWnd::dragDrop(RootWnd *sourceWnd, int x, int y) {
  int nitems = 0;   //Number of items.
  int slot = 0;     //Slot for data type.
  DI *di = new DI(sourceWnd); //Get the drag interface of the source window.

  if(DROPWND_PARENT::dragDrop(sourceWnd, x, y)) return 1; //Call up to our parent.

  //Check the data type, if the type matches, nitems will contain
  //the number of drag items available to read and slot will contain
  //the correct slot number for the data type we want to read.
  if((slot = di->dragCheckData(DD_FILENAME, &nitems)) != -1) {
    for(int i = 0; i < nitems; i++) {
      //Get one drag item (DD_FILENAME drag data contain Filename class ptr's, see /common/filename.h).
      Filename *fn = (Filename *)di->dragGetData(slot, i);
      //Add a DropWndItem to our DropWnd with filename as the label and with child tabs.
      addTreeItem(new DropWndItem(fn->getFilename()), NULL, TRUE, TRUE);
    }
    return 1; //Return 1 because we handled the dragdrop.
  }

  if((slot = di->dragCheckData(DD_DWITEM, &nitems)) != -1) {
    for(int i = 0; i < nitems; i++) {
      //Get one drag item (DD_DWITEM drag data contain FilenameI class ptr's, see /common/filename.h and exampledragdropwnd.h).
      FilenameI *fn = (FilenameI *)di->dragGetData(slot, i);
      //Add a DropWndItem to our DropWnd with filename as the label and with child tabs.
      addTreeItem(new DropWndItem(fn->getFilename()), NULL, TRUE, TRUE);
    }
    return 1; //Return 1 because we handled the drop.
  }

  return 0; //The type does not match anything we handle! return 0.
}

//=======================================
//  DropWndItem Class
//=======================================
DropWndItem::DropWndItem(const char * label) : DROPWNDITEM_PARENT(label) {

}

DropWndItem::~DropWndItem() {

}

/*
  onBeginDrag is a required override if you want
  your TreeWndItem derivites to be able to get drag
  and dropped.
*/
int DropWndItem::onBeginDrag(char *suggestedTitle) {
  //Create a new FilenameI class with the label of the DropWndItem as it's filename.
  FilenameI *fn = new FilenameI(getLabel());  

  /*
    When you're a source for a drop, you must keep a list
    of all the dragdrop items until the drag has been completed.

    This is so the receiving window can get the actual
    data that it wants to read.
  */
  draglist.addItem(fn); //Add the FilenameI instance to our draglist.
  //Add it as a DragItem (now available to be read be receiving window).
  getTree()->addDragItem(DD_DWITEM, draglist.getLast());

  return 1; //We handled the beginning of the drag, return 1 (if we didn't handle it, we'd return 0).
}

int DropWndItem::dragOver(RootWnd *sourceWnd) {
  DI *di = new DI(sourceWnd); //Get the drag interface of the source window.
  
  //Check the data type, if the type matches, nitems will contain
  //the number of drag items available to read and slot will contain
  //the correct slot number for the data type we want to read.
  if(di->dragCheckData(DD_DWITEM) != -1) return 1;

  return 0; //The type does not match anything we handle! return 0.
}

int DropWndItem::dragDrop(RootWnd *sourceWnd) {
  int nitems = 0;
  int slot = 0;
  DI *di = new DI(sourceWnd);

  if((slot = di->dragCheckData(DD_DWITEM, &nitems)) != -1) {
    for(int i = 0; i < nitems; i++) {
      //Get one drag item (DD_DWITEM drag data contain FilenameI class ptr's, see /common/filename.h and exampledragdropwnd.h).
      FilenameI *fn = (FilenameI *)di->dragGetData(slot, i);
      //Add a DropWndItem to our DropWndItem Parent, with the filename as it's label and with child tabs.
      getTree()->addTreeItem(new DropWndItem(fn->getFilename()), this, TRUE, TRUE);
      expand(); //Expand the tree to show our newly added subitem.
    }

    return 1; //Return 1 because we handled the drop.
  }

  return 0; //The type does not match anything we handle! return 0.
}

int DropWndItem::dragComplete(int success) {
  draglist.freeAll(); //The drop was completed, free our draglist.
  return 1; //We think the drop was successful too, so we return 1.
  /*
    if the drop check you perform fails, you can return 0 here to
    tell the system that the drop FAILED.
  */
}
