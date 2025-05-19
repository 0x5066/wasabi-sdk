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

#include "grouptgbutton.h"
#include "../../common/script/guiobject.h"
#include "../notifmsg.h"

GroupToggleButton::GroupToggleButton() {
  status = STATUS_OFF;
  on = new GroupClickWnd();
  off = new GroupClickWnd();
}

GroupToggleButton::~GroupToggleButton() {
  delete on;
  delete off;
}

void GroupToggleButton::setGroups(const char *_on, const char *_off) {
  on_id = _on;
  if (on) on->setContent(on_id);
  off_id = _off;
  if (off) off->setContent(off_id);
}

int GroupToggleButton::onInit() {
  int rt = GROUPTOGGLEBUTTON_PARENT::onInit();
  initGroups();
  return rt;
}

void GroupToggleButton::initGroups() {
  on->setStartHidden(status == STATUS_ON ? 0 : 1); off->setStartHidden(status == STATUS_ON ? 1 : 0);
  on->setContent(on_id); off->setContent(off_id);
  on->setParent(this); off->setParent(this);
  on->init(this); off->init(this);
  rootwndholder_setRootWnd(status == STATUS_ON ? on : off);
}

int GroupToggleButton::childNotify(RootWnd *child, int msg, int param1, int param2) {
  if (child == on || child == off) {
    switch (msg) {
      case ChildNotify::BUTTON_LEFTPUSH: {
        if (wantFullClick()) grouptoggle_onLeftPush();
        return 1;
      }
      case ChildNotify::BUTTON_RIGHTPUSH: {
        if (wantFullClick()) grouptoggle_onRightPush();
        return 1;
      }
      case ChildNotify::CLICKWND_LEFTDOWN: {
        if (!wantFullClick()) grouptoggle_onLeftPush();
        return 1;
      }
      case ChildNotify::CLICKWND_RIGHTDOWN: {
        if (!wantFullClick()) grouptoggle_onRightPush();
        return 1;
      }
    }
  }
  return GROUPTOGGLEBUTTON_PARENT::childNotify(child, msg, param1, param2);
}

void GroupToggleButton::toggle() {
  if (status == STATUS_OFF) {
    if (isInited()) {
      off->setVisible(0);
      on->setVisible(1);
      rootwndholder_setRootWnd(on);
    }
    status = STATUS_ON;
  } else {
    if (isInited()) {
      on->setVisible(0);
      off->setVisible(1);
      rootwndholder_setRootWnd(off);
    }
    status = STATUS_OFF;
  }
  notifyParent(ChildNotify::GROUPCLICKTGBUTTON_TOGGLE, status);
}

void GroupToggleButton::setStatus(int s) { 
  if (s != status) 
    toggle(); 
}

int GroupToggleButton::wantFullClick() {
  return 0;
}

void GroupToggleButton::grouptoggle_onLeftPush() {
  notifyParent(ChildNotify::GROUPCLICKTGBUTTON_CLICKED);
  if (!wantAutoToggle()) return;
  if (status == STATUS_ON && !off_id.isempty() || status == STATUS_OFF && !on_id.isempty())
    toggle();
}

void GroupToggleButton::grouptoggle_onRightPush() {
}

GroupClickWnd *GroupToggleButton::enumGroups(int n) {
  if (n == 0) return on;
  if (n == 1) return off;
  return NULL;
}

int GroupToggleButton::getNumGroups() {
  int i=0;
  if (on != NULL) i++;
  if (off != NULL) i++;
  return i;
}

