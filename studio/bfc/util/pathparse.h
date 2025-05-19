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

#ifndef _PATHPARSE_H
#define _PATHPARSE_H

#include "../../common/common.h"
#include "../ptrlist.h"
#include "../string.h"

/**
  PathParser is a class that parses a DOS/Windows (example: C:\DOS) 
  style path as well as a UNIX style path (example: /usr/local/bin/).
 
  @short    Path parser for Windows and UNIX style paths.
  @author Nullsoft
  @ver  1.0
*/
class COMEXP PathParser {
public:
  /**
    When PathParser is instantiated, the contructor takes the path to 
    parse and the separators to use to parse the path. It will then
    parse the path using the separators and make the parsed elements
    available. If no separators are given \ and / are used.
   
    @param str         The path to parse.
    @param separators  String that contains the separators to use. Single character separators only. 
                       No delimiters between separators in the string.
  */
  PathParser(const char *str, const char *separators="\\/");

  void setString(const char *string, const char *separators="\\/");

  /**
    Gets the number of path elements found in the path.
   
    @ret  The number of path elements found.
  */
  int getNumStrings();

  /**
    Gets the number of path elements found in the path.
   
    @ret  The number of path elements found.
  */
  char *enumString(int i);

  /**
    Gets the last path element from the parsed path.
   
    @ret  The last path element from the parsed path.
  */
  char *getLastString() { return enumString(getNumStrings()-1); }

protected:
  /**
    Override available for pre-processing of the 
    string before it's split. This is done at the start
    of the process() call.
    
    @param str A reference to the string to pre-process.
  */
  virtual void preProcess(String &str) { }
  
  /**
    Override available for post-processing of the pieces
    of the command line that are obtained after it's
    been split.
    
    @param str The command line piece to post-process.
  */
  virtual void postProcess(char *str) { }

private:
  void process();
  int processed;
  String str, separators;
  PtrList<char> strings;
};

#endif
