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

//-----------------------------------------------------------------------------------------
// BucketGroup.cpp : implements a bucket item that hold a dynamically instantiated group
//-----------------------------------------------------------------------------------------

#include "../../studio/api.h"
#include "BucketGroup.h"

BucketGroup::BucketGroup() {
  group = NULL;
}

BucketGroup::~BucketGroup() {
  // we're done with the group, ask the system to destroy it
  api->group_destroy(group);
}

BucketGroup::getPreferences(int what) {
  // Override BucketItem's size to a default Vis object size
  switch (what) {
    case SUGGESTED_W: return 76;
    case SUGGESTED_H: return 16;
  } 
  return BUCKETGROUP_PARENT::getPreferences(what);
}

// Upon initialization (when the component bucket item initializes), instantiate the group
// and display it in our client area

int BucketGroup::onInit() {
  int r = BUCKETGROUP_PARENT::onInit();

  // this has to match XML, instantiates the group and returns a RootWnd* for you to parent/init

  group = api->group_create("ExampleMWGroupId"); 
  if (!group) return 0;
  group->setParent(this);
  group->init(this);
  return r;
}

// link window events

void BucketGroup::onSetVisible(int show) {
  if (group) group->setVisible(show);
  BUCKETGROUP_PARENT::onSetVisible(show);
}

int BucketGroup::onResize() {
  if (group) {
    RECT r = clientRect();
    group->resize(r.left, r.top, r.right-r.left, r.bottom-r.top);
  }
  return BUCKETGROUP_PARENT::onResize();
}


