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
** File: udpconnection.h - JNL UDP connection interface
** License: see jnetlib.h
**
** Usage:
**   1. Create a JNL_Connection object, optionally specifying a JNL_AsyncDNS
**      object to use (or NULL for none, or JNL_CONNECTION_AUTODNS for auto),
**      and the send and receive buffer sizes.
**   2. Call connect() to have it connect to a host/port (the hostname will be 
**      resolved if possible).
**   3. call run() with the maximum send/recv amounts, and optionally parameters
**      so you can tell how much has been send/received. You want to do this a lot, while:
**   4. check get_state() to check the state of the connection. The states are:
**        JNL_Connection::STATE_ERROR
**          - an error has occured on the connection. the connection has closed,
**            and you can no longer write to the socket (there still might be 
**            data in the receive buffer - use recv_bytes_available()). 
**        JNL_Connection::STATE_NOCONNECTION
**          - no connection has been made yet. call connect() already! :)
**        JNL_Connection::STATE_RESOLVING
**          - the connection is still waiting for a JNL_AsycnDNS to resolve the
**            host. 
**        JNL_Connection::STATE_CONNECTING
**          - the asynchronous call to connect() is still running.
**        JNL_Connection::STATE_CONNECTED
**          - the connection has connected, all is well.
**        JNL_Connection::STATE_CLOSING
**          - the connection is closing. This happens after a call to close,
**            without the quick parameter set. This means that the connection
**            will close once the data in the send buffer is sent (data could
**            still be being received when it would be closed). After it is 
**            closed, the state will transition to:
**        JNL_Connection::STATE_CLOSED
**          - the connection has closed, generally without error. There still
**            might be data in the receieve buffer, use recv_bytes_available().
**   5. Use send() and send_string() to send data. You can use 
**      send_bytes_in_queue() to see how much has yet to go out, or 
**      send_bytes_available() to see how much you can write. If you use send()
**      or send_string() and not enough room is available, both functions will 
**      return error ( < 0)
**   6. Use recv() and recv_line() to get data. If you want to see how much data 
**      there is, use recv_bytes_available() and recv_lines_available(). If you 
**      call recv() and not enough data is available, recv() will return how much
**      data was actually read. See comments at the function defs.
**
**   7. To close, call close(1) for a quick close, or close() for a close that will
**      make the socket close after sending all the data sent. 
**  
**   8. delete ye' ol' object.
*/

#ifndef _UDPCONNECTION_H_
#define _UDPCONNECTION_H_

#include "asyncdns.h"

#define JNL_CONNECTION_AUTODNS ((JNL_AsyncDNS*)-1)

class JNL_UDPConnection
{
  public:
    typedef enum 
    { 
      STATE_ERROR, 
      STATE_NOCONNECTION,
      STATE_RESOLVING, 
      STATE_CONNECTED, 
      STATE_CLOSING, 
      STATE_CLOSED 
    } state;

    JNL_UDPConnection(short incoming_port=0, JNL_AsyncDNS *dns=JNL_CONNECTION_AUTODNS, int sendbufsize=8192, int recvbufsize=8192);
    ~JNL_UDPConnection();

    void setpeer(char *hostname, int port);
    void setpeer(struct sockaddr *addr);

    void run(int max_send_bytes=-1, int max_recv_bytes=-1, int *bytes_sent=NULL, int *bytes_rcvd=NULL);
    int  get_state() { return m_state; }
    char *get_errstr() { return m_errorstr; }

    void close(int quick=0);
    void flush_send(void) { m_send_len=m_send_pos=0; }

    int send_bytes_in_queue(void);
    int send_bytes_available(void);
    int send(char *data, int length); // returns -1 if not enough room
    int send_string(char *line);      // returns -1 if not enough room


    int recv_bytes_available(void);
    int recv_bytes(char *data, int maxlength); // returns actual bytes read
    unsigned int recv_int(void);
    int recv_lines_available(void);
    int recv_line(char *line, int maxlength); // returns 0 if the line was terminated with a \r or \n, 1 if not.
                                              // (i.e. if you specify maxlength=10, and the line is 12 bytes long
                                              // it will return 1. or if there is no \r or \n and that's all the data
                                              // the connection has.)
    int peek_bytes(char *data, int maxlength); // returns bytes peeked

    unsigned long get_interface(void);        // this returns the interface the connection is on
    unsigned long get_remote(void) { return m_saddr.sin_addr.s_addr; } // remote host ip.
    short get_remote_port(void) { return m_remote_port; } // this returns the remote port of connection

    void get_last_recv_msg_addr(struct sockaddr *addr) { memcpy(addr, (const void *)&m_laddr, sizeof(struct sockaddr_in)); }
  
  protected:
    int  m_socket;
    short m_remote_port;
    char *m_recv_buffer;
    char *m_send_buffer;
    int m_recv_buffer_len;
    int m_send_buffer_len;

    int  m_recv_pos;
    int  m_recv_len;
    int  m_send_pos;
    int  m_send_len;

    struct sockaddr_in m_saddr;
    struct sockaddr_in m_iaddr;
    struct sockaddr_in m_laddr;
    char m_host[256];

    JNL_AsyncDNS *m_dns;
    int m_dns_owned;

    state m_state;
    char *m_errorstr;

    int getbfromrecv(int pos, int remove); // used by recv_line*

};

#endif // _UDPConnection_H_
