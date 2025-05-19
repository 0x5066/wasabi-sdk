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
#ifndef _EDITWND_H
#define _EDITWND_H

#include "guiobjwnd.h"
#include "../bfc/skinclr.h"
#include "./usermsg.h"
#include "common.h"

#define EDITWND_PARENT GuiObjectWnd
class COMEXP EditWnd : public EDITWND_PARENT {
public:
  EditWnd(char *buffer=NULL, int buflen=0);
  virtual ~EditWnd();

  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  virtual int onResize();
  virtual LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  // mig: Made these virtual to allow to be accessed by 
  // EditWndString object in editwndstring.h
  virtual void setBuffer(char *buffer, int len);
  virtual void getBuffer(char *outbuf, int len);

  void setModal(int modal);	//if modal, deletes self on enter
  void setAutoEnter(int a);	//fake an onEnter event when lose focus
  int getAutoEnter() { return autoenter; }
  void setAutoSelect(int a);	//true==grab the focus on init
  void setIdleTimerLen(int ms);	// how many ms keys are idle before send msg
  virtual void onSetVisible(int show);
  virtual int onGetFocus();
  virtual int wantFocus();
  virtual void selectAll();
  virtual void enter();
  virtual void setIdleEnabled(int i) { idleenabled = i; }
  virtual int getIdleEnabled() { return idleenabled; }

  void setBorder(int border);
  int getTextLength();
  
  HWND getEditWnd();
  virtual int handleRatio() { return 0; }
  virtual int getAutoSelect() { return autoselect; }

  void setMultiline(int ml);
  void setReadOnly(int ro);
  void setAutoHScroll(int hs);
  void setAutoVScroll(int vs);
  void setVScroll(int vs);
  int isEditorKey(int vk);

  // the wndproc for the edit box
  virtual LRESULT editWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
  virtual void timerCallback(int id);

  // call down on these if you override them
  virtual void onEditUpdate();
  virtual void onIdleEditUpdate();
  virtual int onEnter();	// user hit enter.. return 1 to close window
  virtual int onAbort();	// user hit escape.. return 1 to close window
  virtual int onLoseFocus();	// different from onKillFocus() from BaseWnd!

  void setStyle(LONG style, int set);

private:
  HWND editWnd;
  WNDPROC prevWndProc;
  int maxlen;
  int retcode;
  int idletimelen;
  int modal;
  int bordered;
  int autoenter;
  int beforefirstresize;
  int autoselect;
  int multiline;
  int readonly;
  int idleenabled;
  int autohscroll,autovscroll,vscroll;
  int nextenterfaked;
  SkinColor backgroundcolor, textcolor;
  HBRUSH oldbrush;

  // Basically, we're redoing the functionality of EditWndString 
  // (the bigger version), so we'll probably erase EditWndString 
  // completely as an object.
  MemBlock<char> buffer8;
  char *outbuf;
};

#define EDITWND_RETURN_NOTHING	0	// user didn't do nothing
#define EDITWND_RETURN_OK	1	// user hit return
#define EDITWND_RETURN_CANCEL	2	// user hit escape or something

#endif
