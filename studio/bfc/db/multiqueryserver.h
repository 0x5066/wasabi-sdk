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

#ifndef __MULTIQUERYSERVER_H
#define __MULTIQUERYSERVER_H

#include "../dispatch.h"
#include "sharedscanner.h"
#include "scanner.h"
#include "../timeslicer.h"

// {89ED47C9-A5FF-4ef8-9A6D-84BFD298549E}
static const GUID multiQueryServerGuid = 
{ 0x89ed47c9, 0xa5ff, 0x4ef8, { 0x9a, 0x6d, 0x84, 0xbf, 0xd2, 0x98, 0x54, 0x9e } };

class SubQueryServer;
class MultiQueryClient;

class MultiQueryServer : public Dispatchable {
  
  public:
    
    void mqs_onSubQueryChanged(SubQueryServer *server=NULL, int flag=0);
    void mqs_registerSubQueryServer(SubQueryServer *server);
    void mqs_unregisterSubQueryServer(SubQueryServer *server);
    void mqs_registerClient(MultiQueryClient *client);
    void mqs_unregisterClient(MultiQueryClient *client);
    int mqs_getNumSubQueryServers();
    SubQueryServer *mqs_enumSubQueryServer(int n);
    int mqs_getNumMultiQueryClients();
    MultiQueryClient *mqs_enumMultiQueryClient(int n);
    int mqs_isQueryEmpty(); 
    const char *mqs_getMultiQuery();

    enum {
      MULTIQUERYSERVER_QUERYCHANGE=10,
      MULTIQUERYSERVER_REGISTERSUBSERVER=20,
      MULTIQUERYSERVER_UNREGISTERSUBSERVER=30,
      MULTIQUERYSERVER_REGISTERCLIENT=40,
      MULTIQUERYSERVER_UNREGISTERCLIENT=50,
      MULTIQUERYSERVER_GETNUMSUBQUERYSERVERS=60,
      MULTIQUERYSERVER_ENUMSUBQUERYSERVER=70,
      MULTIQUERYSERVER_GETNUMMULTIQUERYCLIENTS=80,
      MULTIQUERYSERVER_ENUMMULTIQUERYCLIENT=90,
      MULTIQUERYSERVER_ISQUERYEMPTY=100,
      MULTIQUERYSERVER_GETMULTIQUERY=110,
    };
};

inline void MultiQueryServer::mqs_onSubQueryChanged(SubQueryServer *whom, int glag) {
  _voidcall(MULTIQUERYSERVER_QUERYCHANGE, whom, glag);
}

inline void MultiQueryServer::mqs_unregisterSubQueryServer(SubQueryServer *server) {
  _voidcall(MULTIQUERYSERVER_UNREGISTERSUBSERVER, server);
}

inline void MultiQueryServer::mqs_registerSubQueryServer(SubQueryServer *server) {
  _voidcall(MULTIQUERYSERVER_REGISTERSUBSERVER, server);
}

inline void MultiQueryServer::mqs_unregisterClient(MultiQueryClient *client) {
  _voidcall(MULTIQUERYSERVER_UNREGISTERCLIENT, client);
}

inline void MultiQueryServer::mqs_registerClient(MultiQueryClient *client) {
  _voidcall(MULTIQUERYSERVER_REGISTERCLIENT, client);
}

inline int MultiQueryServer::mqs_getNumSubQueryServers() {
  return _call(MULTIQUERYSERVER_GETNUMSUBQUERYSERVERS, 0);
}

inline SubQueryServer *MultiQueryServer::mqs_enumSubQueryServer(int n) {
  return _call(MULTIQUERYSERVER_ENUMSUBQUERYSERVER, (SubQueryServer *)NULL, n);
}

inline int MultiQueryServer::mqs_getNumMultiQueryClients() {
  return _call(MULTIQUERYSERVER_GETNUMMULTIQUERYCLIENTS, 0);
}

inline MultiQueryClient *MultiQueryServer::mqs_enumMultiQueryClient(int n) {
  return _call(MULTIQUERYSERVER_ENUMMULTIQUERYCLIENT, (MultiQueryClient *)NULL, n);
}

inline MultiQueryServer::mqs_isQueryEmpty() {
  return _call(MULTIQUERYSERVER_ISQUERYEMPTY, 0);
}

inline const char *MultiQueryServer::mqs_getMultiQuery() {
  return _call(MULTIQUERYSERVER_GETMULTIQUERY, (const char *)NULL);
}

class _MultiQueryServerI : public MultiQueryServer {
  
  public:
    
    virtual void mqs_onSubQueryChanged(SubQueryServer *whom, int flag)=0; 
    virtual void mqs_unregisterSubQueryServer(SubQueryServer *server)=0;
    virtual void mqs_registerSubQueryServer(SubQueryServer *server)=0;
    virtual void mqs_unregisterClient(MultiQueryClient *server)=0;
    virtual void mqs_registerClient(MultiQueryClient *server)=0;
    virtual int mqs_getNumSubQueryServers()=0;
    virtual SubQueryServer *mqs_enumSubQueryServer(int n)=0;
    virtual int mqs_getNumMultiQueryClients()=0;
    virtual MultiQueryClient *mqs_enumMultiQueryClient(int n)=0;
    virtual int mqs_isQueryEmpty()=0; 
    virtual const char *mqs_getMultiQuery()=0;

  protected:
    RECVS_DISPATCH;
};

class SubQueryServerEntry {
  public:
    SubQueryServerEntry(SubQueryServer *s, Dependent *d) : server(s), dep_ptr(d) {}
    virtual ~SubQueryServerEntry() {}

    Dependent *dep_ptr;
    SubQueryServer *server;
};

class MultiQueryClientEntry {
  public:
    MultiQueryClientEntry(MultiQueryClient *s, Dependent *d) : client(s), dep_ptr(d) {}
    virtual ~MultiQueryClientEntry() {}

    Dependent *dep_ptr;
    MultiQueryClient *client;
};

class MultiQueryServerI : public _MultiQueryServerI, 
                          public DependentViewerI, 
                          public DependentI,
                          public TimeSlicer {
  
  public:

    MultiQueryServerI();
    virtual~MultiQueryServerI();
    
    virtual void mqs_onSubQueryChanged(SubQueryServer *whom=NULL, int flag=0); 
    virtual void mqs_onNewMultiQuery() {  }
    virtual void mqs_onAddPlaystring(const char *playstring, int nitems, int thispos) {  }
    virtual void mqs_onCompleteMultiQuery() {  }
    virtual int mqs_isQueryEmpty(); 
    virtual void mqs_unregisterSubQueryServer(SubQueryServer *server);
    virtual void mqs_registerSubQueryServer(SubQueryServer *server);
    virtual int mqs_getNumSubQueryServers() { return subqueries.getNumItems(); }
    virtual SubQueryServer *mqs_enumSubQueryServer(int n) { return subqueries.enumItem(n)->server; }
    virtual int mqs_getNumMultiQueryClients() { return clients.getNumItems(); }
    virtual MultiQueryClient *mqs_enumMultiQueryClient(int n) { return clients.enumItem(n)->client; }
    virtual void mqs_unregisterClient(MultiQueryClient *server);
    virtual void mqs_registerClient(MultiQueryClient *server);
    virtual void mqs_refresh();
    virtual void onSlice();
    virtual const char *mqs_getMultiQuery();

    virtual int viewer_onItemDeleted(Dependent *item);
    virtual Dependent *timeslicer_getDependencyPtr() { return this; }
    virtual void onResetSubqueries() {}

  protected:
    void resetSubQueries();

  private:
    void unjoinChildren();
    void joinChildren();
    void populate(SubQueryServer *modifier, int flag=0);

    PtrList<SubQueryServerEntry> subqueries;
    PtrList<MultiQueryClientEntry> clients;
    PtrList<SharedDbScanner> joined_scanners;
    dbScanner *scanner;
    String multiquery;
};

#endif
