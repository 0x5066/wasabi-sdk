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

#ifndef __DROPDOWNLIST_H
#define __DROPDOWNLIST_H

#include "../popexitcb.h"
#include "embeddedxui.h"
#include "../../common/script/c_script/h_guiobject.h"
#include "../../common/script/c_script/h_button.h"
#include "../../studio/services/svc_textfeed.h"
#include "../depend.h"
#include "../textfeedcb.h"
#include "../../common/script/objcontroller.h"

#define  DROPDOWNLIST_PARENT EmbeddedXuiObject

class COMEXP DDLClicksCallback;
class COMEXP DDLKeyCallback;
extern int __id;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class DDLEntry {
  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    DDLEntry(const char *txt) : text(txt), id(__id++) { } 
    const char *getText() { return text; }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    int getId() { return id; }

  private:
    String text;
    int id;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP SortDDLEntries{
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  static int compareItem(DDLEntry *p1, DDLEntry *p2) {
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    return STRICMP(p1->getText(), p2->getText());
  }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  static int compareAttrib(const char *attrib, DDLEntry *item) {
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    return STRICMP(attrib, item->getText());
  }
};




/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP DropDownList : public DROPDOWNLIST_PARENT, public PopupExitCallbackI, public DependentViewerI, public TextFeedCallbackI {
  
  public:

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    DropDownList();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~DropDownList();

    
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
    void escapeCallback();

    
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
    virtual void setListHeight(int h) { height = h; }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int popupexitcb_onExitPopup();
    virtual Dependent *popupexit_getDependencyPtr() { return rootwnd_getDependencyPtr(); }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void textfeed_onReceiveText(const char *text);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void openList();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void closeList();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setItems(const char *lotsofitems);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    int addItem(const char *text); 
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void delItem(int id);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    int findItem(const char *text);

    int getNumItems() { return items.getNumItems(); }
    DDLEntry *enumItem(int i) { return items.enumItem(i); }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void selectItem(int id, int hover=0);
    const char *getItemText(int id);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    int getSelected() { return selected; }
    const char *getSelectedText() { int a = getSelected(); if (a == -1) return getCustomText(); return getItemText(a); }
    virtual const char *getCustomText() { return noitemtext; }
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void deleteAllItems();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void onSelect(int id, int hover=0);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setNoItemText(const char *txt);

    
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
    virtual int onDeferredCallback(int p1, int p2);

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int viewer_onItemDeleted(Dependent *item);

    
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
    virtual void selectDefault();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void setFeed(const char *feedid);

    virtual int wantTrapButton() { return 1; }
    virtual int wantTrapText() { return 1; }

    virtual const char *dropdownlist_getMainGroupId() { return "wasabi.dropdownlist.main.group"; }
    virtual const char *dropdownlist_getListGroupId() { return "wasabi.dropdownlist.list.group"; }
    virtual const char *dropdownlist_getTextId() { return "dropdownlist.text"; }
    virtual const char *dropdownlist_getButtonId() { return "dropdownlist.button"; }
    virtual const char *dropdownlist_getListId() { return "dropdownlist.list"; }

    virtual void updateTextInControl(const char *txt);

    virtual int setXuiParam(int xuihandle, int xmlattributeid, const char *xmlattributename, const char *value);

    virtual const char *embeddedxui_getContentId() { return dropdownlist_getMainGroupId(); }
    virtual const char *embeddedxui_getEmbeddedObjectId() { return dropdownlist_getTextId(); }

    int isListOpen() { return list_group != NULL; };
    virtual int wantAutoSort() { return 1; }

    virtual void dropdownlist_onCloseList();
    virtual void dropdownlist_onOpenList();

    virtual void dropdownlist_onConfigureList(GuiObject *o);
    virtual int onKeyDown(int keyCode);
    virtual int onAcceleratorEvent(const char *name);

    virtual void onPreCloseList() {}
    virtual void onPreOpenList() {}

  private:

    enum {
      DROPDOWNLIST_SETITEMS = 0,
      DROPDOWNLIST_SETFEED,
      DROPDOWNLIST_SELECT,
    };
    int myxuihandle;

  private:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void updateTextFromConfig();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void trapControls();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void setListParams();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void doCloseList(int cb=1);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void openFeed(const char *feedid);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void closeFeed();

    DDLClicksCallback *clicks_button;
    DDLClicksCallback *clicks_text;
    DDLKeyCallback *list_key;
    RootWnd *list_group;
    PtrListInsertSorted<DDLEntry, SortDDLEntries> items;
    int selected;

    int height;
    String noitemtext;
    int trap_click;
    Dependent *group_dep;
    RootWnd *action_list;
    int disable_cfg_event;
    String last_feed;
    svc_textFeed *feed;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class DDLClicksCallback : public H_GuiObject {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    DDLClicksCallback(ScriptObject *trap, DropDownList *_callback) :
        
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
    DropDownList *callback;
};

class DDLKeyCallback : public H_GuiObject {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    DDLKeyCallback(ScriptObject *trap, DropDownList *_callback) :
        
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

    virtual void hook_onChar(const char *c) {
      if (*c == VK_ESCAPE)
        callback->escapeCallback();
    }
  private:
    DropDownList *callback;
};

// -----------------------------------------------------------------------
class COMEXP DropDownListScriptController: public ScriptObjectControllerI {
public:
  virtual const char *getClassName() { return "DropDownList"; }
  virtual const char *getAncestorClassName() { return "ObjectEmbedder"; }
  virtual ScriptObjectController *getAncestorController() { return api->maki_getController(embeddedXuiGuid); }
  virtual int getNumFunctions();
  virtual const function_descriptor_struct *getExportedFunctions();
  virtual GUID getClassGuid() { return dropDownListGuid; }
  virtual ScriptObject *instantiate();
  virtual void destroy(ScriptObject *o);
  virtual void *encapsulate(ScriptObject *o);
  virtual void deencapsulate(void *o);

private:

  static function_descriptor_struct exportedFunction[];
  static scriptVar DropDownList_getItemSelected(SCRIPT_FUNCTION_PARAMS, ScriptObject *o);
};

extern COMEXP DropDownListScriptController *dropDownListController;

#endif
