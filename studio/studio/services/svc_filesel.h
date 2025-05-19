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

#ifndef _SVC_FILESEL_H
#define _SVC_FILESEL_H

#include "../../bfc/dispatch.h"
#include "services.h"

class RootWnd;

class NOVTABLE svc_fileSelector : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::FILESELECTOR; }

  int testPrefix(const char *prefix) {
    return _call(TESTPREFIX, 0, prefix);
  }
  const char *getPrefix() {
    return _call(GETPREFIX, "");
  }
  int setTitle(const char *title) {
    return _call(SETTITLE, 0, title);
  }
  int setExtList(const char *ext) {
    return _call(SETEXTLIST, 0, ext);
  }
  int runSelector(RootWnd *parWnd, int type, int allow_multiple, const char *ident=NULL, const char *default_dir=NULL) {
    return _call(RUNSELECTOR, 0, parWnd, type, allow_multiple, ident, default_dir);
  }
  int getNumFilesSelected() {
    return _call(GETNUMFILESSELECTED, 0);
  }
  const char *enumFilename(int n) {
    return _call(ENUMFILENAME, "", n);
  }
  const char *getDirectory() {
    return _call(GETDIRECTORY, "");
  }

protected:
  enum {
    TESTPREFIX=0,
    GETPREFIX=10,
    SETTITLE=20,
    SETEXTLIST=30,
    RUNSELECTOR=40,
    GETNUMFILESSELECTED=50,
    ENUMFILENAME=60,
    GETDIRECTORY=70,
  };
};

namespace FileSel {
  enum {
    OPEN=1, SAVEAS=2,
  };
};


class NOVTABLE svc_fileSelectorI : public svc_fileSelector {
public:
  virtual int testPrefix(const char *prefix)=0;
  virtual const char *getPrefix()=0;
  virtual int setTitle(const char *title) { return 0; }
  virtual int setExtList(const char *ext) { return 0; }
  virtual int runSelector(RootWnd *parWnd, int type, int allow_multiple, const char *ident=NULL, const char *default_dir=NULL)=0;
  virtual int getNumFilesSelected()=0;
  virtual const char *enumFilename(int n)=0;
  virtual const char *getDirectory()=0;

protected:
  RECVS_DISPATCH;
};

#include "../../bfc/svc_enum.h"
#include "../../bfc/string.h"

class FileSelectorEnum : public SvcEnumT<svc_fileSelector> {
public:
  FileSelectorEnum(const char *_prefix="files") : prefix(_prefix) { }

protected:
  virtual int testService(svc_fileSelector *svc) {
    return prefix.isempty() || svc->testPrefix(prefix);
  }

private:
  String prefix;
};

#endif
