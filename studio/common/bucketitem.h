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

#ifndef __BUCKETITEM_H
#define __BUCKETITEM_H

#include "common.h"
#include "wndcb.h"
#include "../bfc/wnds/buttwnd.h"
#include "../bfc/depend.h"
#include "../bfc/notifmsg.h"

template <class T> class BucketItemT : public T {
  public:
    BucketItemT(GUID g=INVALID_GUID, const char *text=NULL) : guid_target(g), target_txt(text) {
        setBorders(0);
        setHInstanceColorGroup("Thinger icons");
    }

    virtual ~BucketItemT() {
    }
   
    virtual void setBucketText(const char *txt) {
      notifyParent(ChildNotify::COMPONENTBUCKET_SETTEXT, reinterpret_cast<int>(txt), 0);
    }
    virtual void onLeftPush(int x, int y) {
      T::onLeftPush(x, y);
      if (guid_target != INVALID_GUID) {
        RECT r;
        getClientRect(&r);
        clientToScreen(&r);
        int newstatus = api->skinwnd_toggleByGuid(guid_target);
        setActivatedButton(newstatus);
      }
    }

    virtual int onShowWindow(GUID g, const char *groupid) {
      if (g == guid_target) setActivatedButton(1);
      return 1;
    }

    virtual int onHideWindow(GUID g, const char *groupid) {
      if (g == guid_target) setActivatedButton(0);
      return 1;
    }

    virtual void onEnterArea() {
      T::onEnterArea();
      if (!target_txt.isempty()) setBucketText(target_txt);
    }

    virtual void onLeaveArea() {
      T::onLeaveArea();
      if (!target_txt.isempty()) setBucketText(NULL);
    }

    void setAutoOpen(GUID g) {
      guid_target = g;
    }

    void setAutoText(const char *txt) {
      target_txt = txt;
    }

  private:
    GUID guid_target;
    String target_txt;
};

#define BUCKETITEM_PARENT ButtonWnd
class BucketItem : public BucketItemT<BUCKETITEM_PARENT> {
  public:
    BucketItem(GUID g=INVALID_GUID, const char *text=NULL) : BucketItemT<ButtonWnd> (g, text) {}
    virtual ~BucketItem() {}
};


#endif // __BUCKETITEM_H

