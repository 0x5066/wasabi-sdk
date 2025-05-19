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

#include "slider.h"

#include "../bfc/canvas.h"
#include "../bfc/notifmsg.h"

#include "../studio/api.h"

#define DEFAULT_THUMBWIDTH  16
#define DEFAULT_THUMBHEIGHT 16

SliderWnd::SliderWnd() {
  seeking = 0;
  enabled = 1;
  pos = 0;
  oldpos = -1;
  vertical = 0;
  hilite = 0;
  captured = 0;

  use_base_texture = 0;
  no_default_background = 0;
  base_texture = NULL;
  xShift=0; yShift=0;

  drawOnBorders=0;

  hotPosition=-1;

  origPos = 0;

  thumbCentered=1;
  thumbOffset=0;

  setLimits(START,END);
}

SliderWnd::~SliderWnd() {
}


int SliderWnd::onPaint(Canvas *canvas) {
  PaintBltCanvas paintcanvas;

  if (canvas == NULL) {
    if (!paintcanvas.beginPaint(this)) return 0;
    canvas = &paintcanvas;
  }
  SLIDERWND_PARENT::onPaint(canvas);

  RECT r, or;
  getClientRect(&r);
  or = r;

  if (use_base_texture) {
    if (!base_texture) {
      renderBaseTexture(canvas, r);
    } else {
      RECT cr;
      cr.left = xShift;
      cr.top = yShift;
      cr.right = cr.left + (r.right-r.left);
      cr.bottom = cr.top + (r.bottom-r.top);
      base_texture->blitToRect(canvas, &cr, &r);
    }
  }

  if (vertical) {
    RECT br;
    br.left = r.left;
    br.right = r.right;

    if (left.getBitmap()) {
      br.top = r.top;
      br.bottom = left.getHeight();
      left.getBitmap()->stretchToRectAlpha(canvas, &br, getPaintingAlpha());
    }

    if (right.getBitmap()) {
      br.top = r.bottom-right.getHeight();
      br.bottom = r.bottom;
      right.stretchToRectAlpha(canvas, &br, getPaintingAlpha());
    }

    if (middle.getBitmap()) {
      br.top = r.top + (left.getBitmap() ?left.getHeight():0);
      br.bottom = r.bottom - (right.getBitmap()?right.getHeight():0);
      middle.getBitmap()->stretchToRectAlpha(canvas, &br, getPaintingAlpha());
    }
  } else {
    RECT br;
    br.top = r.top;
    br.bottom = r.bottom;
		 
    if (left.getBitmap()) {
      br.left = r.left;
      br.right = br.left+left.getWidth();
      left.getBitmap()->stretchToRectAlpha(canvas, &br, getPaintingAlpha());
    }

    if (right.getBitmap()) {
      br.left = r.right-right.getWidth();
      br.right = r.right;
      right.getBitmap()->stretchToRectAlpha(canvas, &br, getPaintingAlpha());
    }

    if (middle.getBitmap()) {
      br.left = r.left + (left.getBitmap()?left.getWidth():0);
      br.right = r.right - (right.getBitmap()?right.getWidth():0);
      middle.getBitmap()->stretchToRectAlpha(canvas, &br, getPaintingAlpha());
    }
  }

  if (vertical) {
    int w = (r.bottom - r.top) - thumbHeight();
    ASSERT(w > 0);
    if (w < 0) w = 0;
    r.top += (pos * w) / length;
    r.bottom  = r.top + thumbHeight();
    if(!thumbCentered) {
      r.left=or.left+thumbOffset;
      r.right=or.left+thumbWidth()+thumbOffset;
    } else {
      int w = ((r.right - r.left) - thumbWidth())/2;
      r.left = or.left + w + thumbOffset;
      r.right = or.right - w +thumbOffset;
    }
  } else {
    // offset for left bitmap
    if (!drawOnBorders) {
      if (left.getBitmap() != NULL) r.left += left.getWidth();
      if (right.getBitmap() != NULL) r.right -= right.getWidth();
    }

    int w = (r.right - r.left) - thumbWidth();
    if (w < 0) w = 0;
    r.left += (pos * w) / length;
    r.right = r.left + thumbWidth();
    if (r.right > or.right) {
      r.left -= r.right - or.right;
      r.right = or.right;
    }

    int thumbh = thumb.getBitmap() ? thumb.getHeight() : DEFAULT_THUMBWIDTH;
    if(thumbCentered) {
      int h = ((r.bottom - r.top) - thumbh)/2;
      r.top = or.top + h;
      r.bottom = or.bottom - h;
    } else {
      r.top = or.top+thumbOffset;
      r.bottom = or.top+thumbh+thumbOffset;
    }
  }

  SkinBitmap *sb = getSeekStatus() ? (thumbdown.getBitmap() ? thumbdown.getBitmap() : thumb.getBitmap()) : ((hilite&&thumbhilite.getBitmap()) ? thumbhilite.getBitmap() : thumb.getBitmap());

  if (sb != NULL)
    sb->stretchToRectAlpha(canvas, &r, getPaintingAlpha());
  else
    canvas->fillRect(&r, RGB(255, 0, 0));

  return 1;
}

int SliderWnd::onInit() {
  SLIDERWND_PARENT::onInit();
  if (!no_default_background) {
    if (vertical) {
      // Please note that these bitmaps here do not yet exist.
      if (left.getBitmapName() == NULL) setLeftBmp("studio.seekbar.top");
      if (middle.getBitmapName() == NULL) setMiddleBmp("studio.seekbar.middle");
      if (right.getBitmapName() == NULL) setRightBmp("studio.seekbar.bottom");
    } else {
      if (left.getBitmapName() == NULL) setLeftBmp("studio.seekbar.left");
      if (middle.getBitmapName() == NULL) setMiddleBmp("studio.seekbar.middle");
      if (right.getBitmapName() == NULL) setRightBmp("studio.seekbar.right");
    }
  }

  if (thumb.getBitmapName() == NULL) setThumbBmp("studio.seekbar.button.nonpressed");
  if (thumbdown.getBitmapName() == NULL) setThumbDownBmp("studio.seekbar.button.pressed");

  return 1;
}

int SliderWnd::onLeftButtonDown(int x, int y) {
  SLIDERWND_PARENT::onLeftButtonDown(x, y);
  if (!enabled) return 0;
  seeking = 1;
  
  origPos = 0;
  RECT r;
  getClientRect(&r);
  if(vertical) {
    int w = (r.bottom - r.top) - thumbHeight();
    if (w < 0) w = 0;
    r.top += (pos * w) / length;
    origPos=(y-r.top)-1;
    /*if(origPos<0 || origPos>thumbHeight())*/ origPos=(thumbHeight()/2)-2;
  } else {
		if(!drawOnBorders)
		{
			if (left.getBitmap() != NULL) r.left += left.getWidth();
			if (right.getBitmap() != NULL) r.right -= right.getWidth();
		}
    int w = (r.right - r.left) - thumbWidth();
    if (w < 0) w = 0;
    r.left += (pos * w) / length;
    origPos=(x-r.left)-1;
    if(origPos<0 || origPos>thumbWidth()) origPos=(thumbWidth()/2)-2;
  }

	if(!captured) {
		captured = 1;
		beginCapture();
	}
  oldpos = pos;
  onMouseMove(x, y);
  return 1;
}

//FG>
//removed cross-hierarchy deletion (crashs due to ancestor in common.dll trying to delete pointers in a different 
//heap scope than the one in which they were allocated)
void SliderWnd::setBitmaps(const char *thumbbmp, const char *thumbdownbmp, const char *thumbhighbmp, const char *leftbmp, const char *middlebmp, const char *rightbmp) {
  setThumbBmp(thumbbmp);
  setThumbDownBmp(thumbdownbmp);
  setThumbHiliteBmp(thumbhighbmp);
  setLeftBmp(leftbmp);
  setRightBmp(rightbmp);
  setMiddleBmp(middlebmp);
}

void SliderWnd::setLeftBmp(const char *name) {
  left = name;
  invalidate();
}

void SliderWnd::setMiddleBmp(const char *name) {
  middle = name;
  invalidate();
}

void SliderWnd::setRightBmp(const char *name) {
  right = name;
  invalidate();
}

void SliderWnd::setThumbBmp(const char *name) {
  thumb = name;
  invalidate();
}

void SliderWnd::setThumbDownBmp(const char *name) {
  thumbdown = name;
  invalidate();
}

void SliderWnd::setThumbHiliteBmp(const char *name) {
  thumbhilite = name;
  invalidate();
}

SkinBitmap *SliderWnd::getLeftBitmap() {
  return left;
}

SkinBitmap *SliderWnd::getRightBitmap() {
  return right;
}

SkinBitmap *SliderWnd::getMiddleBitmap() {
  return middle;
}

SkinBitmap *SliderWnd::getThumbBitmap() {
  return thumb;
}

SkinBitmap *SliderWnd::getThumbDownBitmap() {
  return thumbdown;
}

SkinBitmap *SliderWnd::getThumbHiliteBitmap() {
  return thumbhilite;
}

int SliderWnd::getWidth() {
  if (vertical)
    return (getThumbBitmap() ? getThumbBitmap()->getWidth() : 0);
  else {
    return 64;
  }
}
       
int SliderWnd::getHeight() {
  if (!vertical)
    return (getThumbBitmap() ? getThumbBitmap()->getHeight() : 0);
  else {
    return 64;
  }
}

void SliderWnd::setEnable(int en) {
  if (enabled != en) invalidate();
  enabled = en;
}

int SliderWnd::getEnable(void) {
  return enabled;
}

void SliderWnd::setPosition(int newpos, int wantcb) {
  if (newpos < minlimit) newpos = minlimit;
  else if (newpos > maxlimit) newpos = maxlimit;

  if (vertical) pos=maxlimit-newpos;
  else pos = newpos;

  if (wantcb)
    onSetPosition(); 

  invalidate();
}

int SliderWnd::onMouseMove(int x, int y) {
  int p, w, mouseover;

  SLIDERWND_PARENT::onMouseMove(x,y);

  POINT po={x,y};
  clientToScreen(&po);
  mouseover = (api->rootWndFromPoint(&po) == this); 
  if (mouseover&&!seeking&&!captured) {
    beginCapture();
    captured=1;
    onEnterArea();
  }
  int lasthilite = hilite;
  hilite = enabled && mouseover;
  if (hilite!=lasthilite) {
    if (!mouseover && !seeking && captured) {
      endCapture();
      captured=0;
      onLeaveArea();
      invalidate();
      return 0;
    }
    invalidate();
  }

  if (!enabled) return 1;

  RECT r,or;
  getClientRect(&r);
  x -= r.left;
  y -= r.top;

  or = r;
  if (vertical) {
    w = (r.bottom - r.top) - thumbHeight();
//    p = (y - (r.top-or.top)) - (thumbHeight()/2-2);
    p = (y - (r.top-or.top)) - origPos;
  } else {
    if (!drawOnBorders) {
      if (left != NULL) r.left += left.getWidth();
      if (right != NULL) r.right -= right.getWidth();
    }
    w = (r.right - r.left) - thumbWidth();
//    p = (x - (r.left - or.left)) - (thumbWidth()/2-2);
    p = (x - (r.left - or.left)) - origPos;
  }

  if (seeking) {
    pos = (p * length) / w;
    if(pos<0) pos=0;
    else if(pos>length) pos=length;

    if (hotPosition!=-1) {
      int a,b,c;
      if (vertical) a=r.bottom-r.top;
      else a=r.right-r.left;
      b=(int)(a*0.075);
      c=(b*length)/a;
      if(vertical) {
        if ((maxlimit-pos)>(hotPosition-c) && (maxlimit-pos)<(hotPosition+c)) pos=hotPosition+maxlimit;
      } else
        if (pos>(hotPosition-c) && pos<(hotPosition+c)) pos=hotPosition;
    }

    onSetPosition();
    invalidate();
  }

  return 1;
}

int SliderWnd::onLeftButtonUp(int x, int y) {
  SLIDERWND_PARENT::onLeftButtonUp(x, y);
  seeking = 0;
  captured = 0;
  oldpos = -1;
  endCapture();
  onSetFinalPosition();
  invalidate();
  return 1;
}

int SliderWnd::onRightButtonDown(int x, int y) {
  SLIDERWND_PARENT::onRightButtonDown(x,y);
  if((seeking && captured)) {
    abort();
  }
  return 1;
} 

int SliderWnd::onChar(unsigned int c) {
  SLIDERWND_PARENT::onChar(c);
  if(seeking && captured && (c == 27)) {
    abort();
  }
  return 1;
}

int SliderWnd::onSetPosition() {
  if (!isInited()) return 0;
  notifyParent(ChildNotify::SLIDER_INTERIM_POSITION, getSliderPosition());
  return 0;
}

int SliderWnd::onSetFinalPosition() {
  if (!isInited()) return 0;
  notifyParent(ChildNotify::SLIDER_FINAL_POSITION, getSliderPosition());
  return 0;
}

int SliderWnd::getSliderPosition() {
  if (vertical) return maxlimit-pos;
  return pos;
}

int SliderWnd::getSeekStatus() {
  return seeking;
}

int SliderWnd::thumbWidth() {
  if (thumb.getBitmap() == NULL) return DEFAULT_THUMBWIDTH;
  return thumb.getWidth();
}

int SliderWnd::thumbHeight() {
  if (thumb.getBitmap() == NULL) return DEFAULT_THUMBHEIGHT;
  return thumb.getHeight();
}

void SliderWnd::setUseBaseTexture(int useit) {
  use_base_texture = useit;
  invalidate();
}

void SliderWnd::setBaseTexture(SkinBitmap *bmp, int x, int y) {
  base_texture = bmp;
  use_base_texture = TRUE;
  xShift=x;
  yShift=y;
  invalidate();
}

void SliderWnd::setNoDefaultBackground(int no) {
  no_default_background = no;
}

void SliderWnd::setDrawOnBorders(int draw) {
  drawOnBorders=draw;
}

void SliderWnd::onEnterArea() {
}

void SliderWnd::onLeaveArea() {
}

void SliderWnd::setOrientation(int o) {
  vertical=o;
}

void SliderWnd::setHotPosition(int h) {
  hotPosition=h;
}

void SliderWnd::setThumbCentered(int c) {
  thumbCentered=c;
}

void SliderWnd::setThumbOffset(int o) {
  thumbOffset=o;
}

void SliderWnd::abort() {
  if(oldpos != -1) {
    seeking = 0;
    captured = 0;
    endCapture();
    pos = oldpos;
    onSetPosition();
    invalidate();
    oldpos = -1;
  }
  return;
}

void SliderWnd::setLimits(int pminlimit, int pmaxlimit) {
  minlimit=pminlimit;
  maxlimit=pmaxlimit;
  length=maxlimit-minlimit;
}
