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

#ifndef __XMLOBJECT_H
#define __XMLOBJECT_H

#include "nsguid.h"
#include "../bfc/ptrlist.h"
#include "../bfc/dispatch.h"
#include "../bfc/string.h"

#define XML_ATTRIBUTE_IMPLIED  0
#define XML_ATTRIBUTE_REQUIRED 1

class COMEXP XmlObject : public Dispatchable {

 public:

	int setXmlParam(const char *name, const char *strvalue);
	int getNumXmlParams();
	const char *getXmlParamName(int n);
	const char *getXmlParamValue(int n);
  int getXmlParam(const char *paramname);

 enum {
  SETXMLPARAM=10,
  GETNUMXMLPARAMS=20,
  GETXMLPARAMNAME=30,
  GETXMLPARAMVALUE=40,
  GETXMLPARAM=50,
 };

};

inline int XmlObject::setXmlParam(const char *name, const char *strvalue) {
  return _call(SETXMLPARAM, 0, name, strvalue);
}

inline int XmlObject::getNumXmlParams() {
  return _call(GETNUMXMLPARAMS, 0);
}

inline const char *XmlObject::getXmlParamName(int n) {
  return _call(GETXMLPARAMNAME, (const char *)0, n);
}

inline const char *XmlObject::getXmlParamValue(int n) {
  return _call(GETXMLPARAMVALUE, (const char *)0, n);
}

inline int XmlObject::getXmlParam(const char *paramname) {
  return _call(GETXMLPARAM, 0, paramname);
}

class COMEXP XmlObjectParam {

  public:

    XmlObjectParam(int xmlhandle, const char *xmlattribute, int xmlattributeid, int required=0) : handle(xmlhandle), xmlattributename(xmlattribute), attributeid(xmlattributeid), isrequired(required) {}
    virtual ~XmlObjectParam() {}

    const char *getXmlAttribute() { return xmlattributename; }
    int isRequired() { return isrequired; }
    int getXmlAttributeId() { return attributeid; }
    int getXmlHandle() { return handle; }
    void setLastValue(const char *val) { lastvalue = val; }
    const char *getLastValue() {return lastvalue; }

  private:
    String xmlattributename;
    int isrequired, attributeid;
    int handle;
    String lastvalue;
};

class COMEXP XmlObjectParamSort {
public:
  static int compareItem(XmlObjectParam *p1, XmlObjectParam*p2) {
    return STRICMP(p1->getXmlAttribute(), p2->getXmlAttribute());
  }
  static int compareAttrib(const char *attrib, XmlObjectParam *item) {
    return STRICMP(attrib, item->getXmlAttribute());
  }
};

class COMEXP XmlObjectI : public XmlObject {

 public:

  XmlObjectI();
  virtual ~XmlObjectI();

	virtual int setXmlParam(const char *name, const char *strvalue); // receives from system
	virtual int setXmlParamById(int xmlhandle, int xmlattributeid, const char *name, const char *strvalue); // distributes to inheritors
	virtual int getNumXmlParams();
	virtual const char *getXmlParamName(int n);
	virtual const char *getXmlParamValue(int n);
  virtual int getXmlParam(const char *paramname); 

  void addParam(int xmlhandle, const char *xmlattribute, int xmlattributeid, int isrequired=0) {
    addXmlParam(xmlhandle, xmlattribute, xmlattributeid, isrequired);
  }

  void addXmlParam(int xmlhandle, const char *xmlattribute, int xmlattributeid, int isrequired=0);

  virtual int onUnknownXmlParam(const char *param, const char *value); 

  int newXmlHandle();

  private:

    XmlObjectParam *enumParam(int n) { return params[n]; }

    PtrListQuickSorted<XmlObjectParam, XmlObjectParamSort> params;
    int handlepos;

 protected:

  RECVS_DISPATCH;
};


#endif
