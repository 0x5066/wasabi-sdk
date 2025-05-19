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


#include "stringpool.h"

StringPool::StringPool(int _blocksize)
  : blocksize(_blocksize) {
  mem_used = 0;
}

StringPool::~StringPool() {
  blocks.deleteAll();
}

const char *StringPool::dupStr(const char *str) {
  int len = STRLEN(str)+1;
  block *b = blocks.getLast();
  for (;;) {
    if (b == NULL) {
      b = new block(blocksize);
      blocks.addItem(b);
    }
    if (b->freeblock_len < len) {
      if (b->nallocated == 0) return NULL;	// string too big
      b = NULL;
      continue;
    }
    break;
  }
  char *s = b->freeblock;
  STRCPY(s, str);	// including terminating 0
  b->nallocated++;
  b->freeblock += len;
  b->freeblock_len -= len;

  mem_used += len;

  return s;
}

void StringPool::delStr(const char *val) {
  mem_used -= STRLEN(val) + 1;
  for (int w = 0; w < blocks.getNumItems(); w++) {
    block *b = blocks[w];
    if (b->mem.isMine(const_cast<char *>(val))) {
      b->nfreed++;
      if (b->nfreed == b->nallocated) {
        blocks.delItem(b);
        delete b;
      }
      return;
    }
  }
}
