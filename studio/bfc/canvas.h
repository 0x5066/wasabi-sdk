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

//NONPORTABLE: the interface is portable, but the implementation sure isn't
#ifndef _CANVAS_H
#define _CANVAS_H

#include <ddraw.h>
#include "../common/common.h"

class Canvas;
class MemCanvasBmp;
class BaseWnd;
class RootWnd;
class Region;
class SkinBitmap;

#include "stack.h"
#include "ptrlist.h"
#include "string.h"

#include "dispatch.h"

enum {
#ifdef WIN32
  PENSTYLE_SOLID=PS_SOLID,
  PENSTYLE_DASH=PS_DASH,
  PENSTYLE_DOT=PS_DOT,
#else
#error port me!
#endif
};

// abstract base class: safe to use in API
class COMEXP NOVTABLE CanvasBase : public Dispatchable {
protected:
  CanvasBase() {} // protect constructor

public:
  HDC getHDC() {
    return _call(GETHDC, (HDC)0);
  }
  RootWnd *getRootWnd() {
    return _call(GETROOTWND, (RootWnd*)0);
  }
  void *getBits() {
    return _call(GETBITS, (void *)0);
  }
  void getOffsets(int *x, int *y) {
    _voidcall(GETOFFSETS, x, y);
  }
  BOOL isFixedCoords() {//FG> allows onPaint to handle double buffers as well as normal DCs
    return _call(ISFIXEDCOORDS, FALSE);
  }
  BOOL getDim(int *w, int *h, int *p) { // w & h in pixels, pitch in bytes. 0 on success. 
    return _call(GETDIM, FALSE, w, h, p);
  }

  const char *getTextFont() {
    return _call(GETTEXTFONT, "");
  }
  int getTextSize() {
    return _call(GETTEXTSIZE, -1);
  }
  int getTextBold() {
    return _call(GETTEXTBOLD, 0);
  }
  int getTextAntialias() {
    return _call(GETTEXTAA, 0);
  }
  int getTextOpaque() {
    return _call(GETTEXTOPAQUE, 0);
  }
  int getTextUnderline() {
    return _call(GETTEXTUNDERLINE, 0);
  }
  int getTextItalic() {
    return _call(GETTEXTITALIC, 0);
  }
  int getTextAlign() {
    return _call(GETTEXTALIGN, -1);
  }
  COLORREF getTextColor() {
    return _call(GETTEXTCOLOR, RGB(0,0,0));
  }
  COLORREF getTextBkColor() {
    return _call(GETTEXTBKCOLOR, RGB(255,255,255));
  }

  enum {
    GETHDC		= 100,
    GETROOTWND		= 200,
    GETBITS		= 300,
    GETOFFSETS		= 400,
    ISFIXEDCOORDS	= 500,
    GETDIM		= 600,
    GETTEXTFONT		= 700,
    GETTEXTSIZE		= 710,
    GETTEXTBOLD		= 720,
    GETTEXTOPAQUE	= 730,
    GETTEXTALIGN	= 740,
    GETTEXTCOLOR	= 750,
    GETTEXTBKCOLOR	= 760,
    GETTEXTAA     	= 770,
    GETTEXTUNDERLINE	= 780,
    GETTEXTITALIC	= 790,
  };
};

typedef struct {
  int style;
  int width;
  COLORREF color;
  HPEN hpen;
} penstruct;

class COMEXP NOVTABLE Canvas : public CanvasBase {
protected:
  Canvas();
public:
  virtual ~Canvas();

// CanvasBase stuff
  HDC getHDC();
  RootWnd *getRootWnd();
  void *getBits();
  void getOffsets(int *x, int *y);
  BOOL isFixedCoords();
  BOOL getDim(int *w, int *h, int *p);
  void setBaseWnd(BaseWnd *b);
// end CanvasBase stuff

  virtual BaseWnd *getBaseWnd();

  // graphics commands
  void fillRect(const RECT *r, COLORREF color, int alpha=255);
  void drawRect(const RECT *r, int solid, COLORREF color, int alpha=255);

  // text commands
  void setTextFont(const char *font_id_name);
  const char *getTextFont();
  void pushTextFont(const char *font_id_name);
  const char *popTextFont();

  void setTextSize(int points);
  int getTextSize();
  void pushTextSize(int points);
  int popTextSize();

  void setTextBold(int bold);
  int getTextBold();
  void pushTextBold(int bold);
  int popTextBold();

  void setTextAntialias(int aa);
  int getTextAntialias();
  void pushTextAntialias(int aa);
  int popTextAntialias();

  void setTextOpaque(int op);
  int getTextOpaque();
  void pushTextOpaque(int op);
  int popTextOpaque();

  void setTextUnderline(int underlined);
  int getTextUnderline();
  void pushTextUnderline(int underlined);
  int popTextUnderline();

  void setTextItalic(int italicized);
  int getTextItalic();
  void pushTextItalic(int italicized);
  int popTextItalic();

  void setTextAlign(int al);
  int getTextAlign();
  void pushTextAlign(int al);
  int popTextAlign();

  void setTextColor(COLORREF color);
  COLORREF getTextColor();
  void pushTextColor(COLORREF color);
  COLORREF popTextColor();

  void setTextBkColor(COLORREF color);
  COLORREF getTextBkColor();
  void pushTextBkColor(COLORREF color);
  COLORREF popTextBkColor();

  void pushPen(COLORREF color);
  void pushPen(int style, int width, COLORREF color);
  void popPen();

  int getPenStyle();
  COLORREF getPenColor();
  int getPenWidth();

  // normal text
  void textOut(int x, int y, const char *txt);
  void textOut(int x, int y, int w, int h, const char *txt);
  void textOutEllipsed(int x, int y, int w, int h, const char *txt);
  // returns height used
  void textOutWrapped(int x, int y, int w, int h, const char *txt);
  void textOutWrappedPathed(int x, int y, int w, const char *txt);
  void textOutCentered(RECT *r, const char *txt);

  int getTextWidth(const char *text);
  int getTextHeight(const char *text);
  void getTextExtent(const char *text, int *w, int *h);
  int getTextHeight() { return getTextHeight("M"); }
  void selectClipRgn(Region *r);
  int getClipBox(RECT *r); // returns 0 if no clipping region
  int getClipRgn(Region *r); // returns 0 if no clipping region

      // Deprecated?
      void moveTo(int x, int y);
      void lineTo(int x, int y);

  void lineDraw(int fromX, int fromY, int toX, int toY);

  void drawSysObject(const RECT *r, int sysobj, int alpha=255);

  void blit(int srcx, int srcy, Canvas *dest, int dstx, int dsty, int dstw, int dsth);
  
  // src* are in 16.16 fixed point
  void stretchblit(int srcx, int srcy, int srcw, int srch, Canvas *dest, int dstx, int dsty, int dstw, int dsth);

  void antiAliasTo(Canvas *dest, int w, int h, int aafactor);

  int getXOffset() const { return xoffset; }
  int getYOffset() const { return yoffset; }
  void offsetRect(RECT *r);
  void debug();

  void colorToColor(COLORREF from, COLORREF to, RECT *r);
  static float getSystemFontScale();

protected:
  virtual int _dispatch(int msg, void *retval, void **params=NULL, int nparam=0);

  HDC hdc;
  void *bits;
  int width, height, pitch;
  BOOL fcoord;
  int xoffset, yoffset;
  BaseWnd *srcwnd;

private:
  Stack<String*> fontstack;
  Stack<COLORREF> colorstack;
  Stack<COLORREF> bkcolorstack;
  Stack<penstruct> penstack;
  Stack<int> boldstack;
  Stack<int> aastack;
  Stack<int> opstack;
  Stack<int> unstack;
  Stack<int> itstack;
  Stack<int> alstack;
  Stack<int> sizestack;

/*  HFONT makeFont(int size, const char *fontfacename=NULL, int bold=FALSE);
  void deleteFont(HFONT font);*/

  int tsize;
  int align;
  int tbold, taa, topaque, tunderline, titalic;
  int penstyle;
  COLORREF pencolor;
  int penwidth;
  COLORREF tcolor;
  COLORREF bktcolor;
  String *tfont;
  HPEN defpen;
  HPEN curpen;

  static char retfontname[256];
};

namespace DrawSysObj {
  enum {
    BUTTON, BUTTON_PUSHED, BUTTON_DISABLED,
    OSBUTTON, OSBUTTON_PUSHED, OSBUTTON_DISABLED,
    OSBUTTON_CLOSE, OSBUTTON_CLOSE_PUSHED, OSBUTTON_CLOSE_DISABLED,
    OSBUTTON_MINIMIZE, OSBUTTON_MINIMIZE_PUSHED, OSBUTTON_MINIMIZE_DISABLED,
    OSBUTTON_MAXIMIZE, OSBUTTON_MAXIMIZE_PUSHED, OSBUTTON_MAXIMIZE_DISABLED,
  };
};

class COMEXP WndCanvas : public Canvas {
public:
  WndCanvas();
  virtual ~WndCanvas();

  // address client area
  int attachToClient(BaseWnd *basewnd);
//CUT  // address entire window
//CUT  int attachToWnd(HWND _hWnd);	// NONPORTABLE: avoid! mostly for mainwnd
  
private:
  HWND hWnd;
};

class COMEXP PaintCanvas : public Canvas {
public:
  PaintCanvas();
  virtual ~PaintCanvas();

  int beginPaint(BaseWnd *basewnd);
  void getRcPaint(RECT *r);

private:	// NONPORTABLE
  HWND hWnd;
  PAINTSTRUCT ps;
};

class COMEXP PaintBltCanvas : public Canvas {
public:
  PaintBltCanvas();
  virtual ~PaintBltCanvas();
  int beginPaint(BaseWnd *basewnd);
  int beginPaintNC(BaseWnd *basewnd);

  void *getBits();
  void getRcPaint(RECT *r);

private:	// NONPORTABLE
  HWND hWnd;
  PAINTSTRUCT ps;
  HDC wnddc;
  HBITMAP hbmp, prevbmp;
  BOOL nonclient;
};

class COMEXP MemCanvas : public Canvas {
public:
  MemCanvas();
  virtual ~MemCanvas();

  int createCompatible(Canvas *canvas);
private:
};

class COMEXP DCCanvas : public Canvas {
public:
  DCCanvas(HDC clone=NULL, BaseWnd *srcWnd=NULL);
  virtual ~DCCanvas();

  int cloneDC(HDC clone, BaseWnd *srcWnd=NULL);
};

class COMEXP SysCanvas : public Canvas {
public:
  SysCanvas();
  virtual ~SysCanvas();
};

class COMEXP DCBltCanvas : public Canvas {
public:
  DCBltCanvas();
  virtual ~DCBltCanvas();

  int cloneDC(HDC clone, RECT *r, BaseWnd *srcWnd=NULL);
  int setOrigDC(HDC neworigdc); // set to null to prevent commitdc on delete, non null to change destination dc
  int commitDC(void);						// allows commit to DC without deleting
#if 0
  int cloneCanvas(CanvasBase *clone, RECT *r);
#endif

private:
  HDC origdc;
  RECT rect;
  HBITMAP hbmp, prevbmp;
};

// note: getBaseWnd() returns NULL for this class
class COMEXP BaseCloneCanvas : public Canvas {
public:
  BaseCloneCanvas(CanvasBase *cloner=NULL);
  virtual ~BaseCloneCanvas();

  int clone(CanvasBase *cloner);
};

class COMEXP BltCanvas : public Canvas {
public:
  BltCanvas(int w, int h, int nb_bpp=32, unsigned char *pal=NULL,int palsize=0);
  BltCanvas(HBITMAP bmp);
  virtual ~BltCanvas();
  void *getBits();
  HBITMAP getBitmap();
  SkinBitmap *makeSkinBitmap(); // this one makes a new, with own bits
  SkinBitmap *getSkinBitmap(); // this one gives a skinbitmap envoloppe of this bltcanvas
  void disposeSkinBitmap(SkinBitmap *b); // call only after makeSkinBitmap

  void fillBits(COLORREF color);

  void vflip(int vert_cells=1);
  void hflip(int hor_cells=1);
  void maskColor(COLORREF from, COLORREF to);
  void makeAlpha(int newalpha=-1); // -1 = premultiply using current alpha

private:	// NONPORTABLE
  HBITMAP hbmp, prevbmp;
  PtrList<SkinBitmap> *skinbmps;
  SkinBitmap *envelope;
  BITMAP bm;
  BOOL ourbmp;
  int bpp;

  void premultiply(ARGB32 *m_pBits, int nwords, int newalpha=-1);
};

class COMEXP DDSurfaceCanvas : public Canvas {

public:

  DDSurfaceCanvas(LPDIRECTDRAWSURFACE surface, int w, int h);
  virtual ~DDSurfaceCanvas();

  int isready();
  void enter();
  void exit();

private:

  LPDIRECTDRAWSURFACE surf;
  int _w, _h;
};

enum
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

#endif
