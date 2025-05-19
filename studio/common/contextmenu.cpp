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


#include "contextmenu.h"
#include "../studio/services/svc_contextcmd.h"

#include "../studio/api.h"

#define DD_CONTEXTMENUENTRY "ContextMenuEntry v1"

class ContextMenuEntry {
public:
  ContextMenuEntry(DragItem *_item, svc_contextCmd *_svc, int _pos, const char *txt, int _sortval, int _addorder) :
    item(_item), svc(_svc), pos(_pos), text(txt), sortval(_sortval), addorder(_addorder) { }

  svc_contextCmd *svc;
  DragItem *item;
  int pos;
  String text, submenu_text;
  int sortval;
  int addorder;
};

class ContextMenuEntryCompare {
public:
  static int compareItem(void *p1, void* p2) {
    ContextMenuEntry *e1 = static_cast<ContextMenuEntry*>(p1);
    ContextMenuEntry *e2 = static_cast<ContextMenuEntry*>(p2);
    int ret = CMP3(e1->sortval, e2->sortval);
    if (ret == 0) ret = CMP3(e1->addorder, e2->addorder);
    return ret;
  }
};

ContextMenu::ContextMenu(RootWnd *sourceWnd, DragItem *_item, bool autopop, const char *_menu_path)
 : PopupMenu(sourceWnd), item(_item), menu_path(_menu_path), svclist(NULL), entries(NULL) {
  populate();
  if (autopop) popAtMouse();
}

ContextMenu::ContextMenu(DragItem *_item, const char *_menu_path)
 : item(_item), menu_path(_menu_path), svclist(NULL), entries(NULL) {
  populate();
}

ContextMenu::~ContextMenu() {
  if (entries != NULL) {
    entries->deleteAll();
    delete entries;
  }
  if (svclist != NULL) {
    // release all services
    for (int i = 0; i < svclist->getNumItems(); i++)
      SvcEnum::release(svclist->enumItem(i));
    delete svclist;
  }
}

void ContextMenu::addDragItem(DragItem *_item, const char *_menu_path) {
  menu_path = _menu_path;
  item = _item;
  populate();
}

void ContextMenu::populate() {
  if (item == NULL) return;
  svclist = new PtrList<svc_contextCmd>;
  entries = new PtrListQuickSorted<ContextMenuEntry, ContextMenuEntryCompare>;
  ContextCmdEnum cce(item, menu_path); 
  svc_contextCmd *svc;
  int i, j, addorder=0;

  // make a list of all context cmd services that match the menu path
  for (i = 0; (svc = cce.getNext()) != NULL; i++) {
    for (j = 0; ; j++) {
      const char *text = svc->getCommand(item, j);
      if (text == NULL) break;
      if (STREQL(text, "~~~SEP~~~")) text = NULL;	// sorry, magic value
      ContextMenuEntry *entry = new ContextMenuEntry(item, svc, j, text,
       svc->getSortVal(item, j), addorder++);
      entries->addItem(entry);
    }
    // save the service * to release later
    svclist->addItem(svc);
  }

  // sorting is implicit but just making sure
  entries->sort();

  PtrList<String> submenu_list;

  GUID prev = INVALID_GUID;

  // populate the menu from the list
  int n = entries->getNumItems();
  for (i = 0; i < n; i++) {
    ContextMenuEntry *entry = entries->enumItem(i);
    if (entry->text.isempty()) {
      addSeparator();
    } else {
      svc_contextCmd *svc = entry->svc;
      GUID g = api->service_getOwningComponent(svc);
      if (g != prev && prev != INVALID_GUID && i < n-1)
        addSeparator();
      prev = g;
      if (!svc->getSubMenu(item, menu_path)) {
        int checked = entry->svc->getChecked(item, entry->pos);
        int enabled = entry->svc->getEnabled(item, entry->pos);
        addCommand(entry->text, reinterpret_cast<int>(entry), checked,!enabled);
      } else {
        entry->submenu_text = svc->getSubMenuText(menu_path);
        if (!entry->submenu_text.isempty()) {
          for (j = 0; j < submenu_list.getNumItems(); j++)
            if (STRCASEEQL(*submenu_list[j], entry->submenu_text)) break;
          if (j >= submenu_list.getNumItems()) {
            submenu_list.addItem(new String(entry->submenu_text));
            addSubMenuCallback(entry->submenu_text, this, reinterpret_cast<int>(entry));
          }
        }
      }
    }
  }
  submenu_list.deleteAll();
}

void ContextMenu::onPostPop(int result) {
  //if (result == -1 || result == -2 || result == -3) return; //FUCKO need real enums
  if (result < 0) return;
  ASSERT(result != 0xcccccccc);
  ContextMenuEntry *entry = reinterpret_cast<ContextMenuEntry*>(result);
  if (entry == NULL) return;
  entry->svc->onCommand(entry->item, entry->pos);
}

PopupMenu *ContextMenu::popupMenuCallback(PopupMenu *parent, int param) {
  ContextMenuEntry *entry = reinterpret_cast<ContextMenuEntry*>(param);
  String path = menu_path;
  if (!path.isempty()) path.cat("/");
  path.cat(entry->submenu_text);
  ContextMenu *ret = new ContextMenu(entry->item, path);
  if (ret->getNumCommands() <= 0) {
    delete ret;
    ret = NULL;
  }
  return ret;
}
