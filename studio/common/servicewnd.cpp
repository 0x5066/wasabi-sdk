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

//!## ServiceWnd.cpp - Talks to studio's SvcWnd (xml svc window). Inherit from this when creating your own service windows

#include "servicewnd.h"
#include "../bfc/notifmsg.h"

int ServiceWnd::onLeftButtonDown(int x, int y) {
  SERVICEWND_PARENT::onLeftButtonDown(x, y);
  notifyParent(ChildNotify::SVCWND_LBUTTONDOWN, x, y);
  return 1;
}

int ServiceWnd::onLeftButtonUp(int x, int y){
  SERVICEWND_PARENT::onLeftButtonUp(x, y);
  notifyParent(ChildNotify::SVCWND_LBUTTONUP, x, y);
  return 1;
}

int ServiceWnd::onRightButtonDown(int x, int y){
  SERVICEWND_PARENT::onRightButtonDown(x, y);
  notifyParent(ChildNotify::SVCWND_RBUTTONDOWN, x, y);
  return 1;
}

int ServiceWnd::onRightButtonUp(int x, int y){
  SERVICEWND_PARENT::onRightButtonUp(x, y);
  notifyParent(ChildNotify::SVCWND_RBUTTONUP, x, y);
  return 1;
}

int ServiceWnd::onMouseMove(int x, int y){
  SERVICEWND_PARENT::onMouseMove(x, y);
  notifyParent(ChildNotify::SVCWND_MOUSEMOVE, x, y);
  return 1;
}

int ServiceWnd::onLeftButtonDblClk(int x, int y){
  SERVICEWND_PARENT::onLeftButtonDblClk(x, y);
  notifyParent(ChildNotify::SVCWND_LBUTTONDBLCLK, x, y);
  return 1;
}

int ServiceWnd::onRightButtonDblClk(int x, int y){
  SERVICEWND_PARENT::onRightButtonDblClk(x, y);
  notifyParent(ChildNotify::SVCWND_RBUTTONDBLCLK, x, y);
  return 1;
}

