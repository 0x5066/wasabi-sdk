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

#ifndef _MAP_H
#define _MAP_H

#include "pair.h"
#include "ptrlist.h"

// this is a simple map from one data type to another
// no dup checking is currently done... sorry :( --BU

#if 0
template <class T> class SortPair {
public:
  static int compareItem(T *p1, T* p2) {
    if (*p1 < *p2) return -1;
    if (*p1 == *p2) return 0;
    return 1;
  }
  static int compareAttrib(char *attrib, T *item) {
    return compareItem((T*)attrib, item);
  }
};
#endif

//template <class INDEX, class DATA, class SORT=NotSorted>
template <class INDEX, class DATA>
class Map {
public:
  typedef Pair<INDEX, DATA> MapPair;
  Map() {
//    list.setAutoSort(0);	// for now, no sorting
  }
  ~Map() {
    list.deleteAll();
  }

  void addItem(const INDEX &a, const DATA &b) {
    list.addItem(new MapPair(a, b));
  }

  int getItem(const INDEX &a, DATA *b=NULL) {	// FUCKO: use a sorted ptrlist
    //MapPair *mp = list.findItem((char *)&a);
    MapPair *mp = NULL;
    for (int i = 0; i < list.getNumItems(); i++) {
      if (list[i]->a == a) {
        mp = list[i];
        if (b) *b = mp->b;
        return 1;
      }
    }
    return 0;
  }

  int reverseGetItem(const DATA &b, INDEX *a=NULL) {
    MapPair *mp = NULL;
    for (int i = 0; i < list.getNumItems(); i++) {
      if (list[i]->b == b) {
        mp = list[i];
        if (a) *a = mp->a;
        return 1;
      }
    }
    return 0;
  }

  int delItem(INDEX a) {
    for (int i = 0; i < list.getNumItems(); i++) {
      MapPair *pair = list.q(i);
      if (pair->a == a) {
        list.removeByPos(i);
        delete pair;
        return 1;
      }
    }
    return 0;
  }

  int reverseDelItem(const DATA &b) {
    for (int i = 0; i < list.getNumItems(); i++) {
      MapPair *pair = list.q(i);
      if (pair->b == b) {
        list.removeByPos(i);
        delete pair;
        return 1;
      }
    }
    return 0;
  }

  int getNumItems() const {
    return list.getNumItems();
  }

  DATA enumItemByPos(int n, DATA default_val) {
    MapPair *mp = list.enumItem(n);
    if (mp == NULL) return default_val;
    return mp->b;
  }

  INDEX enumIndexByPos(int n, INDEX default_val) {
    MapPair *mp = list.enumItem(n);
    if (mp == NULL) return default_val;
    return mp->a;
  }

  int getNumPairs() const {
    return list.getNumItems();
  }

  // How could we forget this?
  void deleteAll() {
    list.deleteAll();
  }

private:
//  PtrListQuickSorted<MapPair, SORT> list;
  PtrList<MapPair> list;
};

#endif
