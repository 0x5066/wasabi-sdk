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


#include "memblock.h"

VoidMemBlock::VoidMemBlock(int _size, const void *data) {
  mem = NULL;
  size = 0;
  setSize(_size);
  if (data != NULL && size > 0) MEMCPY(mem, data, size);
}

VoidMemBlock::~VoidMemBlock() {
  FREE(mem);
}

void *VoidMemBlock::setSize(int newsize) {
  ASSERT(newsize >= 0);
  if (newsize < 0) newsize = 0;
  if (newsize == 0) {
    FREE(mem);
    mem = NULL;
  } else if (size != newsize) {
    mem = REALLOC(mem, newsize);
  }
  size = newsize;
  return getMemory();
}

void VoidMemBlock::setMemory(const void *data, int datalen, int offsetby) {
  if (datalen <= 0) return;
  ASSERT(mem != NULL);
  ASSERT(offsetby >= 0);
  char *ptr = reinterpret_cast<char *>(mem);
  ASSERT(ptr + offsetby + datalen <= ptr + size);
  MEMCPY(ptr + offsetby, data, datalen);
}

int VoidMemBlock::getSize() const {
  return size;
}

int VoidMemBlock::isMine(void *ptr) {
  return (ptr >= mem && ptr < (char*)mem + size);
}

void VoidMemBlock::zeroMemory() {
  if (mem == NULL || size < 1) return;
  MEMZERO(mem, size);
}
