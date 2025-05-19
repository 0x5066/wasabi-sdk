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

#ifndef __SUBQUERYSERVER_H
#define __SUBQUERYSERVER_H

#include "../dispatch.h"
#include "../depend.h"
#include "../string.h"
#include "../ptrlist.h"
#include "scannerserver.h"
#include "sharedscanner.h"

class MultiQueryServer;
class SharedDbScanner;
class dbScanner;

namespace SetQuery {
  enum {
    FORWARD=0, REVERSE=1, GLOBAL=2,
  };
};

class SubQueryServer : public Dispatchable {
  public:
    SharedDbScanner *sqs_getSharedScanner();
    Dependent *sqs_getDependencyPtr();
    int sqs_getCooperativeId();
    void sqs_reset();
    const char *sqs_getQuery();
    GUID sqs_getTableGuid();

    enum {
      SUBQUERYSERVER_GETSHAREDSCANNER=10,
      SUBQUERYSERVER_GETDEPPTR=20,
      SUBQUERYSERVER_GETCOOPERATIVEID=40,
      SUBQUERYSERVER_RESET=50,
      SUBQUERYSERVER_GETQUERY=60,
      SUBQUERYSERVER_GETTABLEGUID=70,
    };
};

inline SharedDbScanner *SubQueryServer::sqs_getSharedScanner() {
  return _call(SUBQUERYSERVER_GETSHAREDSCANNER, (SharedDbScanner *)NULL);
}

inline Dependent *SubQueryServer::sqs_getDependencyPtr() {
  return _call(SUBQUERYSERVER_GETDEPPTR, (Dependent *)NULL);
}

inline int SubQueryServer::sqs_getCooperativeId() {
  return _call(SUBQUERYSERVER_GETCOOPERATIVEID, 0);
}

inline void SubQueryServer::sqs_reset() {
  _voidcall(SUBQUERYSERVER_RESET);
}

inline const char *SubQueryServer::sqs_getQuery() {
  return _call(SUBQUERYSERVER_GETQUERY, (const char *)NULL);
}

inline GUID SubQueryServer::sqs_getTableGuid() {
  return _call(SUBQUERYSERVER_GETTABLEGUID, INVALID_GUID);
}

class _SubQueryServerI : public SubQueryServer {
  public:
    virtual SharedDbScanner *sqs_getSharedScanner()=0;
    virtual Dependent *sqs_getDependencyPtr()=0;
    virtual int sqs_getCooperativeId()=0;
    virtual void sqs_reset()=0;
    virtual const char *sqs_getQuery()=0;
    virtual GUID sqs_getTableGuid()=0;

  protected:
    RECVS_DISPATCH;
};

#define SUBQUERYSERVER_DBPARENT ScannerServer

class SubQueryServerI : public _SubQueryServerI, 
                        public SUBQUERYSERVER_DBPARENT, 
                        public DependentI {

  public:
    
    SubQueryServerI();
    virtual ~SubQueryServerI();

    virtual void sqs_setTable(GUID table);
    virtual GUID sqs_getTable() { return scannerserver_getTable(); }
    virtual int sqs_setQuery(const char *query, int flag=SetQuery::FORWARD, int force_run=0);
    virtual void sqs_setMultiQueryServer(MultiQueryServer *server);
    virtual SharedDbScanner *sqs_getSharedScanner() { return scannerserver_getScanner(); }
    virtual MultiQueryServer *sqs_getMultiQueryServer() { return server; }
    virtual void sqs_onAttachServer(MultiQueryServer *s) {}
    virtual void sqs_onDetachServer(MultiQueryServer *s) {}

    virtual int sqs_getCooperativeId() { return -1; }
    virtual void sqs_reset() { }
    virtual Dependent *sqs_getDependencyPtr() { return this; }
    virtual Dependent *scannerserver_getDependencyPtr() { return this; }
    virtual void scannerserver_onNewScanner(SharedDbScannerI *scanner);
    virtual const char *sqs_getQuery() { return lastquery; }
    virtual GUID sqs_getTableGuid() { return scannerserver_getTable(); }

  protected:
    String lastquery;
    int lastflag;

  private:
    void queryChanged(int flag=0);
    void reloadScanner();

    MultiQueryServer *server;
};

#endif
