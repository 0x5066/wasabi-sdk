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

#ifndef __EMBEDDEDXUIOBJECT_H
#define __EMBEDDEDXUIOBJECT_H

#include "../../common/guiobjwnd.h"
#include "../../common/script/scriptguid.h"
#include "../../common/script/scriptobj.h"
#include "../../common/script/objcontroller.h"

#define EMBEDDEDXUIOBJECT_PARENT GuiObjectWnd


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP EmbeddedXuiObjectParam {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    EmbeddedXuiObjectParam(const char *p, const char *v) : param(p), value(v) {}
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~EmbeddedXuiObjectParam() {}

    String param;
    String value;
};


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP EmbeddedXuiObject : public EMBEDDEDXUIOBJECT_PARENT {
  public:
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    EmbeddedXuiObject();
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual ~EmbeddedXuiObject();

    
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
    virtual int onInit();

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual void embeddedxui_onNewEmbeddedContent();
    virtual const char *embeddedxui_getContentId() { return NULL; }
    virtual const char *embeddedxui_getEmbeddedObjectId() { return NULL; }

    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int onUnknownXuiParam(const char *xmlattributename, const char *value);
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    virtual int onReloadConfig();
    virtual GuiObject *embeddedxui_getEmbeddedObject() { return embedded; }

  private:
    
    
    /**
      Method
    
      @see 
      @ret 
      @param 
    */
    void syncCfgAttrib();

    int myxuihandle;
    PtrList<EmbeddedXuiObjectParam> paramlist;
    GuiObject *embedded;
};

// -----------------------------------------------------------------------
class COMEXP EmbeddedXuiScriptController: public ScriptObjectControllerI {
public:
  virtual const char *getClassName() { return "ObjectEmbedder"; }
  virtual const char *getAncestorClassName() { return "GuiObject"; }
  virtual ScriptObjectController *getAncestorController() { return api->maki_getController(guiObjectGuid); }
  virtual int getNumFunctions();
  virtual const function_descriptor_struct *getExportedFunctions();
  virtual GUID getClassGuid() { return embeddedXuiGuid; }
  virtual ScriptObject *instantiate();
  virtual void destroy(ScriptObject *o);
  virtual void *encapsulate(ScriptObject *o);
  virtual void deencapsulate(void *o);
  virtual ScriptObject *cast(ScriptObject *o, GUID g);

private:

  static function_descriptor_struct exportedFunction[];
  static scriptVar EmbeddedXui_getEmbeddedObject(SCRIPT_FUNCTION_PARAMS, ScriptObject *o);
};

extern COMEXP EmbeddedXuiScriptController *embeddedXuiController;


#endif