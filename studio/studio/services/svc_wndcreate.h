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

#ifndef _SVC_WNDCREATE_H
#define _SVC_WNDCREATE_H

#include "../../bfc/dispatch.h"
#include "services.h"

#define BUCKETITEM "buck"
#define PLAYLISTSIDECAR "plsc"

class RootWnd;

class NOVTABLE svc_windowCreate : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::WINDOWCREATE; }

  int testGuid(GUID g);
  RootWnd *createWindowByGuid(GUID g, RootWnd *parent);
  int testType(const char *windowtype);
  RootWnd *createWindowOfType(const char *windowtype, RootWnd *parent, int n=0);
  int destroyWindow(RootWnd *w);

  int refcount();	// how many windows created

  enum {
    TESTGUID            =100,
    CREATEWINDOWBYGUID  =200,
    TESTTYPE            =301,
    CREATEWINDOWOFTYPE  =402,
    DESTROYWINDOW       =500,
    REFCOUNT		=600,
  };
};

inline int svc_windowCreate::testGuid(GUID g) {
  return _call(TESTGUID, 0, g);
}

inline RootWnd *svc_windowCreate::createWindowByGuid(GUID g, RootWnd *parent) {
  return _call(CREATEWINDOWBYGUID, (RootWnd*)0, g, parent);
}

inline int svc_windowCreate::destroyWindow(RootWnd *w) {
  return _call(DESTROYWINDOW, 0, w);
}

inline
int svc_windowCreate::testType(const char *windowtype) {
  return _call(TESTTYPE, 0, windowtype);
}

inline RootWnd *svc_windowCreate::createWindowOfType(const char * windowtype, RootWnd *parent, int n) {
  return _call(CREATEWINDOWOFTYPE, (RootWnd*)0, windowtype, parent, n);
}

inline int svc_windowCreate::refcount() {
  return _call(REFCOUNT, -1);
}

class NOVTABLE svc_windowCreateI : public svc_windowCreate {
public:
  virtual ~svc_windowCreateI() { }
  virtual int testGuid(GUID g) { return 0; }
  virtual RootWnd *createWindowByGuid(GUID g, RootWnd *parent) { return NULL; }
  virtual int testType(const char *windowtype) { return 0; }
  virtual RootWnd *createWindowOfType(const char *windowtype, RootWnd *parent, int n) { return NULL; }
  virtual int destroyWindow(RootWnd *w)=0;

  virtual int refcount() { return -1; }	// FUCKO: make pure abstract

protected:
  RECVS_DISPATCH;
};

#include "servicei.h"
template <class T>
class WndCreateCreator : public waServiceFactoryT<svc_windowCreate, T> {};

template <class T>
class WndCreateCreatorSingle : public waServiceFactoryTSingle<svc_windowCreate, T> {};

#include "../../bfc/svc_enum.h"
#include "../../bfc/string.h"

class WindowCreateByGuidEnum : public SvcEnumT<svc_windowCreate> {
public:
  WindowCreateByGuidEnum(GUID _g) : targetguid(_g) { }

protected:
  virtual int testService(svc_windowCreate *svc) {
    return svc->testGuid(targetguid);
  }

private:
  GUID targetguid;
};

class WindowCreateByTypeEnum : public SvcEnumT<svc_windowCreate> {
public:
  WindowCreateByTypeEnum(const char *_wtype) : wtype(_wtype) { }

protected:
  virtual int testService(svc_windowCreate *svc) {
    return svc->testType(wtype);
  }

private:
  String wtype;
};
#endif
