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

#ifndef _DRAGITEM_H
#define _DRAGITEM_H

#include "../bfc/dispatch.h"

// has 1 or more pointers to a named data type
class NOVTABLE DragItem : public Dispatchable {
public:
  const char *getDatatype() { return _call(GETDATATYPE, ""); }
  int getNumData() { return _call(GETNUMDATA, 0); }
  void *getDatum(int pos=0) { return _call(GETDATUM, (void*)NULL, pos); }

protected:
  enum {
    GETDATATYPE=100,
    GETNUMDATA=200,
    GETDATUM=300
  };
};

#endif
