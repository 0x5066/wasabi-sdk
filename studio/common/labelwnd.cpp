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


#include "labelwnd.h"
#include "xlatstr.h"

#include "../studio/api.h"
#include "../studio/paintsets.h"

#include "../bfc/canvas.h"
#include "../bfc/skinclr.h"
#include "../bfc/notifmsg.h"

static SkinColor labelfg("common.labelwnd.foreground");
static SkinColor labelbg("common.labelwnd.background", "Text backgrounds");

#define DEF_LABEL_HEIGHT 0
#define DEF_LABEL_FONTSIZE 16

LabelWnd::LabelWnd() {
  show_label = FALSE;
  labelsize = DEF_LABEL_FONTSIZE;
  labelHeight = 0;
  margin=0;
  setVirtual(0);
}

void LabelWnd::getClientRect(RECT *r) {
  LABELWND_PARENT::getClientRect(r);
  r->top += getLabelHeight();
}

int LabelWnd::onResize() {
  LABELWND_PARENT::onResize();
  invalidateLabel();
/*  if (getLabelHeight() <= 0) return 0;
  RECT r,ir;
  LABELWND_PARENT::getClientRect(&r);
  LABELWND_PARENT::getNonClientRect(&ir);
  ir.bottom = ir.top + getLabelHeight()+margin;
  invalidateRect(&ir);*/
  return 1;
}

int LabelWnd::onPaint(Canvas *canvas) {
  if (getLabelHeight() <= 0) return LABELWND_PARENT::onPaint(canvas);

  PaintBltCanvas paintcanvas;
  if (canvas == NULL) {
    if (!paintcanvas.beginPaintNC(this)) return 0;
    canvas = &paintcanvas;
  }

  RECT r;
  LabelWnd::getNonClientRect(&r);

  if (canvas->isFixedCoords()) { // handle possible double buffer
    // convert to canvas coords
    r.right -= r.left; r.left = 0;
    r.bottom -= r.top; r.top = 0;
  }

  r.bottom = r.top + getLabelHeight();

  if (margin) {
    r.left+=margin;
    r.right-=margin;
    r.bottom+=margin*2;
  }

  LABELWND_PARENT::onPaint(canvas);
  int got_focus = gotFocus() || forceFocus();

  if (wantRenderBaseTexture()) {
    api->skin_renderBaseTexture(getBaseTextureWindow(), canvas, r, this);
  }

  api->paintset_render(Paintset::LABEL, canvas, &r, got_focus ? 255 : 64);

  canvas->setTextOpaque(FALSE);
  canvas->pushTextSize(getLabelHeight()-1);
  const char *name = getName();
  if (name == NULL || *name == '\0') name = "Label";
#define LEFTMARGIN 3
  canvas->pushTextColor(labelbg);
  canvas->textOut(r.left+LEFTMARGIN+1, r.top+1, _(name));
  canvas->popTextColor();
  canvas->pushTextColor(labelfg);
  canvas->textOut(r.left+LEFTMARGIN, r.top, _(name));
  canvas->popTextColor();
  canvas->popTextSize();

  return 1;
}

void LabelWnd::onSetName() {
  LABELWND_PARENT::onSetName();
  // make sure label gets repainted
  if (isInited()) {
    RECT r;
    LabelWnd::getNonClientRect(&r);
    r.bottom = r.top + getLabelHeight();
    invalidateRect(&r);
  }
}

//CUTint LabelWnd::childNotify(RootWnd *child, int msg, int param1, int param2) {
//CUT  switch (msg) {
//CUT    case CHILD_WINDOWSHADE_CAPABLE: return show_label;
//CUT    case CHILD_WINDOWSHADE_ENABLE: return TRUE;
//CUT  }
//CUT  return LABELWND_PARENT::childNotify(child, msg, param1, param2);
//CUT}

int LabelWnd::showLabel(int show) {
  show_label = show;
  setFontSize(-1);
  if (isPostOnInit()) {
    onResize();
  }
  return 1;
}

int LabelWnd::getLabelHeight() {
  return show_label ? labelHeight : 0;
}

void LabelWnd::setMargin(int newmargin) {
  margin = newmargin;
  RECT r;
  getNonClientRect(&r);
  r.bottom = getLabelHeight()+margin;
  invalidateRect(&r);
}

int LabelWnd::onGetFocus() {
  LABELWND_PARENT::onGetFocus();
  invalidateLabel();
  return 1;
}

int LabelWnd::onKillFocus() {
  LABELWND_PARENT::onKillFocus();
  invalidateLabel();
  return 1;
}

void LabelWnd::invalidateLabel() {
  if (labelHeight <= 0) return;
  RECT ncr;
  RECT cr;
  RECT lr;
  LabelWnd::getNonClientRect(&ncr);
  LabelWnd::getClientRect(&cr);
  SubtractRect(&lr, &ncr, &cr); // PORT ME
  invalidateRect(&lr);
}

int LabelWnd::wantFocus() {
  return (labelHeight > 0);
}

void LabelWnd::reloadResources() {
  LABELWND_PARENT::reloadResources();
  if (isPostOnInit())
    onResize();
  invalidateLabel();
}

int LabelWnd::setFontSize(int size) {
  LABELWND_PARENT::setFontSize(size);
  HDC dc = GetDC(NULL); // Nonportable. 
  DCCanvas *c = new DCCanvas(dc);
  c->pushTextSize(labelsize+api->metrics_getDelta());
  labelHeight = c->getTextHeight() + 1;
  c->popTextSize();
  delete c;
  ReleaseDC(NULL, dc);
  invalidate();
  if (isPostOnInit()) onResize();
  return 1;
}

