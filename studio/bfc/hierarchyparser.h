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

#ifndef __PARAMPARSE_H
#define __PARAMPARSE_H

#include "../bfc/util/pathparse.h"
#include "../bfc/node.h"
#include "../bfc/string.h"


typedef NodeC<String> HPNode;

class COMEXP HierarchyParser {
public:    
  HierarchyParser(const char *str = NULL, const char *_sibling=";", const char *_escape="\\", const char *_parent_open="(", const char *_parent_close=")") ;
  HierarchyParser(HPNode *_rootnode, const char *_sibling=";", const char *_escape="\\", const char *_parent_open="(", const char *_parent_close=")") ;
  ~HierarchyParser();

  HPNode *findGuid(GUID g);
  HPNode *findString(const char *str);

  int hasGuid(GUID g) { return findGuid(g) != NULL; }
  int hasString(const char *str) { return findString(str) != NULL; }

  HPNode *rootNode() { return rootnode; }

private:
  HPNode *rootnode;
  int myalloc;

  String sibling;
  String escape;
  String parent_open;
  String parent_close;

  inline int isSibling(char c) {
    return sibling.lFindChar(c) != -1;
  }
  inline int isEscape(char c) {
    return escape.lFindChar(c) != -1;
  }
  inline int isParentOpen(char c) {
    return parent_open.lFindChar(c) != -1;
  }
  inline int isParentClose(char c) {
    return parent_close.lFindChar(c) != -1;
  }
};

#endif
