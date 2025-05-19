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

#include "qpaintwnd.h"
#include "../../bfc/canvas.h"
#include "../../studio/api.h"

#define TIMER_QUICKPAINT 0x650

// -----------------------------------------------------------------------
QuickPaintWnd::QuickPaintWnd() {
  realtime = 1;
  canvas_w = -1;
  canvas_h = -1;
  timerset = 0;
  speed = 25;
  enabled = 0;
  render_canvas = NULL;
  sfe = new SkinFilterEnum();
  svc_skinFilter *obj;
  while (1) {
    obj = sfe->getNext();    
    if (!obj) break;
    filters.addItem(obj);
  }
  invalidated = 0;
}

// -----------------------------------------------------------------------
QuickPaintWnd::~QuickPaintWnd() {
  foreach(filters)
    sfe->release(filters.getfor());
  endfor;
  delete sfe;
  if (timerset)
    killTimer(TIMER_QUICKPAINT);
  delete render_canvas;
}

// -----------------------------------------------------------------------
void QuickPaintWnd::setRealtime(int rt) {
  realtime = rt;
}

// -----------------------------------------------------------------------
int QuickPaintWnd::getRealtime() {
  return realtime;
}

// -----------------------------------------------------------------------
void QuickPaintWnd::setSpeed(int ms) {
  speed = ms;
  if (enabled && timerset) {
    killTimer(TIMER_QUICKPAINT);
    setTimer(TIMER_QUICKPAINT, getSpeed());
  }
}

// -----------------------------------------------------------------------
void QuickPaintWnd::startQuickPaint() {
  enabled = 1;
  if (!isInited()) return;
  if (!timerset)
    setTimer(TIMER_QUICKPAINT, getSpeed());
}

// -----------------------------------------------------------------------
void QuickPaintWnd::stopQuickPaint() {
  enabled = 0;
  if (!isInited()) return;
  if (timerset)
    killTimer(TIMER_QUICKPAINT);
}

// -----------------------------------------------------------------------
int QuickPaintWnd::isQuickPainting() {
  return enabled;
}

// -----------------------------------------------------------------------
int QuickPaintWnd::getSpeed() {
  return speed;
}

// -----------------------------------------------------------------------
int QuickPaintWnd::onInit() {
  QUICKPAINTWND_PARENT::onInit();
  if (enabled) {
    ASSERT(!timerset);
    setTimer(TIMER_QUICKPAINT, getSpeed());
    timerset = 1;
  }
  return 1;
}

// -----------------------------------------------------------------------
void QuickPaintWnd::timerCallback(int id) {
  switch (id) {
    case TIMER_QUICKPAINT:
      quickPaint();
      break;
    default:
      QUICKPAINTWND_PARENT::timerCallback(id);
  }
}

// -----------------------------------------------------------------------
void QuickPaintWnd::quickPaint() {

  int w, h;
  getQuickPaintSize(&w, &h);

  if (wantEvenAlignment()) {
    if (w & 1) w++;
    if (h & 1) h++;
  }

  if (w == 0 && h == 0) return; 

  int newone = 0;

  if (canvas_w != w || canvas_h != h) {
    delete render_canvas;
    render_canvas = new BltCanvas(w, wantNegativeHeight() ? -h : h);
    canvas_w = w;
    canvas_h = h;
    newone = 1;
  }

  int repaint = onQuickPaint(render_canvas, canvas_w, canvas_h, newone);

  if (repaint) {

    invalidated = 1;

    if (getRealtime() && isVisible()) 
      cascadeRepaint();
    else
      invalidate();
  }
}

// -----------------------------------------------------------------------
void QuickPaintWnd::getQuickPaintSize(int *w, int *h) {
  RECT r;
  getClientRect(&r);
  if (w) *w = r.right-r.left;
  if (h) *h = r.bottom-r.top;
}

// -----------------------------------------------------------------------
void QuickPaintWnd::getQuickPaintSource(RECT *r) {
  ASSERT(r != NULL);
  r->left = 0;
  r->right = canvas_w;
  r->top = 0;
  r->bottom = canvas_h;
}

// -----------------------------------------------------------------------
void QuickPaintWnd::getQuickPaintDest(RECT *r) {
  ASSERT(r != NULL);
  getClientRect(r);
}

// -----------------------------------------------------------------------
void QuickPaintWnd::onSetVisible(int show) {
  QUICKPAINTWND_PARENT::onSetVisible(show);
  if (!show) {
    if (timerset) {
      killTimer(TIMER_QUICKPAINT);
	    timerset = 0;
    }
  } else {
    if (enabled && !timerset) {
      setTimer(TIMER_QUICKPAINT, getSpeed());
      timerset = 1;
    }
  }
}

// -----------------------------------------------------------------------
int QuickPaintWnd::onPaint(Canvas *canvas) {

  QUICKPAINTWND_PARENT::onPaint(canvas);

  if (!enabled || !render_canvas) return 1;

  RECT r;
  getQuickPaintDest(&r);
  RECT sr;
  getQuickPaintSource(&sr);

  if (invalidated && wantFilters()) {
    foreach(filters)
      filters.getfor()->filterBitmap((unsigned char *)canvas->getBits(), canvas_w, canvas_h, 32, NULL, "Vis/Eq");
    endfor;
    invalidated = 0;
  }

  render_canvas->getSkinBitmap()->stretchToRectAlpha(canvas, &sr, &r, getPaintingAlpha());

  return 1;
}

