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

#ifndef _FILENAME_H
#define _FILENAME_H

#include "string.h"
#include "playstring.h"
#include "../bfc/dispatch.h"

// a simple class to drag-and-drop filenames around

#define DD_FILENAME "DD_Filename v1"

class Filename : public Dispatchable {
public:
  const char *getFilename();
  operator const char *() { return getFilename(); }

  static const char *dragitem_getDatatype() { return DD_FILENAME; }

protected:
  enum {
    GETFILENAME=100,
  };
};

inline const char *Filename::getFilename() {
  return _call(GETFILENAME, (const char *)NULL);
}

// a basic implementation to use
class COMEXP FilenameI : public Filename, public String {
public:
  FilenameI(const char *nf) : String(nf) { }
  const char *getFilename() { return getValue(); }

protected:
  FilenameI(const FilenameI &fn) {}
  FilenameI& operator =(const FilenameI &ps) { return *this; }
  RECVS_DISPATCH;
};

// another implementation that doesn't provide its own storage
class COMEXP FilenameNC : public Filename {
public:
  FilenameNC(const char *str) { fn = str; }
  const char *getFilename() { return fn; }

protected:
  RECVS_DISPATCH;

private:
  const char *fn;
};

// another implementation that uses the central playstring table
class COMEXP FilenamePS : public Filename, private Playstring {
public:
  FilenamePS(const char *str) : Playstring(str) {}
  const char *getFilename() { return getValue(); }

protected:
  FilenamePS(const FilenamePS &fn) {}
  FilenamePS& operator =(const FilenamePS &ps) { return *this; }

  RECVS_DISPATCH;
};

#endif
