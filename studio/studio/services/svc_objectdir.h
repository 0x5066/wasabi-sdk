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

#ifndef _SVC_OBJECTDIR_H
#define _SVC_OBJECTDIR_H

#include "../../bfc/dispatch.h"
#include "../../bfc/depend.h"
#include "../../bfc/string.h"
#include "services.h"
#include "../runlevelcb.h"

typedef unsigned long ObjectHandle;
#define INVALID_OBJECT_HANDLE ((ObjectHandle)0)

#define DD_OBJECTDIR "service:svc_objectDir"

class RootWnd;

class svc_objectDir : public Dispatchable {
public:
  static int getServiceType() { return WaSvc::OBJECTDIR; }
  static const char *dragitem_getDatatype() { return DD_OBJECTDIR; }
  static const GUID *depend_getClassGuid() {
    // {2364D110-0F12-40d4-BBAE-D2DA174751B5}
    static const GUID ret = 
    { 0x2364d110, 0xf12, 0x40d4, { 0xbb, 0xae, 0xd2, 0xda, 0x17, 0x47, 0x51, 0xb5 } };
    return &ret;
  }

  Dependent *getDependencyPtr();

  const char *getDirType();

  int getNumObjects();
  ObjectHandle enumObject(int n);

  void *getObject(ObjectHandle handle);

  const char *getObjectLabel(ObjectHandle handle);
  int setObjectLabel(ObjectHandle handle, const char *newlabel);

  ObjectHandle insertObject(const char *filename=NULL, const char *label=NULL, const char *path=NULL);
  int removeObject(ObjectHandle handle);

  void clearAll();

  int onAction(int action, RootWnd *from, const char *target, ObjectHandle handle);
  enum {
    ODACTION_SELECTED=10,
    ODACTION_DESELECTED=20,
  };

  // dependency events
  enum {
    Event_OBJECT_ADDED=100,
    Event_OBJECT_REMOVED=110,
    Event_OBJECT_LABELCHANGE=200,
  };

  // dispatchable codes
  enum {
    GETDEPENDENCYPTR=100,
    GETNUMOBJECTS=200,
    ENUMOBJECT=300,
    GETOBJECT=400,
    GETOBJECTLABEL=500,
    SETOBJECTLABEL=510,
    INSERTOBJECT=600,
    REMOVEOBJECT=610,
    CLEARALL=700,
    GETDIRTYPE=800,
    ONACTION=900,
  };
};

inline
Dependent *svc_objectDir::getDependencyPtr() {
  return _call(GETDEPENDENCYPTR, (Dependent*)NULL);
}

inline
const char *svc_objectDir::getDirType() {
  return _call(GETDIRTYPE, (const char *)NULL);
}

inline
int svc_objectDir::getNumObjects() {
  return _call(GETNUMOBJECTS, 0);
}

inline
ObjectHandle svc_objectDir::enumObject(int n) {
  return _call(ENUMOBJECT, INVALID_OBJECT_HANDLE, n);
}

inline
void *svc_objectDir::getObject(ObjectHandle handle) {
  return _call(GETOBJECT, (void*)NULL, handle);
}

inline
const char *svc_objectDir::getObjectLabel(ObjectHandle handle) {
  return _call(GETOBJECTLABEL, (const char *)NULL, handle);
}

inline
int svc_objectDir::setObjectLabel(ObjectHandle handle, const char *newlabel) {
  return _call(SETOBJECTLABEL, 0, handle, newlabel);
}

inline
ObjectHandle svc_objectDir::insertObject(const char *filename, const char *label, const char *path) {
  return _call(INSERTOBJECT, INVALID_OBJECT_HANDLE, filename, label, path);
}

inline
int svc_objectDir::removeObject(ObjectHandle handle) {
  return _call(REMOVEOBJECT, 0, handle);
}

inline
void svc_objectDir::clearAll() {
  _voidcall(CLEARALL);
}

inline
int svc_objectDir::onAction(int action, RootWnd *from, const char *target, ObjectHandle handle) {
  return _call(ONACTION, 0, action, from, target, handle);
}



// derive from this if you want to fully implement the interface yourself
class svc_objectDirI : public svc_objectDir, public DependentI {
public:
  virtual Dependent *getDependencyPtr() { return this; }

  virtual const char *getDirType()=0;

  virtual int getNumObjects()=0;
  virtual ObjectHandle enumObject(int n)=0;

  virtual void *getObject(ObjectHandle handle)=0;

  virtual const char *getObjectLabel(ObjectHandle handle)=0;
  virtual int setObjectLabel(ObjectHandle handle, const char *newlabel)=0;

  virtual ObjectHandle insertObject(const char *filename=NULL, const char *label=NULL, const char *path=NULL)=0;
  virtual int removeObject(ObjectHandle handle)=0;

  virtual void clearAll()=0;

  virtual int onAction(int action, RootWnd *from, const char *target, ObjectHandle handle)=0;

protected:
  RECVS_DISPATCH;
};

#include "servicei.h"

template <class T>
class ObjectDirCreator : public waServiceFactoryTSingle<svc_objectDir, T> { };

#include "../../bfc/svc_enum.h"

class ObjectDirEnum : public SvcEnumT<svc_objectDir> {
public:
  ObjectDirEnum(const char *_name) : name(_name) {}
  virtual int testService(svc_objectDir *svc) {
    return STREQL(svc->getDirType(), name);
  }
private:
  String name;
};

#endif
