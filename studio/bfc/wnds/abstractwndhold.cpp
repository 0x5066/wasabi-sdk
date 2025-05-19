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

#include "abstractwndhold.h"
#include "../svc_enum.h"
#include "../../common/guiobjwnd.h"
#include "../../studio/services/svc_wndcreate.h"
#include "../../common/script/scriptguid.h"
#include "../../common/script/c_script/c_guiobject.h"
#include "../../common/script/c_script/c_group.h"
#include "../../bfc/notifmsg.h"

#define CB_ABSTRACTLOAD 0x1125

AbstractWndHolder::AbstractWndHolder(const char *grpid, int autoresize) : ServiceWndHolder(NULL, NULL) {
  groupid = grpid;
  guid = INVALID_GUID;
  group = NULL;
  cbreg = 0;
  inselfresize = 0;
  autoresizefromcontent = autoresize;
  allow_deferred_content = 0;
  need_deferred_load = 0;
}

AbstractWndHolder::AbstractWndHolder(GUID g, int autoresize) : ServiceWndHolder(NULL, NULL) {
  guid = g;
  group = NULL;
  cbreg = 0;
  inselfresize = 0;
  autoresizefromcontent = autoresize;
  allow_deferred_content = 0;
  need_deferred_load = 0;
}

AbstractWndHolder::~AbstractWndHolder() {
  if (group) {
    api->group_destroy(group);
    group = NULL;
  }
  if (cbreg) api->syscb_deregisterCallback(this);
}

int AbstractWndHolder::onInit() {
  int r = ABSTRACTWNDHOLDER_PARENT::onInit();
  createChild();
  return 1;
}

RootWnd *AbstractWndHolder::rootwndholder_getRootWnd() {
  if (group != NULL) return group;
  return ABSTRACTWNDHOLDER_PARENT::rootwndholder_getRootWnd();
}

void AbstractWndHolder::abstract_setContent(const char *group_id, int autoresize) {
  setBothContent(group_id, INVALID_GUID, autoresize);
}

void AbstractWndHolder::abstract_setContent(GUID g, int autoresize) {
  setBothContent(NULL, g, autoresize);
}

void AbstractWndHolder::setBothContent(const char *group_id, GUID g, int autoresize) {
  if (autoresize != -1)
    autoresizefromcontent = autoresize;
  if (STRCASEEQLSAFE(groupid, group_id) || (guid != INVALID_GUID && guid == g)) return;
  GUID _g = nsGUID::fromChar(group_id);
  if (g == INVALID_GUID && _g != INVALID_GUID) {
    groupid = NULL;
    guid = _g;
  } else {
    groupid = group_id;
    guid = g;
  }
  createChild();
}

int AbstractWndHolder::onGroupChange(const char *grpid) {
  if (STRCASEEQLSAFE(groupid, grpid)) {
    createChild();
    notifyParent(ChildNotify::GROUPRELOAD);
    notifyParent(ChildNotify::AUTOWHCHANGED);
  }
  return 1;
}

void AbstractWndHolder::createChild() {
  if (!isInited()) return;
  destroyContent();
  if (allow_deferred_content && !isVisible())
    need_deferred_load = 1;
  else
    doLoadContent();
}

void AbstractWndHolder::destroyContent() {
  if (group != NULL) {
    api->group_destroy(group);
    if (cbreg) {
      api->syscb_deregisterCallback(this);
      cbreg=0;
    }
    ABSTRACTWNDHOLDER_PARENT::setChild(NULL, NULL);
  }
  group = NULL;
}

int AbstractWndHolder::onDeferredCallback(int p1, int p2) {
  if (p1 == CB_ABSTRACTLOAD) {
    doLoadContent();
    return 1;
  }
  return ABSTRACTWNDHOLDER_PARENT::onDeferredCallback(p1, p2);
}

#include "../../common/profiler.h"
void AbstractWndHolder::doLoadContent() {
  int didsomething = 0;
  need_deferred_load = 0;
  if (!groupid.isempty()) {
  PR_ENTER(StringPrintf("abstract - group create %s", groupid));
    group = api->group_create(groupid);
    if (group) {
      if (!cbreg) {
        api->syscb_registerCallback(this);
        cbreg=1;
      }
      group->setParent(this);
  //PR_ENTER(StringPrintf("abstract - group init %s", groupid));
      group->init(this);
  //PR_LEAVE();
      abstract_onNewContent();
      didsomething = 1;
    }
  PR_LEAVE();
  } else if (guid != INVALID_GUID) {
    svc_windowCreate *svc = WindowCreateByGuidEnum(guid).getNext();
    ABSTRACTWNDHOLDER_PARENT::setChild(svc->createWindowByGuid(guid, this), svc);
    abstract_onNewContent();
      didsomething = 1;
  }
  if (didsomething && isPostOnInit()) {
//  PR_ENTER(StringPrintf("abstract - group onresize %s", groupid));
    onResize();
//  PR_LEAVE();
  }
}

void AbstractWndHolder::abstract_onNewContent() {
  if (isPostOnInit()) {
    RootWnd *w = getDesktopParent();
    if (w->enumMinMaxEnforcer(0)) {
      w->signalMinMaxEnforcerChanged();
    }
  }
}

GuiObject *AbstractWndHolder::abstract_findObject(const char *object_id) {
  RootWnd *w = rootwndholder_getRootWnd();
  if (w == NULL) return NULL;
  GuiObject *o = static_cast<GuiObject *>(w->getInterface(guiObjectGuid));
  return o->guiobject_findObject(object_id);
}

ScriptObject *AbstractWndHolder::abstract_findScriptObject(const char *object_id) {
  RootWnd *w = rootwndholder_getRootWnd();
  if (w == NULL) return NULL;
  GuiObject *o = static_cast<GuiObject *>(w->getInterface(guiObjectGuid));
  GuiObject *fo = o->guiobject_findObject(object_id);
  if (fo != NULL) return fo->guiobject_getScriptObject();
  return NULL;
}


GuiObject *AbstractWndHolder::abstract_getContent() {
  RootWnd *w = rootwndholder_getRootWnd();
  if (w == NULL) return NULL;
  return static_cast<GuiObject *>(w->getInterface(guiObjectGuid));
}

ScriptObject *AbstractWndHolder::abstract_getContentScriptObject() {
  RootWnd *w = rootwndholder_getRootWnd();
  if (w == NULL) return NULL;
  return static_cast<ScriptObject *>(w->getInterface(scriptObjectGuid));
}

int AbstractWndHolder::onResize() {
  int rt = ABSTRACTWNDHOLDER_PARENT::onResize();
  if (group != NULL && abstract_wantAutoResizeFromContent()) {
    if (inselfresize) return rt;
    inselfresize=1;
    int w = group->getPreferences(SUGGESTED_W);
    int h = group->getPreferences(SUGGESTED_H);
    resize(NOCHANGE, NOCHANGE, w == AUTOWH ? NOCHANGE : w, h == AUTOWH ? NOCHANGE : h);
    inselfresize = 0;
  }
  return rt;  
}

void AbstractWndHolder::onSetVisible(int show) {
  ABSTRACTWNDHOLDER_PARENT::onSetVisible(show);
  if (allow_deferred_content) {
    if (show & need_deferred_load) {
      doLoadContent();
    }
    if (!show && !need_deferred_load) {
      destroyContent();
      need_deferred_load = 1;      
    }
  }
}

