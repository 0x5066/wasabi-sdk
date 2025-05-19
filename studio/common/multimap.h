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

#ifndef _MULTIMAP_H
#define _MULTIMAP_H

#include "../bfc/map.h"

// this is a specialized map from a data type to a ptrlist of another type
// I love C++ :) --BU

template <class INDEX, class DATA>
class MultiMap : private Map<INDEX, PtrList<DATA>* > {
  typedef Map<INDEX, PtrList<DATA>* > PMap;
  typedef PtrList<DATA> MList;
public:
  void multiAddItem(const INDEX &a, DATA *ptr, int nodup=FALSE) {
    MList *list = getListForIndex(a, TRUE);
    if (!(nodup && list->haveItem(ptr)))
      list->addItem(ptr);
  }
  int multiGetItem(const INDEX &a, int n, DATA **pptr) {
    ASSERT(pptr != NULL);
    MList *list = getListForIndex(a, FALSE);
    if (list == NULL) return 0;	// don't have any

    DATA* dp = list->enumItem(n);
    if (dp == NULL) return 0;	// not on list at that position
    *pptr = dp;
    return 1;
  }
  int multiGetItemDirect(int pos, int n, DATA **pptr) {
    MList *list = PMap::enumItemByPos(pos, NULL);
    if (list == NULL) return 0;
    DATA *rptr = list->enumItem(n);
    if (rptr == NULL) return 0;
    *pptr = rptr;
    return 1;
  }
  int multiGetNumItems(const INDEX &a) {
    MList *list = getListForIndex(a, FALSE);
    if (list == NULL) return 0;	// don't have any
    return list->getNumItems();
  }
  int multiGetNumPairs() { return PMap::getNumPairs(); }
  int multiHaveItem(const INDEX &a, DATA *ptr) {
    MList *list = getListForIndex(a, FALSE);
    if (list == NULL) return 0;	// don't have any
    return list->haveItem(ptr);
  }
  int multiDelItem(const INDEX &a, DATA *ptr) {
    MList *list = getListForIndex(a, FALSE);
    if (list == NULL) return 0;
    return list->delItem(ptr);
  }
  // delete entire list for item
  int multiDelAllForItem(const INDEX &a, int call_destructors=FALSE) {
    MList *list = getListForIndex(a, FALSE);
    if (list == NULL) return 0;
    PMap::delItem(a);
    int r = list->getNumItems();
    if (call_destructors) list->deleteAll();
    delete list;
    return r;
  }

  const MList *getListForIndex(const INDEX &a) {
    return getListForIndex(a, FALSE);
  }

private:
  MList *getListForIndex(const INDEX &a, int addifnotfound) {
    MList *list = NULL;
    if (PMap::getItem(a, &list) == 0) {
      if (addifnotfound) {
        list = new MList;
        PMap::addItem(a, list);
      }
    }
    return list;
  }
};

#endif
