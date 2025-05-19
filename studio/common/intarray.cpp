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


#include "intarray.h"
#include "../studio/api.h"

enum { MAX_ARRAY=8 };

int IntArray::read(const char *name, int *x1, int *x2, int *x3, int *x4, int *x5, int *x6, int *x7, int *x8) {
  PtrList<int> list;
  if (x1) { list.addItem(x1); }
  if (x2) { list.addItem(x2); }
  if (x3) { list.addItem(x3); }
  if (x4) { list.addItem(x4); }
  if (x5) { list.addItem(x5); }
  if (x6) { list.addItem(x6); }
  if (x7) { list.addItem(x7); }
  if (x8) { list.addItem(x8); }
  ASSERT(list.getNumItems() >= 1);

  int array[MAX_ARRAY];	// gcc rules, msvc drools
  for (int i = 0; i < list.getNumItems(); i++) {
    if (list[i]) array[i] = *list[i];
  }
  if (!api->getIntArrayPrivate(name, array, list.getNumItems())) return 0;
  for (i = 0; i < list.getNumItems(); i++) {
    if (list[i]) *list[i] = array[i];
  }
  return 1;
}

void IntArray::write(const char *name, int x1) {
  int array[] = { x1 };
  api->setIntArrayPrivate(name, array, sizeof(array)/sizeof(int));
}

void IntArray::write(const char *name, int x1, int x2) {
  int array[] = { x1, x2 };
  api->setIntArrayPrivate(name, array, sizeof(array)/sizeof(int));
}

void IntArray::write(const char *name, int x1, int x2, int x3) {
  int array[] = { x1, x2, x3 };
  api->setIntArrayPrivate(name, array, sizeof(array)/sizeof(int));
}

void IntArray::write(const char *name, int x1, int x2, int x3, int x4) {
  int array[] = { x1, x2, x3, x4 };
  api->setIntArrayPrivate(name, array, sizeof(array)/sizeof(int));
}
