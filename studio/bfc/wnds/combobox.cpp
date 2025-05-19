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

#include "combobox.h"
#include "../../common/script/c_script/c_edit.h"
#include "../../common/xmlobject.h"

ComboBox::ComboBox() {
  keys_edit = NULL;
  lastlist = NULL;
  disable_getselection = 0;
}

ComboBox::~ComboBox() {
  delete keys_edit;
}

void ComboBox::abstract_onNewContent() {
  COMBOBOX_PARENT::abstract_onNewContent();
  trapControls();
}

void ComboBox::trapControls() {
  delete keys_edit;

  keys_edit = NULL;

  if (wantTrapEdit()) {
    GuiObject *editGuiObj = getGuiObject()->guiobject_findObject(combobox_getEditId());
    if (editGuiObj) keys_edit = new HEBKeysCallback(*editGuiObj, this);
  }
}

void ComboBox::updateTextInControl(const char *txt) {
  if (txt == NULL) return;
  if (STRCASEEQLSAFE(getText(), txt)) return;
  GuiObject *content = getContent();
  if (content != NULL) {
    if (wantTrapEdit()) {
      GuiObject *text = content->guiobject_findObject(combobox_getEditId());
      if (text != NULL) {
        C_Edit t(*text);
        t.setText(txt);
        curtxt = txt;
      }
    }
  }
}

void ComboBox::dropdownlist_onCloseList() {
  COMBOBOX_PARENT::dropdownlist_onCloseList();
  if (wantTrapEdit()) {
    GuiObject *o = embeddedxui_getEmbeddedObject();
    if (o != NULL) {
      o->guiobject_getRootWnd()->setFocus();
      GuiObject *edit = o->guiobject_findObjectByInterface(editGuid);
      if (edit != NULL) {
        C_Edit e(*edit);
        e.setAutoEnter(savedautoenter);
        e.setIdleEnabled(savedidle);
      }
    }
  }
  if (wantEnterOnSelect())
    enter();
  disable_getselection = 0;
}

void ComboBox::dropdownlist_onOpenList() {
  COMBOBOX_PARENT::dropdownlist_onOpenList();
  if (wantTrapEdit()) {
    GuiObject *o = embeddedxui_getEmbeddedObject();
    if (o != NULL) {
      o->guiobject_getRootWnd()->setFocus();
      GuiObject *edit = o->guiobject_findObjectByInterface(editGuid);
      if (edit != NULL) {
        C_Edit e(*edit);
        savedidle = e.getIdleEnabled();
        savedautoenter = e.getAutoEnter();
        e.setIdleEnabled(0);
        e.setAutoEnter(0);
      }
    }
  }
}

void ComboBox::setText(const char *text, int hover) {
  updateTextInControl(text);
  selectItem(-1, hover);
  selectEditor();
}

const char *ComboBox::getText(int fromcontrol) {
  if (!fromcontrol) return curtxt;
  const char *c = NULL;
  GuiObject *content = getContent();
  if (content != NULL) {
    if (wantTrapEdit()) {
      GuiObject *text = content->guiobject_findObject(combobox_getEditId());
      if (text != NULL) {
        C_Edit t(*text);
        c = t.getText();
      }
    }
  }
  curtxt = c;
  return c;
}

void ComboBox::dropdownlist_onConfigureList(GuiObject *o) {
  COMBOBOX_PARENT::dropdownlist_onConfigureList(o);
  RootWnd *w = o->guiobject_getRootWnd()->findWindowByInterface(listGuid);
  sendAction(w, "register_tempselectnotify");
  //w->getGuiObject()->guiobject_setXmlParam("select", getCustomText());
  lastlist = w->getGuiObject();
}

void ComboBox::onSelect(int id, int hover) {
  COMBOBOX_PARENT::onSelect(id, hover);
  if (!hover) {
    selectEditor();
    if (wantEnterOnSelect())
      enter();
  }
}

void ComboBox::enter() {
  GuiObject *content = getContent();
  if (content != NULL) {
    if (wantTrapEdit()) {
      GuiObject *text = content->guiobject_findObject(combobox_getEditId());
      if (text != NULL) {
        C_Edit t(*text);
        t.enter();
      }
    }
  }
}

void ComboBox::selectEditor() {
  GuiObject *content = getContent();
  if (content != NULL) {
    if (wantTrapEdit()) {
      GuiObject *text = content->guiobject_findObject(combobox_getEditId());
      if (text != NULL) {
        C_Edit t(*text);
        t.selectAll();
      }
    }
  }
}

int ComboBox::onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source) {
  int r = COMBOBOX_PARENT::onAction(action, param, x, y, p1, p2, data, datalen, source);
  if (STRCASEEQLSAFE(action, "tempselectnotify")) {
    if (!disable_getselection)
      setText(param, 1);
  }
  return r;
}

void ComboBox::onEditKeyDown(int vk) {
  if (Std::keyDown(VK_CONTROL)) return;
  if (vk == VK_DOWN) {
    if (wantDownOpenList()) {
      if (!isListOpen())
        openList();
      else {
        if (wantTransferDownToList())
          listDown();
      }
    }
  } else if (vk == VK_UP) {
    if (wantTransferUpToList())
      listUp();
  } else if (vk == VK_HOME) {
    if (wantTransferHomeToList())
      listHome();
  } else if (vk == VK_END) {
    if (wantTransferEndToList())
      listEnd();
  } else if (vk == VK_PRIOR) {
    if (wantTransferPgUpToList())
      listPageUp();
  } else if (vk == VK_NEXT) {
    if (wantTransferPgDnToList())
      listPageDown();
  } else if (vk == VK_ESCAPE) {
    if (isListOpen())
      closeList();
  } else if (vk == VK_BACK || vk == VK_DELETE || vk == VK_LEFT || vk == VK_RIGHT) {
    if (wantCloseListOnChar()) {
      if (isListOpen())
        closeList();
    }
  }
}

void ComboBox::onEditKeyUp(int vk) {
  curtxt = getText(1);
}

void ComboBox::onEditEnter(const char *txt) {
  if (isListOpen()) {
    if (wantTransferEnterToList())
      listSelect();
  }
}

void ComboBox::onEditChar(const char *c) {
  if (wantCloseListOnChar()) {
    if (isListOpen())
      closeList();
  }
}


void ComboBox::listUp() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "up");
  }
}

void ComboBox::listDown() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "down");
  }
}

void ComboBox::listHome() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "home");
  }
}

void ComboBox::listEnd() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "end");
  }
}

void ComboBox::listPageUp() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "pageup");
  }
}

void ComboBox::listPageDown() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "pagedown");
  }
}

void ComboBox::listSelect() {
  if (lastlist != NULL && isListOpen()) {
    sendAction(lastlist->guiobject_getRootWnd(), "select_current");
  }
}

void ComboBox::onPreCloseList() {
  disable_getselection = 1;
}

