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

// ------------------------------------------------------------------
// WndCreateSvc.cpp : Window creation service 
// 
//   Provides window creation service for GUIDS
//   Populates component buckets based on Window type
//   Keeps a list of its pointers to delete multiple inheritances if necessary
// ------------------------------------------------------------------

#include "../../bfc/wnds/blankwnd.h"
#include "../../common/bucketitem.h"
#include "ExampleMW.h"
#include "WndCreateSvc.h"
#include "BucketGroup.h"
#include "MyWindow.h"
#include "resource.h"

// Wasabi is asking us if we can create this GUID's window
int ExampleMWWndCreateSvc::testGuid(GUID g) {
  if (g == MyWindow::getGuid()) return 1;
  return 0;
}

// Create window according to GUID, if a window class does not support multiple instances,
// you should keep a pointer to it and always return it for its GUID, otherwise you should
// keep a list of your pointers for later deletion
RootWnd *ExampleMWWndCreateSvc::createWindowByGuid(GUID g, RootWnd *parent) {
  if (g == MyWindow::getGuid()) {
    RootWnd *r = new MyWindow;
    r->setParent(parent);
    window_list.addItem(r);
    return r;
  }
  return NULL; 
}

// Wasabi is asking us if we can create windows of this type
int ExampleMWWndCreateSvc::testType(FOURCC windowtype) {
  return (windowtype == WindowTypes::BUCKETITEM);
}

// Populate a component bucket with a specific type of windows. This function is called
// as long as you do not return NULL, with n incremented for each pass.
RootWnd *ExampleMWWndCreateSvc::createWindowOfType(FOURCC windowtype, RootWnd *parent, int n) {
  switch (windowtype) {
    case WindowTypes::BUCKETITEM: {  
      switch (n) {
        case 0: {
          BucketItem *bucketitem = new BucketItem;
          bucketitem->setAutoOpen(examplemwwac->getGUID());
          bucketitem->setAutoText("Toggle Single Instance Window");
          bucketitem->setBitmaps(examplemwwac->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
          bucketitem_list.addItem(bucketitem);
          return bucketitem;
        }
        case 1: {
          BucketGroup *bucketgroup = new BucketGroup;
          bucketitem_list.addItem(bucketgroup);
          return bucketgroup;
        }
      }
    }
  }
  return NULL;
}

// Walk our lists and destroy our windows cleanly
int ExampleMWWndCreateSvc::destroyWindow(RootWnd *w) {
  if (bucketitem_list.haveItem(w)) { bucketitem_list.removeItem(w); delete static_cast<BucketItem *>(w); }
  else if (window_list.haveItem(w)) { window_list.removeItem(w); delete static_cast<MyWindow*>(w); }
  else return 0;
  return 1;
}

