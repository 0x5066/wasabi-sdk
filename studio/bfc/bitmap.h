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

//NONPORTABLE
#ifndef _BITMAP_H
#define _BITMAP_H

#include "../common/common.h"
#include "../bfc/string.h"
class CanvasBase;	// see canvas.h

//#define NO_MMX

class Region;

// a skinnable bitmap
class COMEXP SkinBitmap {
public:
#ifndef _NOSTUDIO
  SkinBitmap(HINSTANCE hInst, int _id, const char *colorgroup=NULL);	//NONPORTABLE
  SkinBitmap(const char *elementname, int cached=1);
#endif
//  SkinBitmap(SkinBitmap *source, int w, int h);
  SkinBitmap(int w, int h, DWORD bgcolor=0xffffffff);	//untested --BU
  SkinBitmap(HBITMAP bitmap);
  SkinBitmap(HBITMAP bitmap, HDC dc, int has_alpha=0, void *bits=NULL);
  ~SkinBitmap();

  int getWidth() const { return subimage_w == -1 ? fullimage_w : subimage_w; };
  int getHeight() const { return subimage_h == -1 ? fullimage_h : subimage_h; };
  int getFullWidth() const { return fullimage_w; };
  int getFullHeight() const { return fullimage_h; };
  int getX() const { return x_offset == -1 ? 0 : x_offset; };
  int getY() const { return y_offset == -1 ? 0 : y_offset; };
  int getBpp() const { return 32; };
  int getAlpha() const { return has_alpha; };
  void setHasAlpha(int ha);
  virtual void *getBits();
  int isInvalid();
  
  const char *getBitmapName();

  void blit(CanvasBase *canvas, int x, int y);
  void blitAlpha(CanvasBase *canvas, int x, int y, int alpha=255);
  // blits a chunk of source into dest rect
  void blitToRect(CanvasBase *canvas, RECT *src, RECT *dst, int alpha=255);
  void blitTile(CanvasBase *canvas, RECT *dest, int xoffs=0, int yoffs=0, int alpha=255);
  void blitRectToTile(CanvasBase *canvas, RECT *dest, RECT *src, int xoffs=0, int yoffs=0, int alpha=255);
  void stretch(CanvasBase *canvas, int x, int y, int w, int h);
  void stretchToRect(CanvasBase *canvas, RECT *r);
  void stretchRectToRect(CanvasBase *canvas, RECT *src, RECT *dst);

  void stretchToRectAlpha(CanvasBase *canvas, RECT *r, int alpha=255);
  void stretchToRectAlpha(CanvasBase *canvas, RECT *src, RECT *dst, int alpha=255);
  COLORREF getPixel(int x, int y);


private:

  void bmpToBits(HBITMAP hbmp, HDC defaultDC=NULL);

  int has_alpha;

  int x_offset,y_offset,subimage_w,subimage_h,fullimage_w,fullimage_h;

  ARGB32 *bits;
  int ownbits;
  int last_failed;
  String bitmapname;
};

#endif
