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

#ifndef __CHECKWND_H
#define __CHECKWND_H

#include "common.h"
#include "../bfc/autobitmap.h"
#include "clickwnd.h"

class Canvas;	// see canvas.h

#define CHECKWND_PARENT ClickWnd
class COMEXP CheckWnd : public CHECKWND_PARENT {
public:
  CheckWnd();

  virtual int onPaint(Canvas *c);

  BOOL isChecked() const;
  void setChecked(BOOL is);
  void toggle();

  virtual void onLeftPush(int x, int y);

  // override this to catch clicks
  virtual int onClick() { return 1; }
  
  void setBitmaps(const char *unchecked, const char *checked);

private:  
  AutoSkinBitmap nonchecked, checked;
  int status;
};

#endif
