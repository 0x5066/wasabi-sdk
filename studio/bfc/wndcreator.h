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

#ifndef _WNDCREATOR_H
#define _WNDCREATOR_H

#include "../studio/services/svc_wndcreate.h"
#include "../common/bucketitem.h"
#include "../bfc/wnds/buttwnd.h"

template <class T, bool allow_multiple=true>
class CreateWndByGuid : public svc_windowCreateI {
public:
  CreateWndByGuid() : issued(0) { }
  virtual ~CreateWndByGuid() {
    ASSERTPR(issued == 0, "not all created windows were destroyed");
  }
  static const char *getServiceName() { return T::getWindowTypeName(); }
  virtual int testGuid(GUID g) { return (g == T::getWindowTypeGuid()); }
  virtual RootWnd *createWindowByGuid(GUID g, RootWnd *parent) {
    if (!testGuid(g)) return NULL;
    if (!allow_multiple && issued > 0) return NULL;
    T *ret = new T;
    issued++;
    return ret;
  }
  virtual int destroyWindow(RootWnd *w) {
    ASSERT(issued >= 1);
    T *wnd = static_cast<T*>(w);
    delete wnd;
    issued--;
    return 1;
  }
  virtual int refcount() {
    return issued;
  }
private:
  int issued;
};

template <class T, bool allow_multiple=true>
class CreateWndByType : public svc_windowCreateI {
public:
  CreateWndByType(const char *wtype) : issued(0), type(wtype) { }
  virtual ~CreateWndByType() {
    ASSERTPR(issued == 0, "not all created windows were destroyed");
  }
  static const char *getServiceName() { return T::getWindowTypeName(); }
  virtual int testType(const char *wtype) { return STREQLSAFE(wtype, type); }
  virtual RootWnd *createWindowOfType(const char *wtype, RootWnd *parent, int n) {
    if (n == 0 && testType(wtype)) {
      T *ret = new T;
      issued++;
      return ret;
    }
    return NULL;
  }
  virtual int destroyWindow(RootWnd *w) {
    ASSERT(issued >= 1);
    T *wnd = static_cast<T*>(w);
    delete wnd;
    issued--;
    return 1;
  }
  virtual int refcount() {
    return issued;
  }
private:
  int issued;
  String type;
};

template <class T, class BUCKETCLASS=BucketItem>
class CreateBucketItem : public svc_windowCreateI {
public:
  static const char *getServiceName() { return T::getWindowTypeName(); }
  CreateBucketItem() : issued(0) { }
  virtual int testType(const char *windowtype) { return STREQLSAFE(windowtype, "buck"); }
  virtual RootWnd *createWindowOfType(const char *windowtype, RootWnd *parent, int n) {
    if (!testType(windowtype)) return NULL;
    if (n == 0) {
      BUCKETCLASS *bucketitem = new BUCKETCLASS(T::getWindowTypeGuid(), T::getWindowTypeName());
      T::setIconBitmaps(bucketitem);
      issued++;
      return bucketitem;
    }
    return NULL;
  }
  virtual int destroyWindow(RootWnd *w) {
    BUCKETCLASS *wnd = static_cast<BUCKETCLASS *>(w);
    delete wnd;
    issued--;
    return 1;
  }
  virtual int refcount() {
    return issued;
  }
private:
  int issued;
};

// exposes a group thru a GUID, use it as a param to CreateWndByGuid() and give it a dummy class
// that implements the static callbacks to get group id, name and guid
template <class T>
class GroupGuidWnd : public GuiObjectWnd {
public:
  GroupGuidWnd() { setContent(T::getWindowGroup()); setName(T::getWindowName()); }
  static const char *getWindowTypeName() { return T::getWindowName(); }
  static GUID getWindowTypeGuid() { return T::getWindowGuid(); }
};


#endif
