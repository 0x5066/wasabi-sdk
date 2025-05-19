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

#ifndef _VIRTUALWND_H
#define _VIRTUALWND_H

#include "basewnd.h"

#define VIRTUALWND_PARENT BaseWnd
#define AUTOWH 0xFFFE
#define NOCHANGE 0xFFFD

class COMEXP NOVTABLE VirtualWnd : public VIRTUALWND_PARENT {
protected:
  VirtualWnd();
  virtual ~VirtualWnd();
public:	
  virtual int init(RootWnd *parent, int nochild=FALSE);
  virtual int init(HINSTANCE hInstance, HWND parent, int nochild=FALSE);

  virtual void bringToFront();
  virtual void bringToBack();
  virtual void bringAbove(BaseWnd *w);
  virtual void bringBelow(BaseWnd *w);

#ifdef WIN32
  //NONPORTABLE--avoid prolonged use
  virtual HWND gethWnd();
  virtual HINSTANCE gethInstance();
#endif

public:
  virtual void resize(int x, int y, int w, int h, int wantcb=1);
  virtual void resize(RECT *r, int wantcb=1);
//CUT  virtual void resize(RECT *r);
  virtual void move(int x, int y);
  virtual void invalidate();
  virtual void invalidateRect(RECT *r);
  virtual void invalidateRgn(Region *reg);
  virtual void validate();
  virtual void validateRect(RECT *r);
  virtual void validateRgn(Region *reg);
  virtual void getClientRect(RECT *);
  virtual void getNonClientRect(RECT *);
  virtual void getWindowRect(RECT *);
  virtual int beginCapture();
  virtual int endCapture();
  virtual int getCapture();
	virtual void setVirtualChildCapture(BaseWnd *child);
	virtual void repaint();
/*  virtual int focusNextSibbling(int dochild);
  virtual int focusNextVirtualChild(BaseWnd *child);*/
  virtual int cascadeRepaint(int pack=1); 
  virtual int cascadeRepaintRect(RECT *r, int pack=1); 
  virtual int cascadeRepaintRgn(Region *r, int pack=1); 
  virtual RootWnd *rootWndFromPoint(POINT *pt);
  virtual double getRenderRatio();
  virtual int reparent(RootWnd *newparent);
  virtual int setVirtual(int i);
  virtual RootWnd *getRootParent();
  virtual int gotFocus();
  virtual int onGetFocus();
  virtual int onKillFocus();
  virtual void setFocus(int skipnextautochildfocus=0);
  virtual int onActivate();
  virtual int onDeactivate();
  virtual void setVirtualChildFocus(RootWnd *child);
  virtual int wantFocus() { return 0; }
  virtual void setAllowDeactivation(int allow);
  virtual int allowDeactivation();

public:	
	virtual int isVirtual() { return !bypassvirtual; }

protected:
	int virtualX,virtualY,virtualH,virtualW;
  int bypassvirtual;
  int focus;
  int resizecount;
  double lastratio;
};

#endif
