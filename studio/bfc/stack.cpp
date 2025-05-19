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

#include "stack.h"

#define STACK_SIZE_INCREMENT 250

StackBase::StackBase() {
  nslots = 0;
  cur = 0;
  stack = NULL;
}

StackBase::~StackBase() {
  FREE(stack);
}

int StackBase::push(void *item, int sizeofT) {
  if (stack == NULL) {
    nslots = STACK_SIZE_INCREMENT;
    stack = (char*)MALLOC(sizeofT * nslots);
  } else if (cur >= nslots) {
    int newnslots = nslots + STACK_SIZE_INCREMENT;
    stack = (char*)REALLOC(stack, sizeofT*newnslots);
    nslots = newnslots;
  }
  MEMCPY(stack + cur*sizeofT, item, sizeofT);
  cur++;
  return cur;
}

int StackBase::peek() {
  return cur;
}

void *StackBase::top(int sizeofT) {
  ASSERT(cur >= 0);
  if (cur == 0) return NULL;
  return stack + (cur-1)*sizeofT;
}

int StackBase::pop(void *ptr, int sizeofT) {
  ASSERT(cur >= 0);
  if (cur == 0) return 0;
  ASSERT(stack != NULL);
  --cur;
  if (ptr != NULL) MEMCPY(ptr, stack + cur*sizeofT, sizeofT);
  return 1;
}

int StackBase::isempty() {
  return cur == 0;
}
