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

#include "checkwnd.h"
#include "../bfc/canvas.h"
#include "../bfc/notifmsg.h"

CheckWnd::CheckWnd() {
  status = 0;
  setBitmaps("wasabi.button.checkbox", "wasabi.button.checkbox.pressed");
}

void CheckWnd::setChecked(BOOL is) {
  is = !!is;
  int olds=status;
  status = is;
  if (olds != status)
    invalidate();
}

int CheckWnd::onPaint(Canvas *canvas) {
  PaintBltCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaintNC(this)) return 0;
    canvas = &paintcanvas;
  }

  CHECKWND_PARENT::onPaint(canvas);

  AutoSkinBitmap &bm = status ? checked : nonchecked;

  RECT r;
  getClientRect(&r);

  bm.stretchToRectAlpha(canvas, &r);

  return 1;
}

void CheckWnd::setBitmaps(const char *uncheck, const char *check) {
  nonchecked = uncheck;
  checked = check;
}

BOOL CheckWnd::isChecked() const {
  return status;
}

void CheckWnd::onLeftPush(int x, int y) {
  toggle();
  onClick();
  invalidate();
}

void CheckWnd::toggle() {
  setChecked(!isChecked());
  notifyParent(ChildNotify::CHECKWND_CLICK, status, 0);
}

