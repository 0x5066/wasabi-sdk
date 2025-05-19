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

#ifndef _GUID_H
#define _GUID_H

#include "platform.h"

#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef struct _GUID {
  unsigned long Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[8];
} GUID;

#ifndef _REFCLSID_DEFINED
#define REFGUID const GUID &
#define _REFCLSID_DEFINED
#endif

#endif // !GUID_DEFINED

#ifdef _WIN32
// this is for GUID == and !=
#include <objbase.h>
#ifndef GUID_EQUALS_DEFINED
#define GUID_EQUALS_DEFINED
#endif
#else

#ifdef __cplusplus
#ifndef GUID_EQUALS_DEFINED
#define GUID_EQUALS_DEFINED
static __inline int operator ==(const GUID &a, const GUID &b) {
  return !MEMCMP(&a, &b, sizeof GUID);
}
#endif	//GUID_EQUALS_DEFINED
#endif	//__cplusplus
#endif	//_WIN32



static const GUID INVALID_GUID = { 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0} };
static const GUID GENERIC_GUID = { 0xFFFFFFFF, 0xFFFF, 0xFFFF, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} };

#endif
