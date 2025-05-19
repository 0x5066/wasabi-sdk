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

#include "wndcreate.h"
#include "console.h"
#include "../common/bucketitem.h"
#include "resource.h"

int ConsoleWndCreateSvc::testGuid(GUID g) {
  if (!MEMCMP(&g, &consolewac->getGUID(), sizeof(GUID))) return 1;
  return 0;
}

RootWnd *ConsoleWndCreateSvc::createWindowByGuid(GUID g, RootWnd *parent) {
  if (!MEMCMP(&g, &consolewac->getGUID(), sizeof(GUID))) {
    return consolewac->createWindow(0, parent);
  }
  return NULL; 
}

RootWnd *ConsoleWndCreateSvc::createWindowOfType(FOURCC windowtype, RootWnd *parent, int n) {
  switch (windowtype) {
    case WindowTypes::BUCKETITEM: {  
      if (n > 0) return NULL;
        BucketItem *bucketitem = new BucketItem;
        bucketitem->setAutoOpen(consolewac->getGUID());
        bucketitem->setAutoText(consolewac->getName());
        bucketitem->setBitmaps(consolewac->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
        bucketitem_list.addItem(bucketitem);
      return bucketitem;
    }
  }
  return NULL;
}

int ConsoleWndCreateSvc::destroyWindow(RootWnd *w) {
  if (bucketitem_list.haveItem(w)) { bucketitem_list.removeItem(w); delete static_cast<BucketItem *>(w); }
  return 1;
}

#endif