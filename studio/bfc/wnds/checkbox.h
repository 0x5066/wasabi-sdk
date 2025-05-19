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

#ifndef __CHECKBOX_H
#define __CHECKBOX_H

#include "../../common/guiobjwnd.h"
#include "../../common/script/c_script/h_guiobject.h"
#include "../../common/script/c_script/h_button.h"

#define  CHECKBOX_PARENT GuiObjectWnd

class TextClicks;
class ToggleClicks;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP CheckBox : public CHECKBOX_PARENT {
  
  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    CheckBox(const char *_text = "ERROR", const char *_radioid = NULL);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~CheckBox();

    
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
    virtual int getPreferences(int what);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void toggle(int self_switch);  // this is called by the click catchers.
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setActivated(int activated, int writetocfg=1);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    int isActivated();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setText(const char *_text);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setRadioid(const char *_radioid);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setRadioVal(const char *val, int use_radioval=TRUE);

    
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
    virtual void onNewContent();

  private:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void updateText();

    TextClicks *textclicks;
    ToggleClicks *toggleclicks;
    String text;
    String radioid;
    GuiObject *buttonGuiObj;
    String radioval;
    int use_radioval;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class TextClicks : public H_GuiObject {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    TextClicks(ScriptObject *textobj, CheckBox *_callback) :
        
        /**
          Method
        
          @see 
          @ret 
          @param 
        */
        callback(_callback), H_GuiObject(textobj) {
    }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onLeftButtonDown(int x, int y) {
      callback->toggle(0);
    }
  private:
    CheckBox *callback;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class ToggleClicks : public H_Button {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    ToggleClicks(ScriptObject *button, CheckBox *_callback) :
        
        /**
          Method
        
          @see 
          @ret 
          @param 
        */
        callback(_callback), H_Button(button) {
        inhere=0;
    }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void hook_onActivate(int activate) {
      if (inhere) return;
      inhere=1;
      callback->toggle(1);
      inhere=0;
    }
  private:
    CheckBox *callback;
    int inhere;
};


#endif
