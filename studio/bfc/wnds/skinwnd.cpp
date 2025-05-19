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

#include "skinwnd.h"
#include "../rootwnd.h"
#include "../../common/guiobjwnd.h"
#include "../../common/script/scriptguid.h"
#include "../../common/script/c_script/c_group.h"
#include "../../common/script/c_script/c_layout.h"
#include "../../common/script/c_script/c_container.h"

SkinWnd::SkinWnd(const char *group_id, const char *prefered_container, int container_flag, RECT *animated_rect_source, int transcient, int starthidden) {
  wnd = api->skinwnd_createByGroupId(group_id, prefered_container, container_flag, animated_rect_source, transcient, starthidden);
  notifyMinMaxChanged();
}

SkinWnd::SkinWnd(GUID svc_or_group_guid, const char *prefered_container, int container_flag, RECT *animated_rect_source, int transcient, int starthidden) {
  wnd = api->skinwnd_createByGuid(svc_or_group_guid, prefered_container, container_flag, animated_rect_source, transcient, starthidden);
  notifyMinMaxChanged();
}

SkinWnd::~SkinWnd() {
}

void SkinWnd::destroy(RECT *animated_rect_dest) {
  if (wnd == NULL) return;
  api->skinwnd_destroy(wnd, animated_rect_dest);
}

int SkinWnd::runModal(int center) {
  if (wnd == NULL) return 0;
  RootWnd *w = wnd->getDesktopParent();
  if (center) {
    C_Layout l(getLayout());
    l.center();
  }
  w->setVisible(1);
  return w->runModal();
}

void SkinWnd::endModal(int retcode) {
  if (wnd == NULL) return;
  wnd->endModal(retcode);
}
 
GuiObject *SkinWnd::findObject(const char *object_id) {
  if (wnd == NULL) return NULL;
  GuiObject *obj = NULL;
  obj = static_cast<GuiObject *>(wnd->getInterface(guiObjectGuid));
  return obj->guiobject_findObject(object_id);
}

ScriptObject *SkinWnd::getContainer() {
  if (wnd == NULL) return NULL;
  RootWnd *dw = wnd->getDesktopParent();
  if (!dw) return NULL;
  ScriptObject *o = static_cast<ScriptObject *>(dw->getInterface(scriptObjectGuid));
  if (o != NULL) {
    C_Layout l(o);
    return l.getContainer();
  }
  return NULL;
}

ScriptObject *SkinWnd::getLayout() {
  if (wnd == NULL) return NULL;
  RootWnd *dw = wnd->getDesktopParent();
  if (!dw) return NULL;
  ScriptObject *o = static_cast<ScriptObject *>(dw->getInterface(scriptObjectGuid));
  return o;
}

void SkinWnd::notifyMinMaxChanged() {
  if (wnd != NULL) {
    wnd->signalMinMaxEnforcerChanged();
  }
}