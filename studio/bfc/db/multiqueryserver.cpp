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

#include "multiqueryserver.h"
#include "multiqueryclient.h"
#include "subqueryserver.h"
#include "../../common/nsguid.h"

#define CBCLASS _MultiQueryServerI
START_DISPATCH;
  VCB(MULTIQUERYSERVER_QUERYCHANGE, mqs_onSubQueryChanged);
  VCB(MULTIQUERYSERVER_REGISTERSUBSERVER, mqs_registerSubQueryServer);
  VCB(MULTIQUERYSERVER_UNREGISTERSUBSERVER, mqs_unregisterSubQueryServer);
  VCB(MULTIQUERYSERVER_REGISTERCLIENT, mqs_registerClient);
  VCB(MULTIQUERYSERVER_UNREGISTERCLIENT, mqs_unregisterClient);
  CB(MULTIQUERYSERVER_GETNUMSUBQUERYSERVERS, mqs_getNumSubQueryServers);
  CB(MULTIQUERYSERVER_ENUMSUBQUERYSERVER, mqs_enumSubQueryServer);
  CB(MULTIQUERYSERVER_GETNUMMULTIQUERYCLIENTS, mqs_getNumMultiQueryClients);
  CB(MULTIQUERYSERVER_ENUMMULTIQUERYCLIENT, mqs_enumMultiQueryClient);
  CB(MULTIQUERYSERVER_ISQUERYEMPTY, mqs_isQueryEmpty);
  CB(MULTIQUERYSERVER_GETMULTIQUERY, mqs_getMultiQuery);
END_DISPATCH;


MultiQueryServerI::MultiQueryServerI() {
  scanner = new dbScanner(api->main_getGUID());
}

MultiQueryServerI::~MultiQueryServerI() {
  subqueries.deleteAll();
  delete scanner;
}

void MultiQueryServerI::mqs_onSubQueryChanged(SubQueryServer *whom, int flag) {
  multiquery = "";
  foreach(subqueries)
    SubQueryServerEntry *sqse = subqueries.getfor();
    String q = sqse->server->sqs_getQuery();
    if (q.isempty()) continue;
    GUID g = sqse->server->sqs_getTableGuid();
    char t[256]="";
    nsGUID::toChar(g, t);
// ps. you could use StringPrintf(GUID) here instead
    multiquery += StringPrintf("%s%s;%s", multiquery.isempty() ? "" : ";", t, q);
  endfor;
  populate(whom, flag);
}

void MultiQueryServerI::unjoinChildren() {
  foreach(joined_scanners)
    joined_scanners.getfor()->shareddbscanner_delRef();
    scanner->getScanner()->unjoinScanner(joined_scanners.getfor()->shareddbscanner_getScanner());
  endfor;
  joined_scanners.removeAll();
}

void MultiQueryServerI::joinChildren() {
  foreach(subqueries)
    SharedDbScanner *s = subqueries.getfor()->server->sqs_getSharedScanner();
    if (s && s->shareddbscanner_getScanner()) {
      s->shareddbscanner_addRef();
      scanner->getScanner()->joinScanner(s->shareddbscanner_getScanner(), MT_PLAYSTRING);
      joined_scanners.addItem(s);
    }
  endfor;
}

void MultiQueryServerI::populate(SubQueryServer *modifier, int flag) {
  ASSERT(scanner != NULL);

  if (isSlicerStarted()) {
    stopSlicer();
    unjoinChildren();
    mqs_onCompleteMultiQuery();
    foreach(clients)
      clients.getfor()->client->mqc_onCompleteMultiQuery();
    endfor;
  }

  joinChildren();

  mqs_onNewMultiQuery();

  foreach(clients)
    clients.getfor()->client->mqc_onNewMultiQuery(modifier, flag);
  endfor;

  scanner->getScanner()->setIndexByName(MT_ARTIST);
  scanner->first();
  startSlicer();
}

void MultiQueryServerI::onSlice() {
  ASSERT(scanner != NULL);  

  if (scanner->eof()) {
    stopSlicer();
    unjoinChildren();
    mqs_onCompleteMultiQuery();
    foreach(clients)
      clients.getfor()->client->mqc_onCompleteMultiQuery();
    endfor;
    return;
  }

  const char *ps = scanner->getPlaystring();
  int n = scanner->getScanner()->getNumRows();
  int p = scanner->getScanner()->getCurRow();
  mqs_onAddPlaystring(ps, n, p);
  foreach(clients)
    clients.getfor()->client->mqc_onAddPlaystring(ps, n, p);
  endfor;
  scanner->next();
}


void MultiQueryServerI::mqs_registerSubQueryServer(SubQueryServer *server) {
  subqueries.addItem(new SubQueryServerEntry(server, server->sqs_getDependencyPtr()));
  viewer_addViewItem(server->sqs_getDependencyPtr());
}

void MultiQueryServerI::mqs_unregisterSubQueryServer(SubQueryServer *server) {
  viewer_delViewItem(server->sqs_getDependencyPtr());
  foreach(subqueries) 
    SubQueryServerEntry *e = subqueries.getfor();
    if (e->server == server) {
      subqueries.removeByPos(foreach_index);
      delete e;
      // no break for security
    }
  endfor;
}

int MultiQueryServerI::viewer_onItemDeleted(Dependent *item) {
  foreach(subqueries) 
    SubQueryServerEntry *e = subqueries.getfor();
    if (e->dep_ptr == item) {
      subqueries.removeByPos(foreach_index);
      delete e;
      // no break for seacurity
    }
  endfor;
  foreach(clients) 
    MultiQueryClientEntry *e = clients.getfor();
    if (e->dep_ptr == item) {
      clients.removeByPos(foreach_index);
      delete e;
      // no break for seacurity
    }
  endfor;
  return 1;
}

void MultiQueryServerI::mqs_registerClient(MultiQueryClient *client) {
  clients.addItem(new MultiQueryClientEntry(client, client->mqc_getDependencyPtr()));
  viewer_addViewItem(client->mqc_getDependencyPtr());
  populate(NULL);
}

void MultiQueryServerI::mqs_unregisterClient(MultiQueryClient *client) {
  viewer_delViewItem(client->mqc_getDependencyPtr());
  foreach(subqueries) 
    MultiQueryClientEntry *e = clients.getfor();
    if (e->client == client) {
      clients.removeByPos(foreach_index);
      delete e;
      // no break for security
    }
  endfor;
}

void MultiQueryServerI::mqs_refresh() {
  mqs_onSubQueryChanged(NULL);
}

int MultiQueryServerI::mqs_isQueryEmpty() {
  foreach(joined_scanners) 
    dbSvcScanner *s = joined_scanners.getfor()->shareddbscanner_getScanner();
    const char *q = s->getLastQuery();
    if (q && *q) 
      return 0;
  endfor;
  return 1;
}

void MultiQueryServerI::resetSubQueries() {
  onResetSubqueries();
  foreach(subqueries)
    subqueries.getfor()->server->sqs_reset();
  endfor
}

const char *MultiQueryServerI::mqs_getMultiQuery() {
  return multiquery;
}
