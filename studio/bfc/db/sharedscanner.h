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

#ifndef __SHAREDSCANNER_H
#define __SHAREDSCANNER_H

#include "scanner.h"
#include "../dispatch.h"

class dbSvcScanner;

class SharedDbScanner : public Dispatchable {

  public:

    void shareddbscanner_addRef();
    void shareddbscanner_delRef();
    int shareddbscanner_getRefCount();
    dbSvcScanner *shareddbscanner_getScanner();

    enum {
      SHAREDDBSCANNER_ADDREF=10,
      SHAREDDBSCANNER_DELREF=20,
      SHAREDDBSCANNER_GETREFCOUNT=30,
      SHAREDDBSCANNER_GETSCANNER=40,
    };  
};

inline void SharedDbScanner::shareddbscanner_addRef() {
  _voidcall(SHAREDDBSCANNER_ADDREF);
}

inline void SharedDbScanner::shareddbscanner_delRef() {
  _voidcall(SHAREDDBSCANNER_DELREF);
}

inline int SharedDbScanner::shareddbscanner_getRefCount() {
  return _call(SHAREDDBSCANNER_GETREFCOUNT, 0);
}

inline dbSvcScanner *SharedDbScanner::shareddbscanner_getScanner() {
  return _call(SHAREDDBSCANNER_GETSCANNER, (dbSvcScanner *)NULL);
}

class SharedDbScannerI : public SharedDbScanner, public dbScanner {
  public:

    SharedDbScannerI(GUID table_guid, const char *unique_field=NULL) : dbScanner(table_guid, unique_field) { refcount = 0; }
    virtual ~SharedDbScannerI() { }

    virtual void shareddbscanner_addRef() { refcount++; } 
    virtual void shareddbscanner_delRef() { refcount--; }
    virtual int shareddbscanner_getRefCount() { return refcount; }
    virtual dbSvcScanner *shareddbscanner_getScanner() { return getScanner(); }

  private:
    
    int refcount;

  protected:

    RECVS_DISPATCH;
};

#endif