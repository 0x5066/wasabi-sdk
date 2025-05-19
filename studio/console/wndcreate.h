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

// THIS MODULE GO BYEBYE!
#if 0

#ifndef __WNDCREATE_H
#define __WNDCREATE_H

#include "../studio/services/svc_wndcreate.h"
#include "../bfc/ptrlist.h"

class BucketItem;

class ConsoleWndCreateSvc : public svc_windowCreateI {
  
  public:
    
    static const char *getServiceName() { return "Console Window Creation Service"; }

    int testGuid(GUID g);
    RootWnd *createWindowByGuid(GUID g, RootWnd *parent);
    int testType(FOURCC windowType) { return windowType == WindowTypes::BUCKETITEM; }
    RootWnd *createWindowOfType(FOURCC windowtype, RootWnd *parent, int n);
    int destroyWindow(RootWnd *w);

    PtrList<RootWnd> bucketitem_list;
};

#endif
#endif
