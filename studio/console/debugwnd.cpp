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

#include "debugwnd.h"

#include "../studio/api.h"
#include "../studio/wac.h"
#include "../common/buttwnd.h"
#include "resource.h"

DebugWnd::~DebugWnd() {
  api->syscb_deregisterCallback(this);
}

int DebugWnd::onInit() {
  DEBUGWND_PARENT::onInit();
  api->syscb_registerCallback(this);

  insertColumn(new ListColumn("Message", TRUE));

  return 1;
}

int DebugWnd::consolecb_outputString(int severity, const char *string) {
  addItem(string, 0);
  if (getNumItems() > 128)	// FUCKO: make configurable
    deleteByPos(0);
  ensureItemVisible(getNumItems()-1);
  return 1;
}

void DebugWnd::setIconBitmaps(ButtonWnd *button) {
  extern WAComponentClient *the;
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}
