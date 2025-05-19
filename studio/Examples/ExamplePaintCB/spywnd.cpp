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

#include "spywnd.h"
#include "../../bfc/canvas.h"
#include "../../bfc/region.h"
#include "../../bfc/paramparser.h"
#include "../../common/script/scriptguid.h"
#include "../../studio/api.h"

// -----------------------------------------------------------------------
char SpyWndXuiObjectStr[] = "PaintSpy:Spy"; // This is the xml tag
char SpyWndXuiSvcName[] = "PaintSpy:Spy XuiObject Service"; // and this is the name of the service

// -----------------------------------------------------------------------
SpyWnd::SpyWnd() {
  myxuihandle = newXuiHandle();
  addParam(myxuihandle, "layout", SpyWnd_SETLAYOUT, XUI_ATTRIBUTE_IMPLIED);
  canvas = NULL;
  c_w = 0;
  c_h = 0;
  wnd = NULL;
}

// -----------------------------------------------------------------------
SpyWnd::~SpyWnd() {
  delete canvas;
  canvas = NULL;
}

// -----------------------------------------------------------------------
int SpyWnd::setXuiParam(int xuihandle, int xmlattributeid, const char *xmlattributename, const char *value) {
  if (xuihandle != myxuihandle)
    return SpyWnd_PARENT::setXuiParam(xuihandle, xmlattributeid, xmlattributename, value);

  switch (xmlattributeid) {
    case  SpyWnd_SETLAYOUT:
      setLayout(value);
      break;
    default:
      return 0;
  }
  return 1;
}

// -----------------------------------------------------------------------
int SpyWnd::onInit() {
  SpyWnd_PARENT::onInit();
  hook();
  return 1;
}

// -----------------------------------------------------------------------
int SpyWnd::onPaint(Canvas *c) {
  RECT r = clientRect();
  if (canvas)
    canvas->getSkinBitmap()->blitAlpha(c, r.left, r.top, getPaintingAlpha());
  return 1;
}

// -----------------------------------------------------------------------
void SpyWnd::setLayout(const char *elementname) {
  ParamParser pp(elementname, ";");
  cname = pp.enumItem(0);
  lname = pp.enumItem(1);
  if (isInited()) hook();
}

// -----------------------------------------------------------------------
void SpyWnd::hook() {
  ASSERT(isInited());
  if (wnd != NULL) {
    monitorWindow(NULL);
    wnd = NULL;
  }
  ScriptObject *cont = api->skin_getContainer(cname);
  ScriptObject *layout = api->skin_getLayout(cont, lname);

  if (layout != NULL) {
    GuiObject *go = static_cast<GuiObject *>(layout->vcpu_getInterface(guiObjectGuid));
    if (go != NULL) {
      wnd = go->guiobject_getRootWnd();
      if (wnd) {
        monitorWindow(wnd);
        RECT or = clientRect();
        RECT r;
        wnd->getNonClientRect(&r);
        int w = r.right-r.left;
        int h = r.bottom-r.top;
        r.left = or.left; r.top = or.top;
        r.right = r.left + w; r.bottom = r.top + h;
        resize(r.left, r.top, r.right-r.left, r.bottom-r.top);
        delete canvas;
        canvas = NULL;
        c_w = r.right-r.left;
        c_h = r.bottom-r.top;
        if (w > 0 && h > 0) {
          canvas = new BltCanvas(c_w, c_h);
          c_w = w;
          c_h = h;
        }
        wnd->cascadeRepaint(); 
      }
    }
  }
}

// -----------------------------------------------------------------------
void SpyWnd::onWindowDeleted(RootWnd *w) { // warning, pointer invalid
  if (wnd == w) wnd = NULL;
}

// -----------------------------------------------------------------------
void SpyWnd::onBeforePaint(PaintCallbackInfo *info) {
}

// -----------------------------------------------------------------------
void SpyWnd::onAfterPaint(PaintCallbackInfo *info) {
  if (!canvas) return;

  Canvas *c = info->getCanvas();
  Region *r = info->getRegion();

  RegionI saved;
  c->getClipRgn(&saved);
  canvas->selectClipRgn(r);
  RECT _r;
  r->getBox(&_r);
  canvas->fillRect(&_r, 0);
  c->blit(0, 0, canvas, 0, 0, c_w, c_h);
  c->selectClipRgn(&saved);

  Region *clone = r->clone();
  RECT cr = clientRect();
  clone->offset(cr.left, cr.top);
  invalidateRgn(clone);
  r->disposeClone(clone);
}

