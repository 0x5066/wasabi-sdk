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

#ifndef _RUNLEVELCB_H
#define _RUNLEVELCB_H

#include "syscb.h"

// messages are the same as in services/service.h
#include "services/service.h"

#define RUNLEVELCALLBACKI_PARENT SysCallbackI
class RunlevelCallbackI : public RUNLEVELCALLBACKI_PARENT {
public:
  virtual FOURCC syscb_getEventType() { return SysCallback::RUNLEVEL; }

  // override these
  virtual void runlevelcb_onStartup() {}
  virtual void runlevelcb_onAppRunning() {}
  virtual void runlevelcb_onShutdown() {}

private:
  virtual int syscb_notify(int msg, int param1=0, int param2=0) {
    switch (msg) {
      case SvcNotify::ONSTARTUP:
        runlevelcb_onStartup();
      break;
      case SvcNotify::ONAPPRUNNING:
        runlevelcb_onAppRunning();
      break;
      case SvcNotify::ONSHUTDOWN:
        runlevelcb_onShutdown();
      break;
      default: return 0;
    }
    return 1;
  }
};

#endif
