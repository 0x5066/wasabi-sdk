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

#ifndef _FREELIST_H
#define _FREELIST_H

#include "std.h"
//#include "common.h"
#include "memblock.h"
#include "ptrlist.h"

// actual implementation
class COMEXP FreelistPriv {
protected:
  FreelistPriv();
  ~FreelistPriv();

  void *getRecord(int typesize, int blocksize, int initialblocksize);
  void freeRecord(void *rec);

private:
  int total_allocated;
  typedef unsigned char MBT;
  class FLMemBlock : public MemBlock<MBT> {
  public:
    FLMemBlock(int siz) : MemBlock<MBT>(siz) {
      freelist = getMemory();
      nallocated = 0;
    }
    void *freelist;
    int nallocated;	// # of records assigned from block
  };
  PtrList< FLMemBlock > blocks; // the blocks of mem we alloc from
};

// type-safe template
static const int DEF_BLOCKSIZE=250;
template
<class T, int BLOCKSIZE=DEF_BLOCKSIZE, int INITIALBLOCKSIZE=DEF_BLOCKSIZE, int ALIGNMENT=4>
class Freelist : private FreelistPriv {
public:
  // just returns the memory, you have to call constructor manually
  T *getRecord() {
    return static_cast<T *>(FreelistPriv::getRecord(itemsize(), BLOCKSIZE, INITIALBLOCKSIZE));
  }

  // creates object via default constructor
  T *newRecord() {
    T *ret = getRecord();
#ifdef FORTIFY
#undef new
#endif
    new(ret) T;
#ifdef FORTIFY
#define new ZFortify_New
#endif
    return ret;
  }

  // creates object via 1-parameter constructor
  template<class P1>
  T *newRecord(P1 p1) {
    T *ret = getRecord();
#ifdef FORTIFY
#undef new
#endif
    new(ret) T(p1);
#ifdef FORTIFY
#define new ZFortify_New
#endif
    return ret;
  }

  // just frees it, you have to call destructor manually
  void freeRecord(T *record) { FreelistPriv::freeRecord(record); }

  // calls delete for you, and frees it
  void deleteRecord(T *record) {
    if (record != NULL) {
      record->~T();
      freeRecord(record);
    }
  }

  void deletePtrList(PtrList<T> *list) {
    ASSERT(list != NULL);
    for (int i = 0; i < list->getNumItems(); i++) {
      deleteRecord(list->enumItem(i));
    }
    list->removeAll();
  }

protected:
  int itemsize() { return (sizeof(T) + (ALIGNMENT-1)) - (sizeof(T) % ALIGNMENT); }
};

#endif
