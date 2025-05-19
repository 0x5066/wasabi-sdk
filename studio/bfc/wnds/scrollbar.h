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

#ifndef __SCROLLBAR_H
#define __SCROLLBAR_H

#include "../virtualwnd.h"
#include "../region.h"
#include "../usermsg.h"
#include "../autobitmap.h"

#define SCROLLBAR_FULL 65535

#define POS_NONE    0
#define POS_LEFT    1
#define POS_BUTTON  2
#define POS_RIGHT   3

#define PAGE_NONE   0
#define PAGE_DOWN   1
#define PAGE_UP     2

#define DEFAULT_HEIGHT 16

#define SCROLLBAR_PARENT VirtualWnd

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ScrollBar : public SCROLLBAR_PARENT {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ScrollBar();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ScrollBar();

  virtual int onMouseMove (int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onLeftButtonDown(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onLeftButtonUp(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onRightButtonDown(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onRightButtonUp(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onMouseWheelUp(int clicked, int lines);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onMouseWheelDown(int clicked, int lines);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onPaint(Canvas *canvas);
  
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
  virtual int onInit();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void timerCallback(int id);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantDoubleClicks() { return 0; };

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onSetPosition(BOOL smooth=FALSE);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onSetFinalPosition();

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setBitmaps(char *left, char *lpressed, char *lhilite,
                  char *right, char *rpressed, char *rhilite, 
                  char *button, char *bpressed, char *bhilite);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setPosition(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getPosition();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getHeight();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setHeight(int newheight);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setNPages(int n);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void gotoPage(int n);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setUpDownValue(int newupdown);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setVertical(BOOL isvertical);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getWidth();

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void freeResources();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void reloadResources();

private:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void deleteResources();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getMousePosition();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void calcOverlapping();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void calcXPosition();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void calcPosition();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void handlePageUpDown();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int checkPageUpDown();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void handleUpDown();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int checkUpDown();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int pageUp();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int pageDown();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int upDown(int which);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setPrivatePosition(int pos, BOOL signal=TRUE, BOOL smooth=FALSE);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void loadBmps();

  AutoSkinBitmap bmpleft, bmplpressed, bmplhilite,
             bmpright, bmprpressed, bmprhilite,
             bmpbutton, bmpbpressed, bmpbhilite;

  RegionI *leftrgn, *rightrgn, *buttonrgn;
  int position;

  int moving;
  int lefting;
  int righting;
  int clicked;

  int buttonx;

  int curmouseposition;
  int clickmouseposition;
  int height;

  int shiftleft, shiftright;
  POINT clickpos;
  int clickbuttonx;
  int pageing;
  int firstdelay;
  int timer;
  int npages;
  int pageway;
  int updown;
  int timer2;
  int insetpos;

  int vertical;
  int lastx, lasty;
};


#endif
