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

#include "foreach.h"

__foreach::__foreach(const PtrListRoot &ptrlist) : list(ptrlist.getNumItems(), ptrlist.getItemList()), pos(0) {}
__foreach::__foreach(const PtrListRoot *ptrlist) : list(ptrlist->getNumItems(), ptrlist->getItemList()), pos(0) {}

int __foreach::done() const { return (pos >= list.getSize()); }
void *__foreach::next(int advance) { if (advance) pos++; return getPtr(); }
void *__foreach::getPtr() const { return (pos < list.getSize()) ? list[pos] : NULL; }
int __foreach::getPos() const { return pos; }
