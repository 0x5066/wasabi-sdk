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
#include "c_rootobj.h"

C_RootObject::C_RootObject(ScriptObject *o) {
  inited=0;
  C_hook(o);
}

C_RootObject::C_RootObject() {
  inited=0;
}

void C_RootObject::C_hook(ScriptObject *o) {
  ASSERT(!inited);
  object = o;
  if (count++ == 0) {
    getclassname_id = api->maki_addDlfRef(o->vcpu_getController(), "getClassName", this);
    notify_id = api->maki_addDlfRef(o->vcpu_getController(), "notify", this);
  }
  inited=1;
}

C_RootObject::~C_RootObject() {
  if (!inited) return;
  api->maki_remDlfRef(this);
  //count--;
}

ScriptObject *C_RootObject::getScriptObject() {
  ASSERT(inited);
  return object;
}

const char *C_RootObject::getClassName() {
  ASSERT(inited);
  return GET_SCRIPT_STRING(api->maki_callFunction(object, getclassname_id, NULL));
}

void C_RootObject::notify(const char *a, const char *b, int c, int d) {
  ASSERT(inited);
  scriptVar _a = MAKE_SCRIPT_STRING(a);
  scriptVar _b = MAKE_SCRIPT_STRING(b);
  scriptVar _c = MAKE_SCRIPT_INT(c);
  scriptVar _d = MAKE_SCRIPT_INT(d);
  scriptVar *params[4]={&_a, &_b, &_c, &_d};
  api->maki_callFunction(object, notify_id, params);
}

int C_RootObject::getclassname_id=0;
int C_RootObject::notify_id=0;
int C_RootObject::inited=0;
int C_RootObject::count=0;
