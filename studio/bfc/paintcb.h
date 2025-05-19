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

#ifndef _PAINTCB_H
#define _PAINTCB_H

#include "depview.h"
#include "dispatch.h"

class RootWnd;
class Canvas;
class Region;

class COMEXP PaintCallbackInfo : public Dispatchable {
  public:
    Canvas *getCanvas();
    Region *getRegion();

  enum {
    PAINTCBINFO_GETCANVAS = 10,
    PAINTCBINFO_GETREGION = 20,
  };

};

inline Canvas *PaintCallbackInfo::getCanvas() {
  return _call(PAINTCBINFO_GETCANVAS, (Canvas *)NULL);
}

inline Region *PaintCallbackInfo::getRegion() {
  return _call(PAINTCBINFO_GETREGION, (Region *)NULL);
}

class COMEXP PaintCallbackInfoI : public PaintCallbackInfo {
  public:
    PaintCallbackInfoI(Canvas *_canvas, Region *_region) : canvas(_canvas), region(_region)  {}
    virtual ~PaintCallbackInfoI() {}

    virtual Canvas *getCanvas() { return canvas; }
    virtual Region *getRegion() { return region; }

  private:

    Canvas *canvas;
    Region *region;

  protected:
    RECVS_DISPATCH;
};

class COMEXP PaintCallback : DependentViewerTPtr<RootWnd> {
public:
  PaintCallback() { wnd = NULL; };
  PaintCallback(RootWnd *w);
  virtual ~PaintCallback();

  virtual void monitorWindow(RootWnd *w);
  virtual int viewer_onEvent(RootWnd *item, int event, int param, void *ptr, int ptrlen);
  virtual int viewer_onItemDeleted(RootWnd *item);

  // override those
  virtual void onBeforePaint(PaintCallbackInfo *info) { }
  virtual void onAfterPaint(PaintCallbackInfo *info) { }
  virtual void onWindowDeleted(RootWnd *w)=0; // warning, pointer invalid

  enum {
    BEFOREPAINT = 10,
    AFTERPAINT = 20,
  };

private:
  RootWnd *wnd;
};

#endif
