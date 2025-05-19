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


#include "thread.h"

DWORD WINAPI Thread::ThreadProc(void *param) {
  friend class Thread;
  Thread *th = static_cast<Thread*>(param);
  th->isrunning = 1;
  int ret = th->threadProc();
  th->isrunning = 0;
  return ret;
}

Thread::Thread() {
  isrunning = 0;
  killswitch = 0;
  handle = CreateThread(NULL, 0, ThreadProc, (LPVOID)this, CREATE_SUSPENDED, &threadid);
}

Thread::~Thread() {
  setKillSwitch(TRUE);
  end();
  // FUCKO: might need to get rough here
  CloseHandle(handle);
}

int Thread::start() {
  if (running()) return 1;
  return !(ResumeThread(handle) == 0xffffffff);
}

BOOL Thread::running() {
#if 0
  if (handle == 0) return FALSE;
  DWORD code;
  GetExitCodeThread(handle, &code);
  return (code == STILL_ACTIVE);
#else
  return isrunning;
#endif
}

int Thread::end() {
  while (running()) Sleep(66);
  //if(running()) TerminateThread(handle,0); //CT> maybe kill() should really kill?
  return 1;
}

void Thread::setKillSwitch(int k) {
  killswitch = k;
}

THREADID Thread::getThreadId() const {
  return threadid;
}
