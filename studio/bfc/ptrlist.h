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

#ifndef _PTRLIST_H
#define _PTRLIST_H

#include "std.h"
#include "../common/common.h"
#include "memblock.h"
#include "../studio/assert.h"

// Disable the "identifier was truncated to '255' characters..." warning.
#pragma warning( disable : 4786 )

/*

a generic pointer list template. only takes up 12 bytes when empty. auto-grows
the array as necessary, NEVER shrinks it unless you removeAll() or equivalent

Use, PtrList<typename>, never PtrListRoot

*/

// yes, this really should be an enum or something
#define PTRLIST_POS_LAST -1

// 1k each, leaving 16 bytes for MALLOC overhead
#define DEF_PTRLIST_INCREMENT 252

class __foreach;

// base class, to share as much code as possible
class COMEXP NOVTABLE PtrListRoot {
friend class __foreach;
protected:
  PtrListRoot(int initial_size=0);
  PtrListRoot(const PtrListRoot *from);
  ~PtrListRoot();

  void copyFrom(const PtrListRoot *from);

  void setMinimumSize(int nslots);	// expand to at least this many slots
  int getNumItems() const;

  void *enumItem(int n) const;

  void moveItem(int from, int to);

  int removeItem(void *item);
  void removeEveryItem(void *item);
  void removeByPos(int pos);
  void removeLastItem();
  void removeAll();
  void freeAll();

  // gross-ass linear search to find index of item
  // note that PtrListSorted provides a binary search version
  int searchItem(void *item) const;

#if 0//fuct
  // speedy binary search. although it'll be fuct if it's not sorted right
  int bsearchItem(void *item) const;
#endif

  void *addItem(void *item, int pos, int inc);
  void *setItem(void *item, int pos); // replace what's in the slot with the new value

  void reverse();

  void **getItemList() const { // try to avoid! this is inline to make q() fast
    return items;
  }

private:
  void verify();

  int nitems, nslots;
  void **items;
};

// now we add the methods that refer specifically to the pointer type
template<class T>
class PtrList : public PtrListRoot {
friend class __foreach;
public:
  PtrList(int initial_size=0) : PtrListRoot(initial_size) {}
  PtrList(const PtrList<T> &r) {
    copyFrom(&r);
  }
  ~PtrList() {}

  // copy another PtrList

  void copyFrom(const PtrList<T> *from) { PtrListRoot::copyFrom(from); }

// adding

  // expand freelist to at least this many slots, even if 0 items in list
  void setMinimumSize(int nslots) { PtrListRoot::setMinimumSize(nslots); }

  // provide a public addItem for the pointer type
  T *addItem(const T *item, int pos=PTRLIST_POS_LAST, int inc=DEF_PTRLIST_INCREMENT) {
    return static_cast<T *>(PtrListRoot::addItem(const_cast<T*>(item), pos, inc));
  }

  // replace what's in the slot with the new value
  T *setItem(const T *item, int pos) {
    return static_cast<T *>(PtrListRoot::setItem(const_cast<T*>(item), pos));
  }

  // reverse the order of the list in place
  void reverse() { PtrListRoot::reverse(); }

// enumerating

  // returns # of items in list
  int getNumItems() const { return PtrListRoot::getNumItems(); }

  // basic list enumerator. returns NULL for out of bounds
  T *enumItem(int n) const {
    return static_cast<T *>(PtrListRoot::enumItem(n));
  }
  T *operator[](int n) const { return enumItem(n); }

  // this will safely return NULL if 0 items due to enumItems's boundscheck
  T *getFirst() const { return enumItem(0); }
  T *getLast() const { return enumItem(getNumItems()-1); }

  // this is a NON-BOUNDS-CHECKING lookup
  T *q(int n) { return static_cast<T*>(getItemList()[n]); }

  // gross-ass linear search to find index of item
  // note that PtrListSorted provides a binary search version
  int searchItem(T *item) const { return PtrListRoot::searchItem(item); }
  int haveItem(T *item) const { return searchItem(item) >= 0; }

// deleteing

  // removes first instance of a pointer in list, returns how many are left
  int removeItem(T *item) { return PtrListRoot::removeItem(item); }
//DEPRECATED
int delItem(T *item) { return removeItem(item); }
  // removes all instances of this pointer
  void removeEveryItem(void *item) { PtrListRoot::removeEveryItem(item); }
  // removes pointer at specified position regardless of value
  void removeByPos(int pos) { PtrListRoot::removeByPos(pos); }
// DEPRECATED
void delByPos(int pos) { removeByPos(pos); }

  // removes last item
  void removeLastItem() { PtrListRoot::removeLastItem(); }
  // removes all entries, also deletes memory space
  void removeAll() { PtrListRoot::removeAll(); }
  // removes all entries, calling FREE on the pointers
  void freeAll() { PtrListRoot::freeAll(); }
  // removes all entries, calling delete on the pointers
  void deleteAll() {
    int i, nitems = getNumItems();
    for (i = 0; i < nitems; i++)
      delete enumItem(i);
    removeAll();
  }
  // removes all entries, calling delete on the pointers
  // FG>this version removes each entry as it deletes it so if
  // one of the object uses this list in its destructor, it will
  // still work. It is MUCH slower than deleteAll tho.
  void deleteAllSafe() {
//CUT    ASSERT(!(nitems != 0 && items == NULL));
    while (getNumItems()) {
      T *i = enumItem(0);
      delete i;
      removeItem(i);
    }
  }
  void deleteItem(int item) {
    if (item < getNumItems()) {
      deleteItem(enumItem(item));
    }
  }
  void deleteItem(T *item) {
    delete item;
    removeItem(item);
  }

  void moveItem(int from, int to) { PtrListRoot::moveItem(from, to); }

  static T *castFor(void *ptr) { return static_cast<T*>(ptr); }

protected:
  T **getItemList() {
    return reinterpret_cast<T **>(PtrListRoot::getItemList());
  }
};

class NotSorted {
public:
  // comparator for searching -- override
  static int compareAttrib(const char *attrib, void *item) { return 0; }
  // comparator for sorting -- override	, -1 p1 < p2, 0 eq, 1 p1 > p2
  static int compareItem(void *p1, void* p2) { return 0; }
};

//template <class T, class C> class NoSort {
//  static void _sort(T **, int) {}
//};

// a base class to sort the pointers
// you must implement the comparisons (C) and the sort algorithm (S)
template< class T, class C, class S >
class PtrListSorted : public PtrList<T> {
public:
  PtrListSorted(int initial_size=0) : PtrList<T>(initial_size) {
    need_sorting = 0;
    auto_sort = 1;
  }

  void copyFrom(const PtrList<T> *from) {
    PtrList<T>::copyFrom(from);
    need_sorting = 1;
    if (auto_sort) sort();
  }

  T *addItem(T *item, int pos=PTRLIST_POS_LAST, int inc=DEF_PTRLIST_INCREMENT) {
    need_sorting = 1;
    return PtrList<T>::addItem(item, pos, inc);
  }

  void sort(BOOL force_sort=FALSE) {
    if (need_sorting || force_sort)
      S::_sort(PtrList<T>::getItemList(), getNumItems());
    need_sorting = 0;
  }

  T *enumItem(int n) {	// NOT const since we might call sort()
    if (auto_sort) sort();
    return PtrList<T>::enumItem(n);
  }
  T *operator[](int n) { return PtrListSorted<T,C,S>::enumItem(n); }

  T *findItem(const char *attrib, int *pos=NULL) {
    ASSERTPR(!(!auto_sort && need_sorting), "must call sort() first if auto-sorting is disabled");
    sort();
#if 1	// do binary search
    if (getNumItems() == 0) return NULL;

    int bot = 0, top = getNumItems()-1, mid;

    for (int c = 0; c < getNumItems()+1; c++) {
      if (bot > top) return NULL;
      mid = (bot + top) / 2;
      int r = C::compareAttrib(attrib, getItemList()[mid]);
      if (r == 0) {
        if (pos != NULL) *pos = mid;
        return getItemList()[mid];
      }
      if (r < 0) {
        top = mid-1;
      } else {
        bot = mid+1;
      }
    }
    ASSERTPR(0, "binary search fucked up");
#else
    // re-enable this in case of fuckup
    for (int i = 0; i < nitems; i++) {
      if (C::compareAttrib(attrib, static_cast<T *>(items[i])) == 0) return static_cast<T *>items[i];
    }
#endif
    return NULL;
  }

  T *findItem(T *attrib, int *pos=NULL) {
    return findItem((const char *)attrib, pos);
  }

  int beginEnumDups(const char *attrib) {
    int pos;
    findItem(attrib, &pos);
    if (pos < 0) return -1;
    dups_hi = pos;
    dups_low = pos;

    for (int i=pos-1;i>=0;i--) {
      if (C::compareAttrib(attrib, static_cast<T *>(enumItem(i))) == 0) break;
      dups_low = i;
    }
    for (i=pos+1;i<getNumItems();i++) {
      if (C::compareAttrib(attrib, static_cast<T *>(enumItem(i))) == 0) break;
      dups_hi = i;
    }
    dups_pos = dups_low;
    return dups_pos;
  }
  int getNextDup() { // returns -1 when done
    if (dups_pos >= dups_hi) return -1;
    return ++dups_pos;
  }

#if 0
  // replace search with binary search
  int searchItem(T *item) const {
    ASSERTPR(!(!auto_sort && need_sorting), "must call sort() first if auto-sorting is disabled");
    sort();
    return bsearchItem(item);
  }
#endif

  void setAutoSort(int as) {
    auto_sort = as;
  }
  int getAutoSort() const {
    return auto_sort;
  }

  void removeDups() {
    ASSERTPR(!(!auto_sort && need_sorting), "must call sort() first if auto-sorting is disabled");
    sort();
    for (int i = 1; i < getNumItems(); i++) {
      if (C::compareItem(enumItem(i-1), enumItem(i)) == 0) {
        delByPos(i);
        i--;
      }
    }
  }

private:
  int need_sorting;
  int auto_sort;
  int dups_low, dups_hi, dups_pos;
};

// quicksort -- you still need to override the compare fns
template<class T, class C>
class QuickSorted {
public:
  static void _sort(T **items, int nitems) {
    if (items == NULL || nitems <= 1) return;
    Qsort(items, 0, nitems-1);
  }

private:
  static void swapItem(T **items, int a, int b) { // no bounds checking!
    T *tmp = items[a];
    items[a] = items[b];
    items[b] = tmp;
  }

  static void Qsort(T **items, int lo0, int hi0) {
    int lo = lo0, hi = hi0;
    if (hi0 > lo0) {
      T *mid = items[(lo0 + hi0) / 2];
      while (lo <= hi) {
        while ((lo < hi0) && (C::compareItem(items[lo], mid) < 0)) lo++;
        while ((hi > lo0) && (C::compareItem(items[hi], mid) > 0)) hi--;

        if (lo <= hi) {
          swapItem(items, lo, hi);
          lo++;
          hi--;
        }
      }

      if (lo0 < hi) Qsort(items, lo0, hi);
      if (lo < hi0) Qsort(items, lo, hi0);
    }
  }
};

// easy way to specify quicksorting, just data type and comparison class
template <class T, class C> class PtrListQuickSorted : public PtrListSorted<T, C, QuickSorted<T, C> > {
public:
  PtrListQuickSorted(int initial_size=0) : PtrListSorted<T, C, QuickSorted<T, C> >(initial_size) {}
};


// easy way to get a list sorted by pointer val
class SortByPtrVal {
public:
  static int compareItem(void *p1, void *p2) {
    return CMP3(p1, p2);
  }
  static int compareAttrib(const char *attrib, void *item) {
    return CMP3((void *)attrib, item);
  }
};

template <class T> class PtrListQuickSortedByPtrVal : public PtrListQuickSorted<T, SortByPtrVal > {};


// this class automatically inserts at the correct position, so
// the binary searches are very fast if you need to insert and search often (no need to sort)
template< class T, class C >
class PtrListInsertSorted : public PtrList<T> {
public:
  PtrListInsertSorted() { disable_sort = 0; }
  T *addItem(T *item) {
    if (getNumItems() == 0) return PtrList<T>::addItem(item);

    int bot = 0, top = getNumItems()-1, mid;

    int insertpoint=-1;
    if (!disable_sort ) {
      for (int c = 0; c < getNumItems()+1; c++) {
        if (bot > top) {
          // insert here
          insertpoint=bot;
          break;
        }
        mid = (bot + top) / 2;
        int r = C::compareItem(item, getItemList()[mid]);
        if (r == 0) {
          // insert here
          insertpoint=mid;
          break;
        }
        if (r < 0) {
          top = mid-1;
        } else {
          bot = mid+1;
        }
      }
      ASSERTPR(insertpoint != -1, "insertsort/binary search fucked up");
    } else
      insertpoint = PTRLIST_POS_LAST;
    return PtrList<T>::addItem(item,insertpoint);
  }  
  T *findItem(const char *attrib, int *pos=NULL) {
    if (isSorted()) {
      // binary search
      if (getNumItems() == 0) return NULL;

      int bot = 0, top = getNumItems()-1, mid;

      for (int c = 0; c < getNumItems()+1; c++) {
        if (bot > top) return NULL;
        mid = (bot + top) / 2;
        int r = C::compareAttrib(attrib, getItemList()[mid]);
        if (r == 0) {
          if (pos != NULL) *pos = mid;
          return getItemList()[mid];
        }
        if (r < 0) {
          top = mid-1;
        } else {
          bot = mid+1;
        }
      }
      ASSERTPR(0,"binary search fucked up");
    } else {
      // linear search
      for (int i = 0; i < getNumItems(); i++) {
        int r = C::compareAttrib(attrib, getItemList()[i]);
        if (r == 0) {
          if (pos != NULL) *pos = i;
          return getItemList()[i];
        }
      }
    }
    return NULL;
  }
  T *findItem(T *attrib, int *pos=NULL) {
    return findItem((const char *)attrib, pos);
  }
  void setSorted(int dosort) { disable_sort = !dosort; }
  int isSorted() { return !disable_sort; }
  int disable_sort;
};

// this list allows you to have multiple items with same attrib and adds findLastItem so you can
// sort on more than just one item. this can be used to make autosorting lists of overriding items
// which you can add and remove at will.
template <class T, class C> class PtrListQuickMultiSorted : public PtrListQuickSorted<T, C> {
public:
  PtrListQuickMultiSorted(int initial_size=0) : PtrListQuickSorted<T, C>(initial_size) {}
  T *findLastItem(const char *attrib, int *pos=NULL) {
    sort();
    int p = 0;
    int fp = 0;
    T *item = findItem(attrib, &fp);
    if (!item) return NULL;

    p = fp;

    while (1) {
      p++;
      if (p >= getNumItems()) break;
      T* i = enumItem(p);
      if (!C::compareAttrib(attrib, i)) {
        fp = p;
        item = i;
      } else break;
    }
    
    if (pos != NULL) *pos = fp;
    return item;
  }
};

#include "foreach.h"

#endif
