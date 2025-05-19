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

#ifndef _METRICSCB_H
#define _METRICSCB_H

#include "../bfc/dispatch.h"

// metrics codes
namespace Metric {
  enum {
    TEXTDELTA=10,
  };
};

class MetricsCallback : public Dispatchable {
public:
  int setMetric(int metricid, int param1=0, int param2=0);

  // class Dispatchable codes
  enum {
    SETMETRIC=100,
  };
};

inline int MetricsCallback::setMetric(int metricid, int param1, int param2) {
  return _call(SETMETRIC, 0, metricid, param1, param2);
}

class MetricsCallbackI : public MetricsCallback {
public:
  virtual int metricscb_setTextDelta(int delta) { return 0; }

protected:
  int mcb_setMetric(int metricid, int param1, int param2);

  RECVS_DISPATCH;
};

#endif
