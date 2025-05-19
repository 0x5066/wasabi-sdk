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

#include "subqueryserver.h"
#include "multiqueryserver.h"
#include "sharedscanner.h"

#define CBCLASS _SubQueryServerI
START_DISPATCH;
  CB(SUBQUERYSERVER_GETSHAREDSCANNER, sqs_getSharedScanner);
  CB(SUBQUERYSERVER_GETDEPPTR, sqs_getDependencyPtr);
  CB(SUBQUERYSERVER_GETCOOPERATIVEID, sqs_getCooperativeId);
  VCB(SUBQUERYSERVER_RESET, sqs_reset);
  CB(SUBQUERYSERVER_GETQUERY, sqs_getQuery);
  CB(SUBQUERYSERVER_GETTABLEGUID, sqs_getTableGuid);
END_DISPATCH;

SubQueryServerI::SubQueryServerI() {
  server = NULL; 
  lastflag = -1;
}

SubQueryServerI::~SubQueryServerI() {
}

void SubQueryServerI::sqs_setTable(GUID table) {
  if (scannerserver_getTable() == table) return;
  scannerserver_setTable(table);
}

int SubQueryServerI::sqs_setQuery(const char *query, int flag, int force_run) {
  if (force_run == 0 && STRCASEEQLSAFE(query, lastquery) && flag == lastflag) return 0;

  if (scannerserver_getScanner())
    scannerserver_getScanner()->resetQuery();

  lastquery = query;
  lastflag = flag;
  queryChanged(flag);
  return 1;
}

void SubQueryServerI::sqs_setMultiQueryServer(MultiQueryServer *s) {
  MultiQueryServer *old = server;
  if (server != NULL) {
    server->mqs_unregisterSubQueryServer(this);
    sqs_onDetachServer(server);
    server = NULL;
  }
  server = s;
  if (server != NULL) {
    server->mqs_registerSubQueryServer(this);
    sqs_onAttachServer(server);
  }
}

void SubQueryServerI::scannerserver_onNewScanner(SharedDbScannerI *scan) {
  SUBQUERYSERVER_DBPARENT::scannerserver_onNewScanner(scan);
  queryChanged();
}

void SubQueryServerI::queryChanged(int flag) {
  SharedDbScannerI *scan = scannerserver_getScanner();
  if (scan == NULL) return;
  if (!lastquery.isempty()) {
    scan->setQuery(lastquery);
  } else
    scan->getScanner()->cancelQuery();

  if (!sqs_getMultiQueryServer()) {
    //api->console_outputString(SEVERITY_LINKNOTFOUND, "SubQueryServerI : no MultiQueryServer to notify");
    return;
  }

  sqs_getMultiQueryServer()->mqs_onSubQueryChanged(this, flag);
}
