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


#include "dragitemi.h"

DragItemI::DragItemI(const char *_datatype, void *_data) :
    datatype(_datatype) {
  if (_data != NULL) addVoidDatum(_data);
}

void DragItemI::addVoidDatum(void *newdatum) {
  datalist.addItem(reinterpret_cast<char *>(newdatum));
}

const char *DragItemI::getDatatype() {
  return datatype;
};

int DragItemI::getNumData() {
  return datalist.getNumItems();
}

void *DragItemI::getDatum(int pos) {
  return reinterpret_cast<void *>(datalist[pos]);
}

#define CBCLASS DragItemI
START_DISPATCH;
  CB(GETDATATYPE, getDatatype);
  CB(GETNUMDATA, getNumData);
  CB(GETDATUM, getDatum);
END_DISPATCH;
#undef CBCLASS
