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

#include "prefwnd.h"
#include "../../studio/services/svc_action.h"

PreferencesWnd::PreferencesWnd(GUID g, int modal) {
  const char *action = modal ? "Preferences:OPENMODAL" : "Preferences:OPEN";
  svc_action *pref = ActionEnum(action).getFirst();
  if (!pref) return;
  char strguid[nsGUID::GUID_STRLEN+1];
  nsGUID::toChar(g, strguid);
  pref->onAction(action, strguid);
  SvcEnum::release(pref);
}
