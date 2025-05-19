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

#ifndef _SVC_ENUM_H
#define _SVC_ENUM_H

#include "../common/common.h"

#include "memblock.h"
#include "ptrlist.h"

#include "../studio/api.h"
#include "../common/metatags.h"

#include "../studio/services/service.h"
#include "../studio/services/services.h"

// try to use this one, it does a clientLock for you
template <class T>
class castService {
public:
  castService(waServiceFactory *w) : was(w) { }
  operator T *() {
    if (was == NULL) return NULL;
    T *ret = static_cast<T *>(was->getInterface());
    api->service_clientLock(ret);
    return ret;
  }
private:
  waServiceFactory *was;
};

/*
 * this is a helper class to fetch a service * by GUID
 * usage: svc_something *svc = SvcEnumByGuid<svc_something>(svcguid);
 * @short: Helper class to fetch unique service by GUID
*/
template <class SERVICETYPE>
class SvcEnumByGuid {
public:
/**
@param _guid The GUID of the service factory to fetch the service from.
*/
  SvcEnumByGuid() : guid(SERVICETYPE::getServiceGuid()) {}
  SvcEnumByGuid(GUID _guid) : guid(_guid) {}

/**
@return The pointer to the service.
*/
  SERVICETYPE *getInterface() {
    waServiceFactory *svc = api->service_getServiceByGuid(guid);
    return castService<SERVICETYPE>(svc);
  }

  operator SERVICETYPE *() { return getInterface(); }

private:
  GUID guid;
};

// abstract base class
class NOVTABLE COMEXP SvcEnum {
protected:
  SvcEnum();

  void *_getNext();
  void reset();

  virtual void *_testService(waService *)=0;

public:
#ifdef ASSERTS_ENABLED
  static int release(waService *ptr) { ASSERTALWAYS("never ever call release() with a waService * !!!"); return 0; }
#endif
  static int release(void *ptr);

protected:
  FOURCC type;

private:
  int pos;
};


template <class T>
class SvcEnumT : private SvcEnum {
protected:
  SvcEnumT() { type = T::getServiceType(); }

public:
  void reset() { SvcEnum::reset(); }
  T *getFirst() { reset(); return getNext(); }
  T *getNext() { return static_cast<T*>(_getNext()); }

  using SvcEnum::release;

protected:
  virtual void *_testService(waService *svc) {
    T *ret = castService<T>(svc);	// lock happens here
    if (testService(ret)) return ret;
    release(ret); // you are not it, unlock here
    return NULL;
  }

  // override this one (or don't if you want to return all of them)
  virtual testService(T *svc) { return TRUE; }
};

#endif
