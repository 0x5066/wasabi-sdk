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

#ifndef __CUSTOMOBJECT_H
#define __CUSTOMOBJECT_H

#include "../dispatch.h"
#include "../../common/common.h"

class RootWnd;

// {F5527A4F-C910-48c2-A80B-98A60D317F35}
const GUID customObjectGuid = 
{ 0xf5527a4f, 0xc910, 0x48c2, { 0xa8, 0xb, 0x98, 0xa6, 0xd, 0x31, 0x7f, 0x35 } };


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP CustomObject : public Dispatchable {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void customobject_setRootWnd(RootWnd *w);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  RootWnd *customobject_getRootWnd();

  enum {
    CUSTOMOBJECT_SETROOTWND=10,
    CUSTOMOBJECT_GETROOTWND=20,
  };
};

inline void CustomObject::customobject_setRootWnd(RootWnd *w) {
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  _voidcall(CUSTOMOBJECT_SETROOTWND, w);
}

inline
RootWnd *CustomObject::customobject_getRootWnd() {
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  return _call(CUSTOMOBJECT_GETROOTWND, (RootWnd *)NULL);
}


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP CustomObjectI : public CustomObject {
public:

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void customobject_setRootWnd(RootWnd *w)=0;
  virtual RootWnd *customobject_getRootWnd()=0;

protected:
  RECVS_DISPATCH;
};


#endif
