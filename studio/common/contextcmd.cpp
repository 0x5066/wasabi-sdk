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


#include "contextcmd.h"
class ContextCmd::ContextCmdEntry {
public:
  ContextCmdEntry(const char *_text,
                  ContextCmdCB *_callback=NULL,
                  int _enabled=1, int _checked=0,
                  int _sortval=ContextCmdSortVal::MIDDLE, int id=-1) :
    text(_text), callback(_callback),
    enabled(_enabled), checked(_checked), sortval(_sortval), cmdid(id) {}

  String text;
  int enabled, checked, sortval, cmdid;
  ContextCmdCB *callback;
};

ContextCmd::ContextCmd(const char *_classname, const char *_menu_path) :
  classname(_classname), menu_path(_menu_path) {
}

ContextCmd::~ContextCmd() {
  commands.deleteAll();
}

void ContextCmd::addCommand(const char *text,
                  ContextCmdCB *callback, int enabled, int checked,
                  int sortval) {
  ASSERT(text != NULL);
  commands.addItem(new ContextCmdEntry(text, callback, enabled,
    checked, sortval));
}

void ContextCmd::addCommandById(const char *text, int cmdid,
                                ContextCmdCB *callback,
                                int enabled, int checked, int sortval) {
  ASSERT(text != NULL);
  commands.addItem(new ContextCmdEntry(text, callback, enabled,
    checked, sortval, cmdid));
}

void ContextCmd::addSeparator(int sortval) {
  commands.addItem(new ContextCmdEntry(NULL, NULL, FALSE, FALSE, sortval));
}

void ContextCmd::onCommand(DragItem *item, int n) {
  ContextCmdEntry *entry = commands[n];
  if (entry == NULL || entry->callback == NULL) return;
  entry->callback->onCommand(item);
}

int ContextCmd::testItem(DragItem *item, const char *_menu_path) {
  if (!STREQL(classname, item->getDatatype())) return 0;

  // no path applied = we apply
  if (_menu_path == NULL) return 1;

  // there is a path, but we don't have one
  if (menu_path.isempty()) return 0;

  // there is a path, see if we match
  return STRCASEEQL(_menu_path, menu_path);
}

const char *ContextCmd::getCommand(DragItem *item, int n) {
  if (n == 0) populateCommands();
  ContextCmdEntry *entry = commands[n];
  if (entry == NULL) return NULL;
  if (entry->text.isempty()) return "~~~SEP~~~";	// sorry, magic value
  return entry->text;
}

int ContextCmd::getEnabled(DragItem *item, int n) {
  return commands[n]->enabled;
}

int ContextCmd::getChecked(DragItem *item, int n) {
  return commands[n]->checked;
}

int ContextCmd::getSortVal(DragItem *item, int n) {
  return commands[n]->sortval;
}

int ContextCmd::getSubMenu(DragItem *item, const char *_menu_path) {
  if (menu_path.isempty()) return 0;
  if (_menu_path == NULL) return 1;
  if (STRCASEEQL(_menu_path, menu_path)) return 0;	// we're here
  return !!!STRNINCMP(_menu_path, menu_path);
}

const char *ContextCmd::getSubMenuText(const char *_menu_path) {
  if (_menu_path == NULL) _menu_path = "";
  ASSERT(_menu_path != NULL);
  int l = menu_path.len();
  int pl = STRLEN(_menu_path);
  ASSERT(l >= pl);
  if (l == pl) return NULL;
  return menu_path.getValue() + pl;
}

int ContextCmd::getCmdId(int pos) {
  return commands.q(pos)->cmdid; // no bounds checking, it's-a protected
}
