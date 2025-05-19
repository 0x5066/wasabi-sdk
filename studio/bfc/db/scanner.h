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

#ifndef _SCANNER_H
#define _SCANNER_H

#include "../std.h"
#include "../string.h"
#include "../../common/compdb.h"
#include "../../studio/api.h"
#include "../../common/metatags.h"
#include "../../studio/services/svc_db.h"

class dbScanner {
public:
  dbScanner(GUID table_guid, const char *unique_field=NULL) {
    idxval_is_int = 0;
    scanner = api->metadb_newScanner(table_guid);
    ASSERT(scanner != NULL);
    if (unique_field != NULL) {
      dbSvcScanner *sav = scanner;
      scanner = scanner->newUniqueScannerByName(unique_field);
      ASSERT(scanner != NULL);
      api->metadb_deleteScanner(sav);
    }
    locate_pos = -1;
    first();
  }
  ~dbScanner() {
    api->metadb_deleteScanner(scanner);
  }

  void setIndexName(const char *name) {
    idxname = name;
    locate_pos = -1;
  }
  void setIndexValue(const char *val) {
    idxval = val;
    locate_pos = -1;
    idxval_is_int = 0;
  }
  void setIndexValue(int val) {
    idxval = StringPrintf(val);
    locate_pos = -1;
    idxval_is_int = 1;
  }

  void resetQuery() { scanner->cancelQuery(); }
  int setQuery(const char *query) {
    if (last_query.isempty() && (!query || !*query)) return 0;
    last_query = query;
    int r = scanner->query(query);
    first();
    return r;
  }

  void first() {
    _eof = 0;
    if (idxname) {
      dbSvcDatachunk dc;
      int blep;
      if (idxval_is_int) {
        blep = ATOI(idxval);
        dc.data = (void*)&blep;
        dc.datalen = sizeof(int);
        dc.datatype = DBSVC_DATATYPE_INT;
      } else {
        dc.data = (void*)idxval.getValue();
        dc.datalen = idxval.len()+1;
        dc.datatype = DBSVC_DATATYPE_STRING;
      }
      int r = scanner->locateByName(idxname, -1, &dc);
      if (r != DB_SUCCESS) _eof = 1;
      locate_pos = scanner->getCurRow();
    } else {
      scanner->first();
      if (scanner->eof()) _eof = 1;
    }
  }
  void next() {
    if (idxname) {
      dbSvcDatachunk dc;
      int blep;
      if (idxval_is_int) {
        blep = ATOI(idxval);
        dc.data = (void*)&blep;
        dc.datalen = sizeof(int);
        dc.datatype = DBSVC_DATATYPE_INT;
      } else {
        dc.data = (void *)idxval.getValue();
        dc.datalen = idxval.len()+1;
        dc.datatype = DBSVC_DATATYPE_STRING;
      }
      int r = scanner->locateByName(idxname, locate_pos, &dc);
      if (r != DB_SUCCESS) _eof = 1;
      locate_pos = scanner->getCurRow();
    } else {
      scanner->next();
      if (scanner->eof()) _eof = 1;
    }
  }
//  void previous() { api->db_previous(scanner); }
  int eof() { return _eof; }
  int bof() { return scanner->bof(); }
  const char *getPlaystring() { 
    dbSvcDatachunk *c = scanner->getFieldByName("Playstring"); 
    if (c) return (const char *)c->data; 
    return NULL; 
  }
  int getData(char *field_name, void *data, int data_len, int data_type) {
    ASSERTPR(_eof == 0, "should have checked for eof()");
    dbSvcDatachunk *c = scanner->getFieldByName(field_name);
    if (c != NULL) {
      STRNCPY((char *)data, (const char *)c->data, data_len);
      return DB_SUCCESS;
    } 
    return DB_ERROR;
  }
  int getNumRecords() { return scanner->getNumRows(); }

  dbSvcScanner *getScanner() { return scanner; }
  const char *getQuery() { return last_query; }

private:
  dbSvcScanner *scanner;
  String idxname, idxval;
  int idxval_is_int;
  int locate_pos;
  int _eof;
  String last_query;
};

#endif
