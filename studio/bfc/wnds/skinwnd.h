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

#ifndef __SKINWND_H
#define __SKINWND_H

#include "../../common/common.h"
#include "../../studio/api.h"

#define WASABISTDCONTAINER_RESIZABLE_STATUS      "resizable_status"
#define WASABISTDCONTAINER_RESIZABLE_NOSTATUS    "resizable_nostatus"
#define WASABISTDCONTAINER_STATIC                "static"
#define WASABISTDCONTAINER_MODAL                 "modal"

#define SKINWND_ATTACH_LEFT     1
#define SKINWND_ATTACH_TOP      2
#define SKINWND_ATTACH_RIGHT    3
#define SKINWND_ATTACH_BOTTOM   4

class RootWnd;
class GuiObject;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP SkinWnd {

  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    SkinWnd(GUID svc_or_group_guid, const char *prefered_container=NULL, int container_flag=0, RECT *animated_rect_source=NULL, int transcient=0, int starthidden=0);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    SkinWnd(const char *group_id, const char *prefered_container=NULL, int container_flag=0, RECT *animated_rect_source=NULL, int transcient=0, int starthidden=0);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~SkinWnd();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void destroy(RECT *animated_rect_dest=NULL);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    RootWnd *getWindow() { return wnd; }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    ScriptObject *getContainer();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    ScriptObject *getLayout();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    int runModal(int center=0);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void endModal(int retcode);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    GuiObject *findObject(const char *object_id);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void notifyMinMaxChanged();

  private:

    RootWnd *wnd;


};

#endif
