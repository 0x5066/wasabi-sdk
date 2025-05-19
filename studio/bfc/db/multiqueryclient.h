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

#ifndef __MULTIQUERYCLIENT_H
#define __MULTIQUERYCLIENT_H

#include "../dispatch.h"
#include "../depend.h"

class SubQueryServer;

class MultiQueryClient : public Dispatchable {
  public:

    void mqc_onNewMultiQuery(SubQueryServer *modifier=NULL, int flag=0);
    void mqc_onCompleteMultiQuery();
    void mqc_onAddPlaystring(const char *playstring, int totalitems, int thispos);

    Dependent *mqc_getDependencyPtr();
  
    enum {
      MULTIQUERYCLIENT_ONNEWMULTIQUERY=10,
      MULTIQUERYCLIENT_ONCOMPLETEMULTIQUERY=20,
      MULTIQUERYCLIENT_ONADDPLAYSTRING=30,
      MULTIQUERYCLIENT_GETDEPPTR=40,
    };

};

inline void MultiQueryClient::mqc_onNewMultiQuery(SubQueryServer *modifier, int flag) {
  _voidcall(MULTIQUERYCLIENT_ONNEWMULTIQUERY, modifier, flag);
}

inline void MultiQueryClient::mqc_onCompleteMultiQuery() {
  _voidcall(MULTIQUERYCLIENT_ONCOMPLETEMULTIQUERY);
}

inline void MultiQueryClient::mqc_onAddPlaystring(const char *playstring, int totalitems, int thispos) {
  _voidcall(MULTIQUERYCLIENT_ONADDPLAYSTRING, playstring, totalitems, thispos);
}

inline Dependent *MultiQueryClient::mqc_getDependencyPtr() {
  return _call(MULTIQUERYCLIENT_GETDEPPTR, (Dependent *)NULL);
}

class _MultiQueryClientI : public MultiQueryClient {
  public:

    virtual void mqc_onNewMultiQuery(SubQueryServer *modifier=NULL, int flag=0)=0;
    virtual void mqc_onCompleteMultiQuery()=0;
    virtual void mqc_onAddPlaystring(const char *playstring, int totalitems, int thispos)=0;
    virtual Dependent *mqc_getDependencyPtr()=0;

  protected:
    RECVS_DISPATCH;  
};

class MultiQueryClientI : public _MultiQueryClientI {
  public:
    MultiQueryClientI() { }
    virtual ~MultiQueryClientI() { }

    virtual void mqc_onNewMultiQuery(SubQueryServer *modifier=NULL, int flag=0) { }
    virtual void mqc_onCompleteMultiQuery() { }
    virtual void mqc_onAddPlaystring(const char *playstring, int totalitems, int thispos) {  }
};


#endif
