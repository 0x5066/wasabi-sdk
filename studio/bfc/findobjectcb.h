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

#ifndef __FINDOBJECTCALLBACK_H
#define __FINDOBJECTCALLBACK_H

#include "dispatch.h"
#include "../common/common.h"

class RootWnd;

class COMEXP FindObjectCallback : public Dispatchable {	
  
  public:
  
    int findobjectcb_matchObject(RootWnd *object);
  
  enum {
    FINDOBJECTCB_MATCHOBJECT = 0,
  };

};

inline int FindObjectCallback::findobjectcb_matchObject(RootWnd *object) {
  return _call(FINDOBJECTCB_MATCHOBJECT, 0, object);
}


class COMEXP _FindObjectCallback : public FindObjectCallback {
  public:
    virtual int findobjectcb_matchObject(RootWnd *object)=0;

  protected:
    RECVS_DISPATCH;
};


class COMEXP FindObjectCallbackI : public _FindObjectCallback {
  public:
    
    FindObjectCallbackI() {}
    virtual ~FindObjectCallbackI() {}

    virtual int findobjectcb_matchObject(RootWnd *object)=0;
};

#endif