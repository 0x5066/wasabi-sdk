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

#ifndef _DRAGITEMI_H
#define _DRAGITEMI_H

#include "dragitem.h"
#include "common.h"
#include "../bfc/string.h"
#include "../bfc/ptrlist.h"

class COMEXP DragItemI : public DragItem {
public:
  DragItemI(const char *datatype, void *datum=NULL);
  virtual ~DragItemI() {}

  void addVoidDatum(void *newdatum);	// up to you to cast it right

  const char *getDatatype();
  int getNumData();
  void *getDatum(int pos=0);

private:
  RECVS_DISPATCH;

  String datatype;
  PtrList<char> datalist;
};

template <class T>
class DragItemT : public DragItemI {
public:
  DragItemT(T *item=NULL) : DragItemI(T::dragitem_getDatatype(), item) {}
  static inline DragItemI *create(T *item) { return new DragItemI<T>(item); }

  void addDatum(T *newdatum) {
    addVoidDatum(static_cast<void *>(newdatum));
  }
};

template <class T>
class DragItemCast {
public:
  DragItemCast(DragItem *_item, int _pos=0) : item(_item), pos(_pos) {}
  operator T *() {
    if (item == NULL || !STREQL(T::dragitem_getDatatype(), item->getDatatype()))
      return NULL;
    else
      return static_cast<T*>(item->getDatum(pos));
  }
private:
  DragItem *item;
  int pos;
};

#endif
