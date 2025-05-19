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

//NONPORTABLE (api is portable, implementation is not)
#ifndef _CRITSEC_H
#define _CRITSEC_H

#ifdef WIN32
#include <windows.h>
#endif

#include "common.h"
#include "../studio/assert.h"

class COMEXP CriticalSection {
public:
  CriticalSection();
  virtual ~CriticalSection();

  void enter();
  void leave();

  void inout();

private:
#ifdef ASSERTS_ENABLED
  int within;
#endif
  CRITICAL_SECTION cs;
};

// little helper class to ease the use of CriticalSection
// so you can do stuff like:
// void blah() {
//   InCriticalSection cs(&m_cs);
//   if(test) return 0;
//   return 1;
// }
class InCriticalSection {
public:
  InCriticalSection(CriticalSection *cs) : m_cs(cs) { m_cs->enter(); }
  InCriticalSection(CriticalSection &cs) : m_cs(&cs) { m_cs->enter(); }
  ~InCriticalSection() { m_cs->leave(); }
private:
  CriticalSection *m_cs;
};
#define INCRITICALSECTION(x) InCriticalSection __I_C_S__(x)

#endif
