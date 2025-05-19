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

#ifndef _RECURSEDIR_H
#define _RECURSEDIR_H

#include "../std.h"
#include "../ptrlist.h"
#include "../../common/common.h"
#include "../../common/readdir.h"

class ReadDir;

/**
  Read the contents of a directory, recursively.
  Also possible to use search match patterns.
  
  @short Recursive directory reading.
  @author Nullsoft
  @ver 1.0
  @see ReadDir
*/
class COMEXP RecurseDir {
public:
  /**
    Sets the directory to read and the match pattern.
    If no match pattern is set, it will match against
    all files.
    
    @param path   The path of the directory to read.
    @param match  The match pattern to use.
  */
  RecurseDir(const char *path, const char *match=NULL);
  
  /**
    Deletes the directory stack.
  */
  ~RecurseDir();

  /**
    Advance to the next file.
    
    @ret 0, No more files to read; > 0, Files left to read.
  */
  int next();
  
  /**
    Restart from the top of the directory tree.
    
    @ret 0
  */
  int restart();

  /**
    Get the current directory path.
    
    @ret The path.
  */
  const char *getPath();
  
  /**
    Get the filename for the current file.
    
    @ret The filename.
  */
  const char *getFilename();

private:
  String path, match;
  ReadDir *curdir;
  PtrList<ReadDir> dirstack;
};

#endif
