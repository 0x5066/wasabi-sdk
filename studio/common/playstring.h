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

#ifndef _PLAYSTRING_H
#define _PLAYSTRING_H

#include "common.h"
#include "../bfc/string.h"

class COMEXP Playstring {
public:
  Playstring(const char *val=NULL);
  Playstring(const Playstring &ps);
  ~Playstring();

  const char *getValue() const { return val; }
  operator const char *() const { return getValue(); }

  void setValue(const char *newval);

  // copy
  Playstring& operator =(const Playstring &ps);

protected:
  void _setValue(const char *newval, int tablenum);

private:
  const char *val;
};

class PlaystringComparator {
public:
  // comparator for sorting
  static int compareItem(Playstring *p1, Playstring* p2) {
    return STRCMP(p1->getValue(), p2->getValue());
  }
};

template <int tablenum=0>
class PlaystringT : public Playstring {
public:
  PlaystringT(const char *newval) {
    val = NULL; setValue(newval);
  }
  PlaystringT(const Playstring &ps) {
    val = NULL; setValue(ps.getValue());
  }
  void setValue(const char *newval) { _setValue(newval, tablenum); }
};

#endif
