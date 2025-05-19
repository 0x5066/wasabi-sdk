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

#include "recursedir.h"

RecurseDir::RecurseDir(const char *_path, const char *_match) :
  path(_path), match(_match)
{
  if (match.isempty()) match = Std::matchAllFiles();

  curdir = new ReadDir(path, match);
}

RecurseDir::~RecurseDir() {
  dirstack.deleteAll();
}

int RecurseDir::next() {
  for (;;) {
    if (curdir == NULL) {	// pop one off the stack
      curdir = dirstack.getLast();
      if (curdir == NULL) return 0;	// done
      dirstack.removeLastItem();
    }
    int r = curdir->next();
    if (r <= 0) {
      delete curdir; curdir = NULL;
      continue;	// get another one
    }

    // ok, we have a file to look at
    if (curdir->isDir()) {	// descend into it
      String newpath = curdir->getPath();
      newpath.cat(DIRCHARSTR);
      newpath.cat(curdir->getFilename());

      dirstack.addItem(curdir);	// push the old one
      curdir = new ReadDir(newpath, match);	// start new one

      continue;
    }

    return r;
  }
}

const char *RecurseDir::getPath() {
  if (curdir == NULL) return NULL;
  return curdir->getPath();
}

const char *RecurseDir::getFilename() {
  if (curdir == NULL) return NULL;
  return curdir->getFilename();
}
