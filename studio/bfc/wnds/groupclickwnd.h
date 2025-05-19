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

/* This class simulates a very basic button thru the use of a group. the group needs to have a guiobject (ie: a transparent layer) 
 with id "mousetrap" on top of the rest of its content. */

#ifndef __GROUPCLICKWND_H
#define __GROUPCLICKWND_H

#include "../../common/guiobjwnd.h"
#include "../../common/script/c_script/h_guiobject.h"

class MouseTrap;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP GroupClickWnd : public GuiObjectWnd {

  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    GroupClickWnd();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~GroupClickWnd();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void content_onLeftButtonDown();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void content_onLeftButtonUp();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void content_onRightButtonDown();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void content_onRightButtonUp();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void content_onEnterArea();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void content_onLeaveArea();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void groupclick_onLeftPush();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void groupclick_onRightPush();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void abstract_onNewContent();

  private:
    
    int inarea;  
    MouseTrap *trap;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP MouseTrap : public H_GuiObject {
  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    MouseTrap(GroupClickWnd *w, ScriptObject *obj) : H_GuiObject(obj), window(w) { }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~MouseTrap() {}

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onLeftButtonDown(int x, int y);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onLeftButtonUp(int x, int y);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onRightButtonDown(int x, int y);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onRightButtonUp(int x, int y);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onEnterArea();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onLeaveArea();
  
  private:

    GroupClickWnd *window;
};


#endif
