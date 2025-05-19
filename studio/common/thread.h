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

#ifndef _THREAD_H
#define _THREAD_H

#include "common.h"

//CUT typedef DWORD (WINAPI *ThreadProc)(void *);

class COMEXP Thread {
public:
  Thread();
  virtual ~Thread();

  int start();
  BOOL running();
  int end();	// waits for thread to end, might want to throw killswitch too

  void setKillSwitch(int k);

  THREADID getThreadId() const;

  virtual int threadProc()=0;

protected:
  int killswitch;	// check this and return when it's nonzero

private:
  static DWORD WINAPI ThreadProc(void *param);

  HANDLE handle;
  THREADID threadid;
  int isrunning;
};

#endif
