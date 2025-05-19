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

#include "objectactuator.h"
#include "../bfc/console.h"
#include "../bfc/paramparser.h"
#include "../common/script/scriptguid.h"
#include "../studio/api.h"

// -----------------------------------------------------------------------
ObjectActuator::ObjectActuator() {
  myxuihandle = newXuiHandle();
  if (actuator_wantTargetParam())
    addParam(myxuihandle, "target",  OBJECTACTUATOR_TARGET,    XUI_ATTRIBUTE_IMPLIED);
  if (actuator_wantGroupParam())
    addParam(myxuihandle, "group",   OBJECTACTUATOR_GROUP,     XUI_ATTRIBUTE_IMPLIED);
}

// -----------------------------------------------------------------------
ObjectActuator::~ObjectActuator() {
}

// -----------------------------------------------------------------------
int ObjectActuator::setXuiParam(int xuihandle, int xmlattributeid, const char *xmlattributename, const char *value) {
  if (xuihandle != myxuihandle)
    return OBJECTACTUATOR_PARENT::setXuiParam(xuihandle, xmlattributeid, xmlattributename, value);

  switch (xmlattributeid) {
    case OBJECTACTUATOR_TARGET:
      actuator_setTarget(value);
      break;
    case OBJECTACTUATOR_GROUP:
      actuator_setGroup(value);
      break;
    default:
      return 0;
  }
  return 1; 
}

// -----------------------------------------------------------------------
void ObjectActuator::actuator_setTarget(const char *value) {
  objectsid = value;
  if (isInited() && actuator_wantAutoPerform())
    performActions();
}

// -----------------------------------------------------------------------
void ObjectActuator::actuator_setGroup(const char *value) {
  groupid = value;
}

// -----------------------------------------------------------------------
int ObjectActuator::onInit() {
  int rt = OBJECTACTUATOR_PARENT::onInit();
  
  if (actuator_wantAutoPerform()) performActions();

  return rt;
}


// -----------------------------------------------------------------------
void ObjectActuator::performActions() {
  RootWnd *group = getParent();
  
  if (!groupid.isempty()) {
    GuiObject *o = getGuiObject()->guiobject_findObject(groupid);
    if (o != NULL) {
      group = o->guiobject_getRootWnd();
    }
  }

  GuiObject *go = static_cast<GuiObject *>(group->getInterface(guiObjectGuid));
  if (go == NULL) {
    api->console_outputString(SEVERITY_LINKNOTFOUND, StringPrintf("%s:group:%s", getActuatorTag(), groupid));
    return;
  }

  ParamParser pp(objectsid);
  for (int i=0;i<pp.getNumItems();i++) {
    GuiObject *target = go->guiobject_findObject(pp.enumItem(i));
    if (target != NULL)
      actuator_onPerform(target);
    else
      api->console_outputString(SEVERITY_LINKNOTFOUND, StringPrintf("%s:%s/%s", getActuatorTag(), groupid, objectsid));
  }
}

const char *ObjectActuator::getActuatorTag() {
  return "ObjectActuator";
}
