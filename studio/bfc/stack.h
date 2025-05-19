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

//PORTABLE
#ifndef _STACK_H
#define _STACK_H

#include "../common/common.h"

#include "std.h"

// a self-growing stack. note that it never shrinks (for now)

class COMEXP StackBase {
protected:
  StackBase();
  ~StackBase();

  int push(void *item, int sizeofT);
  int peek();
  void *top(int sizeofT);
  int pop(void *ptr, int sizeofT);
  int isempty();

private:
  int nslots, cur;
  char *stack;
};

template<class T>
class Stack : public StackBase {
public:
  int push(T item) { return StackBase::push(&item, sizeof(T)); }
  using StackBase::peek;
  T top() { return *static_cast<T*>(StackBase::top(sizeof(T))); }
  int pop(T *ptr=NULL) { return StackBase::pop(ptr, sizeof(T)); }
  using StackBase::isempty;
};

#endif
