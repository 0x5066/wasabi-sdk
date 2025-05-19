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


#include "groupswitch.h"

void GroupSwitch::addReplacement(const char *place, const char *groupname) {
  new GroupPair(place, groupname);
}

void GroupSwitch::setGroup(const char *groupname) {
  // rescue the replacements
  foreach(recs)
    Rec *rec = recs.getfor();
    GuiObject *custo = findObject(rec->placement);
    ASSERT(custo != NULL);
    CustomObject *obj = static_cast<CustomObject*>(custo->guiobject_getScriptObject()->vcpu_getInterface(customObjectGuid));
    ASSERT(obj != NULL);
    rec->rw = obj->customobject_getRootWnd();
    obj->customobject_setRootWnd(NULL);
  endfor

  // set the group
  setContent(groupname);
}

void GroupSwitch::onNewContent() {
  // do our replacements
  foreach(recs)
    Rec *rec = recs.getfor();
    GuiObject *custo = findObject(rec->placement);
    ASSERT(custo != NULL);
    CustomObject *obj = static_cast<CustomObject*>(custo->guiobject_getScriptObject()->vcpu_getInterface(customObjectGuid));
    ASSERT(obj != NULL);
    if (rec->rw == NULL) rec->rw = 
    obj->customobject_setRootWnd(rec->rw);
  endfor
}
