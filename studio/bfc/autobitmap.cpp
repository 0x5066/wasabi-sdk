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


#include "autobitmap.h"

#include "../studio/api.h"

AutoSkinBitmap::AutoSkinBitmap(const char *_name) {
  bitmap = NULL;
  use = 0;
  id = 0;
#ifdef WIN32
  myInstance = api->main_gethInstance();
#endif
  setBitmap(_name);
}

AutoSkinBitmap::~AutoSkinBitmap() {
  delete bitmap;
  ASSERT(api != NULL);
  api->syscb_deregisterCallback(this);
}

const char *AutoSkinBitmap::setBitmap(const char *_name) {
  if (_name == NULL) return NULL;
  if (name == NULL || !STREQL(name, _name)) {
    delete bitmap; bitmap = NULL;
    name = _name;
  }
  return name;
}

int AutoSkinBitmap::setBitmap(int _id) {
  if (_id == 0) return 0;
  if (_id != id) {
    delete bitmap; bitmap = NULL;
    id = _id;
  }
  return id;
}

#ifdef WIN32
void AutoSkinBitmap::setHInstance(HINSTANCE hinstance) {
  myInstance = hinstance;
}
#endif

void AutoSkinBitmap::reset() {
  delete bitmap; bitmap = NULL;
}

SkinBitmap *AutoSkinBitmap::getBitmap() {
//FG  ASSERT(name != NULL);
  if (name == NULL && id == NULL) return NULL;
  if (bitmap == NULL) {
    ASSERT(api != NULL);
    if (name) {
      bitmap = new SkinBitmap(name);
    } else {
#ifdef WIN32
      bitmap = new SkinBitmap(myInstance, id, colorgroup);
#endif
    }
    if (bitmap)
      api->syscb_registerCallback(this);
  }
  return bitmap;
}

const char *AutoSkinBitmap::getBitmapName() {
  return name;
}

int AutoSkinBitmap::skincb_onReset() {
  reset();
  return 1;
}
