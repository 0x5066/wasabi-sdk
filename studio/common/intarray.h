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

#ifndef _INTARRAY_H
#define _INTARRAY_H

#include "common.h"
#include "../bfc/named.h"
#include "../bfc/ptrlist.h"

class COMEXP IntArray {
public:
  static int read(const char *name, int *x1, int *x2=NULL, int *x3=NULL, int *x4=NULL, int *x5=NULL, int *x6=NULL, int *x7=NULL, int *x8=NULL);
  static void write(const char *name, int x1);
  static void write(const char *name, int x1, int x2);
  static void write(const char *name, int x1, int x2, int x3);
  static void write(const char *name, int x1, int x2, int x3, int x4);
};

#endif
