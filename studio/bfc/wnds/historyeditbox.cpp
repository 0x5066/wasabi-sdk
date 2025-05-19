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

#include "historyeditbox.h"

HistoryEditBox::HistoryEditBox() {
  history_pos = 0;
  xuihandle = newXuiHandle();
  addParam(xuihandle, "navbuttons", HISTORYEDITBOX_SETNAVBUTTONS, XML_ATTRIBUTE_IMPLIED); // param is implemented by script
  setXmlParam("navbuttons", "1"); // so we need to set a default value in the xml param list
}

HistoryEditBox::~HistoryEditBox() {
  history.deleteAll();
}

void HistoryEditBox::history_forward() {
  if (history_pos > 0 && !isListOpen()) {
    history_pos--;
    if (history_pos > 0)
      setText(history.enumItem(history.getNumItems()-history_pos)->getValue(), 1);
  }
}

void HistoryEditBox::history_back() {
  if (!isListOpen() && history_pos < history.getNumItems()) {
    history_pos++;
    setText(history.enumItem(history.getNumItems()-history_pos)->getValue(), 1);
  }
}

void HistoryEditBox::onEditKeyDown(int vk) {
  HISTORYEDITBOX_PARENT::onEditKeyDown(vk);
  if (Std::keyDown(VK_CONTROL)) return;
  if (vk == VK_DOWN) {
    history_forward();
  } else if (vk == VK_UP) {
    history_back();
  }
}

int HistoryEditBox::onInit() {
  int r = HISTORYEDITBOX_PARENT::onInit();
  loadHistory();
  return r;
}

void HistoryEditBox::dropdownlist_onCloseList() {
  HISTORYEDITBOX_PARENT::dropdownlist_onCloseList();
  history_pos = 0;
}

void HistoryEditBox::onPreOpenList() {
  HISTORYEDITBOX_PARENT::onPreOpenList();
  addHistory(getText());
}

void HistoryEditBox::onEditEnter(const char *txt) {
  HISTORYEDITBOX_PARENT::onEditEnter(txt);
  if (Std::keyDown(VK_CONTROL)) return;
  addHistory(txt);
}

void HistoryEditBox::addHistory(const char *txt) {
  HISTORYEDITBOX_PARENT::onEditEnter(txt);
  history_pos = 0;
  
  if (!txt || !*txt) return;

  // yay multi-instances on unique history
  loadHistory(0);

  int pos = -1;
  foreach(history) 
    String *s = history.getfor();
    if (STRCASEEQL(s->getValue(), txt)) {
      delete s;
      history.removeByPos(foreach_index);
      break;
    }
  endfor;

  history.addItem(new String(txt));

  while (history.getNumItems() > 64) {
    String *s = history.enumItem(0);
    delete s;
    history.removeByPos(1);
  }
  saveHistory();
  loadHistory(1);
}

void HistoryEditBox::loadHistory(int refill) {
  history.deleteAll();
  char d[256];
  char c[WA_MAX_PATH];
  for (int i=0;;i++) {
    SPRINTF(d, "%s_history_%d", getId(), i);
    api->getStringPrivate(d, c, WA_MAX_PATH, "");
    if (!*c)
      break;
    history.addItem(new String(c));
  }
  if (refill) {
    deleteAllItems();
    for (i=history.getNumItems()-1;i>=0;i--) {
      addItem(history.enumItem(i)->getValue());
    }
  }
}

void HistoryEditBox::saveHistory() {
  char d[256];
  for (int i=0;i<history.getNumItems();i++) {
    SPRINTF(d, "%s_history_%d", getId(), i);
    api->setStringPrivate(d, history.enumItem(i)->getValue());
  }
  SPRINTF(d, "%s_history_%d", getId(), i);
  api->setStringPrivate(d, "");
}

int HistoryEditBox::onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source) {
  int r = HISTORYEDITBOX_PARENT::onAction(action, param, x, y, p1, p2, data, datalen, source);
  if (STRCASEEQLSAFE(action, "back")) history_back();
  if (STRCASEEQLSAFE(action, "forward")) history_forward();
  return r;
}
