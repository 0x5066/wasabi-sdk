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

#ifndef _SVC_ACTION_H
#define _SVC_ACTION_H

#include "../../bfc/dispatch.h"
#include "../../bfc/string.h"
#include "../../bfc/ptrlist.h"
#include "services.h"

class RootWnd;

class NOVTABLE svc_action : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::ACTION; }
  
  int hasAction(const char *name);
  int onAction(const char *action, const char *param=NULL, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);

  enum {
    HASACTION=10,
    ONACTION=20,
  };

};

inline int svc_action::hasAction(const char *name) {
  return _call(HASACTION, 0, name);
}

inline int svc_action::onAction(const char *action, const char *param, int p1, int p2, void *data, int datalen, RootWnd *source) {
  return _call(ONACTION, 0, action, param, p1, p2, data, datalen, source);
}

class ActionEntry {
  public:
    ActionEntry(const char *_action, int _id) : action(_action), id(_id) {}
    virtual ~ActionEntry() { }

    const char *getAction() { return action; }
    int getId() { return id; }

  private:
    String action;
    int id;
};

class SortActions {
public:
  static int compareItem(ActionEntry *p1, ActionEntry *p2) {
    return STRICMP(p1->getAction(), p2->getAction());
  }
  static int compareAttrib(const char *attrib, ActionEntry *item) {
    return STRICMP(attrib, item->getAction());
  }
};

class NOVTABLE svc_actionI : public svc_action{
public:
    virtual ~svc_actionI();
    void registerAction(const char *actionid, int pvtid);
    virtual int onActionId(int pvtid, const char *action, const char *param=NULL, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL)=0;

protected:
  virtual int hasAction(const char *name);
  virtual int onAction(const char *action, const char *param=NULL, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);

  PtrListQuickSorted<ActionEntry, SortActions> actions;

  RECVS_DISPATCH;
};
#include "servicei.h"
template <class T>
class ActionCreator : public waServiceFactoryT<svc_action, T> {};

#include "../../bfc/svc_enum.h"
#include "../../bfc/string.h"

class ActionEnum : public SvcEnumT<svc_action> {
public:
  ActionEnum(const char *_action) : action(_action) {}
protected:
  virtual int testService(svc_action *svc) {
    return (svc->hasAction(action));
  }
private:
  String action;
};

class FireAction {
public:
  FireAction(const char *action, const char *param=NULL, int apply_to_all=TRUE) {
    ActionEnum ae(action);
    svc_action *act;
    while ((act = ae.getNext()) != NULL) {
      act->onAction(action, param);
      ae.release(act);
      if (!apply_to_all) break;
    }
  }
};

#endif
