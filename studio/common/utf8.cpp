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


#include "utf8.h"

// this STILL doesn't work perfectly but at least it decodes what we write out
// mostly just waiting on our wide character strategy

void COMEXP UTF8_to_ASCII(const char *in, char *out) {
  unsigned const char *src = (unsigned const char *)in;
  unsigned char *dst = (unsigned char *)out;
  *dst = 0;
  for (; *src; src++) {
    int c = *src;
    if ((c & 0x80) == 0) {
      *dst++ = c;
      continue;
    }
    if ((c & 224) != 192) continue;	// fuck you, we only check for single bytes
    int v = (c & 0x3) << 6;
    ++src;
    v |= *src & 0x3f;
    *dst++ = v;
  }
  *dst = 0;
}
