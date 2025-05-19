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
** File: listen.h - JNL interface for opening a TCP listen
** License: see jnetlib.h
**
** Usage:
**   1. create a JNL_Listen object with the port and (optionally) the interface
**      to listen on.
**   2. call get_connect() to get any new connections (optionally specifying what
**      buffer sizes the connection should be created with)
**   3. check is_error() to see if an error has occured
**   4. call port() if you forget what port the listener is on.
**
*/

#ifndef _LISTEN_H_
#define _LISTEN_H_
#include "connection.h"

class JNL_Listen
{
  public:
    JNL_Listen(short port, unsigned long which_interface=0);
    ~JNL_Listen();

    JNL_Connection *get_connect(int sendbufsize=8192, int recvbufsize=8192);
    short port(void) { return m_port; }
    int is_error(void) { return (m_socket<0); }

  protected:
    int m_socket;
    short m_port;
};

#endif //_LISTEN_H_
