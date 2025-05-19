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

#include "backbufferwnd.h"
#include "../../bfc/canvas.h"
#include "../../studio/api.h"
#include "../region.h"

// -----------------------------------------------------------------------
BackBufferWnd::BackBufferWnd() {
  backbuffer = 0;
  canvas_w = -1;
  canvas_h = -1;
  back_buffer = NULL;
}               

// -----------------------------------------------------------------------
BackBufferWnd::~BackBufferWnd() {
  delete back_buffer;
}

//------------------------------------------------------------------------
BltCanvas *BackBufferWnd::getBackBuffer() {
  return back_buffer;
}

// -----------------------------------------------------------------------
int BackBufferWnd::onPaint(Canvas *canvas) {

  BBWND_PARENT::onPaint(canvas);

  if (!canvas) return 1;

  RECT r;
  getClientRect(&r);

  if (back_buffer && r.right-r.left > 0 && r.bottom -r.top > 0) {

    int w = r.right-r.left;
    int h = r.bottom-r.top;

    if (canvas_w != w || canvas_h != h) {
      delete back_buffer;
      back_buffer = new BltCanvas(w, h);
      canvas_w = w;
      canvas_h = h;
    }

    RegionI *reg = new RegionI;
    canvas->getClipRgn(reg);
    back_buffer->selectClipRgn(reg);
    canvas->blit(r.left, r.top, back_buffer, 0, 0, w, h);
    back_buffer->selectClipRgn(NULL);
    delete reg;
  }

  return 1;
}

int BackBufferWnd::onSiblingInvalidateRgn(Region *r, RootWnd *who, int who_idx, int my_idx) {
  if (who_idx >= my_idx || !wantBackBuffer()) return 0;

  RECT rr;
  getClientRect(&rr);

  Region *_r = getRegion();
  RegionI *__r=NULL;

  if (!_r) {
    __r = new RegionI();
    _r = __r;
    _r->setRect(&rr);
  } else {
    _r->offset(rr.left, rr.top);
  }

  int intersect = _r->intersectRgn(r);
  if (intersect)
    r->add(_r);

  delete __r;

  return intersect;
}

