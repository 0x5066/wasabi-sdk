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


#include "freelist.h"

// define this to turn off freelist behavior
//#define FREELIST_FUCT

FreelistPriv::FreelistPriv() {
  total_allocated = 0;
}

FreelistPriv::~FreelistPriv() {
#ifdef ASSERTS_ENABLED
//  ASSERTPR(total_allocated == 0, "didn't free entire freelist!(1)");
//  ASSERTPR(blocks.getNumItems() == 0, "didn't free entire freelist!(2)");
  if (total_allocated != 0) OutputDebugString("didn't free entire freelist!(1)\n");
  if (blocks.getNumItems() != 0) OutputDebugString("didn't free entire freelist!(2)\n");
#endif
}

void *FreelistPriv::getRecord(int typesize, int blocksize, int initialblocksize) {
#ifdef FREELIST_FUCT
  return MALLOC(typesize);
#else
  ASSERT(typesize >= sizeof(void *));
  FLMemBlock *mem = NULL;
  void *freelist = NULL;
  for (int i = 0; i < blocks.getNumItems(); i++) {
    mem = blocks[i];
    if (mem->freelist != NULL) break;
    mem = NULL;
  }
  if (mem == NULL) {
    // figure record count for this new block
    int siz = (blocks.getNumItems() ? blocksize : initialblocksize);
    // allocate another block of memory
    mem = new FLMemBlock(siz*typesize);
    // prelink it into a freelist
    char *record = static_cast<char *>(mem->freelist);
    void **ptr;
    for (int i = 0; i < siz-1; i++) {
      ptr = reinterpret_cast<void **>(record);
      record += typesize;
      *ptr = static_cast<void *>(record);
    }
    // terminate newly made freelist
    ptr = reinterpret_cast<void **>(record);
    *ptr = NULL;
    blocks.addItem(mem);
  }
  // get first free record
  void *ret = mem->freelist;
  // advance freelist *
  mem->freelist = *(static_cast<void **>(mem->freelist));
  mem->nallocated++;
  total_allocated++;
  return ret;
#endif
}

void FreelistPriv::freeRecord(void *record) {
#ifdef FREELIST_FUCT
  FREE(record);
#else
  FLMemBlock *mem=NULL;
  for (int i = 0; i < blocks.getNumItems(); i++) {
    mem = blocks[i];
    if (mem->isMine(reinterpret_cast<MBT*>(record))) break;
    mem = NULL;
  }
  ASSERTPR(mem != NULL, "attempted to free record with no block");
  // stash it back on the block's freelist
  *reinterpret_cast<void **>(record) = mem->freelist;
  mem->freelist = record;
  ASSERT(mem->nallocated > 0);
  mem->nallocated--;
  if (mem->nallocated == 0) {
    blocks.delItem(mem);
  }
  total_allocated--;
#endif
}
