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

#include "groupclickwnd.h"
#include "../../common/script/guiobject.h"
#include "../notifmsg.h"

GroupClickWnd::GroupClickWnd() {
  trap = NULL;
  inarea = 0;
}

GroupClickWnd::~GroupClickWnd() {
  delete trap;
}

void GroupClickWnd::abstract_onNewContent() {
  delete trap;
  trap = NULL;

  if (!abstract_getContent()) return;

  GuiObject *mousetrap = abstract_getContent()->guiobject_findObject("mousetrap");

  trap = new MouseTrap(this, mousetrap->guiobject_getScriptObject());
}

void GroupClickWnd::groupclick_onLeftPush() {
  notifyParent(ChildNotify::BUTTON_LEFTPUSH);
}

void GroupClickWnd::groupclick_onRightPush() {
  notifyParent(ChildNotify::BUTTON_RIGHTPUSH);
}

void GroupClickWnd::content_onLeftButtonDown() {
  notifyParent(ChildNotify::CLICKWND_LEFTDOWN);
}

void GroupClickWnd::content_onLeftButtonUp() {
  notifyParent(ChildNotify::CLICKWND_LEFTUP);
  if (inarea) groupclick_onLeftPush();
}

void GroupClickWnd::content_onRightButtonDown() {
  notifyParent(ChildNotify::CLICKWND_RIGHTDOWN);
}

void GroupClickWnd::content_onRightButtonUp() {
  notifyParent(ChildNotify::CLICKWND_RIGHTUP);
  if (inarea) groupclick_onRightPush();
}

void GroupClickWnd::content_onEnterArea() {
  inarea = 1;
}

void GroupClickWnd::content_onLeaveArea() {
  inarea = 0;
}


void MouseTrap::hook_onLeftButtonDown(int x, int y) { 
  window->content_onLeftButtonDown(); 
}

void MouseTrap::hook_onLeftButtonUp(int x, int y) { 
  window->content_onLeftButtonUp(); 
}

void MouseTrap::hook_onRightButtonDown(int x, int y) { 
  window->content_onRightButtonDown(); 
}

void MouseTrap::hook_onRightButtonUp(int x, int y) { 
  window->content_onRightButtonUp(); 
}

void MouseTrap::hook_onEnterArea() { 
  window->content_onEnterArea(); 
}

void MouseTrap::hook_onLeaveArea() { 
  window->content_onLeaveArea(); 
}
