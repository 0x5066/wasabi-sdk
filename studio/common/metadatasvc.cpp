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


#include "metadatasvc.h"
#include "metatags.h"

class MetaDataSvc::dbField {
public:
  String name;
  int type;
  BOOL indexed, unique;
};

MetaDataSvc::MetaDataSvc(const char *tablename, GUID guid) :
  Named(tablename), myGuid(guid) {
}

MetaDataSvc::~MetaDataSvc() {
  fields.deleteAll();
}
void MetaDataSvc::addColumn(const char *columnName, int datatype, BOOL indexed, BOOL unique) {
  dbField *df = new dbField;
  df->name = columnName;
  df->type = datatype;
  df->indexed = indexed;
  df->unique = unique;
  fields.addItem(df);
}

int MetaDataSvc::enumMetaData(int n, char *mdName, int mdNameMaxLength, int *mdType, BOOL *mdIndexed, BOOL *mdMaintainUniques) {
  if (n < 0 || n >= fields.getNumItems()) return 0;
  dbField *df = fields[n];
  STRNCPY(mdName, df->name, mdNameMaxLength);
  *mdType = df->type;
  *mdIndexed = df->indexed;
  *mdMaintainUniques = df->unique;
  return 1;
}

int MetaDataSvc::hasMetaData(const char *name, int type) {
  foreach(fields)
    if (fields.getfor()->name.iscaseequal(name) && (type == MDT_NONE || fields.getfor()->type == type))
      return true;
  endfor
  return false;
}

int MetaDataSvc::allowOperation(GUID whom, const char *playstring, int op, char *field, void *data, int data_len) {
  return true;	// FUCKO: allow some better control
}

GUID MetaDataSvc::getGUID() {
  return myGuid;
}

const char *MetaDataSvc::getMetaTableName() {
  return Named::getName();
}
