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
** Copyright (C) 2001 Nullsoft, Inc.
** Author: Justin Frankel
** File: httpserv.cpp - JNL HTTP GET/POST serving implementation
** License: see jnetlib.h
**
** This class just manages the http reply/sending, not where the data 
** comes from, etc.
*/

#include "netinc.h"
#include "util.h"

#include "httpserv.h"

/*
  States for m_state:
    -1 error (connection closed, etc)
    0 not read request yet.
    1 reading headers
    2 headers read, have not sent reply
    3 sent reply
    4 closed
*/

JNL_HTTPServ::JNL_HTTPServ(JNL_Connection *con)
{
  m_con=con;
  m_state=0;
  m_reply_headers=0;
  m_reply_string=0;
  m_recvheaders=0;
  m_recv_request=0;
  m_recvheaders_size=0;
  m_errstr=0;
  m_reply_ready=0;
}

JNL_HTTPServ::~JNL_HTTPServ()
{
  free(m_recv_request);
  free(m_recvheaders);
  free(m_reply_string);
  free(m_reply_headers);
  free(m_errstr);
  delete m_con;
}

int JNL_HTTPServ::run()
{ // returns: < 0 on error, 0 on connection close, 1 if reading request, 2 if reply not sent, 3 if reply sent, sending data.
  int cnt=0;
run_again:
  m_con->run();
  if (m_con->get_state()==JNL_Connection::STATE_ERROR)
  {
    seterrstr(m_con->get_errstr());
    return -1;
  }
  if (m_con->get_state()==JNL_Connection::STATE_CLOSED) return 4;

  if (m_state == 0)
  {
    if (m_con->recv_lines_available()>0)
    {
      char *buf=(char*)malloc(m_con->recv_bytes_available()-1);
      m_con->recv_line(buf,m_con->recv_bytes_available()-1);
      free(m_recv_request);
      m_recv_request=(char*)malloc(strlen(buf)+2);
      strcpy(m_recv_request,buf);
      m_recv_request[strlen(m_recv_request)+1]=0;
      free(buf);
      buf=m_recv_request;
      while (*buf) buf++;
      while (buf >= m_recv_request && *buf != ' ') buf--;
      if (strncmp(buf+1,"HTTP",4) || strncmp(m_recv_request,"GET ",3))
      {
        seterrstr("malformed HTTP request");
        m_state=-1;
      }
      else
      {
        m_state=1;
        cnt=0;
        if (buf >= m_recv_request) buf[0]=buf[1]=0;

        buf=strstr(m_recv_request,"?");
        if (buf)
        {
          *buf++=0; // change &'s into 0s now.
          char *t=buf;
          int stat=1;
          while (*t) 
          {
            if (*t == '&' && !stat) { stat=1; *t=0; }
            else stat=0;
            t++;
          }
        }
      }
    }
    else if (!cnt++) goto run_again;
  }
  if (m_state == 1)
  {
    if (!cnt++ && m_con->recv_lines_available()<1) goto run_again;
    while (m_con->recv_lines_available()>0)
    {
      char buf[4096];
      buf[0]=0;
      m_con->recv_line(buf,4096);
      if (!buf[0]) { m_state=2; break; }
      if (!m_recvheaders)
      {
        m_recvheaders_size=strlen(buf)+1;
        m_recvheaders=(char*)malloc(m_recvheaders_size+1);
        if (m_recvheaders)
        {
          strcpy(m_recvheaders,buf);
          m_recvheaders[m_recvheaders_size]=0;
        }
      }
      else
      {
        int oldsize=m_recvheaders_size;
        m_recvheaders_size+=strlen(buf)+1;
        char *n=(char*)malloc(m_recvheaders_size+1);
        if (n)
        {
          memcpy(n,m_recvheaders,oldsize);
          strcpy(n+oldsize,buf);
          n[m_recvheaders_size]=0;
          free(m_recvheaders);
          m_recvheaders=n;
        }
      }
    }
  }
  if (m_state == 2)
  {
    if (m_reply_ready)
    {
      // send reply
      m_con->send_string((char*)(m_reply_string?m_reply_string:"HTTP/1.1 200 OK"));
      m_con->send_string("\r\n");
      if (m_reply_headers) m_con->send_string(m_reply_headers);
      m_con->send_string("\r\n");
      m_state=3;
    }
  }
  if (m_state == 3)
  {
    // nothing.
  }

  return m_state;
}

char *JNL_HTTPServ::get_request_file()
{
  // file portion of http request
  if (!m_recv_request) return NULL;
  char *t=m_recv_request;
  while (*t != ' ' && *t) t++;
  if (!*t) return NULL;
  while (*t == ' ') t++;
  return t;
}

char *JNL_HTTPServ::get_request_parm(char *parmname) // parameter portion (after ?)
{
  char *t=m_recv_request;
  while (*t) t++;
  t++;
  while (*t)
  {
    while (*t == '&') t++;
    if (!strnicmp(t,parmname,strlen(parmname)) && t[strlen(parmname)] == '=')
    {
      return t+strlen(parmname)+1;
    }
    t+=strlen(t)+1;
  }
  return NULL;
}

char *JNL_HTTPServ::getheader(char *headername)
{
  char *ret=NULL;
  if (strlen(headername)<1||!m_recvheaders) return NULL;
  char *buf=(char*)malloc(strlen(headername)+2);
  strcpy(buf,headername);
  if (buf[strlen(buf)-1]!=':') strcat(buf,":");
  char *p=m_recvheaders;
  while (*p)
  {
    if (!strnicmp(buf,p,strlen(buf)))
    {
      ret=p+strlen(buf);
      while (*ret == ' ') ret++;
      break;
    }
    p+=strlen(p)+1;
  }
  free(buf);
  return ret;
}

void JNL_HTTPServ::set_reply_string(char *reply_string) // should be HTTP/1.1 OK or the like
{
  free(m_reply_string);
  m_reply_string=(char*)malloc(strlen(reply_string)+1);
  strcpy(m_reply_string,reply_string);
}

void JNL_HTTPServ::set_reply_header(char *header) // "Connection: close" for example
{
  if (m_reply_headers)
  {
    char *tmp=(char*)malloc(strlen(m_reply_headers)+strlen(header)+3);
    strcpy(tmp,m_reply_headers);
    strcat(tmp,header);
    strcat(tmp,"\r\n");
    free(m_reply_headers);
    m_reply_headers=tmp;
  }
  else
  {
    m_reply_headers=(char*)malloc(strlen(header)+3);
    strcpy(m_reply_headers,header);
    strcat(m_reply_headers,"\r\n");
  }
}
