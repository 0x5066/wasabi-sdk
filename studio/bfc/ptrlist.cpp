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


#include "ptrlist.h"

// if this symbol is enabled, do sanity checking in a bunch of methods
//#define DO_VERIFY_PTRLIST

#if defined(ASSERTS_ENABLED) && defined(DO_VERIFY_PTRLIST)
#define VERIFY_PTRLIST verify()
#else
#define VERIFY_PTRLIST
#endif

PtrListRoot::PtrListRoot(int initial_size) {
  nitems = nslots = 0;
  items = NULL;
  setMinimumSize(initial_size);
}

PtrListRoot::PtrListRoot(const PtrListRoot *from) {
  nitems = nslots = 0;
  items = NULL;
  copyFrom(from);
}

PtrListRoot::~PtrListRoot() {
  removeAll();
}

void PtrListRoot::copyFrom(const PtrListRoot *from) {
  ASSERT(from != NULL);
  int n = from->getNumItems();
  removeAll();
  if (n > 0) {
    void **mem = from->getItemList();
    ASSERT(mem != NULL);
    setMinimumSize(n);
    if (items != NULL) FREE(items);
    items = (void**)MEMDUP(mem, n*sizeof(void*));
    nitems = n;
  }
  VERIFY_PTRLIST;
}

void PtrListRoot::setMinimumSize(int _nslots) {
  ASSERT(_nslots >= 0);
  if (_nslots == 0 || _nslots <= nslots) return;
  nslots = _nslots;
  if (items)
    items = (void **)REALLOC(items, sizeof(void *)*nslots);
  else
    items = (void **)MALLOC(sizeof(void *)*nslots);
  VERIFY_PTRLIST;
}

int PtrListRoot::getNumItems() const {
  return nitems;
}

void *PtrListRoot::enumItem(int n) const {
  if (items == NULL || n < 0 || n >= nitems) return NULL;
  return items[n];
}

void *PtrListRoot::addItem(void *item, int pos, int increment) {
  ASSERT(increment > 0);
  ASSERTPR(item != NULL, "can't put NULLs into ptrlists");
  ASSERT(nitems <= nslots);

  // expand if necessary
  if (items == NULL || nslots == nitems)
    setMinimumSize(nslots + increment);

  items[nitems++] = item;
  if (pos != PTRLIST_POS_LAST) moveItem(nitems-1, pos);
  VERIFY_PTRLIST;
  return item;
}

// replace what's in the slot with the new value
void *PtrListRoot::setItem(void *item, int pos) {
  ASSERT(nitems >= pos);
  items[pos] = item;
  VERIFY_PTRLIST;
  return item;
}

void PtrListRoot::reverse() {
  if (nitems <= 1) return;
  MemBlock<void *> block(nitems, items);
  void **blockmem = block.getMemory();
  for (int i = 0, j = nitems-1; j >= 0; i++, j--) {
    items[i] = blockmem[j];
  }
}

void PtrListRoot::moveItem(int from, int to) {
  if (from == to) return;
  void *ptr=items[from];
  if (nitems > from+1) // if moving from the end, there is nothing to shift behind our src position
    //IMPORTANT note for future ports: This assumes MEMCPY accepts overlapping segments.
    MEMCPY(&items[from], &items[from+1], (nitems-from)*sizeof(void *)); 
  if (to > from) to--;
  if (nitems > to) // if moving to the end, there is nothing to shift behind our target position
    MEMCPY(&items[to+1], &items[to], (nitems-to-1)*sizeof(void *));
  items[to] = ptr;
  VERIFY_PTRLIST;
}

int PtrListRoot::removeItem(void *item) {
  int c = 0;

  if (item == NULL || items == NULL || nitems <= 0) return 0;

  // count occurences of item in items, remember the first one
  void **p = items;
  int first=-1;
  for (int i=0;i<nitems;i++,p++) {
    if (*p==item) {
      if (c++ == 0)
        first = i;
    }
  }

  // if we found one, remove it
  if (c>0) {
    removeByPos(first); // delByPos is faaast
    c--; // decrease count
  }
  VERIFY_PTRLIST;

  return c; // returns how many occurences of this item left
}

void PtrListRoot::removeEveryItem(void *item) {
  while (removeItem(item));
  VERIFY_PTRLIST;
}

void PtrListRoot::removeByPos(int pos) {
  if (pos < 0 || pos >= nitems) return;  //JC
  --nitems;
  if (pos == nitems) return;	// last one? nothing to copy over
  MEMCPY(items+pos, items+(pos+1), sizeof(void *)*(nitems-pos));  // CT:not (nitems-(pos+1)) as nitems has been decremented earlier
  VERIFY_PTRLIST;
}

void PtrListRoot::removeLastItem() {
  if (nitems == 0 || items == NULL) return;
  nitems--;	// hee hee
  VERIFY_PTRLIST;
}

void PtrListRoot::removeAll() {
  FREE(items); items = NULL;
  nitems = 0;
  nslots = 0;
  VERIFY_PTRLIST;
}

void PtrListRoot::freeAll() {
  for (int i = 0; i < nitems; i++)  //JC
    if (items) FREE(items[i]);
  removeAll();
  VERIFY_PTRLIST;
}

int PtrListRoot::searchItem(void *item) const {
  for (int i=0; i < nitems; i++)
    if (items[i] == item)
      return i;
  return -1;
}

void PtrListRoot::verify() {
#ifdef DO_VERIFY_PTRLIST
  ASSERT(nitems >= 0);
  ASSERT(nslots >= 0);
  ASSERT(nitems <= nslots);
#endif
}

#if 0
int PtrListRoot::bsearchItem(void *item) const {
  // do binary search
  if (nitems == 0 || items == NULL) return -1;

  int bot = 0, top = nitems-1, mid;

  for (int c = 0; c < nitems+16; c++) {
    if (bot > top) return -1;
    mid = (bot + top) / 2;
    if (item == items[mid]) return mid;
    if (item < items[mid]) top = mid-1;
    else bot = mid+1;
  }
  ASSERTPR(0, "binary search fucked up");
  return -1;
}
#endif
