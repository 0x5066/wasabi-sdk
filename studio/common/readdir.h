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

#ifndef _READDIR_H
#define _READDIR_H

#include "common.h"
#include "../bfc/string.h"

/* intended use:
  ReadDir dir(path);
  while (dir.next()) {
    const char *fn = dir.getFilename();
  }
*/

class COMEXP ReadDir {
public:
  ReadDir(const char *path, const char *match=NULL, BOOL skipdots=TRUE);
  ~ReadDir();

  int next();	// done when returns 0
  const char *getFilename();
  int isDir();	// if current file is a dir
  int isReadonly();	// if current file is readonly

  int isDotDir();	// if given dir iteself is being enumerated (usually ".")
  int isDotDotDir();	// if parent dir of cur dir is showing (usually "..")

  const char *getPath() { return path; }

private:
  String path, match;
  int skipdots;
#ifdef WIN32
  HANDLE files;
  int first;
  WIN32_FIND_DATA data;
  WIN32_FIND_DATAW dataW; // (shrug) so we have two?  so what?
  int utf16;
  String filename8;
#endif
};

#endif
