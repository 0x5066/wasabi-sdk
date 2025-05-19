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
#include "editwnd.h"
#include "../bfc/canvas.h"
#include "../bfc/notifmsg.h"

#include "../studio/assert.h"
#include "../bfc/std.h"

#define ID_EDITCHILD 12

enum { IDLETIMER=8 };
#define IDLETIME 350 // comprimises suck ;)


#define UTF8 1
#if UTF8
#ifdef WANT_UTF8_WARNINGS
#pragma CHAT("mig", "all", "UTF8 is enabled in editwnd.cpp -- Things might be screwy till it's all debugged?")
#endif
# include "../bfc/encodedstr.h"
#endif

#include <commctrl.h>


static LRESULT CALLBACK static_editWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  EditWnd *editwnd = (EditWnd *)GetWindowLong(hWnd, GWL_USERDATA);
  if (editwnd == NULL) return DefWindowProc(hWnd, uMsg, wParam, lParam);
  return editwnd->editWndProc(hWnd, uMsg, wParam, lParam);
}

EditWnd::EditWnd(char *buffer, int buflen) {
  nextenterfaked = 0;
  idleenabled = 1;
  beforefirstresize=1;
  editWnd = NULL;
  prevWndProc = NULL;
  setBuffer(buffer, buflen);
  maxlen = 0;
  retcode = EDITWND_RETURN_NOTHING;
  modal = 0;
  autoenter = 0;
  autoselect = 0;
  outbuf = NULL;
//  bordered = 0;
  idletimelen = IDLETIME;
  multiline = 0;
  readonly = 0;
  autohscroll = 1;
  autovscroll = 1;
  vscroll = 0;
  oldbrush = NULL;
  backgroundcolor = "wasabi.edit.background";
  textcolor = "wasabi.edit.text";
  setVirtual(0);
}

EditWnd::~EditWnd() {
  if (oldbrush != NULL) 
    DeleteObject(oldbrush);
  oldbrush = NULL;
  killTimer(IDLETIMER);
  if (editWnd != NULL) {
    SetWindowLong(editWnd, GWL_USERDATA, (LONG)0);
    SetWindowLong(editWnd, GWL_WNDPROC, (LONG)prevWndProc);
    DestroyWindow(editWnd);
  }
  notifyParent(ChildNotify::RETURN_CODE, retcode);
}

int EditWnd::onInit() {
  EDITWND_PARENT::onInit();

  RECT r = clientRect();

  if (Std::encodingSupportedByOS(SvcStrCnv::UTF16)) {
    editWnd = CreateWindowW(L"EDIT", NULL,
      WS_CHILD | (autohscroll?ES_AUTOHSCROLL:0) | (readonly?ES_READONLY:0) | (multiline?ES_MULTILINE:0)
      | (autovscroll?ES_AUTOVSCROLL:0) | (vscroll?WS_VSCROLL:0),
      r.left, r.top, r.right - r.left, r.bottom - r.top,
      gethWnd(), (HMENU)ID_EDITCHILD,
      gethInstance(), NULL);
    ASSERT(editWnd != NULL);
  } else {
    editWnd = CreateWindow("EDIT", NULL,
      WS_CHILD | (autohscroll?ES_AUTOHSCROLL:0) | (readonly?ES_READONLY:0) | (multiline?ES_MULTILINE:0)
      | (autovscroll?ES_AUTOVSCROLL:0) | (vscroll?WS_VSCROLL:0),
      r.left, r.top, r.right - r.left, r.bottom - r.top,
      gethWnd(), (HMENU)ID_EDITCHILD,
      gethInstance(), NULL);
    ASSERT(editWnd != NULL);
  }

  if ((maxlen != 0) && (outbuf != NULL)) {
    setBuffer(outbuf, maxlen);
  }

  // stash a pointer to us
  SetWindowLong(editWnd, GWL_USERDATA, (LONG)this);
  // subclass the edit control -- either by 8 or by 16
  if (Std::encodingSupportedByOS(SvcStrCnv::UTF16)) {
    // If we don't subclass with W, we won't be unicode.
    prevWndProc = (WNDPROC)SetWindowLongW(editWnd, GWL_WNDPROC, (LONG)static_editWndProc);
  } else {
    prevWndProc = (WNDPROC)SetWindowLong(editWnd, GWL_WNDPROC, (LONG)static_editWndProc);
  }

  SendMessage(editWnd, WM_SETFONT, (WPARAM)GetStockObject(ANSI_VAR_FONT), FALSE);
  ShowWindow(editWnd, !getStartHidden() ? SW_NORMAL : SW_HIDE);

  return 1;
}

void EditWnd::onSetVisible(int show) {
  EDITWND_PARENT::onSetVisible(show);
  if (editWnd == NULL) return;
  ShowWindow(editWnd, show ? SW_NORMAL : SW_HIDE);
}

int EditWnd::onPaint(Canvas *canvas) {
//  if (!bordered) return EDITWND_PARENT::onPaint(canvas);

  PaintCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaint(this)) return 0;
    canvas = &paintcanvas;
  }
  EDITWND_PARENT::onPaint(canvas);

  RECT r;
  getClientRect(&r);
  canvas->drawRect(&r, TRUE, RGB(0, 0, 0));	//SKIN
  
  return 1;
}

int EditWnd::onResize() {
  EDITWND_PARENT::onResize();
  RECT r = clientRect();
  if (1/*bordered*/) {
    r.top++;
    r.bottom--;
    r.left++;
    r.right--;
  }
  MoveWindow(editWnd, r.left, r.top, r.right-r.left, r.bottom-r.top, TRUE);

  if (beforefirstresize) { 
    ShowWindow(editWnd, SW_NORMAL); beforefirstresize=0;
    if (modal) {
      SetFocus(editWnd);
      if (getAutoSelect())
        SendMessage(editWnd, EM_SETSEL, 0, -1);
    }
  }

  return TRUE;
}

LRESULT EditWnd::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CTLCOLOREDIT: {
      HDC hdc = (HDC)wParam;
      SetTextColor(hdc, textcolor);
      SetBkColor(hdc, backgroundcolor);
      if (oldbrush != NULL) {
        DeleteObject(oldbrush);
        oldbrush = NULL;
      }
      oldbrush = CreateSolidBrush(backgroundcolor);
      return (LRESULT)oldbrush;
    }

    case WM_COMMAND: {
      switch (HIWORD(wParam)) {
        case EN_CHANGE: {
          if (maxlen > 0) {

            // When there is ANY change to the editbox, update the
            // utf8 buffer with the new string.
            if (Std::encodingSupportedByOS(SvcStrCnv::UTF16)) {
              String buf8;
              WCHAR temp16[WA_MAX_PATH];
              GetWindowTextW(editWnd, temp16, WA_MAX_PATH);
              EncodedStr buf16(SvcStrCnv::UTF16, temp16, (WSTRLEN(temp16)+1)*2, 0/*no delete*/);
              int retcode = buf16.convertToUTF8(buf8);
              if (retcode > 0) {
                STRNCPY(outbuf, buf8, maxlen);
              } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
                GetWindowText(editWnd, outbuf, maxlen);
              }
            } else {
              String buf8;
              WCHAR tempOS[WA_MAX_PATH];
              GetWindowTextW(editWnd, tempOS, WA_MAX_PATH);
              EncodedStr bufOS(SvcStrCnv::OSNATIVE, tempOS, (WSTRLEN(tempOS)+1)*2, 0/*no delete*/);
              int retcode = bufOS.convertToUTF8(buf8);
              if (retcode > 0) {
                STRNCPY(outbuf, buf8, maxlen);
              } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
                GetWindowText(editWnd, outbuf, maxlen);
              }
            }

            onEditUpdate();
          }
        }
        break;
        case EN_KILLFOCUS:
          onLoseFocus();
        break;
      }
    }
    break;
  }
  return EDITWND_PARENT::wndProc(hWnd, uMsg, wParam, lParam);
}

void EditWnd::setBuffer(char *buffer, int len) {
  if (buffer == NULL || len <= 1) return;
  ASSERT(len > 1);
  ASSERT(len < 0x7ffe);
  ASSERT(STRLEN(buffer) <= len);

#define USE_INTERNAL_BUFFER 0

#if USE_INTERNAL_BUFFER
  buffer8.setSize(len+1);
  outbuf = buffer8.getMemory();
  if (len) {
    STRNCPY(outbuf,buffer,len);
  }
  outbuf[len] = 0;
#else
  outbuf = buffer;
#endif

  if (editWnd != NULL) {
    if (Std::encodingSupportedByOS(SvcStrCnv::UTF16)) {
      EncodedStr buf16;
      int retcode = buf16.convertFromUTF8(SvcStrCnv::UTF16, String(buffer));
      if (retcode > 0) {
        SetWindowTextW(editWnd, static_cast<WCHAR *>(buf16.getEncodedBuffer()));
      } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
        SetWindowText(editWnd, buffer);
      }
    } else {
      EncodedStr bufOS;
      int retcode = bufOS.convertFromUTF8(SvcStrCnv::OSNATIVE, String(buffer));
      if (retcode > 0) {
        SetWindowText(editWnd, static_cast<char *>(bufOS.getEncodedBuffer()));
      } else if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
        SetWindowText(editWnd, buffer);
      }
    }

    // This is going to be problematic.  This is where utf8 sucks.
    // Just how many characters CAN we save in our buffer, eh?
    // (shrug) Oh well.  Can't be helped.   At most this many.
    SendMessage(editWnd, EM_LIMITTEXT, (WPARAM)len-1, (LPARAM)0);
    // hooray for halcyon7

    if (getAutoSelect()) {
      SetFocus(editWnd);
      SendMessage(editWnd, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
    }
  }

  maxlen = len;
}

void EditWnd::selectAll() {
  PostMessage(editWnd, EM_SETSEL, 0, -1);
}

void EditWnd::enter() {
  onEnter();
}

void EditWnd::getBuffer(char *buf, int _len) {
  if (_len > maxlen) _len = maxlen;
//  SendMessage(editWnd, WM_GETTEXT, (WPARAM)_len, (LPARAM)buf);
  MEMCPY(buf, outbuf, _len);
}

void EditWnd::setModal(int _modal) {
  modal = _modal;
}

void setBorder(int border) {
//  bordered = border;
}

int EditWnd::isEditorKey(int vk) {
  if (vk >= VK_F1) return 0;
  if ((vk == VK_UP || vk == VK_DOWN) || ((Std::keyDown(VK_CONTROL) || Std::keyDown(VK_MENU)) && (vk == VK_LEFT || vk == VK_RIGHT)))
    return 0;
  if (vk == VK_RETURN && Std::keyDown(VK_CONTROL)) return 0;
  if (vk == VK_CONTROL || vk == VK_MENU) return 0;
  return 1;
}

LRESULT EditWnd::editWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  switch (uMsg) {
    case WM_KEYDOWN:
      if (!isEditorKey((int)wParam) && !onKeyDown((int)wParam)) api->forwardOnKeyDown(this, (int) wParam);
      break;
    case WM_KEYUP:
      if (!isEditorKey((int)wParam) && !onKeyUp((int)wParam)) api->forwardOnKeyUp((int) wParam);
      break;
    case WM_CHAR:
      if (!(wParam == VK_RETURN && nextenterfaked && !autoenter)) {
        notifyParent(ChildNotify::EDITWND_KEY_PRESSED, wParam);
        onChar((TCHAR)wParam);
      }
      if (wParam == VK_RETURN) {
        if (!(nextenterfaked && !autoenter))
          if (onEnter()) return 0;
        nextenterfaked = 0;
        return 0;
      } else if (wParam == VK_ESCAPE) {
        if (onAbort()) return 0;
      }
    break;
  }
  return CallWindowProc(prevWndProc, hWnd, uMsg, wParam, lParam);
}

void EditWnd::timerCallback(int id) {
  switch (id) {
    case IDLETIMER:
      killTimer(IDLETIMER);
      if (idleenabled) onIdleEditUpdate();
    break;
    default:
      EDITWND_PARENT::timerCallback(id);
  }
}

void EditWnd::onEditUpdate() {
  killTimer(IDLETIMER);
  setTimer(IDLETIMER, idletimelen);
  notifyParent(ChildNotify::EDITWND_DATA_MODIFIED);
}

void EditWnd::onIdleEditUpdate() {
  notifyParent(ChildNotify::EDITWND_DATA_MODIFIED_ONIDLE);
}

int EditWnd::onEnter() {
  notifyParent(ChildNotify::EDITWND_ENTER_PRESSED);
  if (modal) {
    retcode = EDITWND_RETURN_OK;
    delete this;
    return 1;
  }
  return 0;
}

int EditWnd::onAbort() {
  notifyParent(ChildNotify::EDITWND_CANCEL_PRESSED);
  if (modal) {
    retcode = EDITWND_RETURN_CANCEL;
    delete this;
    return 1;
  }
  return 0;
}

int EditWnd::onLoseFocus() {	// fake an onEnter()
  if (autoenter) {
    nextenterfaked = 1;
    PostMessage(editWnd, WM_CHAR, VK_RETURN, 0);
  }
  return 0;
}

void EditWnd::setAutoEnter(int a) {
  autoenter = a;
}

void EditWnd::setAutoSelect(int a) {
  autoselect = a;
};

void EditWnd::setIdleTimerLen(int ms) {
  if (ms < 0) ms = 0;
  idletimelen = ms;
}

int EditWnd::getTextLength() { // TOTALLY NONPORTABLE AND TOTALLY DIRTY

  HFONT font = (HFONT)SendMessage(editWnd, WM_GETFONT, 0, 0);
  
  HDC sdc = GetDC(NULL);
  HDC dc = CreateCompatibleDC(sdc);
  ReleaseDC(NULL, sdc);

  HFONT oldfont = (HFONT)SelectObject(dc, font);
  
  SIZE s;
  GetTextExtentPoint32(dc, outbuf, STRLEN(outbuf), &s);
  SelectObject(dc, oldfont);
  DeleteDC(dc);
  return s.cx + SendMessage(editWnd, EM_GETMARGINS, 0, 0)*2+2;
}

HWND EditWnd::getEditWnd() {
  return editWnd;
}

void EditWnd::setMultiline(int ml) {
  multiline=ml;
  setStyle(ES_MULTILINE|ES_WANTRETURN,ml);
}

void EditWnd::setReadOnly(int ro) {
  readonly=ro;
}

void EditWnd::setAutoHScroll(int hs) {
  autohscroll=hs;
  setStyle(ES_AUTOHSCROLL,hs);
}

void EditWnd::setAutoVScroll(int vs) {
  autovscroll=vs;
  setStyle(ES_AUTOVSCROLL,vs);
}

void EditWnd::setVScroll(int vs) {
  vscroll=vs;
  setStyle(WS_VSCROLL,vs);
}

void EditWnd::setStyle(LONG style, int set) {
  if(editWnd) {
    LONG s=GetWindowLong(editWnd,GWL_STYLE);
    if(set) s|=style;
    else s&=~style;
    SetWindowLong(editWnd,GWL_STYLE,s);
  }
}

int EditWnd::onGetFocus() {
  int r = EDITWND_PARENT::onGetFocus();
  if (editWnd != NULL) 
  SetFocus(editWnd); 
  return r;
}	

int EditWnd::wantFocus() { 
  return 1; 
}
