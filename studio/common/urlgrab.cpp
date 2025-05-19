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


#include "../../jnetlib/jnetlib.h"
#include "../../jnetlib/httpget.h"

#include "urlgrab.h"

#include "../bfc/std.h"

#include "../studio/api.h"

UrlGrab::UrlGrab(const char *_url, const char *filename, const char *useragent) {
  url = _url;
  dns = new JNL_AsyncDNS();
  httpGetter = new JNL_HTTPGet(dns);
  connected = 0;
  fp = NULL;
  buffer.setSize(4096);

  if (useragent != NULL)
    httpGetter->addheader(StringPrintf("User-Agent:%s", useragent));
  else 
    httpGetter->addheader(StringPrintf("User-Agent:%s build %i (Mozilla)", api->main_getVersionString(), api->main_getBuildNumber()));
  httpGetter->addheader("Accept:*/*");

  if(filename != NULL) {
    tmpfilename = filename;
    fp = fopen(tmpfilename, "wb");
    ASSERT(fp != NULL);
  }
  JNL::open_socketlib();
  ready = 0;
  in_pos = 0;
  size = 0;
}

UrlGrab::~UrlGrab() {
  if (ready != 1 && !tmpfilename.isempty()) {
    unlink(tmpfilename.getValue());
  }
  shutdown();
//  JNL::close_socketlib();
}

int UrlGrab::run() {
  ASSERT(httpGetter != NULL);
  if (ready != 0) return ready;
 // if (!httpGetter) return 1; // GL> Uhh? Useless. Oops :P
  if (!connected) {
    httpGetter->connect(url, 0);
    connected = 1;
    return 0;
  }
  //ASSERT(fp != NULL); GL> //CUT
  int r = httpGetter->run();
  if (r == -1) {
    shutdown();
    ready = -1;
    return -1;
  }
  
  int bytes_read = 0;
  while (httpGetter->bytes_available() > 0) {
    char tmpbuf[4096];
    int bytes = 0;

    // read up to 4k into a temporary buffer.
    bytes = httpGetter->get_bytes(tmpbuf, 4096);

    // now our downloaded file is this big
    size += bytes;

    // if our holding buffer isn't large enough to hold the new data, 
    // make it 4x bigger than it needs to be (reallocs get expensive).
    if (buffer.getSize() <= in_pos + bytes) {
       buffer.setSize(in_pos + (bytes * 4));
    }

    // push the bytes to the end of the holding block.
    buffer.setMemory(tmpbuf, bytes, in_pos);

    // advance the buffer position for input.
    in_pos += bytes;

    if (in_pos > 65536 && fp != NULL) {
      // write the current buffer
    	fwrite(buffer, in_pos, 1, fp);
      // and start over.
    	buffer.zeroMemory();
      in_pos = 0;
    }    
  }
  
  if (r == 1) {
    // buffer write first.
    if(fp != NULL) {
      // catch any straggling bytes here.
      fwrite(buffer, in_pos, 1, fp);
    }
    // file close next.
    shutdown();
    ready = 1;
  }
  return ready;
}

int UrlGrab::shutdown() {
  delete httpGetter; httpGetter = NULL;
  delete dns; dns = NULL;
  if (fp != NULL) fclose(fp); fp = NULL;
  connected = 0;
  return 1;
}

const char *UrlGrab::getErrorStr() {
  if (!httpGetter) return "";
  return httpGetter->geterrorstr();
}

void *UrlGrab::getBuffer() {
	return buffer.getMemory();
}