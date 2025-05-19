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

#ifndef _CONTEXTCMD_H
#define _CONTEXTCMD_H

#include "common.h"
#include "../studio/services/svc_contextcmd.h"
#include "../studio/services/servicei.h"
#include "dragitemi.h"

class ContextCmdCB {
public:
  virtual ~ContextCmdCB() {}

  virtual void onCommand(DragItem *item)=0;
};

// this calls a method on the exposed objects, once for each subitem
// make sure you're calling a dispatchable or code you own!
template <class T>
class ContextCmdCB_method : public ContextCmdCB {
public:
  typedef void (T::*cbfn)();
  ContextCmdCB_method(cbfn _fn) : fn(_fn) {}

private:
  virtual void onCommand(DragItem *item) {
    int n = item->getNumData();
    for (int i = 0; i < n; i++) {
      T *obj = DragItemCast<T>(item);
      (obj->*fn)();
    }
  }

  cbfn fn;
};

// a helper layer - don't derive from directly
class COMEXP NOVTABLE ContextCmd : public svc_contextCmdI {
public:
  static const char *getServiceName() { return "Context menu entry"; }

protected:
  ContextCmd(const char *classname, const char *menu_path=NULL);
  virtual ~ContextCmd();

public:
  void addCommand(const char *text,
                  ContextCmdCB *callback=NULL,
                  int enabled=1, int checked=0,
                  int sortval=ContextCmdSortVal::MIDDLE);

  // includes cmdid
  void addCommandById(const char *text, int cmdid,
                  ContextCmdCB *callback=NULL,
                  int enabled=1, int checked=0,
                  int sortval=ContextCmdSortVal::MIDDLE);

  void addSeparator(int sortval=ContextCmdSortVal::MIDDLE);
protected:
  virtual void onCommand(DragItem *item, int n);

  // override this one if you want to add commands just in time
  virtual void populateCommands() {}

  // from svc_contextCmdI
  // override this if you need to do more detailed testing of the DragItem
  virtual int testItem(DragItem *item, const char *_menu_path);
  virtual const char *getCommand(DragItem *item, int n);
  virtual int getEnabled(DragItem *item, int n);
  virtual int getChecked(DragItem *item, int n);
  virtual int getSortVal(DragItem *item, int n);

  virtual int getSubMenu(DragItem *item, const char *_menu_path);
  virtual const char *getSubMenuText(const char *_menu_path);

  int getCmdId(int pos);

private:
  String classname, menu_path;

  class ContextCmdEntry;

  PtrList<ContextCmdEntry> commands;
};

// use or derive from this one, and call addCommand as needed
template <class T>
class ContextCmdT : public ContextCmd {
public:
  ContextCmdT(const char *menu_path=NULL) :
    ContextCmd(T::dragitem_getDatatype(), menu_path) {}

protected:
  // you can either catch your commands here or use the callbacks
  // you will be called once for each subitem in the dragitem
  virtual void onCommand(T* item, int n, int cmdid) {}

private:
  virtual void onCommand(DragItem *item, int n) {
    int c = item->getNumData();
    for (int i = 0; i < c; i++) {
      // call us first
      onCommand(DragItemCast<T>(item, i), i, getCmdId(n));
    }
    // then call any callbacks that were set
    ContextCmd::onCommand(item, n);
  }
};

// derive from this class to add a new single context menu command
template <class T>
class ContextCmdSingle : public ContextCmdT<T> {
public:
  ContextCmdSingle(const char *text=NULL, ContextCmdCB *callback=NULL, const char *menu_path=NULL, int enabled=1, int checked=0, int sortval=ContextCmdSortVal::MIDDLE) :
    ContextCmdT<T>(menu_path) {
    if (text != NULL)
      addCommand(text, callback, enabled, checked, sortval);
  }
};

// same as above, but auto-generates the callback to a method
template <class T>
class ContextCmdSingleMethod : public ContextCmdSingle<T> {
public:
  typedef void (T::*cbfn)();
  ContextCmdSingleMethod(const char *text, cbfn fn, const char *menu_path=NULL, int enabled=1, int checked=0, int sortval=ContextCmdSortVal::MIDDLE) :
    ContextCmdSingle<T>(text, new ContextCmdCB_method<T>(fn), menu_path, enabled, checked, sortval) { }
};

#endif
