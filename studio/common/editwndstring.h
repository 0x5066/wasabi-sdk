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

#ifndef _EDITWNDSTRING_H
#define _EDITWNDSTRING_H

#include "../common/editwnd.h"
#include "../bfc/string.h"
#include "../bfc/memblock.h"

// new christophe's code. or why make it a thousand lines when you can make it 10?
class EditWndString : public EditWnd {
public:
  void setBuffer(const char *buffer, int len=0) {
    b.setSize(len+1);
    char *bufmem=b.getMemory();
    if(len) STRNCPY(bufmem,buffer,len);
    bufmem[len]=0;
    EditWnd::setBuffer(bufmem,len);
  }
  const char *getBuffer() { return b.getMemory(); }
private:
  MemBlock<char> b;
};

#if 0
// old mig's code
// ============================================================================
//
//  class EditWndString
//
//    This class wraps the char * based methods of EditWnd into String-based
//  parameters and returns.  This object is much slower than EditWnd, but is
//  also much easier to use.  Welcome to the trade-offs of Object Oriented
//  Programming.  Luckily for us, the general "way code works" tells us that
//  90% of the lines of code in your app don't have an appreciable impact on
//  the real performance of functionality.  By that, they mean all this user
//  interface glop.  Who cares if it takes .005 ms or 1 ms to run your function
//  if you're only calling your function a few times every minute?
//
//    Of course, assuming that such things are okay without doublechecking your
//  code using a profiler is a really good way to write a slow app.  :)
//
//  *** This object is designed to be syntactically equivalent to EditWnd.
//  In other words, if you derive or use an EditWnd object, you can change
//  it to an EditWndString object without having to change your current
//  code (except to change the target of calls to the parent virtual).
//
//    Moreover, you don't even need to really figure this object out -- it's
//  the same as EditWnd, except you can use strings instead.
#define EDITWNDSTRING_PARENT EditWnd
class COMEXP EditWndString : public EDITWNDSTRING_PARENT {
public:
  EditWndString(char * buffer = NULL, int buflen = 0);
  EditWndString(const String & initParam, int buflen = 0);

  virtual ~EditWndString();
  virtual void setBuffer(const String & buffer, int buflen = 0);
  // Note that this is part of why this object is "slow" -- it is
  // copying a string as a return value on the stack.  When you
  // copy that to your internal string, you then must copy the
  // data from one string to the other.  Slow?  Absolutely.
  // Easier than getBuffer/setBuffer?  Indubitably.
//  virtual String getBuffer();
  virtual const char *getBuffer() { return str_buffer; }   // CT> un-gay-ed
  // Because the "fast" version of this object remains unchanged, you
  // can (pretty much) just use the other one if you need higher
  // performance from your objects.  But how much performance does
  // one really need to squeeze out of one's Edit UI object?

  //
  //  These methods are overridden virtually to provide backwards
  // compatibility with legacy code - even if you only have a pointer
  // to EditWnd and not EditWndString.  This is an easily overlooked
  // but very important issue in legacy codebase management.
  virtual void getBuffer(char *outbuf, int len); 
  virtual void setBuffer(char * buffer, int len);

protected:
  virtual void allocFromString(const String & fromString, int buflen = 0);
  virtual void allocFromChar(const char * fromChar, int len);
  // Memory deallocation specific to this object & any descendants.
  virtual void deallocEWS();
  // Memory allocation specific to this object & any descendants.
  virtual void allocEWS(int size);

private:
  char * str_buffer;
  int    str_buflen;
  int    req_buflen;
};
#endif

#endif//_EDITWNDSTRING_H