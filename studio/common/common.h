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

#ifndef _COMMON_H
#define _COMMON_H

#ifdef WIN32
// disable "dll-interface to be used by clients of class" warning message.
#pragma warning(disable: 4251)

//CUT #ifndef _DEBUG
//CUT // release optimizations
//CUT // /Og (global optimizations), /Os (favor small code), /Oy (no frame pointers)
//CUT #pragma optimize("gsy",on)
//CUT #pragma comment(linker,"/RELEASE")
//CUT // set the 512-byte alignment
//CUT #pragma comment(linker,"/opt:nowin98")
//CUT #endif /* _DEBUG */
#endif /* WIN32 */

#if defined(COMMON_DLL)
#define COMEXP __declspec (dllexport) 
#elif defined(COMMON_STATIC_LINK)
#define COMEXP __declspec (dllimport)
#else
#define COMEXP
#endif

#ifndef __cplusplus 
#define EXTC extern 
#else
#define EXTC extern "C" 
#endif

#include "../bfc/std.h"

#endif
