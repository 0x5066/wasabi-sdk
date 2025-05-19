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

#include "groupholder.h"
#include "../../common/script/c_script/c_group.h"
#include "../../common/script/scriptguid.h"

GroupHolder::GroupHolder() {
}

GroupHolder::GroupHolder(const char *cont) {
  content = cont;
  setContent(content);
}

GroupHolder::~GroupHolder() {
}

RootWnd *GroupHolder::setContent(const char *grouporguid) {
  setChild(grouporguid);
  RootWnd *w = rootwndholder_getRootWnd();
  if (w != NULL) onSetContent(w, grouporguid);
  return w;
}

GuiObject *GroupHolder::getContent() {
  RootWnd *w = rootwndholder_getRootWnd();
  if (w == NULL) return NULL;
  return static_cast<GuiObject *>(w->getInterface(guiObjectGuid));
}

RootWnd *GroupHolder::getContentRootWnd() {
  return rootwndholder_getRootWnd();
}

GuiObject *GroupHolder::findObject(const char *object_id) {
  RootWnd *w = rootwndholder_getRootWnd();
  if (w == NULL) return NULL;
  GuiObject *o = static_cast<GuiObject *>(w->getInterface(guiObjectGuid));
  C_GuiObject go(o->guiobject_getScriptObject());
  return go.findObject(object_id);
}

void GroupHolder::onSetContent(RootWnd *content, const char *grouporguid) {
}
