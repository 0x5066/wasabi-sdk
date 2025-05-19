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

#ifndef _CONTEXTMENU_H
#define _CONTEXTMENU_H

#include "popup.h"
#include "common.h"

class DragItem;
class ContextMenuEntry;
class ContextMenuEntryCompare;
class svc_contextCmd;

class COMEXP ContextMenu : private PopupMenu, private PopupMenuCallback {
public:
  ContextMenu(RootWnd *sourceWnd, DragItem *item, bool autopop=TRUE, const char *menu_path=NULL);
  ContextMenu(RootWnd *sourceWnd, int x, int y, DragItem *item, bool autopop=TRUE, const char *menu_path=NULL);
  virtual ~ContextMenu();

  void addDragItem(DragItem *item, const char *menu_path=NULL);

  using PopupMenu::popAtXY;
  using PopupMenu::popAtMouse;
  using PopupMenu::addCommand;
  using PopupMenu::addSeparator;
  using PopupMenu::getNumCommands;

protected:
  ContextMenu(DragItem *item, const char *menu_path);

private:
  virtual void onPostPop(int result);

  void populate();
  virtual PopupMenu *popupMenuCallback(PopupMenu *parent, int param);

  DragItem *item;
  String menu_path;
  PtrList<svc_contextCmd> *svclist;
  PtrListQuickSorted<ContextMenuEntry, ContextMenuEntryCompare> *entries;
};

#endif
