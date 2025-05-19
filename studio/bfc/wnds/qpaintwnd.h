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

#ifndef __QPAINTWND_H
#define __QPAINTWND_H

#include "../../common/guiobjwnd.h"
#include "../../bfc/autobitmap.h"

//#include "../../common/svc_enum.h"
#include "../../studio/services/svc_skinfilter.h"

#define QUICKPAINTWND_PARENT GuiObjectWnd 

/**
  class QuickPaintWnd .

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
  @cat BFC
*/
class COMEXP QuickPaintWnd : public QUICKPAINTWND_PARENT {
  
  public:
    /**
      QuickPaintWnd constructor .

      @see ~QuickPaintWnd()
    */
    QuickPaintWnd();

    /**
      Destructor for QuickPaintWnd .

      @see QuickPaintWnd()
    */
    virtual ~QuickPaintWnd();

    /**
      QuickPaintWnd method onInit .

      @ret 1
    */
    virtual int onInit(); 
    virtual int onPaint(Canvas *c); 
  
    /**
      QuickPaintWnd method timerCallback .

      @param id Identifies requested action
    */
    virtual void timerCallback(int id);
    virtual void onSetVisible(int show);

    /**
      QuickPaintWnd method setRealtime .

      @see getRealtime()
      @param rt 
    */
    virtual void setRealtime(int rt);

    /**
      QuickPaintWnd method getRealtime .

      @see setRealtime()
      @ret realtime
    */
    virtual int getRealtime();

    /**
      QuickPaintWnd method setSpeed sets the timer interval in milliseconds. 

      @see getSpeed()
      @param ms The timer interval in milliseconds.
    */
    virtual void setSpeed(int ms);
    
    /**
      QuickPaintWnd method getSpeed gets the timer interval in milliseconds. 

      @see setSpeed()
      @param ms The timer interval in milliseconds.
    */
    virtual int getSpeed();

    /**
      QuickPaintWnd method startQuickPaint .
    */
    virtual void startQuickPaint();

    /**
      QuickPaintWnd method stopQuickPaint .
    */
    virtual void stopQuickPaint();
    
    /**
      QuickPaintWnd method isQuickPainting .
    */
    virtual int isQuickPainting();

    virtual int onQuickPaint(BltCanvas *c, int w, int h, int newone) { return 0; } // return 1 if your content has changed, or 0 to cancel update of your buffer to the window
    virtual int wantEvenAlignment() { return 0; } // if you need even coordinates for your framebuffer, return 1 here
    
    /**
      QuickPaintWnd method getQuickPaintSize gets the client area width and 
      height.

      @param w A pointer to the width to fill.
      @param h A pointer to the height to fill.
    */
    virtual void getQuickPaintSize(int *w, int *h); // by default returns client width/height

    /**
      QuickPaintWnd method getQuickPaintSource .

      @see getQuickPaintSize()
      @assert r exists.
      @ret None
      @except 
      @param r 
    */
    virtual void getQuickPaintSource(RECT *r); // by default returns the size of the quickpaint canvas

    /**
      QuickPaintWnd method getQuickPaintDest .

      @see getQuickPaintSource()
      @assert r exists.
      @param r
    */
    virtual void getQuickPaintDest(RECT *r); // by default returns the size of client area
    virtual int wantNegativeHeight() { return 0; }
    virtual int wantFilters() { return 0; }
    virtual const char *getFiltersGroup() { return "Vis/Eq"; }
 
  private:
    /**
      QuickPaintWnd method quickPaint .
    */
    void quickPaint();

    int realtime;
    BltCanvas *render_canvas;
    int canvas_w, canvas_h;
    int speed;
    int timerset;
    int enabled;

    int invalidated;
    PtrList<svc_skinFilter>filters;
    SkinFilterEnum *sfe;
};


#endif
