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

#ifndef _CLICKWND_H
#define _CLICKWND_H

// this class defines clicking behavior, i.e. detecting mouse downs and ups
// and doing captures to determine clicks

#include "common.h"
#include "../bfc/wnds/backbufferwnd.h"

#define CLICKWND_PARENT BackBufferWnd

class COMEXP NOVTABLE ClickWnd : public CLICKWND_PARENT {
protected:
  ClickWnd();
  virtual ~ClickWnd();

public:
  void setHandleRightClick(int tf);
  int getHandleRightClick();

  // override these to get clicks!
  virtual void onLeftPush(int x, int y) {}
  virtual void onRightPush(int x, int y) {}
  virtual void onLeftDoubleClick(int x, int y) {}  
  virtual void onRightDoubleClick(int x, int y) {} 

  virtual void onEnterArea();
  virtual void onLeaveArea();

  virtual void onSetVisible(int show);
  virtual void onCancelCapture();
  virtual int isInClick() { return mousedown; }

protected:
  virtual int onLeftButtonDown(int x, int y);
  virtual int onRightButtonDown(int x, int y);
  virtual int onLeftButtonUp(int x, int y);
  virtual int onRightButtonUp(int x, int y);
  virtual int onMouseMove(int x, int y);

  // override this and return 0 to ignore clicks
  virtual int wantClicks() { return 1; }
  // override this and return 1 to force down-ness
  virtual int userDown() { return 0; }

  int getHilite() { return hilite; }	// mouse is over, period
  int getDown() { return down; }	// mouse is over and pushing down

  int onButtonDown(int which, int x, int y);
  int onButtonUp(int which, int x, int y);
  void _enterCapture();

private:
  void _onEnterArea();
  void _onLeaveArea();

  int button;	// 0 == left, 1 == right, which button was pushed
  int handleRight:1;
  int mousedown:1;
  int mcaptured:1;	// we are capturing the mouse
  int hilite:1;	// mouse is over but not down
  int down:1;
  int areacheck;
};

#endif
