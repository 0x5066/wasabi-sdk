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

#ifndef __TITLEBOX_H
#define __TITLEBOX_H

#include "../../common/guiobjwnd.h"

#define TITLEBOX_PARENT GuiObjectWnd 


/**
  Titlebox  

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP TitleBox : public TITLEBOX_PARENT {

  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    TitleBox();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~TitleBox();

    
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
    virtual int getCentered() { return centered; }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setCentered(int _centered);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setTitle(const char *t);
    virtual const char *getTitle() { return title; }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setSuffix(const char *suffix);
    const char *getSuffix() { return suffix; }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void onNewContent();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setChildGroup(const char *grp);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int getPreferences(int what);

  private:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setSubContent(int insertcontent=1);

    GuiObjectWnd *titleleft;
    GuiObjectWnd *titleright;
    GuiObjectWnd *titlecenter;
    GuiObjectWnd *content;
    int centered;
    String title;
    String content_id;
    String suffix;
};


#endif
