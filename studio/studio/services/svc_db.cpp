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


#include "svc_db.h"

#define CBCLASS svc_dbI
START_DISPATCH;
  CB(OPENTABLE, openTable);
  VCB(CLOSETABLE, closeTable);
  CB(TESTQUERYFORMAT, testQueryFormat);
END_DISPATCH;
#undef CBCLASS


#define CBCLASS dbSvcTableI
START_DISPATCH;
  CB(GETSCANNER, getScanner);
  CB(NEWSCANNER, newScanner);
  VCB(DELETESCANNER, deleteScanner);
  VCB(CBNEW, _new);
  VCB(CBINSERT, insert);
  VCB(CBCANCEL, cancel);
  VCB(CBEDIT, edit);
  VCB(CBPOST, post);
  VCB(CBDELETE, _delete);
  CB(EDITING, editing);
  VCB(SETFIELDBYNAME, setFieldByName);
  VCB(SETFIELDBYID, setFieldById);
  VCB(DELETEFIELDBYNAME, deleteFieldByName);
  VCB(DELETEFIELDBYID, deleteFieldById);
  VCB(ADDCOLUMN, addColumn);
  VCB(ADDINDEXBYNAME, addIndexByName);
  VCB(ADDINDEXBYID, addIndexById);
  VCB(DROPINDEXBYNAME, dropIndexByName);
  VCB(DROPINDEXBYID, dropIndexById);
  VCB(SYNC, sync);                                             
END_DISPATCH;
#undef CBCLASS

#define CBCLASS dbSvcScannerI
START_DISPATCH;
  VCB(CBFIRST, first);
  VCB(CBLAST, last);
  VCB(CBNEXT, next);
  VCB(CBPREVIOUS, previous);
  VCB(CBPUSH, push);
  VCB(CBPOP, pop);
  CB(CBEOF, eof);
  CB(CBBOF, bof);
  CB(GETNUMROWS, getNumRows);
  VCB(MOVETOROW, moveToRow);
  CB(GETCURROW, getCurRow);
  CB(LOCATEBYNAME, locateByName);
  CB(LOCATEBYID, locateById);
  CB(GETNUMCOLS, getNumCols);
  CB(ENUMCOL, enumCol);
  CB(GETCOLBYNAME, getColByName);
  CB(GETCOLBYID, getColByName);
  CB(GETFIELDBYNAME, getFieldByName);
  CB(GETFIELDBYID, getFieldById);
  VCB(SETINDEXBYNAME, setIndexByName);
  VCB(SETINDEXBYID, setIndexById);
  CB(UNIQUEBYNAME, newUniqueScannerByName);
  CB(UNIQUEBYID, newUniqueScannerById);
  VCB(DELETEUNIQUE, deleteUniqueScanner);
  CB(QUERY, query);
  VCB(CANCELQUERY, cancelQuery);
  CB(INDEXCHANGED, hasIndexChanged);
  VCB(CLEARDIRTYBIT, clearDirtyBit);
  VCB(JOINSCANNER, joinScanner);
  VCB(UNJOINSCANNER, unjoinScanner);
  CB(GETLASTQUERY, getLastQuery);
END_DISPATCH;
#undef CBCLASS


