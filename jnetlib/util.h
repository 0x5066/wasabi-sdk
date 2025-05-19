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

/*
** JNetLib
** Copyright (C) 2000-2001 Nullsoft, Inc.
** Author: Justin Frankel
** File: util.h - JNL interface for basic network utilities
** License: see jnetlib.h
**
** routines you may be interested in:
**   JNL::open_socketlib(); 
**    opens the socket library. Call this once before using any network
**    code. If you create a new thread, call this again. Only really an
**    issue for Win32 support, but use it anyway for portability/
**
**   JNL::close_Socketlib();
**    closes the socketlib. Call this when you're done with the network,
**    after all your JNetLib objects have been destroyed.
**
**   unsigned long JNL::ipstr_to_addr(const char *cp); 
**    gives you the integer representation of a ip address in dotted 
**    decimal form.
**
**  JNL::addr_to_ipstr(unsigned long addr, char *host, int maxhostlen);
**    gives you the dotted decimal notation of an integer ip address.
**
*/

#ifndef _UTIL_H_
#define _UTIL_H_

class JNL
{
  public:
    static int open_socketlib();
    static void close_socketlib();
    static unsigned long ipstr_to_addr(const char *cp);
    static void addr_to_ipstr(unsigned long addr, char *host, int maxhostlen);
};

#endif //_UTIL_H_
