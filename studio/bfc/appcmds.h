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

#ifndef _APPCMDS_H
#define _APPCMDS_H

#include "dispatch.h"
#include "depend.h"
#include "../common/common.h"
#include "string.h"
#include "ptrlist.h"

class DragItem;

// this will be fully dispatched later on
class COMEXP AppCmds : public Dispatchable {
public:

  int appcmds_getNumCmds();

  const char *appcmds_enumCmd(int n, int *side, int *id);

  enum {
    SIDE_LEFT=0, SIDE_RIGHT=1
  };

  void appcmds_onCommand(int id, const RECT *buttonRect);//onscreen coords

  enum {
    APPCMDS_GETNUMCMDS=100,
    APPCMDS_ENUMCMD=200,
    APPCMDS_ONCOMMAND=300,
  };
};

inline int AppCmds::appcmds_getNumCmds() {
  return _call(APPCMDS_GETNUMCMDS, 0);
}

inline const char *AppCmds::appcmds_enumCmd(int n, int *side, int *id) {
  return _call(APPCMDS_ENUMCMD, (const char *)NULL, n, side, id);
}

inline void AppCmds::appcmds_onCommand(int id, const RECT *buttonRect) {
  _voidcall(APPCMDS_ONCOMMAND, id, buttonRect);
}

class CmdRec {
public:
  CmdRec(const char *name, int _id, int _side, int _autodelete=0) : cmdname(name), id(_id), side(_side), autodelete(_autodelete) {}
  virtual ~CmdRec() {}
  String cmdname;
  int id, side;
  int autodelete;

  virtual void onCommand(const RECT *buttonRect) {}
};

class COMEXP AppCmdsI : public AppCmds {
public:
  AppCmdsI() { }
  virtual ~AppCmdsI();

protected:
  void appcmds_addCmd(CmdRec *cmdrec);
  void appcmds_addCmd(const char *name, int id, int side=SIDE_LEFT);
  void appcmds_deleteAll();//calls delete on each one

public:
  virtual int appcmds_getNumCmds();
  virtual const char *appcmds_enumCmd(int n, int *side, int *id);

  // override this and catch your commands, otherwise it'll call the CmdRec
  virtual void appcmds_onCommand(int id, const RECT *buttonRect);

protected:
  RECVS_DISPATCH;
  PtrList<CmdRec> cmds;
};

#endif
