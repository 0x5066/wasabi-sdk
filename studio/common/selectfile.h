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

#ifndef _SELECTFILE_H
#define _SELECTFILE_H

#include "common.h"

#include "../bfc/ptrlist.h"
#include "../bfc/string.h"

class svc_fileSelector;
class RootWnd;

class COMEXP SelectFile {
public:
  SelectFile(RootWnd *parent, const char *menu_prefix=NULL, const char *menu_suffix=NULL);
  ~SelectFile();

  void setDefaultDir(const char *dir);	// default dir to use
  const char *getDirectory(); // return base directory after ok clicked
  void setIdent(const char *id);	// unless you saved one under this id

  void setPopPosition(int x, int y);	// in screen coords

  int runSelector(const char *type=NULL, int allow_multiple=FALSE, const char *extlist=NULL);	// if NULL, generate popup
  const char *getType();

  int getNumFiles();
  const char *enumFilename(int n);

private:
  int xpos, ypos;
  int pos_set;
  RootWnd *parentWnd;
  svc_fileSelector *svc;
  PtrList<String> types;
  String prefix_str, suffix_str;
  String default_dir, ident;
  String saved_type;
};

#endif
