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
** File: connection.cpp - JNL TCP connection implementation
** License: see jnetlib.h
*/

#include "netinc.h"
#include "util.h"
#include "connection.h"


JNL_Connection::JNL_Connection(JNL_AsyncDNS *dns, int sendbufsize, int recvbufsize)
{
  m_errorstr="";
  if (dns == JNL_CONNECTION_AUTODNS)
  {
    m_dns=new JNL_AsyncDNS();
    m_dns_owned=1;
  }
  else
  {
    m_dns=dns;
    m_dns_owned=0;
  }
  m_recv_buffer_len=recvbufsize;
  m_send_buffer_len=sendbufsize;
  m_recv_buffer=(char*)malloc(m_recv_buffer_len);
  m_send_buffer=(char*)malloc(m_send_buffer_len);
  m_socket=-1;
  memset(m_recv_buffer,0,recvbufsize);
  memset(m_send_buffer,0,sendbufsize);
  m_remote_port=0;
  m_state=STATE_NOCONNECTION;
  m_recv_len=m_recv_pos=0;
  m_send_len=m_send_pos=0;
  m_host[0]=0;
  m_saddr = new struct sockaddr_in;
  memset(m_saddr,0,sizeof(m_saddr));
}

void JNL_Connection::connect(int s, struct sockaddr_in *loc)
{
  close(1);
  m_socket=s;
  m_remote_port=0;
  m_dns=NULL;
  if (loc) *m_saddr=*loc;
  else memset(m_saddr,0,sizeof(m_saddr));
  if (m_socket != -1)
  {
    SET_SOCK_BLOCK(m_socket,0);
    m_state=STATE_CONNECTED;
  }
  else 
  {
    m_errorstr="invalid socket passed to connect";
    m_state=STATE_ERROR;
  }
}

void JNL_Connection::connect(char *hostname, int port)
{
  close(1);
  m_remote_port=(short)port;
  m_socket=::socket(AF_INET,SOCK_STREAM,0);
  if (m_socket==-1)
  {
    m_errorstr="creating socket";
    m_state=STATE_ERROR;
  }
  else
  {
    SET_SOCK_BLOCK(m_socket,0);
    strncpy(m_host,hostname,sizeof(m_host)-1);
    m_host[sizeof(m_host)-1]=0;
    memset(m_saddr,0,sizeof(m_saddr));
    if (!m_host[0])
    {
      m_errorstr="empty hostname";
      m_state=STATE_ERROR;
    }
    else
    {
      m_state=STATE_RESOLVING;
      m_saddr->sin_family=AF_INET;
      m_saddr->sin_port=htons((unsigned short)port);
      m_saddr->sin_addr.s_addr=inet_addr(hostname);
    }
  }
}

JNL_Connection::~JNL_Connection()
{
  if (m_socket >= 0)
  {
    ::shutdown(m_socket, SHUT_RDWR);
    ::closesocket(m_socket);
    m_socket=-1;
  }
  free(m_recv_buffer);
  free(m_send_buffer);
  if (m_dns_owned) 
  {
    delete m_dns;
  }
  delete m_saddr;
}

void JNL_Connection::run(int max_send_bytes, int max_recv_bytes, int *bytes_sent, int *bytes_rcvd)
{
  int bytes_allowed_to_send=(max_send_bytes<0)?m_send_buffer_len:max_send_bytes;
  int bytes_allowed_to_recv=(max_recv_bytes<0)?m_recv_buffer_len:max_recv_bytes;

  if (bytes_sent) *bytes_sent=0;
  if (bytes_rcvd) *bytes_rcvd=0;

  switch (m_state)
  {
    case STATE_RESOLVING:
      if (m_saddr->sin_addr.s_addr == INADDR_NONE)
      {
        int a=m_dns?m_dns->resolve(m_host,(unsigned long int *)&m_saddr->sin_addr.s_addr):-1;
        if (!a) { m_state=STATE_CONNECTING; }
        else if (a == 1)
        {
          m_state=STATE_RESOLVING; 
          break;
        }
        else
        {
          m_errorstr="resolving hostname"; 
          m_state=STATE_ERROR; 
          return;
        }
      }
      if (!::connect(m_socket,(struct sockaddr *)m_saddr,16)) 
      {
        m_state=STATE_CONNECTED;
      }
      else if (ERRNO!=EINPROGRESS)
      {
        m_errorstr="connecting to host";
        m_state=STATE_ERROR;
      }
      else { m_state=STATE_CONNECTING; }
    break;
    case STATE_CONNECTING:
      {		
        fd_set f[3];
        FD_ZERO(&f[0]);
        FD_ZERO(&f[1]);
        FD_ZERO(&f[2]);
        FD_SET(m_socket,&f[0]);
        FD_SET(m_socket,&f[1]);
        FD_SET(m_socket,&f[2]);
        struct timeval tv;
        memset(&tv,0,sizeof(tv));
        if (select(m_socket+1,&f[0],&f[1],&f[2],&tv)==-1)
        {
          m_errorstr="connecting to host (calling select())";
          m_state=STATE_ERROR;
        }
        else if (FD_ISSET(m_socket,&f[1])) 
        {
          m_state=STATE_CONNECTED;
        }
        else if (FD_ISSET(m_socket,&f[2]))
        {
          m_errorstr="connecting to host";
          m_state=STATE_ERROR;
        }
      }
    break;
    case STATE_CONNECTED:
    case STATE_CLOSING:
      if (m_send_len>0 && bytes_allowed_to_send>0)
      {
        int len=m_send_buffer_len-m_send_pos;
        if (len > m_send_len) len=m_send_len;
        if (len > bytes_allowed_to_send) len=bytes_allowed_to_send;
        if (len > 0)
        {
          int res=::send(m_socket,m_send_buffer+m_send_pos,len,0);
          if (res==-1 && ERRNO != EWOULDBLOCK)
          {            
//            m_state=STATE_CLOSED;
//            return;
          }
          if (res>0)
          {
            bytes_allowed_to_send-=res;
            if (bytes_sent) *bytes_sent+=res;
            m_send_pos+=res;
            m_send_len-=res;
          }
        }
        if (m_send_pos>=m_send_buffer_len) 
        {
          m_send_pos=0;
          if (m_send_len>0)
          {
            len=m_send_buffer_len-m_send_pos;
            if (len > m_send_len) len=m_send_len;
            if (len > bytes_allowed_to_send) len=bytes_allowed_to_send;
            int res=::send(m_socket,m_send_buffer+m_send_pos,len,0);
            if (res==-1 && ERRNO != EWOULDBLOCK)
            {
//              m_state=STATE_CLOSED;
            }
            if (res>0)
            {
              bytes_allowed_to_send-=res;
              if (bytes_sent) *bytes_sent+=res;
              m_send_pos+=res;
              m_send_len-=res;
            }
          }
        }
      }
      if (m_recv_len<m_recv_buffer_len)
      {
        int len=m_recv_buffer_len-m_recv_pos;
        if (len > m_recv_buffer_len-m_recv_len) len=m_recv_buffer_len-m_recv_len;
        if (len > bytes_allowed_to_recv) len=bytes_allowed_to_recv;
        if (len>0)
        {
          int res=::recv(m_socket,m_recv_buffer+m_recv_pos,len,0);
          if (res == 0 || (res < 0 && ERRNO != EWOULDBLOCK))
          {        
            m_state=STATE_CLOSED;
            break;
          }
          if (res > 0)
          {
            bytes_allowed_to_recv-=res;
            if (bytes_rcvd) *bytes_rcvd+=res;
            m_recv_pos+=res;
            m_recv_len+=res;
          }
        }
        if (m_recv_pos >= m_recv_buffer_len)
        {
          m_recv_pos=0;
          if (m_recv_len < m_recv_buffer_len)
          {
            len=m_recv_buffer_len-m_recv_len;
            if (len > bytes_allowed_to_recv) len=bytes_allowed_to_recv;
            if (len > 0)
            {
              int res=::recv(m_socket,m_recv_buffer+m_recv_pos,len,0);
              if (res == 0 || (res < 0 && ERRNO != EWOULDBLOCK))
              {        
                m_state=STATE_CLOSED;
                break;
              }
              if (res > 0)
              {
                bytes_allowed_to_recv-=res;
                if (bytes_rcvd) *bytes_rcvd+=res;
                m_recv_pos+=res;
                m_recv_len+=res;
              }
            }
          }
        }
      }
      if (m_state == STATE_CLOSING)
      {
        if (m_send_len < 1) m_state = STATE_CLOSED;
      }
    break;
    default: break;
  }
}

void JNL_Connection::close(int quick)
{
  if (quick || m_state == STATE_RESOLVING || m_state == STATE_CONNECTING)
  {
    m_state=STATE_CLOSED;
    if (m_socket >= 0)
    {
      ::shutdown(m_socket, SHUT_RDWR);
      ::closesocket(m_socket);
    }
    m_socket=-1;
    memset(m_recv_buffer,0,m_recv_buffer_len);
    memset(m_send_buffer,0,m_send_buffer_len);
    m_remote_port=0;
    m_recv_len=m_recv_pos=0;
    m_send_len=m_send_pos=0;
    m_host[0]=0;
    memset(m_saddr,0,sizeof(m_saddr));
  }
  else
  {
    if (m_state == STATE_CONNECTED) m_state=STATE_CLOSING;
  }
}

int JNL_Connection::send_bytes_in_queue(void)
{
  return m_send_len;
}

int JNL_Connection::send_bytes_available(void)
{
  return m_send_buffer_len-m_send_len;
}

int JNL_Connection::send(const void *_data, int length)
{
  const char *data = static_cast<const char *>(_data);
  if (length > send_bytes_available())
  {
    return -1;
  }
  
  int write_pos=m_send_pos+m_send_len;
  if (write_pos >= m_send_buffer_len) 
  {
    write_pos-=m_send_buffer_len;
  }

  int len=m_send_buffer_len-write_pos;
  if (len > length) 
  {
    len=length;
  }

  memcpy(m_send_buffer+write_pos,data,len);
  if (length > len)
  {
    memcpy(m_send_buffer,data+len,length-len);
  }
  m_send_len+=length;
  return 0;
}

int JNL_Connection::send_string(const char *line)
{
  return send(line,strlen(line));
}

int JNL_Connection::recv_bytes_available(void)
{
  return m_recv_len;
}

int JNL_Connection::peek_bytes(void *_data, int maxlength)
{
  char *data = static_cast<char *>(_data);
  if (maxlength > m_recv_len)
  {
    maxlength=m_recv_len;
  }
  int read_pos=m_recv_pos-m_recv_len;
  if (read_pos < 0) 
  {
    read_pos += m_recv_buffer_len;
  }
  int len=m_recv_buffer_len-read_pos;
  if (len > maxlength)
  {
    len=maxlength;
  }
  if (data != NULL) {
    memcpy(data,m_recv_buffer+read_pos,len);
    if (len < maxlength)
    {
      memcpy(data+len,m_recv_buffer,maxlength-len);
    }
  }

  return maxlength;
}

int JNL_Connection::recv_bytes(void *_data, int maxlength)
{
  char *data = static_cast<char *>(_data);
  
  int ml=peek_bytes(data,maxlength);
  m_recv_len-=ml;
  return ml;
}

int JNL_Connection::getbfromrecv(int pos, int remove)
{
  int read_pos=m_recv_pos-m_recv_len + pos;
  if (pos < 0 || pos > m_recv_len) return -1;
  if (read_pos < 0) 
  {
    read_pos += m_recv_buffer_len;
  }
  if (read_pos >= m_recv_buffer_len)
  {
    read_pos-=m_recv_buffer_len;
  }
  if (remove) m_recv_len--;
  return m_recv_buffer[read_pos];
}

int JNL_Connection::recv_lines_available(void)
{
  int l=recv_bytes_available();
  int lcount=0;
  int lastch=0;
  int pos;
  for (pos=0; pos < l; pos ++)
  {
    int t=getbfromrecv(pos,0);
    if (t == -1) return lcount;
    if ((t=='\r' || t=='\n') &&(
         (lastch != '\r' && lastch != '\n') || lastch==t
        )) lcount++;
    lastch=t;
  }
  return lcount;
}

int JNL_Connection::recv_line(char *line, int maxlength)
{
  if (maxlength > m_recv_len) maxlength=m_recv_len;
  while (maxlength--)
  {
    int t=getbfromrecv(0,1);
    if (t == -1) 
    {
      *line=0;
      return 0;
    }
    if (t == '\r' || t == '\n')
    {
      int r=getbfromrecv(0,0);
      if ((r == '\r' || r == '\n') && r != t) getbfromrecv(0,1);
      *line=0;
      return 0;
    }
    *line++=(char)t;
  }
  return 1;
}

unsigned long JNL_Connection::get_interface(void)
{
  if (m_socket==-1) return 0;
  struct sockaddr_in sin;
  memset(&sin,0,sizeof(sin));
  socklen_t len=16;
  if (::getsockname(m_socket,(struct sockaddr *)&sin,&len)) return 0;
  return (unsigned long) sin.sin_addr.s_addr;
}

unsigned long JNL_Connection::get_remote()
{
  return m_saddr->sin_addr.s_addr;
}

short JNL_Connection::get_remote_port()
{
  return m_remote_port;
}
