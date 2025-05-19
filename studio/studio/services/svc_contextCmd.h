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

#ifndef _SVC_CONTEXTCMD_H
#define _SVC_CONTEXTCMD_H

#include "../../bfc/dispatch.h"
#include "services.h"

namespace ContextCmdSortVal {
  enum {
    BEGINNING = 0,
    MIDDLE = 32767,
    END = 65535,
  };
};

class DragItem;

class NOVTABLE svc_contextCmd : public Dispatchable {
protected:
  svc_contextCmd() {}
  ~svc_contextCmd() {}
public:
  static FOURCC getServiceType() { return WaSvc::CONTEXTCMD; }

  int testItem(DragItem *item, const char *menu_path);

  int getSubMenu(DragItem *item, const char *menu_path);
  const char *getSubMenuText(const char *menu_path);

  const char *getCommand(DragItem *item, int n);

  int getEnabled(DragItem *item, int n);
  int getChecked(DragItem *item, int n);
  int getSortVal(DragItem *item, int n);

  void onCommand(DragItem *item, int n);

protected:
  enum {
    TESTITEM,
    GETSUBMENU,
    GETSUBMENUTEXT,
    GETCOMMAND,
    GETENABLED,
    GETCHECKED,
    GETSORTVAL,
    ONCOMMAND,
  };
};

inline int svc_contextCmd::testItem(DragItem *item, const char *menu_path) {
  return _call(TESTITEM, 0, item, menu_path);
}

inline
int svc_contextCmd::getSubMenu(DragItem *item, const char *menu_path) {
  return _call(GETSUBMENU, 0, item, menu_path);
}

inline
const char *svc_contextCmd::getSubMenuText(const char *menu_path) {
  return _call(GETSUBMENUTEXT, (const char *)NULL, menu_path);
}

inline const char *svc_contextCmd::getCommand(DragItem *item, int n) {
  return _call(GETCOMMAND, (const char *)0, item, n);
}

inline int svc_contextCmd::getEnabled(DragItem *item, int n) {
  return _call(GETENABLED, TRUE, item, n);
}

inline int svc_contextCmd::getChecked(DragItem *item, int n) {
  return _call(GETCHECKED, FALSE, item, n);
}

inline int svc_contextCmd::getSortVal(DragItem *item, int n) {
  return _call(GETSORTVAL, ContextCmdSortVal::MIDDLE, item, n);
}

inline void svc_contextCmd::onCommand(DragItem *item, int n) {
  _voidcall(ONCOMMAND, item, n);
}

class NOVTABLE svc_contextCmdI : public svc_contextCmd {
public:
  virtual int testItem(DragItem *item, const char *menu_path)=0;

  virtual int getSubMenu(DragItem *item, const char *menu_path) { return 0; }
  virtual const char *getSubMenuText(const char *menu_path) { return NULL; }

  virtual const char *getCommand(DragItem *item, int n)=0;

  // override these as needed
  virtual int getEnabled(DragItem *item, int n) { return TRUE; }
  virtual int getChecked(DragItem *item, int n) { return FALSE; }
  virtual int getSortVal(DragItem *item, int n) { return ContextCmdSortVal::MIDDLE; }

  virtual void onCommand(DragItem *item, int n)=0;

protected:
  RECVS_DISPATCH;
};

#include "servicei.h"

template <class T>
class ContextCmdCreator : public waServiceFactoryT<svc_contextCmd, T> { };

#include "../../bfc/svc_enum.h"
#include "../../bfc/string.h"

class ContextCmdEnum : public SvcEnumT<svc_contextCmd> {
public:
  ContextCmdEnum(DragItem *_item, const char *_menu_path)
    : item(_item), menu_path(_menu_path) {}

protected:
  virtual int testService(svc_contextCmd *svc) {
    return svc->testItem(item, menu_path);
  }

private:
  DragItem *item;
  String menu_path;
};

#endif
