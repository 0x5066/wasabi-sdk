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

#ifndef _SVC_SKINFILTER_H
#define _SVC_SKINFILTER_H

#include "../../bfc/dispatch.h"
#include "services.h"

class NOVTABLE svc_skinFilter : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::SKINFILTER; }
  int filterBitmap(unsigned char *bits, int w, int h, int bpp, const char *element_id, const char *forcegroup=NULL);
  COLORREF filterColor(COLORREF color, const char *element_id, const char *forcegroup=NULL);
                                                                                      
  enum {
    FILTERBITMAP=100,
    FILTERCOLOR=200,
  };
};

inline int svc_skinFilter::filterBitmap(unsigned char *bits, int w, int h, int bpp, const char *element_id, const char *forcegroup) {
  return _call(FILTERBITMAP, 0, bits, w, h, bpp, element_id, forcegroup);
}

inline COLORREF svc_skinFilter::filterColor(COLORREF color, const char *element_id, const char *forcegroup) {
  return _call(FILTERCOLOR, (COLORREF)0, color, element_id, forcegroup);
}

// derive from this one
class NOVTABLE svc_skinFilterI : public svc_skinFilter {
public:
  virtual int filterBitmap(unsigned char *bits, int w, int h, int bpp, const char *element_id, const char *forcegroup=NULL)=0;
  virtual COLORREF filterColor(COLORREF color, const char *element_id, const char *forcegroup=NULL)=0;

protected:
  RECVS_DISPATCH;
};

#include "../../bfc/svc_enum.h"

class SkinFilterEnum : public SvcEnumT<svc_skinFilter> {
public:
  SkinFilterEnum() {}
protected:
  virtual int testService(svc_skinFilter* svc) {
    return TRUE;
  }
};

#endif
