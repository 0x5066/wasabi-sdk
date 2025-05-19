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

#ifndef _XML_H
#define _XML_H

#include <stdio.h>

#include "common.h"
#include "../bfc/stack.h"

class COMEXP XMLWrite {
public:
  XMLWrite(const char *filename, const char *doctype);
  XMLWrite(FILE *file, const char *doctype);
  ~XMLWrite();

  void Init(FILE *file, const char *doctype);

  void comment(const char *comment);

  void pushCategory(const char *title);

  void pushCategoryAttrib(const char *title, int nodata=FALSE);
  void writeCategoryAttrib(const char *title, const int val);
  void writeCategoryAttrib(const char *title, const char *val);
  void closeCategoryAttrib();

  void writeAttrib(const char *title, const char *text);
  void writeAttrib(const char *title, int val);
  int popCategory();	// returns nonzero if more categories are open

  static void efprintf(FILE *fp, char *format, ...);

private:
  FILE *fp;
  int indent;
  Stack<char *>titles;
};

void COMEXP xmlUnescape(char *str);

#endif
