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

#ifndef _LOADER_H
#define _LOADER_H

#include "../studio/services/svc_imgload.h"

class XmlReaderParams;

class PngLoad : public svc_imageLoaderI {
public:
  // service
  static const char *getServiceName() { return "PNG loader"; }

  virtual int getHeaderSize();
  virtual int testData(const void *data, int datalen);
  virtual int getDimensions(const void *data, int datalen, int *w, int *h);
  virtual ARGB32 *loadImage(const void *data, int datalen, int *w, int *h, XmlReaderParams *params=NULL);

private:
  void premultiply(ARGB32 *m_pBits, int nwords);
  ARGB32 *read_png(const void *data, int datalen, int *w, int *h, int dimensions_only);
};
#endif
