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

#ifndef _WIN32_H
#define _WIN32_H

#ifndef WIN32
#error this file is only for win32
#endif

#ifndef _PLATFORM_H
#error this file should only be included from platform.h
#endif

// this should be the *only* place windows.h gets included!
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>

#if defined(_MSC_VER)	// msvc
# define WASABIDLLEXPORT __declspec(dllexport)
# if _MSC_VER >= 1100
#  define NOVTABLE __declspec(novtable)
# endif
#endif

#ifndef NOVTABLE
#define NOVTABLE
#endif


#ifndef _DEBUG
// release optimizations
#pragma optimize("gsy",on)
#pragma comment(linker,"/RELEASE")
// set the 512-byte alignment
#pragma comment(linker,"/opt:nowin98")
#endif	// _DEBUG

#endif
