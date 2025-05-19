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


#include "playstring.h"

#include "../studio/api.h"

#define USE_TABLE

Playstring::Playstring(const char *_val) {
  val = NULL;
  setValue(_val);
}

Playstring::Playstring(const Playstring &ps) {
  val = NULL;
  setValue(ps.getValue());
}

Playstring::~Playstring() {
  setValue(NULL);
}

void Playstring::setValue(const char *newval) {
  _setValue(newval, 0);
}

void Playstring::_setValue(const char *newval, int tablenum) {
  ASSERT((int)newval != 0xdddddddd);
#ifdef USE_TABLE
  if (val != NULL) api->metadb_releasePlaystring(val, tablenum);
#else
  FREE((void*)val);
#endif

  val = NULL;

  if (newval != NULL /*&& *newval != 0*/) {
#ifdef USE_TABLE
    val = api->metadb_dupPlaystring(newval, tablenum);
#else
    val = STRDUP(newval);
#endif
  }
}

Playstring& Playstring::operator =(const Playstring &ps) {
  if (this != &ps) {
    setValue(ps.getValue());
  }
  return *this;
}
