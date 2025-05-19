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

#ifndef _SVCWNDHOLD_H
#define _SVCWNDHOLD_H

#include "rootwndholder.h"
#include "../../common/common.h"

class svc_windowCreate;

// for some reason if this derives from virtualwnd typesheet won't show it
#define SERVICEWNDHOLDER_PARENT RootWndHolder

/**
  class ServiceWndHolder .

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ServiceWndHolder : public SERVICEWNDHOLDER_PARENT {
public:
  /**
    ServiceWndHolder constructor .

    @param _child A pointer to the child we want to set.
    @param _svc A pointer to the window creation service associated with the window we want to set as a child.
  */
  ServiceWndHolder(RootWnd *child=NULL, svc_windowCreate *svc=NULL);
  
  /**
    ServiceWndHolder destructor
  */
  virtual ~ServiceWndHolder();

  /**
    ServiceWndHolder method setChild .

    @ret 1
    @param _child A pointer to the child we want to set.
    @param _svc A pointer to the window creation service associated with the window we want to set as a child.
  */
  int setChild(RootWnd *child, svc_windowCreate *svc);

  virtual RootWnd *rootwndholder_getRootWnd();

private:
  RootWnd *child;
  svc_windowCreate *svc;
};

#endif
