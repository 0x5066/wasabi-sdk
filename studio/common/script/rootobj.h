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

#ifndef __ROOTOBJ_H
#define __ROOTOBJ_H

#include "../../bfc/dispatch.h"
#include "rootobjcb.h"

class ScriptObject;
class ScriptObjectI;

class COMEXP RootObject : public Dispatchable {

  public:

  const char *rootobject_getClassName();
  void rootobject_notify(const char *s, const char *t, int u, int v);
  ScriptObject *rootobject_getScriptObject();
  void rootobject_setScriptObject(ScriptObject *o);
  void rootobject_addCB(RootObjectCallback *cb);

  enum {
    ROOT_GETCLASSNAME=10,
    ROOT_SCRIPTNOTIFY=20,
    ROOT_GETSCRIPTOBJECT=30,
    ROOT_SETSCRIPTOBJECT=40,
    ROOT_ADDCB=50,
  };
};

inline const char *RootObject::rootobject_getClassName() {
  return _call(ROOT_GETCLASSNAME, (const char *)NULL);
}

inline void RootObject::rootobject_notify(const char *s, const char *t, int u, int v) {
  _voidcall(ROOT_SCRIPTNOTIFY, s, t, u, v);
}

inline ScriptObject *RootObject::rootobject_getScriptObject() {
  return _call(ROOT_GETSCRIPTOBJECT, (ScriptObject *)NULL);
}

inline void RootObject::rootobject_setScriptObject(ScriptObject *o) {
  _voidcall(ROOT_SETSCRIPTOBJECT, o);
}

inline void RootObject::rootobject_addCB(RootObjectCallback *cb) {
  _voidcall(ROOT_ADDCB, cb);
}

// RootObjectInstance::this == RootObjectInstance::getScriptObject()->vcpu_getInterface(rootObjectInstanceGuid);
// {F6D49468-4036-41a1-9683-C372416AD31B}
static const GUID rootObjectInstanceGuid = 
{ 0xf6d49468, 0x4036, 0x41a1, { 0x96, 0x83, 0xc3, 0x72, 0x41, 0x6a, 0xd3, 0x1b } };

// Instantiate this class to create an object from which you can trap notify events, or inherit from
// it if you want to implement your own descendant of script class 'Object' (see GuiObjectWnd)
class COMEXP RootObjectInstance : public RootObjectCallbackI {
  public:

    RootObjectInstance();
    virtual ~RootObjectInstance();

    virtual RootObject *getRootObject();
    virtual ScriptObject *getScriptObject();

    virtual void rootobjectcb_onNotify(const char *a, const char *b, int c, int d) {};
    virtual const char *getClassName();
    virtual void notify(const char *s, const char *t, int u, int v);

  private:
    ScriptObjectI *my_script_object;
    RootObject *my_root_object;
};


#endif
