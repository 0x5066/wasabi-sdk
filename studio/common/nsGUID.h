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

#ifndef _NSGUID_H
#define _NSGUID_H

#include "../bfc/platform/guid.h"
#include "common.h"

// Some conversion functions to allow
// us to have GUIDs translatable to and from other data types.
class COMEXP nsGUID {
public:
  // To the "Human Readable" character format.
  // {1B3CA60C-DA98-4826-B4A9-D79748A5FD73}
  static char *toChar(const GUID &guid, char *target);
  static GUID fromChar(const char *source);
  // To the "C Structure" character format.
  //	{ 0x1b3ca60c, 0xda98, 0x4826, { 0xb4, 0xa9, 0xd7, 0x97, 0x48, 0xa5, 0xfd, 0x73 } };
  static char *toCode(const GUID &guid, char *target);
  static GUID fromCode(const char *source);

  // Compare function, returns -1, 0, 1
  static int compare(const GUID &a, const GUID &b);

  // strlen("{xx xxx xxx-xxxx-xxxx-xxxx-xxx xxx xxx xxx}"
  enum { GUID_STRLEN = 38 };
};

inline
int operator <(const GUID &a, const GUID &b) {
  return (nsGUID::compare(a, b) < 0);
}

#endif	//_NSGUID_H
