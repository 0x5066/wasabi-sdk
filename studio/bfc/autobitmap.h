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

#ifndef _AUTOBITMAP_H
#define _AUTOBITMAP_H

#include "../common/common.h"
#include "bitmap.h"
#include "string.h"

#include "../studio/skincb.h"

class COMEXP AutoSkinBitmap : public SkinCallbackI {
public:
  AutoSkinBitmap(const char *_name=NULL);
  ~AutoSkinBitmap();

  const char *setBitmap(const char *_name=NULL);
  int setBitmap(int _id=0);

  // call this when you get freeResources called on you
  // doesn't hurt to call as much as you want
  void reset();
  void reload() { getBitmap(); }	// force a reload

  // this loads the bitmap if necessary
  SkinBitmap *getBitmap();
  operator SkinBitmap *() { return getBitmap(); }

  const char *operator =(const char *_name) { return setBitmap(_name); }
  int operator =(int _id) { return setBitmap(_id); }

  const char *getBitmapName();

  void setHInstanceBitmapColorGroup(const char *_colorgroup) { colorgroup = _colorgroup; }

  // feel free to add more methods here to help make using this class
  // transparent...
  int getWidth() { return getBitmap()->getWidth(); };
  int getHeight() { return getBitmap()->getHeight(); }; 
  void stretchToRectAlpha(CanvasBase *canvas, RECT *r, int alpha=255) {
    getBitmap()->stretchToRectAlpha(canvas, r, alpha);
  }
  void stretchToRectAlpha(CanvasBase *canvas, RECT *r, RECT *dest, int alpha=255) {
    getBitmap()->stretchToRectAlpha(canvas, r, dest, alpha);
  }
  void stretchToRect(CanvasBase *canvas, RECT *r) {
    getBitmap()->stretchToRect(canvas, r);
  }
  void blitAlpha(CanvasBase *canvas, int x, int y, int alpha=255) {
    getBitmap()->blitAlpha(canvas, x, y, alpha);
  }

#ifdef WIN32
  void setHInstance(HINSTANCE hinstance); // use this if you use autoskinbitmap and resource in a wac
#endif

protected:
  virtual int skincb_onReset();

private:
  int use;
  int id;
  String name;
  String colorgroup;
  SkinBitmap *bitmap;
#ifdef WIN32
  HINSTANCE myInstance;
#endif
};

#endif
