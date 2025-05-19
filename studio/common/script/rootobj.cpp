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

#include "scriptobj.h"
#include "rootobj.h"
#include "../../studio/api.h"
#include "scriptguid.h"

RootObjectInstance::RootObjectInstance() {
  my_script_object = new ScriptObjectI;
  my_root_object = static_cast<RootObject *>(api->maki_encapsulate(rootObjectGuid, my_script_object)); // creates an encapsulated RootObject
  my_script_object->vcpu_setInterface(rootObjectInstanceGuid, static_cast<void *>(this));
  my_script_object->vcpu_setClassName("Object");
  my_script_object->vcpu_setController(api->maki_getController(rootObjectGuid));
}

RootObjectInstance::~RootObjectInstance() {
  api->maki_deencapsulate(rootObjectGuid, my_root_object);
  delete my_script_object;
}

void RootObjectInstance::notify(const char *s, const char *t, int u, int v) {
  my_root_object->rootobject_notify(s, t, u, v);
}

const char *RootObjectInstance::getClassName() {
  return my_root_object->rootobject_getClassName();
}

RootObject *RootObjectInstance::getRootObject() {
  return my_root_object;
}

ScriptObject *RootObjectInstance::getScriptObject() {
  return my_script_object;
}

