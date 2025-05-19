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

#include "../../../studio/api.h"
#include "../scriptobj.h"
#include "../objcontroller.h"
#include "scripthook.h"

#define CBCLASS ScriptHookI
START_DISPATCH;
  CB(EVENTCALLBACK,    eventCallback);
END_DISPATCH;

ScriptHookI::ScriptHookI() {
}

ScriptHookI::~ScriptHookI() {
  api->maki_remDlfRef(this);
  foreach(controllers)
    controllers.getfor()->removeHooks(this);
  endfor
  
}


void ScriptHookI::addMonitorObject(ScriptObject *o, const GUID *hookedclass) {
  api->maki_addDlfClassRef(o->vcpu_getController(), this);
  ScriptObjectController *cont = o->vcpu_getController();
  if (hookedclass == NULL) {
    cont->addObjectHook(this, o);
  } else {
    while (cont) {
      if (cont->getClassGuid() == *hookedclass) {
        cont->addObjectHook(this, o);
        break;
      }
      cont = cont->getAncestorController();
    }
  }
  if (cont == NULL) // guid not found
    return;
  controllers.addItem(cont);
}

void ScriptHookI::addMonitorClass(ScriptObject *o) {
  api->maki_addDlfClassRef(o->vcpu_getController(), this);
  o->vcpu_getController()->addClassHook(this);
  controllers.addItem(o->vcpu_getController());
}
