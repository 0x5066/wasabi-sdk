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

#ifndef _SHOWPREF_H
#define _SHOWPREF_H

#include "../../common/nsguid.h"
#include "../../studio/services/svc_action.h"

// usage:
//  ShowPreferences(page_guid);

/**
  Helper class to pop-open a preferences pane based
  on the components GUID.
  
  This can be called by anyone to pop-open any preference
  pane for any component.
  
  Example:
    ShowPreferences(page_guid);
*/
class ShowPreferences {
public:
  /**
    Pops open the preferences window to the preferences page
    for the component of GUID g or the page that has GUID g
    associated to it.
    
    @param g      GUID of the page to display.
    @param modal  0, No modal; 1, Modal;
  */
  ShowPreferences(GUID g, int modal=0) {
    const char *action = modal ? "Preferences:OPENMODAL" : "Preferences:OPEN";
    svc_action *pref = ActionEnum(action).getFirst();
    if (!pref) return;
    char strguid[nsGUID::GUID_STRLEN+1];
    nsGUID::toChar(g, strguid);
    pref->onAction(action, strguid);
    SvcEnum::release(pref);
  }
};

#endif
