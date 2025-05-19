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

#ifndef _NAMED_H
#define _NAMED_H

#include "../common/common.h"
#include "string.h"

class COMEXP NOVTABLE Named {
public:
  Named(const char *initial_name=NULL) : name(initial_name) {}
  virtual ~Named() {}	// placeholder to ensure name is destructed properly
  const char *getName() const { return name; }
  const char *getNameSafe(const char *defval=NULL) const {
    const char *str = name;
    return str ? str : (defval ? defval : "(null)");
  }
  void setName(const char *newname) { name = newname; onSetName(); }
  // override this to catch name settings
  virtual void onSetName() {}

private:
  String name;
};

#endif
