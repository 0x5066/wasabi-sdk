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

#ifndef __WILDCHARSENUM_H
#define __WILDCHARSENUM_H

#include "../bfc/tlist.h"
#include "../bfc/string.h"

using namespace wasabi;

class COMEXP WildcharsEnumerator {
  
  public:
  
    WildcharsEnumerator(const char *_selection);
    virtual ~WildcharsEnumerator() {};

    int getNumFiles();
    const char *enumFile(int n);
    void rescan();

    static int isWildchars(const char *filename);

  private:

    int utf16;
    String selection, path;
    HANDLE findhandle;
    TList<WIN32_FIND_DATA> finddatalist;
    TList<WIN32_FIND_DATAW> finddatalistW;
    String singfiledup;
    String enumFileString;
};

#endif
