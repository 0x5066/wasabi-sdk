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

#ifndef _STATUS_H
#define _STATUS_H

#include "../../common/guiobjwnd.h"
#include "../../bfc/string.h"
#include "../guistatuscb.h"
#include "../../bfc/depend.h"

class AbstractWndHolder;
class ButtBar;
class AppCmds;

#define STATUSBAR_PARENT GuiObjectWnd

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP StatusBar : public STATUSBAR_PARENT, public GuiStatusCallbackI {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  StatusBar();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~StatusBar();

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onInit();

  // completeness indicator
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void pushCompleted(int max);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void incCompleted(int add);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void setCompleted(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void popCompleted();

  
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
  virtual int onResize();

  virtual Dependent *status_getDependencyPtr() { return this; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onSetStatusText(const char *text, int overlay);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onAddAppCmds(AppCmds *commands);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onRemoveAppCmds(AppCmds *commands);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void fakeButtonPush(const char *name);

protected:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int fakeButtonPush(ButtBar *bb, const char *name);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setExclude(const char *val);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setIncludeOnly(const char *val);
  String exclude_list, include_only;

protected:
  void regenerate();

private:
  String contentgroupname;

  String status_text;
  int overtimer;

  // completeness
  int active;
  int max;
  int completed;
  int progress_width;

  GuiObjectWnd *bg;

  ButtBar *bbleft, *bbright;

  PtrList<AppCmds> appcmds;
};

#endif
