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

#include "guiradiogroup.h"
#include "../../common/script/c_script/c_button.h"
#include "../../common/script/guiobject.h"
#include "../../common/script/scriptguid.h"

void GuiRadioGroup::toggleChild(GuiObject *who) {
  int i, num = children.getNumItems();
  for (i = 0; i < num; i++) {
    GuiObject *childGuiObj = children.enumItem(i);
    if (childGuiObj != NULL) {
      GuiObject *toggleGuiObj = childGuiObj->guiobject_findObject("checkbox.toggle");
      if (toggleGuiObj != NULL) {
        C_Button buttonObj(*toggleGuiObj);
        if (childGuiObj != who) {
          buttonObj.setActivated(0);
        }
      }
    }
  }  
}

void GuiRadioGroup::registerChild(GuiObject *who) {
  children.addItem(who);
}

int GuiRadioGroup::onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source) {
  if (STRCASEEQL(action, "REGISTER") && source != NULL) {
    registerChild(static_cast<GuiObject *>(source->getInterface(guiObjectGuid)));
  } else if (STRCASEEQL(action, "TOGGLE")) {
    toggleChild(static_cast<GuiObject *>(source->getInterface(guiObjectGuid)));
    return 1;
  }
  return GUIRADIOGROUP_PARENT::onAction(action, param, x, y, p1, p2, data, datalen, source);
}

