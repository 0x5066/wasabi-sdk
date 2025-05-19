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

#ifndef _SVC_IMGLOAD_H
#define _SVC_IMGLOAD_H

#include "../../bfc/platform/platform.h"
#include "../../bfc/dispatch.h"
#include "services.h"

class XmlReaderParams;

class NOVTABLE svc_imageLoader : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::IMAGELOADER; }

  // returns how many bytes needed to get image info
  int getHeaderSize();

  // test image data, return TRUE if you can load it
  int testData(const void *data, int datalen);

  // just gets the width and height from the data, if possible
  int getDimensions(const void *data, int datalen, int *w, int *h);

  // converts the data into pixels, use api->sysFree to deallocate
  ARGB32 *loadImage(const void *data, int datalen, int *w, int *h, XmlReaderParams *params=NULL);

  enum {
    GETHEADERSIZE=100,
    TESTDATA=200,
    GETDIMENSIONS=300,
    LOADIMAGE=400,
  };
};

inline int svc_imageLoader::getHeaderSize() {
  return _call(GETHEADERSIZE, -1);
}

inline int svc_imageLoader::testData(const void *data, int datalen) {
  return _call(TESTDATA, 0, data, datalen);
}

inline int svc_imageLoader::getDimensions(const void *data, int datalen, int *w, int *h) {
  return _call(GETDIMENSIONS, 0, data, datalen, w, h);
}

inline ARGB32 *svc_imageLoader::loadImage(const void *data, int datalen, int *w, int *h, XmlReaderParams *params) {
  return _call(LOADIMAGE, (ARGB32*)0, data, datalen, w, h, params);
}

// derive from this one
class NOVTABLE svc_imageLoaderI : public svc_imageLoader {
public:
  // return the header size needed to get w/h and determine if it can be loaded
  virtual int getHeaderSize() { return -1; }//don't know
  // test image data, return TRUE if you can load it
  virtual int testData(const void *data, int datalen)=0;
  // just gets the width and height from the data, if possible
  virtual int getDimensions(const void *data, int datalen, int *w, int *h) { return 0; }
  // convert the data into pixels, use api->sysMalloc to allocate the space
  virtual ARGB32 *loadImage(const void *data, int datalen, int *w, int *h, XmlReaderParams *params=NULL)=0;

protected:
  RECVS_DISPATCH;
};

#include "../../bfc/svc_enum.h"

class ImgLoaderEnum : public SvcEnumT<svc_imageLoader> {
public:
  ImgLoaderEnum(unsigned char *data, int datalen) : mem(datalen, data) { }

protected:
  virtual int testService(svc_imageLoader *svc) {
    return svc->testData(mem, mem.getSizeInBytes());
  }

private:
  MemBlock<unsigned char> mem;
};

#endif
