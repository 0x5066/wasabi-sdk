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

#ifndef __SpyWnd_H
#define __SpyWnd_H

#include "../../common/guiobjwnd.h"
#include "../../bfc/paintcb.h"

#define  SpyWnd_PARENT GuiObjectWnd

// -----------------------------------------------------------------------
class SpyWnd : public SpyWnd_PARENT, public PaintCallback {
  
  public:

    SpyWnd();
    virtual ~SpyWnd();

    // BaseWnd

    virtual int onPaint(Canvas *c); // this is for painting our object
    virtual void onWindowDeleted(RootWnd *w); // warning, pointer invalid

    // XuiObject automatically calls this back for all parameters registered using addParam
    // encountered in the xml source
    virtual int setXuiParam(int xuihandle, int xmlattributeid, const char *xmlattributename, const char *value);

    // those are called by yourself in setXuiParam
    void setLayout(const char *elementname);
//    void setSomeMoreOption(const char *paramvalue);
//    void setSomeMoreOption(const char *paramvalue);
    virtual int onInit();
    virtual void onBeforePaint(PaintCallbackInfo *info);
    virtual void onAfterPaint(PaintCallbackInfo *info);

  private:

    void hook();

    // a list of IDs for our xml attributes, we use them in addParam() in the constructor
    enum {
       SpyWnd_SETLAYOUT        = 0,
  //     SpyWnd_SOMEMOREOPTION,
  //     SpyWnd_SOMEMOREOPTION,
    };
    int myxuihandle;
    BltCanvas *canvas;
    int c_w, c_h;
    String cname, lname;
    RootWnd *wnd;
};


// -----------------------------------------------------------------------
extern char SpyWndXuiObjectStr[];
extern char SpyWndXuiSvcName[];
class SpyWndXuiSvc : public XuiObjectSvc<SpyWnd, SpyWndXuiObjectStr, SpyWndXuiSvcName> {};

#endif
