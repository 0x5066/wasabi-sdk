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

//PORTABLE
#ifndef _TIPWND_H
#define _TIPWND_H

#include "../bfc/basewnd.h"
#include "common.h"

#define TIPWND_PARENT BaseWnd
class COMEXP TipWnd : public BaseWnd {
public:
  TipWnd();
  ~TipWnd();

  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
	virtual void timerCallback(int id);
  virtual int onMouseWheelUp(int click, int lines);
  virtual int onMouseWheelDown(int click, int lines);
  void setTipSize(int newsize);
  void setTip(char *newtip);
  void spawnTip(HINSTANCE inst, BaseWnd *tipparent);

private:
  int size;
  char *tip;
  BOOL spawned;
  BaseWnd *tipparent;

  virtual LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif
