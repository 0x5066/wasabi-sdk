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

#ifndef __TABSHEETBAR_H
#define __TABSHEETBAR_H

#include "../../common/guiobjwnd.h"

class GroupTabButton;

#define TABSHEETBAR_PARENT GuiObjectWnd


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class TabSheetBar : public TABSHEETBAR_PARENT {

  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    TabSheetBar();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~TabSheetBar();

    
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
    
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void addChild(GroupTabButton *child);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int getHeight();

    
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
    void setMargin(int m) { margin = m; if (isInited()) onResize(); }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setSpacing(int s) { spacing = s; if (isInited()) onResize(); }

  private:

    int maxheightsofar;
    PtrList<GroupTabButton> btns;
    int margin, spacing;
    GuiObjectWnd *bottombar;
};

#endif
