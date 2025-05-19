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

#ifndef __ROOTOBJCB_H
#define __ROOTOBJCB_H

#include "../../bfc/dispatch.h"
#include "../common.h"


/*  RootObjectCallback is the dispatchable class being called by RootObject script events (onNotify).
    You should not inherit directly from it, but rather from ScriptRootObject.               */

class COMEXP NOVTABLE RootObjectCallback : public Dispatchable {
  public:
    void rootobjectcb_onNotify(const char *a, const char *b, int c, int d);
    enum {
      ROOT_ONNOTIFY=10,
    };
};

inline void RootObjectCallback::rootobjectcb_onNotify(const char *a, const char *b, int c, int d) {
  _voidcall(ROOT_ONNOTIFY, a, b, c, d);
}

class COMEXP NOVTABLE RootObjectCallbackI : public RootObjectCallback {
  public:
    virtual void rootobjectcb_onNotify(const char *a, const char *b, int c, int d)=0;
  protected:
    RECVS_DISPATCH;
};


#endif
