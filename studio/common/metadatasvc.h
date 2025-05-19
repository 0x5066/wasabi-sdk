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

#ifndef _METADATASVC_H
#define _METADATASVC_H

#include "../studio/services/svc_metadata.h"
#include "../studio/services/servicei.h"
#include "common.h"
#include "../bfc/named.h"
#include "../bfc/ptrlist.h"

class COMEXP MetaDataSvc : public svc_metaDataI, private Named {
public:
  MetaDataSvc(const char *tablename, GUID myGuid);
  ~MetaDataSvc();

  static const char *getServiceName() { return "MetaData Service"; }

  void addColumn(const char *columnName, int datatype, BOOL indexed, BOOL unique);

  virtual int enumMetaData(int n, char *mdName, int mdNameMaxLength, int *mdType, BOOL *mdIndexed, BOOL *mdMaintainUniques);
  virtual int hasMetaData(const char *name, int type);
  virtual int allowOperation(GUID whom, const char *playstring, int op, char *field, void *data, int data_len);
  virtual GUID getGUID();
  virtual const char *getMetaTableName();

private:
  GUID myGuid;
  class dbField;
  PtrList<dbField> fields;
};

// insert this via WaComponentClient::registerService()
class MetaDataSvcFactory : public waServiceFactoryTSingle<svc_metaData, MetaDataSvc> {
public:
  MetaDataSvcFactory(MetaDataSvc *mds) : waServiceFactoryTSingle<svc_metaData, MetaDataSvc>(mds) {}
};

#endif
