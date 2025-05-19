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
#include "../objcontroller.h"
#include "h_rootobj.h"

H_RootObject::H_RootObject(ScriptObject *o) {
  inited=0;
  H_hook(o);
}

H_RootObject::H_RootObject() {
  inited=0;
}

void H_RootObject::H_hook(ScriptObject *o) {
  ASSERT(!inited);
  me = o;
  addMonitorObject(o, &rootObjectGuid);
  if (count++ == 0) {
    onnotify_id = api->maki_addDlfRef(o->vcpu_getController(), "onNotify", this);
  }
  inited=1;
}

H_RootObject::~H_RootObject() {
  if (!inited) return;
  api->maki_remDlfRef(this);
  //count--;
}

int H_RootObject::eventCallback(ScriptObject *object, int dlfid, scriptVar **params, int nparams) {
  if (object != getHookedObject()) return 0;
  if (dlfid == onnotify_id) { hook_onNotify(GET_SCRIPT_STRING(*params[0]), GET_SCRIPT_STRING(*params[1]), GET_SCRIPT_INT(*params[2]), GET_SCRIPT_INT(*params[3])); return 1; }
  return 0;
}

int H_RootObject::onnotify_id=0;
int H_RootObject::inited=0;
int H_RootObject::count=0;
