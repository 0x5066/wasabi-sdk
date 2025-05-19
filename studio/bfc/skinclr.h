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

#ifndef _SKINCLR_H
#define _SKINCLR_H

#include "../common/common.h"
#include "std.h"
#include "filteredcolor.h"

// note: only pass in a const char *
class COMEXP SkinColor : FilteredColor {
public:
  explicit SkinColor(const char *name=NULL, const char *colorgroup=NULL);

  DWORD v();
  operator int() { return v(); }

  void setElementName(const char *name);
  const char *operator =(const char *name) { setElementName(name); return name;}

private:
  const char *name;
  DWORD *val;
  static int *skin_iterator;
  int latest_iteration;
  COLORREF filteredcolor;
};

#endif
