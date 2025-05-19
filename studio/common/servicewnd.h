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

#ifndef __SERVICEWND_H
#define __SERVICEWND_H

#include "clickwnd.h"

#define SERVICEWND_PARENT VirtualWnd

class COMEXP ServiceWnd : public SERVICEWND_PARENT {
  public:

  virtual int onLeftButtonDown(int x, int y);
  virtual int onLeftButtonUp(int x, int y);
  virtual int onRightButtonDown(int x, int y);
  virtual int onRightButtonUp(int x, int y);
  virtual int onMouseMove(int x, int y);
  virtual int onLeftButtonDblClk(int x, int y);
  virtual int onRightButtonDblClk(int x, int y);
  virtual int wantDoubleClicks() { return 1; }
};

#if 0
template <class T>
class ServiceWndHolder : public ServiceWnd {
public:
  ServiceWndHolder() {
    child = new T;
  }
  virtual ~ServiceWndHolder() {
    delete child;
  }

  virtual int onInit() {
    ServiceWnd::onInit();
    child->init(this);
    child->setParent(this);
    return 1;
  }

  virtual int onResize() {
    ServiceWnd::onResize();
    if (child->isInited()) child->resize(&clientRect());
    return 1;
  }

  static GUID getWindowTypeGuid() { return T::getWindowTypeGuid(); }
  static const char *getWindowTypeName() { return T::getWindowTypeName(); }

private:
  T *child;
};
#endif

#endif
