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

#ifndef _LOADLIB_H
#define _LOADLIB_H

#include "../bfc/std.h"
#include "../bfc/named.h"

class COMEXP Library : public Named {
public:
  Library(const char *filename=NULL);
  Library(const Library &l) {
    lib = NULL; // FG> overrides default constructor, so we need to init this too...
    load(l.getName());
  }
  ~Library();

  Library& operator =(const Library &l) {
    if (this != &l) {
      unload();
      load(l.getName());
    }
    return *this;
  }

  int load(const char *filename=NULL);
  void unload();

  void *getProcAddress(const char *procname);
#ifdef WIN32
  HINSTANCE getHandle() const { return lib; }
#endif

private:
#ifdef WIN32
  HINSTANCE lib;
#else
# error port me!
#endif
};

#endif
