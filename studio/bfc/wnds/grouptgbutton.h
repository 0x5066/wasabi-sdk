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

#ifndef __GROUPTGBUTTON_H
#define __GROUPTGBUTTON_H

#include "groupclickwnd.h"
#include "../../common/guiobjwnd.h"

#define GROUPTOGGLEBUTTON_PARENT GuiObjectWnd

#define STATUS_OFF 0
#define STATUS_ON  1


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP GroupToggleButton : public GROUPTOGGLEBUTTON_PARENT {

  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    GroupToggleButton();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~GroupToggleButton();

    
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
    virtual int childNotify(RootWnd *child, int msg, int param1=0, int param2=0);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setGroups(const char *on, const char *off);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void toggle();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int wantFullClick();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void grouptoggle_onLeftPush();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void grouptoggle_onRightPush();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setStatus(int s);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int getStatus() { return status; }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int wantAutoToggle() { return 1; }

    virtual GroupClickWnd *enumGroups(int n);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int getNumGroups();

  private:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void initGroups();
    
    GroupClickWnd *on;
    GroupClickWnd *off;

    String on_id, off_id;

    int status;
};

#endif
