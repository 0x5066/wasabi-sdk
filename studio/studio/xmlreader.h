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

#ifndef _XMLREADER_H
#define _XMLREADER_H

#include "../bfc/tlist.h"
#include "../bfc/ptrlist.h"
#include "../bfc/string.h"
#include "../bfc/stack.h"
#include "../bfc/dispatch.h"

using namespace wasabi;

class svc_xmlProvider;

class XmlReaderParams : public Dispatchable {
protected:
  XmlReaderParams() {} // protect constructor

public:
  const char *getItemName(int i) { return _call(GETITEMNAME, "", i); }
  const char *getItemValue(int i) { return _call(GETITEMVALUE, "", i); }
  const char *getItemValue(const char *name) { return _call(GETITEMVALUE2, "", name); }
  const char *enumItemValues(const char *name, int nb) { return _call(ENUMITEMVALUES, (const char *)NULL, name, nb); }
  int getItemValueInt(const char *name, int def=0) { return _call(GETITEMVALUEINT, def, name, def); }
  int getNbItems() { return _call(GETNBITEMS, 0); }
  void addItem(const char *parm, const char *value) { _voidcall(ADDITEM, parm, value); }

  enum {
    GETITEMNAME     = 100,
    GETITEMVALUE    = 200,
    GETITEMVALUE2   = 201,
    ENUMITEMVALUES  = 202,
    GETITEMVALUEINT = 300,
    GETNBITEMS      = 400,
    ADDITEM         = 500,
  };
};

class XmlReaderParamsI : public XmlReaderParams {
public:
  virtual ~XmlReaderParamsI() { parms_list.deleteAll(); }
  const char *getItemName(int i);
  const char *getItemValue(int i);
  const char *getItemValue2(const char *name);
  const char *enumItemValues(const char *name, int nb);
  int getItemValueInt(const char *name, int def=0);
  int getNbItems();

  void addItem(const char *parm, const char *value);

  RECVS_DISPATCH;

private:
  typedef struct { 
    String parm;
    String value;
  } parms_struct;

  PtrList<parms_struct> parms_list;
};

class XmlReaderCallback : public Dispatchable {
protected:
  XmlReaderCallback() {} // protect constructor

public:
  void xmlReaderOnStartElementCallback(const char *xmlpath, const char *xmltag, XmlReaderParamsI *params) {
    _voidcall(ONSTARTELEMENT, xmlpath, xmltag, params);
  }
  void xmlReaderOnEndElementCallback(const char *xmlpath, const char *xmltag) {
    _voidcall(ONENDELEMENT, xmlpath, xmltag);
  }
  void xmlReaderOnCharacterDataCallback(const char *xmlpath, const char *xmltag, const char *str) {
    _voidcall(ONCHARDATA, xmlpath, xmltag, str);
  }
  int xmlReaderDisplayErrors() {
    return _call(DISPLAYERRORS, 1);
  }
  int xmlReaderIncludeFiles() {
    return _call(INCLUDEFILES, 1);
  }

  enum {
    ONSTARTELEMENT  = 100,
    ONENDELEMENT    = 200,
    ONCHARDATA      = 300,
    DISPLAYERRORS   = 1000,
    INCLUDEFILES    = 1100,
  };
};

class XmlReaderCallbackI : public XmlReaderCallback {
public:
  virtual void xmlReaderOnStartElementCallback(const char *xmlpath, const char *xmltag, XmlReaderParams *params) { }
  virtual void xmlReaderOnEndElementCallback(const char *xmlpath, const char *xmltag) { }
  virtual void xmlReaderOnCharacterDataCallback(const char *xmlpath, const char *xmltag, const char *str) { }
  virtual int xmlReaderDisplayErrors() { return 1; }
  virtual int xmlReaderIncludeFiles() { return 1; }

#undef CBCLASS
#define CBCLASS XmlReaderCallbackI
START_DISPATCH;
      VCB(ONSTARTELEMENT, xmlReaderOnStartElementCallback);
      VCB(ONENDELEMENT, xmlReaderOnEndElementCallback);
      VCB(ONCHARDATA, xmlReaderOnCharacterDataCallback);
      CB(DISPLAYERRORS, xmlReaderDisplayErrors);
END_DISPATCH;
#undef CBCLASS
};

class XmlReader {
public:
  // matchstr is a regexp string such as "WinampAbstractionLayer/Layer[a-z]"
  // or "Winamp*Layer/*/Layout"
  static void registerCallback(const char *matchstr, XmlReaderCallback *callback);
  static void registerCallback(const char *matchstr, void (*static_callback)(int start, const char *xmlpath, const char *xmltag, XmlReaderParams *params));

  static void unregisterCallback(void *callback);

  // if only_this_class param is specified, only this class will be called back
  // returns 1 on success, 0 on error
  static int loadFile(const char *filename, const char *incpath=NULL, void *only_this_callback=NULL, const char **params=NULL);
  static const char *getIncludePath();

private:
  static void parserCallbackOnStartElement(void *userData, const char *name, const char **parms);
  static void parserCallbackOnEndElement(void *userData, const char *nameC);
  static void parserCallbackOnCharacterData(void *userData, const char *s, int len);

  static const char *peekParam(const char *txt, const char **parms);
  static int doLoadFile(FILE *fp, svc_xmlProvider *svc, const char *filename, const char *incpath, void *only_this_callback, const char **params=NULL);

  typedef enum {
    TYPE_CLASS_CALLBACK,
    TYPE_STATIC_CALLBACK,
  } callbackType;

  typedef struct {
    RecycleString *matchstr;
    callbackType type;
    XmlReaderCallback *callback;
  } cb_struct;

  static TList<cb_struct> callback_list;
  static RecycleString currentpos;
  static RecycleString cont_char;
  static Stack<RecycleString *> include_stack;
};

#endif
