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

#include "common.h"
#include "../bfc/platform/guid.h"

#include "../studio/api.h"

#ifdef USE_MEM_CHECK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef COMMON_DLL
ComponentAPI *api;

// {06FFFA46-B009-40b1-91EA-953DD2EA5573}
GUID COMEXP commonGUID =
{ 0x6fffa46, 0xb009, 0x40b1, { 0x91, 0xea, 0x95, 0x3d, 0xd2, 0xea, 0x55, 0x73 } };


int COMEXP setCommonAPI(ComponentAPI *pApi) {
  api=pApi;
#ifdef USE_MEM_CHECK
  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
#endif
  return 1;
}
#endif

// here is our unbloated new/delete operators
#ifndef _DEBUG
void * __cdecl operator new(size_t size)
{
	return(GlobalAlloc(GPTR,size));
}
void __cdecl operator delete(void *pointer)
{
	GlobalFree(pointer);
}
#endif
