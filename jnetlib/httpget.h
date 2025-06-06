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
** File: httpget.h - JNL interface for doing HTTP GETs.
** License: see jnetlib.h
**
** Usage:
**   1. Create a JNL_HTTPGet object, optionally specifying a JNL_AsyncDNS
**      object to use (or NULL for none, or JNL_CONNECTION_AUTODNS for auto),
**      and the receive buffer size, and a string specifying proxy (or NULL 
**      for none). See note on proxy string below.
**   2. call addheader() to add whatever headers you want. It is recommended to
**      add at least the following two:
**        addheader("User-Agent:MyApp (Mozilla)");
*///      addheader("Accept:*/*");
/*         ( the comment weirdness is there so I Can do the star-slash :)
**   3. Call connect() with the URL you wish to GET (see URL string note below)
**   4. Call run() once in a while, checking to see if it returns -1 
**      (if it does return -1, call geterrorstr() to see what the error is).
**      (if it returns 1, no big deal, the connection has closed).
**   5. While you're at it, you can call bytes_available() to see if any data
**      from the http stream is available, or getheader() to see if any headers
**      are available, or getreply() to see the HTTP reply, or getallheaders() 
**      to get a double null terminated, null delimited list of headers returned.
**   6. If you want to read from the stream, call get_bytes (which returns how much
**      was actually read).
**   7. content_length() is a helper function that uses getheader() to check the
**      content-length header.
**   8. Delete ye' ol' object when done.
**
** Proxy String:
**   should be in the format of host:port, or user@host:port, or 
**   user:password@host:port. if port is not specified, 80 is assumed.
** URL String:
**   should be in the format of http://user:pass@host:port/requestwhatever
**   note that user, pass, port, and /requestwhatever are all optional :)
**   note that also, http:// is really not important. if you do poo://
**   or even leave out the http:// altogether, it will still work.
*/

#ifndef _HTTPGET_H_
#define _HTTPGET_H_

#include "connection.h"

class JNL_HTTPGet
{
  public:
    JNL_HTTPGet(JNL_AsyncDNS *dns=JNL_CONNECTION_AUTODNS, int recvbufsize=16384, char *proxy=NULL);
    ~JNL_HTTPGet();

    void addheader(const char *header);

    void connect(const char *url, int ver=0);

    int run(); // returns: 0 if all is OK. -1 if error (call geterrorstr()). 1 if connection closed.

    int   get_status(); // returns 0 if connecting, 1 if reading headers, 
                        // 2 if reading content, -1 if error.

    char *getallheaders(); // double null terminated, null delimited list
    char *getheader(char *headername);
    char *getreply() { return m_reply; }
    int   getreplycode(); // returns 0 if none yet, otherwise returns http reply code.

    char *geterrorstr() { return m_errstr;}

    int bytes_available();
    int get_bytes(char *buf, int len);
    int peek_bytes(char *buf, int len);

    int content_length() { char *p=getheader("content-length"); if (p) return atoi(p); return 0; }

    JNL_Connection *get_con() { return m_con; }

  protected:
    void reinit();
    void deinit();
    void seterrstr(char *str) { if (m_errstr) free(m_errstr); m_errstr=(char*)malloc(strlen(str)+1); strcpy(m_errstr,str); }

    void do_parse_url(char *url, char **host, int *port, char **req, char **lp);
    void do_encode_mimestr(char *in, char *out);

    JNL_AsyncDNS *m_dns;
    JNL_Connection *m_con;
    int m_recvbufsize;

    int m_http_state;

    int m_http_port;
    char *m_http_url;
    char *m_http_host;
    char *m_http_lpinfo;
    char *m_http_request;

    char *m_http_proxylpinfo;
    char *m_http_proxyhost;
    int   m_http_proxyport;

    char *m_sendheaders;
    char *m_recvheaders;
    int m_recvheaders_size;
    char *m_reply;

    char *m_errstr;
};

#endif // _HTTPGET_H_
