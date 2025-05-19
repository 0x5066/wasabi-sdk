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

#include "radiogroup.h"
#include "checkwnd.h"
#include "../bfc/notifmsg.h"

RadioGroup::RadioGroup() {
  ignoreChildEvents = 0;
}

void RadioGroup::addChild(CheckWnd *child) {
  checkWndList.addItem(child);
  child->setNotifyWindow(this);
}

CheckWnd *RadioGroup::enumChild(int i) {
  return checkWndList.enumItem(i);
}

// Poll the state of the radio buttons as a group.
int RadioGroup::setSelectedItem(int item) {
  int i, num = checkWndList.getNumItems();
  for (i = 0; i < num; i++) {
    if (checkWndList[i]->isChecked()) {
      checkWndList[i]->setChecked(0);
    }
  }
  // Make sure it's there before you use its pointer.
  CheckWnd *newItem = checkWndList.enumItem(item);
  if (newItem) {
    newItem->setChecked(1);
  }
  // This allows you to unset all if you use an item value of -1

  return 1;
}

int RadioGroup::getSelectedItem() {
  int i, num = checkWndList.getNumItems();
  for (i = 0; i < num; i++) {
    if (checkWndList[i]->isChecked()) {
      return i;
    }
  }
  return -1;
}

int RadioGroup::childNotify(RootWnd *which, int msg, int param1, int param2) {
	int checkid = -1, uncheckid = -1;
  if (msg == ChildNotify::CHECKWND_CLICK) {
    // Don't forget to static cast to be able to compare the pointers.
    CheckWnd *whichCheckWnd = static_cast<CheckWnd *>(which);
    int i, num = checkWndList.getNumItems();
    for (i = 0; i < num; i++) {
      if (whichCheckWnd == checkWndList[i]) {
		    checkWndList[i]->setChecked(1);
				checkid = checkWndList[i]->getNotifyId();
      } else if (checkWndList[i]->isChecked()) {
		    checkWndList[i]->setChecked(0);
				uncheckid = checkWndList[i]->getNotifyId();
      }
    }
  }

	// This then allows for our CHECKWND message to pass to the parent,
	// but the param is altered to note that the item is always checked.
	if (checkid != -1) {
	  passNotifyUp(which, msg, 1, param2);

		// If the user only clicked on the same button again,
		// send a -1 as the unchecked status.
		return notifyParent(ChildNotify::RADIOGROUP_STATECHANGE,checkid,uncheckid);
	}
	// WTF?!@?
	return 0;
}
