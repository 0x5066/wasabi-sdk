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

#ifndef __PATHPICKER_H
#define __PATHPICKER_H

#include "../../common/guiobjwnd.h"
#include "../../common/script/c_script/h_guiobject.h"
#include "../../common/script/c_script/h_button.h"

#define  PATHPICKER_PARENT GuiObjectWnd

class COMEXP PPClicksCallback;
extern int __id;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP PathPicker : public PATHPICKER_PARENT {
  
  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    PathPicker();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~PathPicker();

    
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
    void clickCallback();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int onReloadConfig();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void abstract_onNewContent();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setPath(const char *newpath);
    const char *getPath() { return curpath; }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void onPathChanged(const char *newpath);
   
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setDefault();

  private:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void updatePathInControl();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void updatePathFromConfig();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void trapControls();

    PPClicksCallback *clicks_button;
    String curpath;
    int disable_cfg_event;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class PPClicksCallback : public H_GuiObject {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    PPClicksCallback(ScriptObject *trap, PathPicker *_callback) :
        
        /**
          Method
        
          @see 
          @ret 
          @param 
        */
        callback(_callback), H_GuiObject(trap) {
    }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onLeftButtonDown(int x, int y) {
      callback->clickCallback();
    }
  private:
    PathPicker *callback;
};

#endif
