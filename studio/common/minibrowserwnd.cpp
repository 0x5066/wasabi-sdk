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

#include "minibrowserwnd.h"
#include "minibrowser.h"
#include "../studio/services/svc_minibrowser.h"
#include "../studio/api.h"
#include "../bfc/svc_enum.h"

MiniBrowserWnd::MiniBrowserWnd(GUID mb_provider) {
  mb = NULL;
  mbsvc = MiniBrowserSvcEnum(mb_provider).getFirst();
}

MiniBrowserWnd::~MiniBrowserWnd() {
  if (mbsvc) {
    if (mb) mbsvc->destroyMiniBrowser(mb);
    api->service_release(mbsvc);  
  }
}

int MiniBrowserWnd::onInit() {
  int r = MBWND_PARENT::onInit();
  if (mb) {
    mb->minibrowser_getRootWnd()->setStartHidden(1);
    mb->minibrowser_getRootWnd()->setParent(this);
    r &= mb->minibrowser_getRootWnd()->init(this);
  }
  onSetVisible(1);
  return r;
}

void MiniBrowserWnd::onSetVisible(int i) {
  MBWND_PARENT::onSetVisible(i);
  if (i) {
    if (!mb && mbsvc) {
      mb = mbsvc->createMiniBrowser();
      if (mb) {
        mb->minibrowser_addCB(this);
        mb->minibrowser_getRootWnd()->setStartHidden(1);
        mb->minibrowser_getRootWnd()->setParent(this);
        mb->minibrowser_getRootWnd()->init(this);
        if (isPostOnInit())
          onResize();
      }
    }
  } else {
    if (mb) {
      mbsvc->destroyMiniBrowser(mb);
      mb = NULL;
    }
  }
  if (mb && mb->minibrowser_getRootWnd()) {
    mb->minibrowser_getRootWnd()->setVisible(i);
  }
}

int MiniBrowserWnd::onResize() {
  int r = MBWND_PARENT::onResize();
  if (mb && mb->minibrowser_getRootWnd()) {
    RECT r;
    getClientRect(&r);
    mb->minibrowser_getRootWnd()->resize(r.left, r.top, r.right-r.left, r.bottom-r.top);
  }
  return r;
}


int MiniBrowserWnd::handleDesktopAlpha() {
  if (mb && mb->minibrowser_getRootWnd()) return mb->minibrowser_getRootWnd()->handleDesktopAlpha();
  return 0;
}

int MiniBrowserWnd::handleRatio() { 
  if (mb && mb->minibrowser_getRootWnd()) return mb->minibrowser_getRootWnd()->handleRatio();
  return 0; 
}

int MiniBrowserWnd::navigateUrl(const char *url) {
  if (mb) return mb->minibrowser_navigateUrl(url);
  return 0;
}

int MiniBrowserWnd::back() {
  if (mb) return mb->minibrowser_back();
  return 0;
}

int MiniBrowserWnd::forward() {
  if (mb) return mb->minibrowser_forward();
  return 0;
}

int MiniBrowserWnd::home() {
  if (mb) return mb->minibrowser_home();
  return 0;
}

int MiniBrowserWnd::refresh() {
  if (mb) return mb->minibrowser_refresh();
  return 0;
}

int MiniBrowserWnd::stop() {
  if (mb) return mb->minibrowser_stop();
  return 0;
}

void MiniBrowserWnd::setTargetName(const char *name) {
  if (mb) mb->minibrowser_setTargetName(name);
}

const char *MiniBrowserWnd::getTargetName() {
  if (mb) return mb->minibrowser_getTargetName();
  return NULL;
}

const char *MiniBrowserWnd::getCurrentUrl() {
  if (mb) return mb->minibrowser_getCurrentUrl();
  return NULL;
}

int MiniBrowserWnd::onBeforeNavigate(const char *url, int flags, const char *frame) {
  return 0; // return 1 to cancel navigation
}

void MiniBrowserWnd::onDocumentComplete(const char *url) {
}

int MiniBrowserWnd::minibrowsercb_onBeforeNavigate(const char *url, int flags, const char *frame) {
  return onBeforeNavigate(url, flags, frame);
}

void MiniBrowserWnd::minibrowsercb_onDocumentComplete(const char *url) {
  onDocumentComplete(url);
}

