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
** File: listen.cpp - JNL TCP listen implementation
** License: see jnetlib.h
*/

#include "netinc.h"
#include "util.h"
#include "listen.h"

JNL_Listen::JNL_Listen(short port, unsigned long which_interface)
{
  m_port=port;
  m_socket = ::socket(AF_INET,SOCK_STREAM,0);
  if (m_socket < 0) 
  {
  }
  else
  {
    struct sockaddr_in sin;
    SET_SOCK_BLOCK(m_socket,0);
#ifndef _WIN32
    int bflag = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &bflag, sizeof(bflag));
#endif
    memset((char *) &sin, 0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons( (short) port );
    sin.sin_addr.s_addr = which_interface?which_interface:INADDR_ANY;
    if (::bind(m_socket,(struct sockaddr *)&sin,sizeof(sin))) 
    {
      closesocket(m_socket);
      m_socket=-1;
    }
    else
    {  
      if (::listen(m_socket,8)==-1) 
      {
        closesocket(m_socket);
        m_socket=-1;
      }
    }
  }
}

JNL_Listen::~JNL_Listen()
{
  if (m_socket>=0)
  {
    closesocket(m_socket);
  }
}

JNL_Connection *JNL_Listen::get_connect(int sendbufsize, int recvbufsize)
{
  if (m_socket < 0)
  {
    return NULL;
  }
	struct sockaddr_in saddr;
	socklen_t length = sizeof(struct sockaddr_in);
	int s = accept(m_socket, (struct sockaddr *) &saddr, &length);
  if (s != -1)
  {
    JNL_Connection *c=new JNL_Connection(NULL,sendbufsize, recvbufsize);
    c->connect(s,&saddr);
    return c;
  }
  return NULL;
}
