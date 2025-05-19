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

#include "rootwndholder.h"
#include "../../bfc/notifmsg.h"
#include "../../bfc/canvas.h"
#include "../../studio/api.h"

RootWndHolder::RootWndHolder() {
  privptr = NULL;
}

RootWndHolder::~RootWndHolder() {
}

void RootWndHolder::rootwndholder_getRect(RECT *r) {
  if (isInited())
    getClientRect(r);
  else
    MEMSET(r, 0, sizeof(RECT));
}

int RootWndHolder::onInit() {
  int r = ROOTWNDHOLDER_PARENT::onInit();
  RootWnd *w = rootwndholder_getRootWnd();
  if (w) {
    checkInit(w);
    setName(rootwndholder_getRootWnd()->getRootWndName());
  }
  return 1;
}

void RootWndHolder::checkInit(RootWnd *w) {
  if (w && !w->isInited()) {
    if (w->getParent() == NULL)
      w->setParent(this);
//    w->setStartHidden(getStartHidden());
    w->init(this);
  }
}

int RootWndHolder::onResize() {
  int rv = ROOTWNDHOLDER_PARENT::onResize();
  if (!isInited()) return 1;
  RootWnd *held = rootwndholder_getRootWnd();
  if (!held) return rv;
  RECT r;
  rootwndholder_getRect(&r);
  if (renderRatioActive() && !held->handleRatio())
    multRatio(&r);
  held->resize(r.left, r.top, r.right-r.left, r.bottom-r.top);
  return rv;
}

/*void RootWndHolder::onSetVisible(int v) {
  ROOTWNDHOLDER_PARENT::onSetVisible(v);
  if (!rootwndholder_getRootWnd()) return;
  rootwndholder_getRootWnd()->setVisible(v);
}*/

int RootWndHolder::onActivate() {
  int r = ROOTWNDHOLDER_PARENT::onActivate();
  if (rootwndholder_getRootWnd())
    rootwndholder_getRootWnd()->onActivate();
  return r;
}

int RootWndHolder::onDeactivate() {
  int r = ROOTWNDHOLDER_PARENT::onDeactivate();
    if (rootwndholder_getRootWnd()) 
    rootwndholder_getRootWnd()->onDeactivate();
  return r;
}

int RootWndHolder::getPreferences(int what) {
  if (rootwndholder_getRootWnd())
    return rootwndholder_getRootWnd()->getPreferences(what);
  return ROOTWNDHOLDER_PARENT::getPreferences(what);
}

RootWnd *RootWndHolder::rootwndholder_getRootWnd() {
  return privptr; 
} 

void RootWndHolder::rootwndholder_setRootWnd(RootWnd *w) { 
  if (privptr == w) return;
  privptr = w;
  checkInit(w);
  if (isPostOnInit())
    onResize(); 
}

int RootWndHolder::onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source) {
  if (rootwndholder_getRootWnd())
    return rootwndholder_getRootWnd()->onAction(action, param, x, y, p1, p2, data, datalen, source);
  return ROOTWNDHOLDER_PARENT::onAction(action, param, x, y, p1, p2, data, datalen, source);
}

int RootWndHolder::childNotify(RootWnd *child, int msg, int param1, int param2) {
  if (msg == ChildNotify::NAMECHANGED && child == rootwndholder_getRootWnd())
    setName(child->getRootWndName());
  return passNotifyUp(child, msg, param1, param2);
}

int RootWndHolder::onPaint(Canvas *c) {
  int rt = ROOTWNDHOLDER_PARENT::onPaint(c);
  if (wantRenderBaseTexture()) {
    RECT r;
    rootwndholder_getRect(&r);
    api->skin_renderBaseTexture(getBaseTextureWindow(), c, r, this);
  }
  return rt;
}