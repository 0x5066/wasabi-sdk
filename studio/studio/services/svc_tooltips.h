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

#ifndef _SVC_TOOLTIPS_H
#define _SVC_TOOLTIPS_H

#include "../../bfc/dispatch.h"
#include "services.h"

class NOVTABLE svc_toolTipsRenderer : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::TOOLTIPSRENDERER; }

  int spawnTooltip(const char *text);

  enum {
    SPAWNTOOLTIP        =10,
  };
};

inline int svc_toolTipsRenderer::spawnTooltip(const char *text) {
  return _call(SPAWNTOOLTIP, 0, text);
}

class NOVTABLE svc_toolTipsRendererI : public svc_toolTipsRenderer {
public:
  virtual int spawnTooltip(const char *text)=0;

protected:
  RECVS_DISPATCH;
};

#endif
