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

#include "embeddedxui.h"

EmbeddedXuiObject::EmbeddedXuiObject() {
  embedded = NULL;
  myxuihandle = newXuiHandle();
  getScriptObject()->vcpu_setInterface(embeddedXuiGuid, (void *)static_cast<EmbeddedXuiObject *>(this));
  getScriptObject()->vcpu_setClassName("ObjectEmbedded"); // this is the script class name
  getScriptObject()->vcpu_setController(embeddedXuiController);
}

EmbeddedXuiObject::~EmbeddedXuiObject() {
  paramlist.deleteAll();
}

void EmbeddedXuiObject::onNewContent() {
  embeddedxui_onNewEmbeddedContent();
}

void EmbeddedXuiObject::embeddedxui_onNewEmbeddedContent() {
  embedded = NULL;
  const char *id = embeddedxui_getEmbeddedObjectId();
  if (id != NULL && *id) {
    embedded = getGuiObject()->guiobject_findObject(id);  
    if (embedded != NULL) {
      foreach(paramlist)
        EmbeddedXuiObjectParam *p = paramlist.getfor();
        embedded->guiobject_setXmlParam(p->param, p->value);
      endfor;
      syncCfgAttrib();
    }
  }
}

int EmbeddedXuiObject::onUnknownXuiParam(const char *xmlattributename, const char *value) {
  int r = EMBEDDEDXUIOBJECT_PARENT::onUnknownXuiParam(xmlattributename, value);
  paramlist.addItem(new EmbeddedXuiObjectParam(xmlattributename, value));
  if (embedded)
    r = embedded->guiobject_setXmlParam(xmlattributename, value);
  return r;
}

int EmbeddedXuiObject::onInit() {
  int r = EMBEDDEDXUIOBJECT_PARENT::onInit();
  const char *id = embeddedxui_getContentId();
  if (id != NULL && *id)
    setContent(id);
  return r;
}

int EmbeddedXuiObject::onReloadConfig() {
  int r = EMBEDDEDXUIOBJECT_PARENT::onReloadConfig();
  syncCfgAttrib();
  return r;
}

void EmbeddedXuiObject::syncCfgAttrib() {
  if (embedded == NULL) return;
  CfgItem *item = getGuiObject()->guiobject_getCfgItem();
  const char *attrib = getGuiObject()->guiobject_getCfgAttrib();
  if (item != embedded->guiobject_getCfgItem() ||
      attrib != embedded->guiobject_getCfgAttrib()) {
    embedded->guiobject_setCfgAttrib(item, attrib);
  }
}

// -----------------------------------------------------------------------
// Script Object

EmbeddedXuiScriptController _embeddedXuiController;
EmbeddedXuiScriptController *embeddedXuiController = &_embeddedXuiController;

// -- Functions table -------------------------------------
function_descriptor_struct EmbeddedXuiScriptController::exportedFunction[] = {
  {"getEmbeddedObject",       0, EmbeddedXuiScriptController::EmbeddedXui_getEmbeddedObject},
};
                                      
ScriptObject *EmbeddedXuiScriptController::instantiate() {
  EmbeddedXuiObject *ex = new EmbeddedXuiObject;
  ASSERT(ex != NULL);
  return ex->getScriptObject();
}

void EmbeddedXuiScriptController::destroy(ScriptObject *o) {
  EmbeddedXuiObject *ex= static_cast<EmbeddedXuiObject *>(o->vcpu_getInterface(embeddedXuiGuid));
  ASSERT(ex != NULL);
  delete ex;
}

void *EmbeddedXuiScriptController::encapsulate(ScriptObject *o) {
  return NULL; // no encapsulation for DropDownlist yet
}

void EmbeddedXuiScriptController::deencapsulate(void *o) {
}

int EmbeddedXuiScriptController::getNumFunctions() { 
  return sizeof(exportedFunction) / sizeof(function_descriptor_struct); 
}

const function_descriptor_struct *EmbeddedXuiScriptController::getExportedFunctions() { 
  return exportedFunction; 
}


scriptVar EmbeddedXuiScriptController::EmbeddedXui_getEmbeddedObject(SCRIPT_FUNCTION_PARAMS, ScriptObject *o) {
  SCRIPT_FUNCTION_INIT
  EmbeddedXuiObject *ex = static_cast<EmbeddedXuiObject*>(o->vcpu_getInterface(embeddedXuiGuid));
  ScriptObject *_o = NULL;
  if (ex) {
    GuiObject *go = ex->embeddedxui_getEmbeddedObject();
    if (go != NULL)
      _o = go->guiobject_getScriptObject();
  }
  return MAKE_SCRIPT_OBJECT(_o);
}

ScriptObject *EmbeddedXuiScriptController::cast(ScriptObject *o, GUID g) {
  EmbeddedXuiObject *exo = static_cast<EmbeddedXuiObject *>(o->vcpu_getInterface(embeddedXuiGuid));
  if (!exo) return NULL;
  GuiObject *go = exo->embeddedxui_getEmbeddedObject();
  if (go != NULL) {
    ScriptObject *eo = go->guiobject_getScriptObject();
    if (eo != NULL) {
      void *i = eo->vcpu_getInterface(g);
      if (i != NULL)
        return eo;
    }
  }
  return NULL;
}


