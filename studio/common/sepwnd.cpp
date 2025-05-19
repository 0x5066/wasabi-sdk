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

#include "../studio/api.h"
#include "../bfc/bitmap.h"
#include "../bfc/basewnd.h"
#include "sepwnd.h"
#include "../bfc/canvas.h"

SepWnd::SepWnd() {
  bitmap = NULL;
  orientation = SEP_UNKNOWN;
}

SepWnd::~SepWnd() {
  if (bitmap) delete bitmap;
}

int SepWnd::onPaint(Canvas *canvas) {

  PaintBltCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaintNC(this)) return 0;
    canvas = &paintcanvas;
  }


  if (!bitmap) {
    switch (orientation) {
      case SEP_VERTICAL:
        bitmap = new SkinBitmap("studio.FrameVerticalDivider");
        break;
      case SEP_HORIZONTAL:
        bitmap = new SkinBitmap("studio.FrameHorizontalDivider");
        break;
      case SEP_UNKNOWN:
        return 1;
    }
    ASSERT(bitmap != NULL);
  }
  RECT r;
  getClientRect(&r);
  RenderBaseTexture(canvas, r);
  if (bitmap) {
    bitmap->stretchToRectAlpha(canvas, &r, 128);
  }
  return 1;
}

int SepWnd::setOrientation(int which) {
  orientation = which;
  return 1;  
}

int SepWnd::onInit() {
  SEPWND_PARENT::onInit();
  return 1;
}

void SepWnd::freeResources() {
  SEPWND_PARENT::freeResources();
  if (bitmap) delete bitmap;
  bitmap = NULL;
}