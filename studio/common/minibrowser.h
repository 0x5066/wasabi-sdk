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

#ifndef __MINIBROWSER_H
#define __MINIBROWSER_H

#include "../bfc/dispatch.h"
#include "common.h"

class RootWnd;

class COMEXP MiniBrowserCallback : public Dispatchable {
  public:
    int minibrowsercb_onBeforeNavigate(const char *url, int flags, const char *frame);
    void minibrowsercb_onDocumentComplete(const char *url);

  enum {
    MINIBROWSER_ONBEFORENAVIGATE   = 10,
    MINIBROWSER_ONDOCUMENTCOMPLETE = 20,
  };
};

inline int MiniBrowserCallback ::minibrowsercb_onBeforeNavigate(const char *url, int flags, const char *frame) {
  return _call(MINIBROWSER_ONBEFORENAVIGATE, 0, url, flags, frame);
}

inline void MiniBrowserCallback ::minibrowsercb_onDocumentComplete(const char *url) {
  _voidcall(MINIBROWSER_ONDOCUMENTCOMPLETE, url);
}

class COMEXP MiniBrowserCallbackI : public MiniBrowserCallback {
  public:
    virtual int minibrowsercb_onBeforeNavigate(const char *url, int flags, const char *frame)=0;
    virtual void minibrowsercb_onDocumentComplete(const char *url)=0;

  protected:
    RECVS_DISPATCH;
};

class COMEXP MiniBrowser : public Dispatchable  {

  public:

    RootWnd *minibrowser_getRootWnd();
    int minibrowser_navigateUrl(const char *url);
    int minibrowser_back();
    int minibrowser_forward();
    int minibrowser_home();
    int minibrowser_refresh();
    int minibrowser_stop();
    void minibrowser_setTargetName(const char *name);
    const char *minibrowser_getTargetName();
    const char *minibrowser_getCurrentUrl();
    void minibrowser_addCB(MiniBrowserCallback *cb);
    void minibrowser_setHome(const char *url);

  enum {
    MINIBROWSER_GETROOTWND      =  100,
    MINIBROWSER_NAVIGATEURL     =  200,
    MINIBROWSER_BACK            =  300,
    MINIBROWSER_FORWARD         =  400,
    MINIBROWSER_HOME            =  500,
    MINIBROWSER_REFRESH         =  600,
    MINIBROWSER_STOP            =  700,
    MINIBROWSER_SETTARGETNAME   =  800,
    MINIBROWSER_GETTARGETNAME   =  900,
    MINIBROWSER_GETCURRENTURL   = 1000,
    MINIBROWSER_ADDCB           = 1100,
    MINIBROWSER_SETHOME         = 1200,
  };

};

inline RootWnd *MiniBrowser::minibrowser_getRootWnd() {
  return _call(MINIBROWSER_GETROOTWND, (RootWnd *)NULL);
}

inline int MiniBrowser::minibrowser_navigateUrl(const char *url) {
  return _call(MINIBROWSER_NAVIGATEURL, 0, url);
}

inline int MiniBrowser::minibrowser_back() {
  return _call(MINIBROWSER_BACK, 0);
}

inline int MiniBrowser::minibrowser_forward() {
  return _call(MINIBROWSER_FORWARD, 0);
}

inline int MiniBrowser::minibrowser_home() {
  return _call(MINIBROWSER_HOME, 0);
}

inline int MiniBrowser::minibrowser_refresh() {
  return _call(MINIBROWSER_REFRESH, 0);
}

inline int MiniBrowser::minibrowser_stop() {
  return _call(MINIBROWSER_STOP, 0);
}

inline void MiniBrowser::minibrowser_setHome(const char *url) {
  _voidcall(MINIBROWSER_SETHOME, url);
}

inline void MiniBrowser::minibrowser_setTargetName(const char *name) {
  _voidcall(MINIBROWSER_SETTARGETNAME, name);
}

inline const char *MiniBrowser::minibrowser_getTargetName() {
  return _call(MINIBROWSER_GETTARGETNAME, (const char *)NULL);
}

inline const char *MiniBrowser::minibrowser_getCurrentUrl() {
  return _call(MINIBROWSER_GETCURRENTURL, (const char *)NULL);
}

inline void MiniBrowser::minibrowser_addCB(MiniBrowserCallback *cb) {
  _voidcall(MINIBROWSER_ADDCB, cb);
}

class COMEXP MiniBrowserI : public MiniBrowser {

  public:

    virtual RootWnd *minibrowser_getRootWnd()=0;
    virtual int minibrowser_navigateUrl(const char *url)=0;
    virtual int minibrowser_back()=0;
    virtual int minibrowser_forward()=0;
    virtual int minibrowser_home()=0;
    virtual int minibrowser_refresh()=0;
    virtual int minibrowser_stop()=0;
    virtual void minibrowser_setTargetName(const char *name)=0;
    virtual const char *minibrowser_getTargetName()=0;
    virtual const char *minibrowser_getCurrentUrl()=0;
    virtual void minibrowser_addCB(MiniBrowserCallback *cb)=0;
    virtual void minibrowser_setHome(const char *url)=0;

  protected:
    RECVS_DISPATCH;
};


#endif

