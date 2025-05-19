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

#ifndef _OBJECTDIR_H
#define _OBJECTDIR_H

#include "common.h"
#include "../studio/services/svc_objectdir.h"
#include "../bfc/string.h"
#include "../studio/runlevelcb.h"

// helper layer... you probably want to derive from this one
class COMEXP ObjectDir : public svc_objectDirI, public RunlevelCallbackI {
public:
  class ObjRec {
  public:
    ObjRec() {}
    virtual ~ObjRec() {}

    virtual void *getObj()=0;
    virtual void shutdown()=0;

    ObjectHandle handle;
    String filename, label, path;
  };
  virtual ObjRec *createObjRec(const char *filename)=0;

//---------
protected:
  ObjectDir(const char *_config_prefix, const char *_dirname);
  virtual ~ObjectDir();

public:
  virtual const char *getDirType() { return config_prefix; }
  virtual int getNumObjects();
  virtual ObjectHandle enumObject(int n);
  virtual void *getObject(ObjectHandle handle);
  virtual const char *getObjectLabel(ObjectHandle handle);
  virtual int setObjectLabel(ObjectHandle handle, const char *newlabel);
  virtual ObjectHandle insertObject(const char *filename, const char *label=NULL, const char *path=NULL);
  int removeObject(ObjectHandle handle);
  void clearAll();

protected:
  // override as needed
  virtual int onAction(int action, RootWnd *from, const char *target, ObjectHandle handle) { return 0; }

//  // from RunlevelCallbackI
//  virtual void runlevelcb_onAppRunning();
//  virtual void runlevelcb_onShutdown();

private:
  ObjRec *handleToSlot(ObjectHandle handle);
  int id_gen;
  String config_prefix, dir_name;
  PtrList<ObjRec> objlist;
};

template <class T>
class ObjectDirT : public ObjectDir {
protected:
  class ObjRecT {
    ObjRecT(const char *filename) : obj(new T(filename)) {}
    ~ObjRecT() { delete obj; }

    virtual void *getObj() { return obj; }
    virtual void shutdown() { }

    int handle;
    String filename, label, path;
    T *obj;
  };
public:
  ObjectDirT(const char *_config_prefix, const char *_dirname)
    : ObjectDir(_config_prefix, _dirname) { }

  virtual ObjRec *createObjRec(const char *filename) {
    return new T(filename);
  }
};

#endif
