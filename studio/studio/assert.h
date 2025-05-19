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


#ifndef _ASSERT_H
#define _ASSERT_H

#include "../bfc/std.h"

#ifdef CPLUSPLUS
extern "C" {
#endif

COMEXP void _assert_handler(const char *reason, const char *file, int line);
COMEXP void _assert_handler_str(const char *string, const char *reason, const char *file, int line);

#ifdef CPLUSPLUS
}
#endif

#ifdef ASSERT
#undef ASSERT
#endif

#define ASSERTS_ENABLED

#ifdef ASSERTS_ENABLED

  #define ASSERT(x) ((x) ? void() : _assert_handler(#x, __FILE__, __LINE__))
  #define ASSERTPR(x, str) ((x) ? void() : _assert_handler_str((str), #x, __FILE__, __LINE__))
  #define ASSERTALWAYS(str) _assert_handler_str((str), 0, __FILE__, __LINE__)
#else
  #define ASSERT(x) ;
  #define ASSERTPR(x,y) ;
  #define ASSERTALWAYS ;
#endif

#endif
