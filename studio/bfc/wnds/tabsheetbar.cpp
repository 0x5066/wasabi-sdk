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

#include "tabsheetbar.h"
#include "tabsheet.h"
#include "../notifmsg.h"

TabSheetBar::TabSheetBar() {
  margin = 0;
  spacing = -4;
  maxheightsofar = 0;
  bottombar = new GuiObjectWnd;
  bottombar->setParent(this);
}

TabSheetBar::~TabSheetBar() {
  btns.deleteAll();
  delete bottombar;
}           

int TabSheetBar::onInit() {
  int rt = TABSHEETBAR_PARENT::onInit();

  bottombar->setContent("wasabi.tabsheet.nobutton.group");
  bottombar->init(this);

  foreach(btns) 
    GroupTabButton *gtb = btns.getfor();
    gtb->setParent(this);
    if (!gtb->isInited()) {
      gtb->init(this);
    }
    if (foreach_index == 0) 
      gtb->setStatus(STATUS_ON);
    else
      gtb->setStatus(STATUS_OFF);
    int h = gtb->getPreferences(SUGGESTED_H);
    if (h == AUTOWH) h = maxheightsofar;
    maxheightsofar = MAX(maxheightsofar, h);
  endfor;

  return rt;
}

void TabSheetBar::addChild(GroupTabButton *child) {
  ASSERT(!btns.haveItem(child));
  btns.addItem(child);
  if (isInited()) {
    child->setParent(this);
    if (!child->isInited()) {
      child->init(this);
    }
    int h = child->getPreferences(SUGGESTED_H);
    if (h == AUTOWH) h = maxheightsofar;
    maxheightsofar = MAX(maxheightsofar, h);
    onResize();
  }
  if (btns.getNumItems() == 1) 
    child->setStatus(STATUS_ON);
  else
    child->setStatus(STATUS_OFF);
}

int TabSheetBar::getHeight() {
  return maxheightsofar;
}

int TabSheetBar::onResize() {
  int rt = TABSHEETBAR_PARENT::onResize();
  GroupTabButton *selected=NULL;
  foreach(btns) 
    if (btns.getfor()->getStatus() == STATUS_ON) {
      selected = btns.getfor();
      break;
    }
  endfor;
  if (selected == NULL) selected = btns.getFirst();

  RECT r;
  getClientRect(&r);

  int x = margin;

  foreach(btns) 
    GroupTabButton *gtb = btns.getfor();

    int w = gtb->getPreferences(SUGGESTED_W);
    if (w == AUTOWH) w = 66;

    RECT dr;
    dr.left = r.left + x;
    dr.top = r.top;
    dr.right = dr.left + w;
    dr.bottom = dr.top + getHeight();

    gtb->resize(&dr);

    x += w + spacing;
  endfor;

  x -= spacing;
  RECT dr;
  dr.left = r.left + x;
  dr.top = r.top;
  dr.right = r.right;
  dr.bottom = r.top + getHeight();
  bottombar->resize(&dr);

  if (selected != NULL)
    selected->bringToFront();

  return rt;
}

int TabSheetBar::childNotify(RootWnd *child, int msg, int param1/* =0 */, int param2/* =0 */) {
  if (msg == ChildNotify::GROUPCLICKTGBUTTON_CLICKED && btns.haveItem(static_cast<GroupTabButton *>(child))) {
    GroupToggleButton *but = NULL;
    foreach(btns)
      if (btns.getfor() != child) 
        btns.getfor()->setStatus(STATUS_OFF);
      else
        but = btns.getfor();
    endfor;
    if (but != NULL)
      but->setStatus(STATUS_ON);
    else
      (static_cast<GroupToggleButton *>(child))->setStatus(STATUS_ON);
    onResize();    
  }
 if (msg == ChildNotify::AUTOWHCHANGED && btns.haveItem(static_cast<GroupTabButton *>(child)) && isPostOnInit()) 
  onResize();
 return TABSHEETBAR_PARENT::childNotify(child, msg, param1, param2);
}