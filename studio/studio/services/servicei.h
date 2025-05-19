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

#ifndef _SERVICEI_H
#define _SERVICEI_H

// here is where we define some helper implementations of the service factory
// interface

#include "service.h"
#include "../../bfc/ptrlist.h"
#include "../api.h"

// you can derive directly from this, but usually you'll want to use the
// helper classes below

class NOVTABLE waServiceFactoryI : public waServiceFactory {
public:
  virtual ~waServiceFactoryI() {}

protected:
  virtual FOURCC svc_serviceType()=0;
  virtual const char *svc_getServiceName()=0;
  virtual GUID svc_getGuid()=0;
  virtual void *svc_getInterface()=0;
  virtual int svc_releaseInterface(void *)=0;
  virtual CfgItem *svc_getCfgInterface()=0;
  virtual int svc_notify(int msg, int param1=0, int param2=0)=0;

  RECVS_DISPATCH;
};

// if you derive from this, all you have to do is override the newService()
// and delService() methods... if all you need to do is instantiate a class
// and destroy it use the helper below (waServiceFactoryT)
template <class SERVICETYPE, class SERVICE>
class NOVTABLE waServiceFactoryBase : public waServiceFactoryI {
public:
  waServiceFactoryBase(GUID myGuid=INVALID_GUID) : guid(myGuid) {}
  virtual FOURCC svc_serviceType() { return SERVICETYPE::getServiceType(); }
  virtual const char *svc_getServiceName() { return SERVICE::getServiceName(); }
  virtual GUID svc_getGuid() { return guid; }
  virtual void *svc_getInterface() {
    SERVICETYPE *ret = newService();
    api->service_lock(this, ret);
    return ret;
  }
  virtual int svc_releaseInterface(void *ptr) {
    return delService(static_cast<SERVICE*>(static_cast<SERVICETYPE*>(ptr)));
  }
  virtual CfgItem *svc_getCfgInterface() { return NULL; }

  virtual int svc_notify(int msg, int param1=0, int param2=0) { return 0; }

protected:
  virtual SERVICETYPE *newService()=0;
  virtual int delService(SERVICETYPE *service)=0;

private:
  GUID guid;
};

// this is a service factory template that will manufacture any number
// of a given class SERVICE, which is derived from service class SERVICETYPE
template <class SERVICETYPE, class SERVICE>
class waServiceFactoryT : public waServiceFactoryBase<SERVICETYPE, SERVICE> {
public:
  waServiceFactoryT(GUID myGuid=INVALID_GUID) :
    waServiceFactoryBase<SERVICETYPE, SERVICE>(myGuid) {}
  virtual SERVICETYPE *newService() {
    SERVICE *ret = new SERVICE;
    issued.addItem(ret);	//implicit cast to SERVICETYPE
    return ret;
  }
  virtual int delService(SERVICETYPE *service) {
    int pos = issued.searchItem(service);
    if (pos >= 0) {
      issued.removeByPos(pos);
      delete static_cast<SERVICE*>(service);
      return 1;
    }
    return 0;
  }

private:
  PtrList<SERVICETYPE> issued;
};

// this is a service factory template that holds one copy of a class
// and reissues the pointer as needed, with reference counting
template <class SERVICETYPE, class SERVICE>
class waServiceFactoryTSingle : public waServiceFactoryBase<SERVICETYPE, SERVICE> {
public:
  waServiceFactoryTSingle(GUID myGuid=INVALID_GUID) :
    refcount(0), waServiceFactoryBase<SERVICETYPE, SERVICE>(myGuid) {
    singleService = new SERVICE;
  }
  waServiceFactoryTSingle(SERVICE *svc, GUID myGuid=INVALID_GUID) :
    refcount(0), singleService(svc),
    waServiceFactoryBase<SERVICETYPE, SERVICE>(myGuid) { }
  ~waServiceFactoryTSingle() {
    delete singleService;
  }
  virtual SERVICETYPE *newService() {
    ASSERT(singleService != NULL);
    refcount++;
    return singleService;
  }
  virtual int delService(SERVICETYPE *service) {
    ASSERT(static_cast<SERVICE*>(service) == singleService);
    refcount--;
    ASSERT(refcount >= 0);
    return 1;
  }

  SERVICE *getSingleService() { return singleService; }

private:
  SERVICE *singleService;
  int refcount;
};

// DEPRECATED
#define waServiceT waServiceFactoryT
#define waServiceTSingle waServiceFactoryTSingle

#endif
