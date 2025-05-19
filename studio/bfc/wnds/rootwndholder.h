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

#ifndef __ROOTWNDHOLD_H
#define __ROOTWNDHOLD_H

#include "../virtualwnd.h"

/**
 A simple wnd that holds another window. Initializes it if needed, but DOES not delete it (and for a good reason, this is a RootWnd),
 so your inheritor has to call whoever is needed to destroy the wnd
*/

#define ROOTWNDHOLDER_PARENT VirtualWnd 


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP RootWndHolder : public ROOTWNDHOLDER_PARENT {
  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    RootWndHolder();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~RootWndHolder();

    // override this
    virtual RootWnd *rootwndholder_getRootWnd(); 
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void rootwndholder_getRect(RECT *r);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void rootwndholder_setRootWnd(RootWnd *w);

    
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
    virtual int onResize();
//    virtual void onSetVisible(int v);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int wantRenderBaseTexture() { return 0; }
    
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
    virtual int onActivate();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int onDeactivate();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int getPreferences(int what);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int childNotify(RootWnd *child, int msg, int param1=0, int param2=0);

  private:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void checkInit(RootWnd *w);
    
    RootWnd *privptr;
};

#endif