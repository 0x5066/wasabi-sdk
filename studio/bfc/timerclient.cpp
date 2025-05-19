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

#include "timerclient.h"
#include "../studio/api.h"

#define CBCLASS TimerClientI
START_DISPATCH;
  VCB(TIMERCLIENT_TIMERCALLBACK,   timerclient_timerCallback);
  CB(TIMERCLIENT_GETMASTERCLIENT,  timerclient_getMasterClient);
  VCB(TIMERCLIENT_ONMASTERMUX,     timerclient_onMasterClientMultiplex);
  CB(TIMERCLIENT_GETDEPPTR,        timerclient_getDependencyPtr);
  CB(TIMERCLIENT_GETSKIPPED,       timerclient_getSkipped);
  VCB(TIMERCLIENT_SETSKIPPED,      timerclient_setSkipped);
  VCB(TIMERCLIENT_POSTDEFERREDCB , timerclient_postDeferredCallback);
  CB(TIMERCLIENT_ONDEFERREDCB ,    timerclient_onDeferredCallback);
  CB(TIMERCLIENT_GETNAME,          timerclient_getName);
END_DISPATCH;

int TimerClientI::timerclient_setTimer(int id, int ms) {
  ASSERTPR(id > 0, "A timer id cannot be <= 0");
  ASSERTPR(id != DEFERREDCB_TIMERID, "please chose another timer id");
  api->timer_add(this, id, ms);
  return 1;
}

int TimerClientI::timerclient_killTimer(int id) {
  api->timer_remove(this, id);
  return 1;
}

void TimerClientI::timerclient_postDeferredCallback(int param1, int param2, int mindelay) {
  defered_callback *c = new defered_callback;
  c->origin = this;
  c->param1 = param1;
  c->param2 = param2;
  cbs.addItem(c);
  api->timer_add(this, DEFERREDCB_TIMERID, MAX(1, mindelay));
}

void TimerClientI::timerclient_timerCallback(int id) {
  if (id == DEFERREDCB_TIMERID) {
    api->timer_remove(this, DEFERREDCB_TIMERID);
    PtrList<defered_callback> temp;
    foreach(cbs)
      temp.addItem(cbs.getfor());
    endfor;
    cbs.removeAll();
    while (temp.getNumItems()>0) {
      defered_callback *c = temp.enumItem(0);
      temp.removeByPos(0);
      c->origin->timerclient_onDeferredCallback(c->param1, c->param2);
      delete c;
    }
  }
}