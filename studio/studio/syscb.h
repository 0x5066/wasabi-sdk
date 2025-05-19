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

#ifndef _SYSCB_H
#define _SYSCB_H

#include "../bfc/dispatch.h"
#include "../common/common.h"

class COMEXP SysCallback : public Dispatchable {
public:
  FOURCC getEventType() { return _call(GETEVENTTYPE, 0); }
  int notify(int msg, int param1=0, int param2=0) {
    return _call(NOTIFY, 0, msg, param1, param2);
  }

  // dispatchable codes
  enum {
    GETEVENTTYPE=101,
    NOTIFY=200,
    // skincb uses 10000-11000
  };

  enum {	// event types
    NONE = 0,
    RUNLEVEL = 'runl',	// system runlevel
    CONSOLE = 'con',	// debug messages
    SKINCB = 'skin',	// skin unloading/loading
    DB = 'db',		// database change messages
    WINDOW = 'wnd',	// window events
    GC = 'gc',		// garbage collection event
    POPUPEXIT = 'popx', // popup exit 
    CMDLINE = 'cmdl',	// command line sent (possibly from outside)
  };
};

//derive from this one (see skincb.h for a good example)
class COMEXP SysCallbackI : public SysCallback {
protected:
  virtual FOURCC syscb_getEventType()=0;

  virtual int syscb_notify(int msg, int param1=0, int param2=0)=0;

  RECVS_DISPATCH;
};

#endif
