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

#ifndef __ABSTRACTWNDHOLD_H
#define __ABSTRACTWNDHOLD_H

#include "svcwndhold.h"
#include "../../common/wndcb.h"

class GuiObject;
class ScriptObject;

#define ABSTRACTWNDHOLDER_PARENT ServiceWndHolder


/**
  The AbstractWndHolder enables you to display deferred content. This means
  the content does not actually need to exist at creation. It will be automatically
  loaded when it is found.
  
  This class is meant to be derived from. You shouldn't create an AbstractWndHolder 
  directly to implement this kind of functionality. Please use GuiObjectWnd along 
  with the setContent() method. 

  @short An abstracted window holder with deferred content loading.
  @author Nullsoft
  @ver 1.0
  @see GuiObjectWnd
*/
class COMEXP AbstractWndHolder : public ABSTRACTWNDHOLDER_PARENT, public WndCallbackI {
  
  protected:

    // don't create me directly, create a GuiObjectWnd and setContent("group.id") on it
    // these are for inheriting and have a deferred content (so if you _were_ using that,
    // your findObjects would fail right after init and before abstract_onNewContent)
    
    /**
      Sets the group id of the content and the auto resize flag.
      
      Setting the auto resize flag to 1 will enable the automatic resize of
      the window according to the size of the content attempting to be
      displayed.
    
      @param groupid The content group.
      @param _autoresizefromcontent 0, disable auto resize; 1, enable auto resize;
    */
    AbstractWndHolder(const char *groupid=NULL, int _autoresizefromcontent=0);
    
    /**
      Finds the group with the matching GUID and uses that group as the content.
      This insures you get the exact group and not a replacement.
      
      Setting the auto resize flag to 1 will enable the automatic resize of
      the window according to the size of the content attempting to be
      displayed.
    
      @param _guid The GUID the content group.
      @param _autoresizefromcontent 0, disable auto resize; 1, enable auto resize;
    */
    AbstractWndHolder(GUID _guid, int _autoresizefromcontent=0);

  public:

    
    /**
      Destroys the instance of the group used for content automatically.
    */
    virtual ~AbstractWndHolder();

    
    /**
      Sets the content group using the group id.
    
      @param groupid The content group id.
      @param _autoresizefromcontent -1, no change; 0, disable auto resize; 1, enable auto resize;
    */
    virtual void abstract_setContent(const char *groupid, int _autoresizefromcontent=-1); // -1 = no change, 0 = false, 1 = true
    
    /**
      Sets the content group using the group's GUID.
      This insures you get the exact group and not a replacement.
    
      @param g The content group's GUID.
      @param _autoresizefromcontent -1, no change; 0, disable auto resize; 1, enable auto resize;
    */
    virtual void abstract_setContent(GUID g, int _autoresizefromcontent=-1); // -1 = no change, 0 = false, 1 = true
    
    /**
      This event is triggered when the content is loaded or the content changes.
      Override it to implement your own handling of this event.
    */
    virtual void abstract_onNewContent();
    
    /**
      This event is triggered when the content group is changed.
      Override it to implement your own handling of this event.
    
      @see abstract_setContant()
      @ret 1, if you handle the event;
      @param grpid The new group that was set.
    */
    virtual int onGroupChange(const char *grpid);
    
    /**
      This event is triggered when the visibility of the window changes.
      Override it to implement your own handling of this event.
    
      @see onGroupChange()
      @param show 0, hide window; 1, show window;
    */
    virtual void onSetVisible(int show);

    
    /**
      This event is triggered when the window is being initialized.
      Override it to implement your own handling of this event.
    
      @ret 1, if you handle the event;
    */
    virtual int onInit();
    
    /**
      This event is triggered when the window is resized.
      Override it to implement your own handling of this event.
    
      @ret 1, if you handle the event;
    */
    virtual int onResize();

    
    /**
      Get the RootWnd.
    
      @ret 
    */
    RootWnd *rootwndholder_getRootWnd();

    /**
      Find an object in the content group. This is done via
      the object's text id.
    
      @see abstract_findScriptObject()
      @ret !NULL, The requested object pointer; NULL, Failed to find object.
      @param The id of the object to find.
    */
    virtual GuiObject *abstract_findObject(const char *object_id);

    /**
      Find a script object in the content group. This is done via the
      script object's text id.
    
      @see abstract_findObject()
      @ret !NULL, The requested script object pointer; NULL, Failed to find the object.
      @param The id of the script object to find.
    */
    virtual ScriptObject *abstract_findScriptObject(const char *object_id);

    /**
      Get the content group.
    
      @see abstract_getContentScriptObject()
      @ret A pointer to the content group GuiObject.
    */
    virtual GuiObject *abstract_getContent();

    /**
      Get the content script object.
    
      @see abstract_getContent()
      @ret A pointer to the content ScriptObject.
    */
    virtual ScriptObject *abstract_getContentScriptObject();

    /**
      Get the RootWnd that is holding the content group.
    
      @see rootwndholder_getRootWnd()
      @ret A pointer to the RootWnd holding the content group.
    */
    virtual RootWnd *abstract_getContentRootWnd() { return group; }
    
    /**
      Read the auto-resize from content flag.
    
      @see abstract_setAutoResizeFromContent()
      @ret 0, disable auto resize; 1, enable auto resize;
      @param 
    */
    virtual int abstract_wantAutoResizeFromContent() { return autoresizefromcontent; }
    
    /**
      Set the auto-resize from content flag.
    
      @see abstract_wantAutoResizeFromContent()
      @param i 0, disable auto resize; 1, enable auto resize;
    */
    virtual void abstract_setAutoResizeFromContent(int i) {  autoresizefromcontent = i; }
    
    /**
      Set the allow deferred content flag. Allowing deferred content enables content to be
      loaded after the window is shown.
    
      @param allow 0, Do not allow; 1, Allow
    */
    virtual void abstract_setAllowDeferredContent(int allow) { allow_deferred_content = allow; }

    
    /**
      The deferred callback.
    
      @ret 1, If you handle the event.
      @param p1 
      @param p2
    */
    virtual int onDeferredCallback(int p1, int p2);

  private:

    
    /**
      Creates the child content window when required.
    */
    void createChild();
    
    /**
      Set Both Content.
    
      @param guid 
      @param g
      @param _autoresizefromcontent
    */
    void setBothContent(const char *guid, GUID g, int _autoresizefromcontent);
    
    /**
      Loads the content from the previously specified group.
      
      @see abstract_setContent()
    */
    void doLoadContent();
    
    /**
      Destroys the instantiated copy of the content group.
    */
    void destroyContent();

    String groupid;
    GUID guid;

    RootWnd *group;
    int cbreg;
    int inselfresize;
    int autoresizefromcontent;
    int allow_deferred_content;
    int need_deferred_load;
};

#endif
