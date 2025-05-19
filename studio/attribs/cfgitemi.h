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

#ifndef _CFGITEMI_H
#define _CFGITEMI_H

#include "cfgitem.h"

#include "../bfc/named.h"
#include "../bfc/ptrlist.h"
#include "../bfc/depend.h"

#include "../common/multimap.h"

class AttrCallback;
class Attribute;

// this is the one you inherit from/use
/**
  
  @short Configuration Item
  @ver 1.0
  @author Nullsoft
  @see Attribute
  @see _bool
  @see _int
  @see _float
  @see _string
*/
class CfgItemI : public CfgItem, public DependentI, private Named {
public:
  /**
    Optionally sets the name and the GUID of the
    configuration item if they are specified
    upon creation of the object.
    
    @param name Name of the configuration item.
    @param guid GUID of the configuration item.
  */
  CfgItemI(const char *name=NULL, GUID guid=INVALID_GUID);
  
  /**
    Does nothing.
  */
  virtual ~CfgItemI();

  /**
    Get the name of the configuration item.
    
    @ret Name of the configuration item.
  */
  const char *cfgitem_getName();
  
  /**
    Get the GUID of the configuration item.
    
    @ret GUID of the configuration item.
  */
  GUID cfgitem_getGuid();
  
  /**
    Get the number of attributes registered
    to this configuration item.
    
    @ret Number of attributes.
  */
  int cfgitem_getNumAttributes();
  
  /**
    Enumerate the attributes registered
    with this configuration item.
    
    @ret 
  */
  const char *cfgitem_enumAttribute(int n);

  const char *cfgitem_getConfigXML();
  virtual void cfgitem_onCfgGroupCreate(RootWnd *cfggroup, const char *attrname) {}
  virtual void cfgitem_onCfgGroupDelete(RootWnd *cfggroup) {}

  virtual int cfgitem_getNumChildren();
  virtual CfgItem *cfgitem_enumChild(int n);
  virtual GUID cfgitem_getParentGuid();

  virtual void cfgitem_onRegister();
  virtual void cfgitem_onDeregister();

  int cfgitem_getAttributeType(const char *name);
  const char *cfgitem_getAttributeConfigGroup(const char *name);
  int cfgitem_getDataLen(const char *name);
  int cfgitem_getData(const char *name, char *data, int data_len);
  int cfgitem_setData(const char *name, const char *data);

  // override these to catch notifications from attribs, call down
  virtual int cfgitem_onAttribSetValue(Attribute *attr);

protected:
  void cfgitem_setGUID(GUID guid);

public:
  int setName(const char *name);
  int registerAttribute(Attribute *attr, AttrCallback *acb=NULL);
  // does not call delete on the attribute
  int deregisterAttribute(Attribute *attr);
  void deregisterAll();

  void addCallback(Attribute *attr, AttrCallback *acb);

protected:
  RECVS_DISPATCH;

  Attribute *getAttributeByName(const char *name);
 
  void addChildItem(CfgItemI *child);

  void setCfgXml(const char *groupname);

  void setParentGuid(GUID guid);

private:
  Dependent *cfgitem_getDependencyPtr() { return this; };
  virtual void *dependent_getInterface(const GUID *classguid);

  PtrList<Attribute> attributes;
  MultiMap<Attribute*, AttrCallback> callbacks;	//CUT
  PtrList<CfgItemI> children;
  String cfgxml;
  GUID myguid, parent_guid;
};

#endif
