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

//NONPORTABLE

#include "droplist.h"
#include "../bfc/canvas.h"
#include "../bfc/notifmsg.h"
#include "../studio/api.h"

#include "../studio/assert.h"

#define ID_DROPLIST 4858

DropList::DropList() {
  droplistWnd = NULL;
  label = NULL;
  labelwidth = 0;
  dropheight = 128;
}
DropList::~DropList() {
  if (droplistWnd != NULL) DestroyWindow(droplistWnd);
  FREE(label);
}

int DropList::onInit() {
  DROPLIST_PARENT::onInit();

  RECT r;
  getClientRect(&r);

  droplistWnd = CreateWindow("COMBOBOX", NULL,
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT | WS_VSCROLL,
    r.left, r.top, r.right - r.left, r.bottom - r.top,
    gethWnd(), (HMENU)ID_DROPLIST,
    gethInstance(), NULL);
  ASSERT(droplistWnd != NULL);

  SendMessage(droplistWnd, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), FALSE);

  return 1;
}

int DropList::onPaint(Canvas *canvas) {
  PaintCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaint(this)) return 0;
    canvas = &paintcanvas;
  }
  DROPLIST_PARENT::onPaint(canvas);

  RECT r;
  getClientRect(&r);
  RenderBaseTexture(canvas, r);

  if (labelwidth > 0 && label != NULL) {
    canvas->fillRect(&r, RGB(0, 0, 0));//SKIN
    canvas->setTextOpaque(FALSE);
    canvas->setTextColor(RGB(0, 255, 0));//SKIN
    int h = (r.bottom - r.top) - canvas->getTextHeight();
    canvas->textOut(r.left, r.top+h/2, label);
  }

  return TRUE;
}

int DropList::onResize() {
  DROPLIST_PARENT::onResize();
  RECT r;
  getClientRect(&r);
  r.left += labelwidth;
  RECT d;
  GetClientRect(droplistWnd, &d);
  if (!EqualRect(&d, &r))
    SetWindowPos(droplistWnd, NULL, r.left, r.top, r.right-r.left, r.bottom-r.top, SWP_NOZORDER|SWP_NOSENDCHANGING);
  setDroppedHeight(dropheight);
  return 1;
}

LRESULT DropList::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_COMMAND:
      switch (HIWORD(wParam)) {
        case CBN_SELCHANGE: {
          int pos = SendMessage(droplistWnd, CB_GETCURSEL, 0, 0);
          onItemSel(pos);
	  notifyParent(ChildNotify::DROPLIST_SEL_CHANGED, pos);
        }
        break;
      }
    break;
  }
  return DROPLIST_PARENT::wndProc(hWnd, uMsg, wParam, lParam);
}

int DropList::addItem(const char *item) {
  ASSERTPR(droplistWnd != NULL, "must add items after initing");
  return SendMessage(droplistWnd, CB_ADDSTRING, (WPARAM)0, (LPARAM)item);
}

int DropList::setCurSel(int pos) {
  int r;
  ASSERTPR(droplistWnd != NULL, "must call initing");
  r = SendMessage(droplistWnd, CB_SETCURSEL, (WPARAM)pos, (LPARAM)0);
  if (r != CB_ERR) onItemSel(pos);
  return r;
}

int DropList::getCurSel() {
  int r = SendMessage(droplistWnd, CB_GETCURSEL, 0, 0);
  return (r == CB_ERR) ? -1 : r;
}

int DropList::getItemText(int pos, char *text, int textlen) {
  int l = SendMessage(droplistWnd, CB_GETLBTEXTLEN, (WPARAM)pos, 0);
  if (l == CB_ERR) return -1;
  if (l+1 < textlen)
  {
    return CB_ERR == SendMessage(droplistWnd, CB_GETLBTEXT,(WPARAM)pos, (LPARAM)text);
  }
  char *tmp=(char*)MALLOC(l+1);
  l = SendMessage(droplistWnd, CB_GETLBTEXT,(WPARAM)pos, (LPARAM)tmp);
  MEMCPY(text,tmp,textlen-1);
  FREE(tmp);

  text[textlen-1]=0;
  return CB_ERR == l;
}


int DropList::deleteItem(int item)
{
  ASSERTPR(droplistWnd != NULL, "must delete items after initing");
  return CB_ERR == SendMessage(droplistWnd,CB_DELETESTRING,(WPARAM)item,0);
}

int DropList::deleteAllItems()
{
  ASSERTPR(droplistWnd != NULL, "must delete all items after initing");
  return CB_ERR == SendMessage(droplistWnd,CB_RESETCONTENT,0,0);
}


int DropList::getNumItems()
{
  if (!droplistWnd) return 0;
  return SendMessage(droplistWnd,CB_GETCOUNT,0,0);
}

void DropList::setLabel(char *_label) {
  FREE(label); label = NULL;
  labelwidth = 0;

  if (_label != NULL) {
    label = STRDUP(_label);
    WndCanvas wc;
    wc.attachToClient(this);
    labelwidth = wc.getTextWidth(label)+2;
  }

  onResize();
}

int DropList::onEraseBkgnd(HDC dc) {
  return 1;
}

void DropList::setDroppedHeight(int height) {
  if (!gethWnd() || !isInited()) return;
  dropheight = height;
  RECT r;
  GetClientRect(droplistWnd, &r);
  MoveWindow(droplistWnd, r.left, r.top, r.right, height, TRUE);
}
