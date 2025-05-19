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

#include "blending.h"
#include "std.h"

class BlenderInit {
public:
  BlenderInit() { Blenders::init(); }
};
static BlenderInit blender_init;

void Blenders::init() {
  if (!alphatable[127][127]) {
    int i,j;
    for (j=0;j<256;j++)
      for (i=0;i<256;i++)
        alphatable[i][j] = (i*(j+1))>>8;
#ifndef NO_MMX
    DWORD retval1,retval2;	
    __try { 		
      _asm {
        mov eax, 1  // set up CPUID to return processor version and features
                    // 0 = vendor string, 1 = version info, 2 = cache info
        _emit 0x0f  // code bytes = 0fh,  0a2h
        _emit 0xa2
        mov retval1, eax		
        mov retval2, edx
      }	
    } __except(EXCEPTION_EXECUTE_HANDLER) { retval1 = retval2 = 0;}
    mmx_available = retval1 && (retval2&0x800000);
#endif
  }
}

#ifndef NO_MMX
int Blenders::mmx_available=0;
#endif

unsigned char Blenders::alphatable[256][256];
