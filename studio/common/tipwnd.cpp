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

//#include "../studio/waclient.h"
#include "tipwnd.h"
#include "../bfc/canvas.h"

#define TIMER_ID 1020

TipWnd::TipWnd() {
  size = 14;
  tip = NULL;
  tipparent = NULL;
  spawned = FALSE;
}

TipWnd::~TipWnd() {
  if (tip) FREE(tip);
}

int TipWnd::onInit() {
  TIPWND_PARENT::onInit();
  return 1;
}

int TipWnd::onPaint(Canvas *canvas) {
  PaintCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaint(this)) return 0;
    canvas = &paintcanvas;
  }

  RECT r;
  getClientRect(&r);
  canvas->fillRect(&r, RGB(255, 255, 210));
  canvas->drawRect(&r, TRUE, RGB(0,0,0));

//  canvas->setTextVariable(TRUE); //FGTEMP!
  canvas->setTextOpaque(FALSE);
  canvas->pushTextSize(size);
  canvas->pushTextColor(RGB(0, 0, 0));
  canvas->textOut(2,1,tip);
  canvas->popTextColor();
  canvas->popTextSize();

return 1;
}

void TipWnd::setTipSize(int newsize) {
  size = newsize;
}

void TipWnd::setTip(char *newtip) {
  if (tip) FREE(tip);
  tip = STRDUP(newtip);
}

void TipWnd::spawnTip(HINSTANCE inst, BaseWnd *_tipparent) {
  tipparent = _tipparent;
  spawned=TRUE;
  setStartHidden(TRUE);
  init(inst, NULL);
  SetWindowLong(gethWnd(), GWL_STYLE, (GetWindowLong(gethWnd(), GWL_STYLE) & ~WS_SYSMENU));
  SetWindowLong(gethWnd(), GWL_EXSTYLE, (GetWindowLong(gethWnd(), GWL_EXSTYLE) | WS_EX_TOOLWINDOW));
  int x,y;
  Std::getMousePos(&x, &y);

  WndCanvas *c = new WndCanvas;
  c->attachToClient(this);
  int w,h;
//  c->setTextVariable(TRUE); //FGTEMP!
  c->pushTextSize(size);
  w = c->getTextWidth(tip);
  h = c->getTextHeight();
  c->popTextSize();
  delete c;
  RECT r;
  r.left = x;
  r.right = x+w+4;
  r.top = y+16;
  r.bottom = y+h*2+4;
  Std::ensureVisible(&r);
  resize(&r);
  setVisible(TRUE);
  SetWindowPos(gethWnd(), HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

LRESULT TipWnd::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_ACTIVATE) {
    SetActiveWindow((HWND) lParam);
    return 0;
  }
  return TIPWND_PARENT::wndProc(hWnd, uMsg, wParam, lParam);
}

void TipWnd::timerCallback(int id) {
BOOL deleteme=FALSE;

switch (id)
  {
  case TIMER_ID:
    setVisible(0);
    deleteme = TRUE;
    break;
  default:
    TIPWND_PARENT::timerCallback(id);
    break;
  }
if (deleteme)
  delete this; 
}

int TipWnd::onMouseWheelDown(int clicked, int lines) {
  PostMessage(tipparent->gethWnd(), WM_USER+8976, 0, 0);
  return 1;
}

int TipWnd::onMouseWheelUp(int clicked, int lines) {
  PostMessage(tipparent->gethWnd(), WM_USER+8976, 0, 0);
  return 1;
}

