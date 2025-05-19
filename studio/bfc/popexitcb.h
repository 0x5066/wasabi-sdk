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

#ifndef _POPUPCB_H
#define _POPUPCB_H

#include "../common/common.h"
#include "dispatch.h"

class Dependent;

class COMEXP PopupExitCallback : public Dispatchable {
  public:

  int popupexitcb_onExitPopup();
  Dependent *popupexit_getDependencyPtr();

  enum {
    POPUPEXIT_ONEXITPOPUP=100,	
    POPUPEXIT_GETDEPENDENCYPTR=110,	
  };
};

inline int PopupExitCallback::popupexitcb_onExitPopup() {
  return _call(POPUPEXIT_ONEXITPOPUP, 0);
}

inline Dependent *PopupExitCallback::popupexit_getDependencyPtr() {
  return _call(POPUPEXIT_GETDEPENDENCYPTR, (Dependent *)NULL);
}

class COMEXP PopupExitCallbackI : public PopupExitCallback {
  public:

    virtual int popupexitcb_onExitPopup()=0;
    virtual Dependent *popupexit_getDependencyPtr()=0;
  
  protected:

    RECVS_DISPATCH;
};

#endif
