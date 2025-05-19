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

#include "status.h"

#include "../../studio/api.h"

#include "../skinclr.h"
#include "../canvas.h"

#include "buttbar.h"
#include "buttwnd.h"
#include "../appcmds.h"
#include "../paramparser.h"

#include "../../common/script/c_script/c_text.h"

#define STATUS_TIMER_DECAY 1

#define COMPLETED_WIDTH 96

static SkinColor textcolor("wasabi.statusbar.text");

class CmdButton : public ButtonWnd {
public:
  CmdButton(const char *name, AppCmds *_cmd, int _id) : ButtonWnd(name), cmd(_cmd), id(_id) {}

  virtual void onLeftPush(int x, int y) {
    cmd->appcmds_onCommand(id, &windowRect());
  }

  AppCmds *cmd;
  int id;
};

StatusBar::StatusBar() {
  overtimer = 0;

  completed = 0;
  progress_width = 0;

  bg = new GuiObjectWnd();
  bg->setContent("wasabi.statusbar");

  bbleft = bbright = NULL;
}

StatusBar::~StatusBar() {
  killTimer(STATUS_TIMER_DECAY);
  delete bg;
  delete bbleft;
  delete bbright;
}

int StatusBar::onInit() {
  STATUSBAR_PARENT::onInit();

  bg->init(this);

  getGuiObject()->guiobject_registerStatusCB(this); // watched

  regenerate();

  return 1;
}

void StatusBar::timerCallback(int id) {
  switch (id) {
    case STATUS_TIMER_DECAY: {
      killTimer(STATUS_TIMER_DECAY);
      onSetStatusText(status_text, FALSE);	// revert to main text
    }
    break;
    default:
      STATUSBAR_PARENT::timerCallback(id);
      break;
  }
}

void StatusBar::pushCompleted(int _max) {
  max = MAX(_max, 0);
  completed = 0;

  GuiObject *outer = bg->findObject("wasabi.statusbar.progress.outline");
  outer->guiobject_setXmlParam("visible", "0");
  ASSERT(outer != NULL);
  RootWnd *outerw = outer->guiobject_getRootWnd();
  RECT cr;
  outerw->getClientRect(&cr);
  progress_width = cr.right - cr.left;

  outerw->setVisible(TRUE);//CUT
  outer->guiobject_setTargetA(255);
  outer->guiobject_setTargetSpeed(0.1f);
  outer->guiobject_gotoTarget();

  GuiObject *inner = bg->findObject("wasabi.statusbar.progress.inside");
  inner->guiobject_setTargetA(255);
  inner->guiobject_setTargetSpeed(1.0f);
  inner->guiobject_gotoTarget();
  inner->guiobject_setXmlParam("visible", "0");

  incCompleted(0);
}

void StatusBar::incCompleted(int add) {
  setCompleted(completed + add);
}

void StatusBar::setCompleted(int _completed) {
  completed = _completed;
  GuiObject *inner = bg->findObject("wasabi.statusbar.progress.inside");
  ASSERT(inner != NULL);
  if (!inner->guiobject_getRootWnd()->isVisible(1)) {
    inner->guiobject_setXmlParam("visible", "1");
    inner->guiobject_setTargetA(255);
    inner->guiobject_setTargetSpeed(0.75);
    inner->guiobject_gotoTarget();
  }
  int pos = (int)(((float)completed / (float)max)*(float)progress_width);
  inner->guiobject_setXmlParam("w", StringPrintf("%d", pos));
}

void StatusBar::popCompleted() {
  completed = 0;
  max = 0;
  GuiObject *inner = bg->findObject("wasabi.statusbar.progress.inside");
  inner->guiobject_setXmlParam("w", "0");
  inner->guiobject_setTargetA(0);
  inner->guiobject_setTargetSpeed(0.75);
  inner->guiobject_gotoTarget();

//CUT later
  inner->guiobject_setXmlParam("visible", "0");
  GuiObject *outer = bg->findObject("wasabi.statusbar.progress.outline");
  outer->guiobject_setXmlParam("visible", "0");
}

int StatusBar::onResize() {
  STATUSBAR_PARENT::onResize();

  RECT cr = clientRect();

  bbleft->resize(cr.left, cr.top, bbleft->getWidth(), cr.bottom - cr.top);

  bbright->resize(cr.right-bbright->getWidth(), cr.top, bbright->getWidth(), cr.bottom - cr.top);

  cr.left += bbleft->getWidth();
  cr.right -= bbright->getWidth();

  bg->resizeToRect(&cr);	// put bg group in place

  invalidate();
  return 1;
}

void StatusBar::onSetStatusText(const char *text, int overlay) {
  killTimer(STATUS_TIMER_DECAY);
  if (!overlay) status_text = text;
  else setTimer(STATUS_TIMER_DECAY, 4000);
  ScriptObject *tx = bg->findScriptObject("wasabi.statusbar.text");
  if (tx == NULL) return;
  C_Text(tx).setText(text ? text : "");
}

void StatusBar::onAddAppCmds(AppCmds *commands) {
  if (appcmds.haveItem(commands)) appcmds.removeItem(commands);
  appcmds.addItem(commands);
  regenerate();
}

void StatusBar::onRemoveAppCmds(AppCmds *commands) {
  if (appcmds.haveItem(commands)) {
    appcmds.removeItem(commands);
    regenerate();
  }
}

void StatusBar::regenerate() {
  if (!isInited()) return;

  delete bbleft; bbleft = new ButtBar;
  delete bbright; bbright = new ButtBar;
  bbleft->init(this);
  bbright->init(this);

  ParamParser exclude(exclude_list, ";");
  ParamParser showonly(include_only, ";");

  foreach(appcmds)
    int n = appcmds.getfor()->appcmds_getNumCmds();
    for (int i = 0; i < n; i++) {
      int side, id;
      const char *name = appcmds.getfor()->appcmds_enumCmd(i, &side, &id);
      if (name == NULL) break;
      if (exclude.hasString(name)) continue;	// exclusion list
      if (showonly.getNumItems()) {
        if (!showonly.hasString(name)) continue;	// include-only list
      }
      CmdButton *cb = new CmdButton(name, appcmds.getfor(), id);
      if (side == AppCmds::SIDE_LEFT) bbleft->addChild(cb);
      else bbright->addChild(cb);
    }
  endfor
  if (isPostOnInit())
    onResize();
}

void StatusBar::fakeButtonPush(const char *name) {
  if (!fakeButtonPush(bbleft, name))
    fakeButtonPush(bbright, name);
}

int StatusBar::fakeButtonPush(ButtBar *bb, const char *name) {
  for (int i = 0; i < bb->getNumChildren(); i++) {
    ButtonWnd *cmdb = bb->enumChild(i);
    if (STREQLSAFE(cmdb->getName(), name)) {
      int x, y;
      Std::getMousePos(&x, &y);
      cmdb->screenToClient(&x, &y);
      cmdb->onLeftPush(x, y);
      return 1;
    }
  }
  return 0;
}

void StatusBar::setExclude(const char *val) {
  exclude_list = val;
  regenerate();
}

void StatusBar::setIncludeOnly(const char *val) {
  include_only = val;
  regenerate();
}
