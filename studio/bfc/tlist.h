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

//PORTABLE
#ifndef _TLIST_H
#define _TLIST_H

#include "../common/common.h"
#include "std.h"
#include "named.h"

/*

A generic std::vector wannabe...

NOTE: you must only use this with basic types! constructors and destructors
will not be called.

*/

#include "../studio/assert.h"

#define TLIST_INCREMENT 32

namespace wasabi {
template<class T>
class COMEXP TList {
public:
  TList() {
    nitems = 0;
    nslots = 0;
    items = NULL;
  }
  virtual ~TList() {
    FREE(items);
  }

  int getNumItems() const { return nitems; };

  // CAN'T PROVIDE BOUNDS CHECKING!!! (except by ASSERT() :( )
  T enumItem(int n) const {
    ASSERT(items != NULL);
    ASSERT(n >= 0 && n < nitems);
    return items[n];
  }
  T operator[](int n) const { return enumItem(n); }

  T getFirst() const { return enumItem(0); }
  T getLast() const { return enumItem(getNumItems()-1); }

  T *enumRef(int n) const {
    ASSERT(items != NULL);
    ASSERT(n >= 0 && n < nitems);
    return items+n;
  }

  T *addItem(T item) {
    ASSERT(nitems <= nslots);
    if (items == NULL) {
      nslots = TLIST_INCREMENT;
      items = (T *)MALLOC(sizeof(T) * nslots);
      nitems = 0;
    } else if (nslots == nitems) {	// time to expand
      T *newitems;
      nslots += TLIST_INCREMENT;
      newitems = (T *)MALLOC(sizeof(T) * nslots);
      MEMCPY(newitems, items, nitems * sizeof(T));
      FREE(items);
      items = newitems;
    }
    items[nitems++] = item;

    return &items[nitems-1];
  }

  T *insertItem(T item, int pos) { // position to insert
    ASSERT(pos >= 0 && pos <= nitems);
    T *tmp=addItem(item);
    int n=nitems-1;
    while (n > pos) {
      items[n]=items[n-1];
      n--;
    }
    tmp=items+pos;
    *tmp=item;
    return tmp;
  }

  T *replaceItemByPos(T item, int pos) {
    ASSERT(items != NULL);
    ASSERT(nitems != 0);
    ASSERT(pos >= 0 && pos < nitems);
    items[pos]=item;
    return &items[pos];
  }

  void setItem(int pos, T newval) {
    if (pos < 0) return;
    if (pos >= nitems) return;
    items[pos] = newval;
  }

  int delItem(T item) {
    int c = 0;

    ASSERT(items != NULL);
    ASSERT(nitems != 0);

    T *src = items;
    T *dst = items;
    for (int i = 0; i < nitems; i++) {
      if (*src != item) {
        *dst++ = *src;
        c++;
      }
      src++;
    }
    nitems -= c;

    return c;
  }

  int delByPos(int pos) {
    if (pos < 0 || pos >= nitems) return 0;
    --nitems;
    if (pos == nitems) return 1;	// last one? nothing to copy over
    MEMCPY(items+pos, items+(pos+1), sizeof(T)*(nitems-pos));  // CT:not (nitems-(pos+1)) as nitems has been decremented earlier
    return 1;
  }

  void removeAll() {
    FREE(items); items = NULL;
    nitems = 0;
    nslots = 0;
  }

  void freeAll() {
    for (int i = 0; i < nitems; i++)
      FREE(items[i]);
    removeAll();
  }

  int getItemPos(T it) const {
    for (int i=0;i<getNumItems();i++)
      if(!MEMCMP(&items[i],&it,sizeof(T))) return i; // CT     if (items[i] == it) return i;
    return -1;
  }

private:
  int nitems, nslots;
  T *items;
};
}

namespace wasabi {
template<class T>
class TListNamed : public TList<T>, public Named {};
}  // namespace wasabi

#endif
