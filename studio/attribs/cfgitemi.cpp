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


#include "cfgitemi.h"
#include "attrcb.h"
#include "attribute.h"

#include "../bfc/std.h"
#include "../studio/api.h"
#include "../bfc/memblock.h"

CfgItemI::CfgItemI(const char *name, GUID guid) :
  Named(name), myguid(guid), parent_guid(INVALID_GUID) { }

CfgItemI::~CfgItemI() {
  deregisterAll();
}

const char *CfgItemI::cfgitem_getName() {
  return Named::getName();
}

GUID CfgItemI::cfgitem_getGuid() {
  return myguid;
}

int CfgItemI::cfgitem_getNumAttributes() {
  return attributes.getNumItems();
}

const char *CfgItemI::cfgitem_enumAttribute(int n) {
  Attribute *attr = attributes[n];
  if (attr) return attr->getAttributeName();
  return NULL;
}

const char *CfgItemI::cfgitem_getConfigXML() {
  return cfgxml;
}

int CfgItemI::cfgitem_getNumChildren() {
  return children.getNumItems();
}

CfgItem *CfgItemI::cfgitem_enumChild(int n) {
  return children[n];
}

GUID CfgItemI::cfgitem_getParentGuid() {
  return parent_guid;
}

void CfgItemI::cfgitem_onRegister() {
  foreach(children)
    api->config_registerCfgItem(children.getfor());
  endfor
}
void CfgItemI::cfgitem_onDeregister() {
  foreach(children)
    api->config_deregisterCfgItem(children.getfor());
  endfor
}

Attribute *CfgItemI::getAttributeByName(const char *name) {
  Attribute *attr;
  foreach(attributes)
    attr = attributes.getfor();
    if (!STRICMP(name, attr->getAttributeName())) return attr;
  endfor
  return NULL;
}

int CfgItemI::cfgitem_getAttributeType(const char *name) {
  Attribute *attr = getAttributeByName(name);
  if (attr == NULL) return AttributeType::NONE;
  return attr->getAttributeType();
}

const char *CfgItemI::cfgitem_getAttributeConfigGroup(const char *name) {
  Attribute *attr = getAttributeByName(name);
  if (attr == NULL) return NULL;
  return attr->getConfigGroup();
}

int CfgItemI::cfgitem_getDataLen(const char *name) {
  Attribute *attr = getAttributeByName(name);
  if (attr == NULL) return -1;
  return attr->getDataLen();
}

int CfgItemI::cfgitem_getData(const char *name, char *data, int data_len) {
  Attribute *attr = getAttributeByName(name);
  if (attr == NULL) return -1;
  return attr->getData(data, data_len);
}

int CfgItemI::cfgitem_setData(const char *name, const char *data) {
  Attribute *attr = getAttributeByName(name);
  if (attr == NULL) return -1;
  int ret = attr->setDataNoCB(data);
  if (ret) cfgitem_onAttribSetValue(attr);
  return ret;
}

int CfgItemI::cfgitem_onAttribSetValue(Attribute *attr) {
  // notify dependency watchers that an attribute changed
  dependent_sendEvent(CfgItem::depend_getClassGuid(), Event_ATTRIBUTE_CHANGED, 0, (void*)attr->getAttributeName());

  for (int i = 0; ; i++) {
    AttrCallback *acb;
    if (!callbacks.multiGetItem(attr, i, &acb)) break;
    acb->onValueChange(attr);
  }
  
  return 0;
}

void CfgItemI::cfgitem_setGUID(GUID guid) {
  myguid = guid;
}

int CfgItemI::setName(const char *name) {
  Named::setName(name);
//FUCKO: notify dependency watchers that name changed?
  return 1;
}

int CfgItemI::registerAttribute(Attribute *attr, AttrCallback *acb) {
  if (attributes.haveItem(attr)) return 0;
  int ret = attributes.addItem(attr) != NULL;
  if (!ret) return ret;

  attr->setCfgItem(this);

  // set optional callback
  if (acb != NULL) {
    addCallback(attr, acb);
  }

  // notify dependency watchers of new attribute
  dependent_sendEvent(CfgItem::depend_getClassGuid(), Event_ATTRIBUTE_ADDED, 0, (void*)attr->getAttributeName());

  return ret;
}

int CfgItemI::deregisterAttribute(Attribute *attr) {
  if (!attributes.haveItem(attr)) return 0;
  int ret = attributes.removeItem(attr);
  // notify dependency watchers of attribute going away
  dependent_sendEvent(CfgItem::depend_getClassGuid(), Event_ATTRIBUTE_REMOVED, 0, (void*)attr->getAttributeName());

  // remove callbacks
  callbacks.multiDelAllForItem(attr, TRUE);

  return ret;
}

void CfgItemI::addCallback(Attribute *attr, AttrCallback *acb) {
  ASSERT(attr != NULL);
  ASSERT(acb != NULL);
  callbacks.multiAddItem(attr, acb);
}

void CfgItemI::deregisterAll() {
  foreach(children)
    children.getfor()->deregisterAll();
  endfor
  while (attributes.getNumItems()) deregisterAttribute(attributes[0]);
}

void CfgItemI::addChildItem(CfgItemI *child) {
  ASSERT(child != NULL);
  if (!children.haveItem(child)) {
    children.addItem(child);
    child->setParentGuid(myguid);
  }
}

void CfgItemI::setCfgXml(const char *groupname) {
  cfgxml = groupname;
}

void CfgItemI::setParentGuid(GUID guid) {
  parent_guid = guid;
}

void *CfgItemI::dependent_getInterface(const GUID *classguid) {
  HANDLEGETINTERFACE(CfgItem);
  return NULL;
}

#define CBCLASS CfgItemI
START_DISPATCH
  CB(CFGITEM_GETNAME, cfgitem_getName);
  CB(CFGITEM_GETGUID, cfgitem_getGuid);
  CB(CFGITEM_GETNUMATTRIBUTES, cfgitem_getNumAttributes);
  CB(CFGITEM_ENUMATTRIBUTE, cfgitem_enumAttribute);
  CB(CFGITEM_GETCONFIGXML, cfgitem_getConfigXML);
  VCB(CFGITEM_ONCFGGROUPCREATE, cfgitem_onCfgGroupCreate);
  VCB(CFGITEM_ONCFGGROUPDELETE, cfgitem_onCfgGroupDelete);
  CB(CFGITEM_GETNUMCHILDREN, cfgitem_getNumChildren);
  CB(CFGITEM_ENUMCHILD, cfgitem_enumChild);
  CB(CFGITEM_GETPARENTGUID, cfgitem_getParentGuid);
  VCB(CFGITEM_ONREGISTER, cfgitem_onRegister);
  VCB(CFGITEM_ONDEREGISTER, cfgitem_onDeregister);
  CB(CFGITEM_GETATTRIBUTETYPE, cfgitem_getAttributeType);
  CB(CFGITEM_GETATTRIBUTECONFIGGROUP, cfgitem_getAttributeConfigGroup);
  CB(CFGITEM_GETDATALEN, cfgitem_getDataLen);
  CB(CFGITEM_GETDATA, cfgitem_getData);
  CB(CFGITEM_SETDATA, cfgitem_setData);
  CB(CFGITEM_GETDEPENDENCYPTR, cfgitem_getDependencyPtr);
END_DISPATCH
#undef CBCLASS
