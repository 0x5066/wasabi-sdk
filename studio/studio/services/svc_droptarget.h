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

#ifndef _SVC_DROPTARGET_H
#define _SVC_DROPTARGET_H

#include "../../bfc/dispatch.h"
#include "services.h"

class DragInterface;	// see bfc/drag.h

class svc_dropTarget : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::DROPTARGET; }

  int testTarget(FOURCC type);

  DragInterface *getDragInterfaceForType(FOURCC type);
  int releaseDragInterface(DragInterface *di);

protected:
  enum {
    TESTTARGET=100,
    GETDRAGINTERFACEFORTYPE=200,
    RELEASEDRAGINTERFACE=210,
  };
};

inline
int svc_dropTarget::testTarget(FOURCC type) {
  return _call(TESTTARGET, 0, type);
}

inline
DragInterface *svc_dropTarget::getDragInterfaceForType(FOURCC type) {
  return _call(GETDRAGINTERFACEFORTYPE, (DragInterface*)NULL, type);
}

inline
int svc_dropTarget::releaseDragInterface(DragInterface *di) {
  return _call(RELEASEDRAGINTERFACE, 0, di);
}

class svc_dropTargetI : public svc_dropTarget {
public:
  virtual int testTarget(FOURCC type)=0;

  virtual DragInterface *getDragInterfaceForType(FOURCC type)=0;
  virtual int releaseDragInterface(DragInterface *di)=0;

protected:
  RECVS_DISPATCH;
};

#include "servicei.h"

template <class T>
class DropTargetCreator : public waServiceFactoryTSingle<svc_dropTarget, T> { };

#include "../../bfc/svc_enum.h"
#include "../../bfc/drag.h"

class DropTargetEnum : public SvcEnumT<svc_dropTarget> {
public:
  DropTargetEnum(FOURCC type) : dt_type(type) {}
  static int throwDrop(FOURCC type, RootWnd *sourceWnd, int x=0, int y=0) {
    DropTargetEnum dte(type);
    svc_dropTarget *sdt = dte.getFirst();
    if (sdt == NULL) return 0;
    DragInterface *di = sdt->getDragInterfaceForType(type);
    int r = 0;
    if (di != NULL) r = di->dragDrop(sourceWnd, 0, 0);
    sdt->releaseDragInterface(di);
    return r;
  }
protected:
  virtual int testService(svc_dropTarget *svc) {
    return (svc->testTarget(dt_type));
  }
private:
  FOURCC dt_type;
};

#endif
