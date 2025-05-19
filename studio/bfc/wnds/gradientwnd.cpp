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


#include "gradientwnd.h"
#include "../canvas.h"
#include <math.h>//floor

// NOTE
// this doesn't work, it's not even close to working, I'm just playing with it

class GradientPoint {
public:
  GradientPoint(float p, ARGB32 c) : pos(p), color(c) {}
  float pos;
  ARGB32 color;
};

GradientWnd::GradientWnd() {
  last_w = last_h = -1;
  recreate = 1;
  gradient = NULL;
  x1 = y1 = 0.f;
  x2 = y2 = 1.f;
  list.addItem(new GradientPoint(0., 0xff00ff00));
  list.addItem(new GradientPoint(1., 0x00ff00ff));
}

GradientWnd::~GradientWnd() {
  list.deleteAll();
  delete gradient;
}

inline unsigned int LERP(unsigned int a, unsigned int b, float p) {
  ASSERT(p >= 0);
  ASSERT(p <= 1.f);
  unsigned int ret = (unsigned int)((float)b * p + (float)a * (1.f - p));
  return ret;
}

int GradientWnd::onPaint(Canvas *canvas) {
  ASSERT(canvas != NULL);
  RECT cr = clientRect();
  int w = cr.right - cr.left, h = cr.right - cr.left;
  if (w != last_w || h != last_h) {
    delete gradient;
    gradient = new BltCanvas(w, h, 32);
    recreate = 1;
  }

  if (recreate) {
    int nitems = list.getNumItems();
//    if (nitems <= 1) {
//      ARGB32 color = nitems ? list.q(0)->color : RGB(0,0,0);
//      gradient->fillBits(color);
//    } else {
      ARGB32 *bits = static_cast<ARGB32*>(gradient->getBits());
//ASSERT(x1 - x2 > 0.1);
if (x1 - x2 < 0.05) x2 = x1+0.05;
      // convert to y = mx + b
      float m = (y2 - y1)/(x2 - x1);
      float b = m * x1 - y1;
      m = -m;	// invert the slope

nitems = 2;
      MemBlock<float> pos(nitems);
      for (int py = 0; py < h; py++) {
        // project all the color points onto this scanline
        // need x and y given pos
//        float x = LERP(x1, x2, gp->pos);
//        float y = LERP(y1, y2, gp->pos);

pos[0] = 0; pos[1] = w-1;
        

//        for (int i = 0; i < nitems; i++) {
//        }
#if 0
        GradientPoint *point = list.q(
        float cx = (py - b) / m;
y - b = mx
x = (y - b) / m
y = mx + b
#endif
        renderGrad(bits+py*w, w, 2, NULL);
      }
//    }
    gradient->makeAlpha();
  }

  gradient->getSkinBitmap()->blitAlpha(canvas, cr.left, cr.top, getPaintingAlpha());
//canvas->drawRect(&cr, 1, RGB(255,255,255));
  recreate = 0;

  return 1;
}

inline ARGB32 colorLerp(ARGB32 color1, ARGB32 color2, float pos) {
  unsigned int a1 = (color1>>24) & 0xff;
  unsigned int a2 = (color2>>24) & 0xff;
  unsigned int r1 = (color1>>16) & 0xff;
  unsigned int r2 = (color2>>16) & 0xff;
  unsigned int g1 = (color1>>8) & 0xff;
  unsigned int g2 = (color2>>8) & 0xff;
  unsigned int b1 = (color1) & 0xff;
  unsigned int b2 = (color2) & 0xff;
  return (LERP(a1, a2, pos)<<24) | (LERP(r1, r2, pos) << 16) | (LERP(g1,g2,pos)<<8) | LERP(b1, b2, pos);
}

void GradientWnd::renderGrad(ARGB32 *bits, int len, int npos, int *positions) {
  ARGB32 color1, color2;
#if 0
  int pp = 0;
  ASSERT(npos >= 2);
  if (x < positions[0]) {
    color1 = color2 = list.q(0)->color;
  }
  for (x = 0; x < len; x++) {
    for (int i = 0; i < npos; i++) {
      if (x < positions[i]) {
        color1 = list[i-1]->color;
      }
    }
    *bits = 
  }
  while (len-- > 0) *bits++ = Std::random();
#endif
//__asm int 3;
  color1 = 0xff00ff00;
  color2 = 0x00ff0000;
  for (int x = 0; x < len; x++) {
    *bits ++ = colorLerp(color1, color2, (float)x / (float)(len-1));
  }
}
