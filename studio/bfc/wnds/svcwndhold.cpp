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


#include "svcwndhold.h"

#include "../../studio/api.h"
#include "../../common/common.h"

#include "../../studio/services/svc_wndcreate.h"

#include "blankwnd.h"

ServiceWndHolder::ServiceWndHolder(RootWnd *_child, svc_windowCreate *_svc) :
 child(NULL), svc(NULL) {
  setChild(_child, _svc);
}

ServiceWndHolder::~ServiceWndHolder() {
  if (svc != NULL) {
    svc->destroyWindow(child);
    if (!svc->refcount()) 
      api->service_release(svc);
  } else {
    delete static_cast<BaseWnd*>(child);
  }
}

int ServiceWndHolder::setChild(RootWnd *_child, svc_windowCreate *_svc) {
  if (child == _child && svc == _svc) return 0;

  if (child != NULL) {
    if (svc != NULL) {
      svc->destroyWindow(child);
      if (!svc->refcount()) 
        api->service_release(svc);
      svc = NULL;
    } else {
      delete static_cast<BaseWnd*>(child);
    }
  child = NULL;
  } 
  
  child = _child;
  svc = _svc;

  return 1;
}

RootWnd *ServiceWndHolder::rootwndholder_getRootWnd() {
  return child ? child : SERVICEWNDHOLDER_PARENT::rootwndholder_getRootWnd();
}
