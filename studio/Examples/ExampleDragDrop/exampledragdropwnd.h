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

#ifndef _EXAMPLEDRAGDROPWND_H
#define _EXAMPLEDRAGDROPWND_H

//=====================================
//  Wasabi Includes
//=====================================
#include "../../common/framewnd.h"
#include "../../bfc/wnds/treewnd.h"
#include "../../bfc/ptrlist.h"

//=====================================
//  Forward References
//=====================================
class DropWnd;
class DropWndItem;
class FilenameI;

//Our Window parent.
#define EXAMPLEDRAGDROPWND_PARENT FrameWnd

class ExampleDragDropWnd : public EXAMPLEDRAGDROPWND_PARENT {
public:
  ExampleDragDropWnd();
  virtual ~ExampleDragDropWnd();

  virtual int onPostOnInit(); //We want to do something after our window has been initted.

  //Our Window Name.
  static const char *getWindowTypeName() { return "ExampleDragDrop Window"; }
  //Our Window GUID.
  static GUID getWindowTypeGuid();

protected:
  DropWnd *rightPane; //The right pane DropWnd.
  DropWnd *leftPane;  //The left pane DropWnd.
};

//Our DropWnd parent.
#define DROPWND_PARENT TreeWnd

class DropWnd : public DROPWND_PARENT {
friend DropWndItem;
public:
  DropWnd();
  virtual ~DropWnd();

  virtual int onInit(); //We want to do something during init.

  //Our Window Name.
  static const char *getWindowTypeName() { return "DropWnd Window"; }

  //Our Window GUID.
  static GUID getWindowTypeGuid();

/*
  !!! Drag And Drop Overrides !!!
*/

  //We want to accept external drops (ie. From the OS).
  virtual int acceptExternalDrops() { return 1; }

  //This gets called when an item is being dragged over our window.
  virtual int dragOver(int x, int y, RootWnd *sourceWnd);

  //This gets called when an item is dropped over our window.
  virtual int dragDrop(RootWnd *sourceWnd, int x, int y);

protected:
  
};

//Our DropWndItem Parent.
#define DROPWNDITEM_PARENT TreeItem

//Our private drag and drop data type.
#define DD_DWITEM "DD_DWItem v1 (Private)"

class DropWndItem : public DROPWNDITEM_PARENT {
friend DropWnd;
public:
  DropWndItem(const char * label=NULL);
  virtual ~DropWndItem();

/*
  !!! Drag and Drop Overrides !!!
*/

  //This is called when the user begins to drag an item (from our window).
  virtual int onBeginDrag(char *suggestedTitle);

  //This gets called when an item is being dragged over one of our items.
  virtual int dragOver(RootWnd *sourceWnd);
  
  //This gets called when an item is dropped over one of our items.
  virtual int dragDrop(RootWnd *sourceWnd);

  //This gets called when the API thinks the drag has completed. 
  //It sends a success flag (not zero on success).
  virtual int dragComplete(int success);

protected:
  PtrList<FilenameI> draglist;  //The list of items the user is dragging (where we are the source!).
};

#endif