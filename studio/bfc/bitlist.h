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

#ifndef _BITLIST_H
#define _BITLIST_H

#include "memblock.h"

class BitList {
public:
  BitList(unsigned int size=0) {
    setSize(size);
  }

  int getitem(int n) const { 
    if (n < 0 || n >= m_size) return 0; 
    return (m_list[n>>3]>>(n&7))&1;
  }
  void setitem(int n, int v) {
    if (n >= 0 && n < m_size) {
      int lv=1<<(n&7);
      if (v) m_list[n>>3]|=lv;
      else m_list[n>>3]&=~lv;
    }
  }
  
  int operator[](int n) const { return getitem(n); }

  int getSize() const {	// in bits
    return m_size;
  }
  void setSize(unsigned int newsize) {
    m_list.setSize((newsize+7)>>3);
    m_size = newsize;
  }
  
  int getsize() const { return m_size; }

private:
  MemBlock<unsigned char> m_list;
  int m_size;
};

#endif
