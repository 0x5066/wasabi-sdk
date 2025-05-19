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

#ifndef _DEPEND_H
#define _DEPEND_H

#include "platform/platform.h"
#include "../common/common.h"
#include "ptrlist.h"

// a pair of classes to implement data dependency. a viewer can register
// a list of things it wants to know about

// WARNING: this file is still under development. check back for changes
// in subsequent SDK releases. over time it is going to become more generic


class Dependent;

// we define some common event codes here. if your object needs more send
// them as parameters to OBJECTSPECIFIC

// some system-level codes for cb param. You can implement your own events
// with EVENT and the parameters
namespace DependentCB {
  enum {
    NOP=0,
    DELETED=100,		// object being deleted
    EVENT=1000,			// object-specific event. use param1 etc to send your messages
  };
};

// DO NOT DERIVE DIRECTLY FROM THIS!
class NOVTABLE DependentViewer {
protected:
friend class DependentI;
  // item calls when it changes or disappears, or whatever
  virtual int dependentViewer_callback(Dependent *item, const GUID *classguid, int cb, int param1=0, int param2=0, void *ptr=NULL, int ptrlen=0)=0;
};

// inherit from this one
class NOVTABLE COMEXP DependentViewerI : public DependentViewer {
protected:
/**
 @param classguid	If set, incoming events are restricted to those in that GUID namespace. Can be NULL.
*/
  DependentViewerI();
  DependentViewerI(const DependentViewerI &dep);
  virtual ~DependentViewerI();
  // copy
  DependentViewerI& operator =(const DependentViewerI &dep);

  // derived classes call this on themselves when they want to view a new item
  // everything else gets handled automagically
  void viewer_addViewItem(Dependent *item);
  void viewer_delViewItem(Dependent *item);
  void viewer_delAllItems();

  // call this whenever you need to know what you're looking at
  Dependent *viewer_enumViewItem(int which);
  int viewer_getNumItems();
  // returns TRUE if item is in our list
  int viewer_haveItem(Dependent *item);

// convenience callback methods

  // item you were looking at is gone: WARNING: pointer no longer valid!
  virtual int viewer_onItemDeleted(Dependent *item) { return 1; }
  // item you are looking at issuing an event
  virtual int viewer_onEvent(Dependent *item, const GUID *classguid, int event, int param, void *ptr, int ptrlen) { return 1; }

private:
  // don't override this; override the individual convenience callbacks
  virtual int dependentViewer_callback(Dependent *item, const GUID *classguid, int cb, int param1=0, int param2=0, void *ptr=NULL, int ptrlen=0);
  typedef PtrList<Dependent> DependentList;
  DependentList *viewed_items;
};

template <class VT>
class DependentViewerT : private DependentViewerI {
public:
  DependentViewerT() { }
  DependentViewerT(VT *first) {
    if (first) viewer_addViewItem(first);
  }
  using DependentViewerI::viewer_addViewItem;
  using DependentViewerI::viewer_delViewItem;
  using DependentViewerI::viewer_delAllItems;
  VT *viewer_enumViewItem(int which) {
    return static_cast<VT*>(DependentViewerI::viewer_enumViewItem(which));
  }
  using DependentViewerI::viewer_getNumItems;
  using DependentViewerI::viewer_haveItem;

  // spiffy callbacks to override
  // item you were looking at is gone: WARNING: pointer no longer valid!
  virtual int viewer_onItemDeleted(VT *item) { return 1; }
  // item you are looking at issuing an event (filtered by class guid of VT)
  virtual int viewer_onEvent(VT *item, int event, int param2, void *ptr, int ptrlen) { return 1; }

private:
  virtual int viewer_onItemDeleted(Dependent *item) {
    return viewer_onItemDeleted(static_cast<VT*>(item));
  }
  virtual int viewer_onEvent(Dependent *item, const GUID *classguid, int event, int param, void *ptr, int ptrlen) {
    if (*classguid != *VT::depend_getClassGuid()) return 0;	// filter namespace
    return viewer_onEvent(static_cast<VT*>(item), event, param, ptr, ptrlen);
  }
};

// ------------------------------------------------------------

// don't derive directly from this one
class NOVTABLE Dependent {
protected:
friend class DependentViewerI;
  // the viewer calls this
  virtual void dependent_regViewer(DependentViewer *viewer, int add)=0;
public:
  virtual void *dependent_getInterface(const GUID *classguid)=0;
};

class COMEXP NOVTABLE DependentI : public Dependent {
protected:
  DependentI(const GUID *class_guid=NULL);
  DependentI(const DependentI &dep);
  virtual ~DependentI();

public:
  // copy
  DependentI& operator =(const Dependent &dep);

protected:
  // override this to catch when viewers register and deregister
  virtual void dependent_onRegViewer(DependentViewer *viewer, int add) {}
  // override this to help people cast you to various classes
  virtual void *dependent_getInterface(const GUID *classguid) { return NULL; }

  // call this on yourself to send an event
  void dependent_sendEvent(const GUID *classguid, int event, int param=0, void *ptr=NULL, int ptrlen=0);

private:
  virtual void dependent_regViewer(DependentViewer *viewer, int add);
  void sendViewerCallbacks(const GUID *classguid, int msg, int param1=0, int param2=0, void *ptr=NULL, int ptrlen=0);
  typedef PtrList<DependentViewer> ViewerList;
  ViewerList *viewers;
  GUID class_guid;
};

// this is a helper for dependent_getInterface
#define HANDLEGETINTERFACE(x) { \
  if (*classguid == *x::depend_getClassGuid()) return static_cast<x *>(this); \
}

// use like MyClass *myobj = dynamic_guid_cast<MyClass>(some_dependent_ptr);
// MyClass::getClassGuid() must be defined
// MyClass::dependent_getInterface() must be defined too
template <class T>
class dynamic_guid_cast {
public:
  dynamic_guid_cast(Dependent *_dp, const GUID *_g) : dp(_dp), g(_g) { }
  operator T*() {
    return (*g == *T::depend_getClassGuid()) ? static_cast<T*>(dp->dependent_getInterface(g)) : NULL;
  }
private:
  Dependent *dp;
  const GUID *g;
};

#endif
