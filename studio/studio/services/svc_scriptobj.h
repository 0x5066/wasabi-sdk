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

#ifndef _SVC_SCRIPTOBJECT_H
#define _SVC_SCRIPTOBJECT_H

#include "../../bfc/dispatch.h"
#include "services.h"

class ScriptObjectController;

class svc_scriptObject : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::SCRIPTOBJECT; }
  ScriptObjectController *getController(int n);
  void onRegisterClasses(ScriptObjectController *rootController);

  enum {
    GETCONTROLLER=10,
    ONREGISTER=20,
  };
};

inline ScriptObjectController *svc_scriptObject::getController(int n) {
  return _call(GETCONTROLLER, (ScriptObjectController *)0, n);
}

inline void svc_scriptObject::onRegisterClasses(ScriptObjectController *rootController) {
  _voidcall(ONREGISTER, rootController);
}

// derive from this one
class svc_scriptObjectI : public svc_scriptObject {
public:
  virtual ScriptObjectController *getController(int n)=0;
  virtual void onRegisterClasses(ScriptObjectController *rootController) {};

protected:
  RECVS_DISPATCH;
};

#include "servicei.h"
template <class T>
class ScriptObjectCreator : public waServiceFactoryT<svc_scriptObject, T> {};

#include "../../bfc/svc_enum.h"

class ExternalScriptObjectEnum : public SvcEnumT<svc_scriptObject> {
public:
  ExternalScriptObjectEnum() { }

protected:
  virtual int testService(svc_scriptObject*svc) {
    return (svc->getController(0) != NULL);
  }
};

#endif
