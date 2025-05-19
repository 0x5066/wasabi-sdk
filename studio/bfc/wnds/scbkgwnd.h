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

#ifndef __SCRLBKGWND_H
#define __SCRLBKGWND_H

#include "../canvas.h"
#include "../autobitmap.h"
#include "../../common/labelwnd.h"
#include "scrollbar.h"
#include "../../common/sepwnd.h"

#define SCRLBKGWND_PARENT LabelWnd


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ScrlBkgWnd : public SCRLBKGWND_PARENT {
protected:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ScrlBkgWnd();
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ScrlBkgWnd();
  
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onInit();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onPaint(Canvas *c);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void drawBackground(Canvas *canvas);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onEraseBkgnd(HDC dc);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int childNotify(RootWnd *child, int msg, int param1, int param2);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onResize();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void getClientRect(RECT *r);
//  virtual void getNonClientRect(RECT *r);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getHeaderHeight();
  virtual void timerCallback (int id);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onHScrollToggle(int set);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onVScrollToggle(int set);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onSetVisible(int show);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantHScroll() { return 1; }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantVScroll() { return 1; }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void makeWindowOverlayMask(Region *r);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  SkinBitmap *getBgBitmap(void);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setBgBitmap(char *b);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setBgColor(COLORREF rgb);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getBgColor(void);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getContentsWidth();	// not safe to call getclientrect!
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getContentsHeight();	// not safe to call getclientrect!

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setLineHeight(int h);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getLinesPerPage();

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getScrollX();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getScrollY();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getScrollbarWidth();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void scrollToY(int y, int signal=TRUE);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void scrollToX(int x, int signal=TRUE);

protected:

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setSlidersPosition();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int needDoubleBuffer();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  BOOL needHScroll();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  BOOL needVScroll();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getMaxScrollY();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getMaxScrollX();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void updateScrollY(BOOL smooth=FALSE);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void updateScrollX(BOOL smooth=FALSE);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void smoothScrollToY(int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void smoothScrollToX(int x);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void updateVScroll(int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void updateHScroll(int x);

  AutoSkinBitmap bmp;
  
  int dbbuffer;
  BOOL inDestroy;

  ScrollBar *hScroll;
  ScrollBar *vScroll;
	SepWnd *hSep;
	SepWnd *vSep;

	COLORREF bgColor;

	int scrollX;
	int scrollY;

	BOOL needSetSliders;
	BOOL wantsep;

	int lineHeight;

	float smoothScrollYInc, smoothScrollXInc;
	float smoothScrollYCur, smoothScrollXCur;
	int smoothScrollYTimerCount, smoothScrollXTimerCount;
	int smoothYTimer, smoothXTimer;
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void killSmoothYTimer();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void killSmoothXTimer();
  double lastratio;
  RECT smsqr;
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void _setSlidersPosition();
  int in_set_slider_position;
};

#endif
