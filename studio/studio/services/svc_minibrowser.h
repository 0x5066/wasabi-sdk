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

#ifndef _SVC_MINIBROWSER_H
#define _SVC_MINIBROWSER_H

#include "../../bfc/dispatch.h"
#include "services.h"

class MiniBrowser;

#include "../../common/nsguid.h"

// {2E41D2E8-19A5-4029-9339-8FDF7481000A}
static const GUID GUID_MINIBROWSER_ANY = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

// {C0A3D1AC-2430-45a7-B51B-AB04B74DD9EA}
static const GUID GUID_MINIBROWSER_IEACTIVEX = 
{ 0xc0a3d1ac, 0x2430, 0x45a7, { 0xb5, 0x1b, 0xab, 0x4, 0xb7, 0x4d, 0xd9, 0xea } };

class NOVTABLE svc_miniBrowser : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::MINIBROWSER; }

  int testGuid(GUID g);
  MiniBrowser *createMiniBrowser();
  void destroyMiniBrowser(MiniBrowser *b);

  enum {
    TESTGUID            =10,
    CREATEMINIBROWSER   =20,
    DESTROYMINIBROWSER  =30,
  };
};

inline int svc_miniBrowser::testGuid(GUID g) {
  return _call(TESTGUID, 0, g);
}

inline MiniBrowser *svc_miniBrowser::createMiniBrowser() {
  return _call(CREATEMINIBROWSER, (MiniBrowser *)0);
}

inline void svc_miniBrowser::destroyMiniBrowser(MiniBrowser *b) {
  _voidcall(DESTROYMINIBROWSER, b);
}

class NOVTABLE svc_miniBrowserI : public svc_miniBrowser {
public:
  virtual int testGuid(GUID g)=0;
  virtual MiniBrowser *createMiniBrowser()=0;
  virtual void destroyMiniBrowser(MiniBrowser *b)=0;

protected:
  RECVS_DISPATCH;
};

#include "../../bfc/svc_enum.h"

class MiniBrowserSvcEnum : public SvcEnumT<svc_miniBrowser> {
public:
  MiniBrowserSvcEnum(GUID g) : guid(g) {}
protected:
  virtual int testService(svc_miniBrowser *svc) {
    return (svc->testGuid(guid));
  }
private:
  GUID guid;
};

#endif
