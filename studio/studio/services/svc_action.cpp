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


#include "svc_action.h"

#define CBCLASS svc_actionI
START_DISPATCH;
  CB(HASACTION, hasAction);
  CB(ONACTION, onAction);
END_DISPATCH;
#undef CBCLASS

svc_actionI::~svc_actionI() {
  actions.deleteAll();
}

void svc_actionI::registerAction(const char *actionid, int pvtid) {
  actions.addItem(new ActionEntry(actionid, pvtid));
}

int svc_actionI::hasAction(const char *name) {
  return (actions.findItem(name) != NULL);
}

int svc_actionI::onAction(const char *action, const char *param, int p1, int p2, void *data, int datalen, RootWnd *source) {
  int pos=-1;
  if (actions.findItem(action, &pos)) {
    return onActionId(actions.enumItem(pos)->getId(), action, param, p1, p2, data, datalen, source);
  }
  return 0;
}
