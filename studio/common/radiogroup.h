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

#ifndef __RADIOGROUP_H
#define __RADIOGROUP_H

#include "../bfc/virtualwnd.h"
#include "../bfc/ptrlist.h"

//!## A RadioGroup is a non-displaying user interface object whose purpose is
//!## to act as an event-handling object for radio group functionality to a set
//!## of any number of objects derived from CheckWnd.

// Remember, RadioWnd is identical to CheckWnd save that it uses different default art.

class CheckWnd;

#define RADIOGROUP_PARENT VirtualWnd
class COMEXP RadioGroup : public RADIOGROUP_PARENT {
public:
  RadioGroup();
  void addChild(CheckWnd *child);
  CheckWnd *enumChild(int i);
  // Poll the state of the radio buttons as a group.
  int setSelectedItem(int item); // return code is notifyParent return code.
  int getSelectedItem();
  // for both get and set, an item number of -1 means "nothing is set"

  virtual int childNotify(RootWnd *which, int msg, int param1, int param2);
protected:
  PtrList<CheckWnd>   checkWndList;
  int                 ignoreChildEvents; // used to discern the difference
                                         // between events we create and events
                                         // to which we respond.
};

#endif
