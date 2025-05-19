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

#ifndef _DRAG_H
#define _DRAG_H

#include "std.h"
#include "rootwnd.h"

class NOVTABLE DragInterface {
public:
  // (called on dest) when dragged item enters the winder
  virtual int dragEnter(RootWnd *sourceWnd)=0;
  // (called on dest) during the winder
  virtual int dragOver(int x, int y, RootWnd *sourceWnd)=0;
  // (called on src)
  virtual int dragSetSticky(RootWnd *wnd, int left, int right, int up, int down)=0;
  // (called on dest) when dragged item leaves the winder
  virtual int dragLeave(RootWnd *sourceWnd)=0;
  // (called on dest) here is where we actually drop it
  virtual int dragDrop(RootWnd *sourceWnd, int x, int y)=0;

  // must be called from within dragDrop();			(receiver)
  virtual const char *dragGetSuggestedDropTitle(void)=0;
  // must be called from within your dragEnter, Over, Leave, or Drop
  // return the slot # if you support this form of the data, -1 otherwise
  // nitems can be NULL if you're just checking validity
  virtual int dragCheckData(const char *type, int *nitems=NULL)=0;
  // fetches a specific pointer that was stored
  virtual void *dragGetData(int slot, int itemnum)=0;
  virtual int dragCheckOption(int option)=0;
};

class DragInterfaceI : public DragInterface {
public:
  // (called on dest) when dragged item enters the winder
  virtual int dragEnter(RootWnd *sourceWnd) { return 0; }
  // (called on dest) during the winder
  virtual int dragOver(int x, int y, RootWnd *sourceWnd) { return 0; }
  // (called on src)
  virtual int dragSetSticky(RootWnd *wnd, int left, int right, int up, int down) { return 0; }
  // (called on dest) when dragged item leaves the winder
  virtual int dragLeave(RootWnd *sourceWnd) { return 0; }
  // (called on dest) here is where we actually drop it
  virtual int dragDrop(RootWnd *sourceWnd, int x, int y) { return 0; }

  // must be called from within dragDrop();			(receiver)
  virtual const char *dragGetSuggestedDropTitle(void) { return NULL; }
  // must be called from within your dragEnter, Over, Leave, or Drop
  // return the slot # if you support this form of the data, -1 otherwise
  // nitems can be NULL if you're just checking validity
  virtual int dragCheckData(const char *type, int *nitems=NULL) { return 0; }
  // fetches a specific pointer that was stored
  virtual void *dragGetData(int slot, int itemnum) { return 0; }
  virtual int dragCheckOption(int option) { return 0; }
};

class DI {
public:
  DI(RootWnd *rw) {
    if (rw == NULL) di = NULL;
    else di = rw->getDragInterface();
  }
  int dragEnter(RootWnd *sourceWnd) {
    return di ? di->dragEnter(sourceWnd) : 0;
  }
  int dragOver(int x, int y, RootWnd *sourceWnd) {
    return di ? di->dragOver(x, y, sourceWnd) : 0;
  }
  int dragSetSticky(RootWnd *wnd, int left, int right, int up, int down) {
    return di ? di->dragSetSticky(wnd, left, right, up, down) : 0;
  }
  int dragLeave(RootWnd *sourceWnd) {
    return di ? di->dragLeave(sourceWnd) : 0;
  }
  int dragDrop(RootWnd *sourceWnd, int x, int y) {
    return di ? di->dragDrop(sourceWnd, x, y) : 0;
  }

  const char *dragGetSuggestedDropTitle(void) {
    return di ? di->dragGetSuggestedDropTitle() : NULL;
  }
  int dragCheckData(char *type, int *nitems=NULL) {
    return di ? di->dragCheckData(type, nitems) : 0;
  }
  void *dragGetData(int slot, int itemnum) {
    return di ? di->dragGetData(slot, itemnum) : NULL;
  }
  int dragCheckOption(int option) {
    return di ? di->dragCheckOption(option) : NULL;
  }
private: 
  DragInterface *di;
};

#endif
