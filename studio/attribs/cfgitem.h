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

#ifndef _CFGITEM_H
#define _CFGITEM_H

#include "../bfc/dispatch.h"
#include "../bfc/std.h"

class Dependent;
class RootWnd;

// abstract base class presented to the world
/**


  @short Base Config Item
  @ver 1.0
  @author Nullsoft
  @see CfgItemI
*/
class NOVTABLE CfgItem : public Dispatchable {
public:
  /**
  */
  static const GUID *depend_getClassGuid() {
    // {B4BE480E-2005-457c-A445-294F12387E74}
    static const GUID ret = 
    { 0xb4be480e, 0x2005, 0x457c, { 0xa4, 0x45, 0x29, 0x4f, 0x12, 0x38, 0x7e, 0x74 } };
    return &ret;
  }
  
  /**
    
  */
  const char *getName();
  
  /**
    Get the GUID 
  */
  GUID getGuid();
  
  /**
    Get the number of attributes
    associated with this configuration
    item.
    
    @ret Number of attributes for this configuration item.
  */
  int getNumAttributes();
  
  /**
  */
  const char *enumAttribute(int n);

  // so people can watch you for changes
  Dependent *getDependencyPtr();

  // return * to your config xml if you want to specify it
  const char *getConfigXML();
  void onCfgGroupCreate(RootWnd *cfggroup, const char *attrname=NULL);
  void onCfgGroupDelete(RootWnd *cfggroup);

  // if you have child cfgitems, list them here
  int getNumChildren();
  CfgItem *enumChild(int n);
  GUID getParentGuid();

  void onRegister();	// kernel calls these
  void onDeregister();

  int getAttributeType(const char *name);
  const char *getAttributeConfigGroup(const char *name);
  int getDataLen(const char *name);
  int getData(const char *name, char *data, int data_len);
  int setData(const char *name, const char *data);

  int getDataAsInt(const char *name, int def_val=0) {
    char buf[256];
    if (!getData(name, buf, sizeof buf)) return def_val;
    return ATOI(buf);
  }
  void setDataAsInt(const char *name, int val) {
    char buf[256];
    SPRINTF(buf, "%d", val);	// this uses SPRINTF ON PURPOSE, motherfucker BU
    setData(name, buf);
  }

  double getDataAsFloat(const char *name, double def_val=0) {
    char buf[256];
    if (!getData(name, buf, sizeof buf)) return def_val;
    return ATOF(buf);
  }
  void setDataAsFloat(const char *name, double val) {
    char buf[256];
    SPRINTF(buf, "%f", val);	// this uses SPRINTF ON PURPOSE, motherfucker BU
    setData(name, buf);
  }
  enum {
    Event_ATTRIBUTE_ADDED=100,	// ptr is name of attrib
    Event_ATTRIBUTE_REMOVED=200,// ptr is name of attrib
    Event_ATTRIBUTE_CHANGED=300	// ptr is name of attrib
  };

protected:
  enum {
    CFGITEM_GETNAME=100,
    CFGITEM_GETGUID=110,
    CFGITEM_GETNUMATTRIBUTES=200,
    CFGITEM_ENUMATTRIBUTE=210,
    CFGITEM_GETDEPENDENCYPTR=300,
    CFGITEM_GETNUMCHILDREN=400,
    CFGITEM_ENUMCHILD=410,
    CFGITEM_GETPARENTGUID=420,
    CFGITEM_ONREGISTER=500,
    CFGITEM_ONDEREGISTER=510,
    CFGITEM_GETCONFIGXML=600,
    CFGITEM_ONCFGGROUPCREATE=610,
    CFGITEM_ONCFGGROUPDELETE=620,
    CFGITEM_GETATTRIBUTETYPE=700,
    CFGITEM_GETATTRIBUTECONFIGGROUP=710,
    CFGITEM_GETDATALEN=800,
    CFGITEM_GETDATA=810,
    CFGITEM_SETDATA=820,
  };
};

inline const char *CfgItem::getName() {
  return _call(CFGITEM_GETNAME, "");
}

inline GUID CfgItem::getGuid() {
  return _call(CFGITEM_GETGUID, INVALID_GUID);
}

inline int CfgItem::getNumAttributes() {
  return _call(CFGITEM_GETNUMATTRIBUTES, 0);
}

inline const char *CfgItem::enumAttribute(int n) {
  return _call(CFGITEM_ENUMATTRIBUTE, (const char*)NULL, n);
}

inline Dependent *CfgItem::getDependencyPtr() {
  return _call(CFGITEM_GETDEPENDENCYPTR, (Dependent*)NULL);
}

inline const char *CfgItem::getConfigXML() {
  return _call(CFGITEM_GETCONFIGXML, (const char*)NULL);
}

inline void CfgItem::onCfgGroupCreate(RootWnd *cfggroup, const char *attrname) {
  _voidcall(CFGITEM_ONCFGGROUPCREATE, cfggroup, attrname);
}

inline void CfgItem::onCfgGroupDelete(RootWnd *cfggroup) {
  _voidcall(CFGITEM_ONCFGGROUPDELETE, cfggroup);
}

inline int CfgItem::getNumChildren() {
  return _call(CFGITEM_GETNUMCHILDREN, 0);
}

inline CfgItem *CfgItem::enumChild(int n) {
  return _call(CFGITEM_ENUMCHILD, (CfgItem*)NULL, n);
}

inline
GUID CfgItem::getParentGuid() {
  return _call(CFGITEM_GETPARENTGUID, INVALID_GUID);
}

inline void CfgItem::onRegister() { _voidcall(CFGITEM_ONREGISTER); }
inline void CfgItem::onDeregister() { _voidcall(CFGITEM_ONDEREGISTER); }

inline
int CfgItem::getAttributeType(const char *name) {
  return _call(CFGITEM_GETATTRIBUTETYPE, 0, name);
}

inline
const char *CfgItem::getAttributeConfigGroup(const char *name) {
  return _call(CFGITEM_GETATTRIBUTECONFIGGROUP, (const char *)NULL, name);
}

inline
int CfgItem::getDataLen(const char *name) {
  return _call(CFGITEM_GETDATALEN, -1, name);
}

inline
int CfgItem::getData(const char *name, char *data, int data_len) {
  return _call(CFGITEM_GETDATA, -1, name, data, data_len);
}

inline
int CfgItem::setData(const char *name, const char *data) {
  return _call(CFGITEM_SETDATA, -1, name, data);
}

inline int _intVal(CfgItem *cfgitem, const char *name, int def_val=0) {
  if (cfgitem == NULL) return def_val;
  return cfgitem->getDataAsInt(name, def_val);
}
#define _int_getValue _intVal

//CUT kill these
inline void _int_setValue(CfgItem *cfgitem, const char *name, int val) {
  cfgitem->setDataAsInt(name, val);
}
// CfgItemI is in cfgitemi.h if you need it


#endif
