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

#ifndef _FOREACH_H
#define _FOREACH_H

#include "ptrlist.h"

// foreach stuff
/* use like this:
  PtrList<blah> list;
  foreach(list)
    list.getfor()->booga();
    something(list.getfor());
  endfor
*/

// foreach stuff
class COMEXP __foreach {
public:
  __foreach(const PtrListRoot &ptrlist);
  __foreach(const PtrListRoot *ptrlist);

  int done() const;
  void *next(int advance=TRUE);
  void *getPtr() const;

  int getPos() const;

private:
  MemBlock<void *> list;
  int pos;
};

#define foreach(x) \
{ \
  void *__fe_void; \
  __foreach ___f(x); \
  for (__fe_void = ___f.getPtr(); !___f.done(); __fe_void = ___f.next()) {
#define getfor() castFor(__fe_void)
#define endfor \
  } \
}
#define foreach_index (___f.getPos())

#endif
