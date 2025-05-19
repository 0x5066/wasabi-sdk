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

#ifndef _SKINCB_H
#define _SKINCB_H

#include "syscb.h"

namespace SkinCallback {
  enum {
    UNLOADING=100,	// beginning, haven't killed anything yet
    RESET=200,		// skin is gone
    RELOAD=300,		// stuff is loading
    GUILOADED=400,	// skin gui objects loaded from xml
    LOADED=500,		// all done, new skin in place
    CHECKPREVENTSWITCH=600,		// we're about to switch skin, wanna abort ? return 1 if so
  };
};

#define SKINCALLBACKI_PARENT SysCallbackI
class SkinCallbackI : public SKINCALLBACKI_PARENT {
public:
  virtual FOURCC syscb_getEventType() { return SysCallback::SKINCB; }

protected:
  // override these
  virtual int skincb_onUnloading() { return 0; }
  virtual int skincb_onReset() { return 0; }
  virtual int skincb_onReload() { return 0; }
  virtual int skincb_onGuiLoaded() { return 0; }
  virtual int skincb_onLoaded() { return 0; }
  virtual int skincb_onCheckPreventSwitch(const char *skinname) { return 0; }

private:
  virtual int syscb_notify(int msg, int param1=0, int param2=0);
};

#endif
