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

#ifndef CHARPTRSEQUENCE_H
#define CHARPTRSEQUENCE_H

#include "..\studio\sequence.h"
#include "..\bfc\string.h"

template< int LOOP = 0 >
class CharPtrSequencer : public ListSequencer {
public:
  CharPtrSequencer() : itemList() {
    current = 0;
  }
  ~CharPtrSequencer() {
    itemList.deleteAll();
  }

public:
  //
  //  Sequencer Methods
  //
  virtual int getNumEntries() {
    return itemList.getNumItems();
  }
  virtual const char *enumItem(int pos) {
    return *(itemList[pos]);
  }
  virtual int getCurrent() {
    return current;
  }
  virtual int setCurrent(int cur) {
    return current = cur;
  }
  virtual int loop() { return LOOP; }	// override as necessary

public:
  //
  //  Pointer List Methods
  //
  virtual const char *addItem(const char *item) {
    return *(itemList.addItem(new String(item)));
  }
  virtual void deleteAll() {
    itemList.deleteAll();
  }
  virtual int findItem(const char *item) {
    int i,num = itemList.getNumItems();
    for (i = 0; i < num; i++) {
      // Lexigraphical compare, for those paying attention.
      if (*(itemList[i]) == item) {
        return i;
      }
    }
    return -1;
  }
  virtual void deleteItem(const char *item) {
    int i = findItem(item);
    if (i > 0) {
      deleteItem(i);
    }
  }
  virtual void deleteItem(int item) {
    itemList.deleteItem(item);
  }
  virtual const char *getFirst() {
    int num = itemList.getNumItems();
    if (num > 0) {
      return *(itemList[0]);
    }
    return NULL;
  }
  virtual const char *getLast() {
    int num = itemList.getNumItems();
    if (num > 0) {
      return *(itemList[num-1]);
    }
    return NULL;
  }
  virtual int getNumItems() {
    return itemList.getNumItems();
  }


private:
  int current;
  PtrList< String > itemList;
};

#endif//CHARPTRSEQUENCE_H