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

#ifndef _DEBUGWND_H
#define _DEBUGWND_H

#include "../common/listwnd.h"
#include "../studio/consolecb.h"

class ButtonWnd;

#define DEBUGWND_PARENT ListWnd
class DebugWnd : public DEBUGWND_PARENT, private ConsoleCallbackI {
public:
  virtual ~DebugWnd();

  virtual int onInit();

  // from ConsoleCallbackI
  virtual int consolecb_outputString(int severity, const char *string);

  static GUID getWindowTypeGuid() {
    // {7383A6FB-1D01-413b-A99A-7E6F655F4591}
    const GUID ret = 
    { 0x7383a6fb, 0x1d01, 0x413b, { 0xa9, 0x9a, 0x7e, 0x6f, 0x65, 0x5f, 0x45, 0x91 } };
    return ret;
  }
  static const char *getWindowTypeName() { return "Debug console"; }
  static void setIconBitmaps(ButtonWnd *button);
};

#endif
