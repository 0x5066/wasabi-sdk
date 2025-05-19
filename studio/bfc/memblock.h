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

#ifndef _MEMBLOCK_H
#define _MEMBLOCK_H

#include "std.h"
#include "../common/common.h"

class COMEXP VoidMemBlock {
protected:
  VoidMemBlock(int size=0, const void *data=NULL);
  ~VoidMemBlock();

  void *setSize(int newsize);
  void *getMemory() const { return mem; }
  void setMemory(const void *data, int datalen, int offsetby=0);
  int getSize() const;

  int isMine(void *ptr);

  void zeroMemory();

private:
  void *mem;
  int size;
};

// just a convenient resizeable block of memory wrapper
// doesn't handle constructors or anything, meant for int, char, etc.
template <class T>
class MemBlock : private VoidMemBlock {
public:
  MemBlock(int _size=0, const T *data=NULL) : VoidMemBlock(_size*sizeof(T), data) {}

  T *setSize(int newsize) {
    VoidMemBlock::setSize(newsize * sizeof(T));
    return getMemory();
  }

  T *getMemory() const { return static_cast<T *>(VoidMemBlock::getMemory()); }
  void setMemory(const T *data, int datalen, int offsetby=0) { VoidMemBlock::setMemory(data, datalen*sizeof(T), offsetby*sizeof(T)); }	// # of T's
  operator T *() const { return getMemory(); }
  int getSize() const { return VoidMemBlock::getSize()/sizeof(T); }// # of T's
  int getSizeInBytes() const { return VoidMemBlock::getSize(); }

  int isMine(T *ptr) { return VoidMemBlock::isMine(ptr); }

  void zeroMemory() { VoidMemBlock::zeroMemory(); }
};

#endif
