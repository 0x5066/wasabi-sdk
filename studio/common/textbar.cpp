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


#include "textbar.h"
#include "../bfc/canvas.h"
#include "../bfc/string.h"
#include "../bfc/skinclr.h"
#include "../bfc/autobitmap.h"
#include "checkwnd.h"

static SkinColor bgcolor("studio.textBar.background", "Text backgrounds");
static SkinColor fgcolor("studio.textBar.text");

TextBar::TextBar() {
  size = 16;
  usebt = 0;
  centered = 0;
  checkwndtarget = NULL;

  textshadowed = 1; // display a shadow of the text in bgcolor.  default: on
  textoutlined = 0; // draw an outline of the text in bgcolor.  default: off
  drawbox = 0;      // draw a box of bgcolor the size of the boundsrect.  default: off

//  bgbitmap = "studio.textBar.background";
}

int TextBar::onLeftButtonDown(int x, int y) {
  TEXTBAR_PARENT::onLeftButtonDown(x, y);
  if (checkwndtarget) checkwndtarget->toggle();
  return 1;
}

void TextBar::setUseBaseTexture(int u) {
  usebt = u;
  invalidate();
}

int TextBar::onPaint(Canvas *canvas) {
  RECT r;

  PaintCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaint(this)) return 0;
    canvas = &paintcanvas;
  }
  TEXTBAR_PARENT::onPaint(canvas);

  getClientRect(&r);

  if (!usebt) {
    if (drawbox) {
      canvas->fillRect(&r, bgcolor);
    }
/*
    if (bgbitmap.getBitmap()->isInvalid())
      canvas->fillRect(&r, bgcolor);
    else {
      RECT br;
      br.left = 0;
      br.top = 0;
      br.right = bgbitmap.getWidth();
      br.bottom = bgbitmap.getHeight();
      bgbitmap.getBitmap()->blitToRect(canvas, &br, &r, 255);
    }
*/
  } else
    renderBaseTexture(canvas, r);

  const char *name = getName();

  if (name != NULL) {
    canvas->setTextOpaque(FALSE);
    canvas->pushTextSize(size);
    int w, h;
    canvas->getTextExtent(name, &w, &h);
    int y = (r.bottom-r.top - h) / 2;
    int x = centered ? (r.right-r.left - w) / 2 : TEXTBAR_LEFTMARGIN;
    if (!drawbox && textoutlined) {
      canvas->setTextColor(bgcolor);
      canvas->textOut(r.left+x+1, r.top+y+1, getName());
      canvas->setTextColor(bgcolor);
      canvas->textOut(r.left+x+1, r.top+y-1, getName());
      canvas->setTextColor(bgcolor);
      canvas->textOut(r.left+x-1, r.top+y+1, getName());
      canvas->setTextColor(bgcolor);
      canvas->textOut(r.left+x-1, r.top+y-1, getName());
    } else if (!drawbox && textshadowed) {
      canvas->setTextColor(bgcolor);
      canvas->textOut(r.left+x+1, r.top+y+1, getName());
    }
    canvas->setTextColor(fgcolor);
    canvas->textOut(r.left+x, r.top+y, getName());
    canvas->popTextSize();
  }
  return 1;
}

int TextBar::setTextSize(int newsize) {
  if (newsize < 1 || newsize > 72) return 0;
  size = newsize;
  invalidate();
  return 1;
}

int TextBar::setInt(int i) {
  setName(StringPrintf(i));
  invalidate();
  return 1;
}

void TextBar::onSetName() {
  TEXTBAR_PARENT::onSetName();
  invalidate();
}

int TextBar::getTextWidth() {
  if (!getName()) return 0;
  BltCanvas *c = new BltCanvas(10, 10);
  c->pushTextSize(size);
  int r = c->getTextWidth(getName());
  c->popTextSize();
  delete c;
  return r+4;
}

int TextBar::getTextHeight() {
  return size;
}

void TextBar::setCenter(int center) {
  int was = centered;
  centered = !!center;
  if (was != centered) invalidate();
}

BOOL TextBar::getCentered() {
  return centered;
}
