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

#ifndef _SERVICE_H
#define _SERVICE_H

// this file defines the wasabi service factory class. this isn't the actual
// service, just the interface to get the service pointer

#include "../../bfc/dispatch.h"
#include "services.h"

namespace SvcNotify {
  enum {
    ONREGISTERED=100,	// init yourself here -- not all other services are registered yet
    ONSTARTUP=200,	// everyone is initialized, safe to talk to other services
    ONAPPRUNNING=210,	// app is showing and processing events
    ONSHUTDOWN=300,	// studio is shutting down, release resources from other services
    ONDEREGISTERED=400,	// bye bye
    ONDBREADCOMPLETE=500,// after db is read in (happens asynchronously after ONSTARTUP)
  };
};

class CfgItem;

// this is a wasabi service factory. it's a static object you can query to
// fetch the * to your real service interface
class waServiceFactory : public Dispatchable {
public:
  FOURCC getServiceType();	// see services.h
  const char *getServiceName();
  GUID getGuid();	// GUID per service factory, can be INVALID_GUID

  void *getInterface();
  int releaseInterface(void *);	// when they're done w/ it

  CfgItem *getCfgInterface();

  int serviceNotify(int msg, int param1=0, int param2=0);

protected:
  enum {
    GETSERVICETYPE=100,
    GETSERVICENAME=200,
    GETGUID=210,
    GETINTERFACE=300,
    RELEASEINTERFACE=310,
    GETCFGINTERFACE=400,
    SERVICENOTIFY=500,
  };
};

inline FOURCC waServiceFactory::getServiceType() {
  return _call(GETSERVICETYPE, WaSvc::NONE);
}

inline const char *waServiceFactory::getServiceName() {
  return _call(GETSERVICENAME, "");
}

inline
GUID waServiceFactory::getGuid() {
  return _call(GETGUID, INVALID_GUID);
}

inline void *waServiceFactory::getInterface() {
  return _call(GETINTERFACE, (void*)0);
}

inline int waServiceFactory::releaseInterface(void *ptr) {
  return _call(RELEASEINTERFACE, 0, ptr);
}

inline CfgItem *waServiceFactory::getCfgInterface() {
  return _call(GETCFGINTERFACE, (CfgItem*)NULL);
}

inline int waServiceFactory::serviceNotify(int msg, int param1, int param2) {
  return _call(SERVICENOTIFY, 0, msg, param1, param2);
}

// DEPRECATED
#define waService waServiceFactory

#endif
