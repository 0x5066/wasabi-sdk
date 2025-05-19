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

#ifndef __TIMER_CLIENT_H
#define __TIMER_CLIENT_H

#include "dispatch.h"
#include "../common/common.h"
#include "../bfc/depend.h"

#define DEFERREDCB_TIMERID -2

class TimerClient;

typedef struct {
  TimerClient *origin;
  int param1;
  int param2;
} defered_callback;

class COMEXP TimerClient : public Dispatchable {

  protected:

    TimerClient() { }	

  public:

    int timerclient_setTimer(int id, int ms);
    int timerclient_killTimer(int id);
    void timerclient_postDeferredCallback(int param1, int param2=0, int mindelay=0);
    int timerclient_onDeferredCallback(int param1, int param2);
    void timerclient_timerCallback(int id);

    TimerClient *timerclient_getMasterClient(); 
    void timerclient_onMasterClientMultiplex();
    Dependent *timerclient_getDependencyPtr(); 
    void timerclient_setSkipped(int s);
    int timerclient_getSkipped();
    void timerclient_setTimerDelay(int td);
    int timerclient_getTimerDelay();
    const char *timerclient_getName();

    enum {
      TIMERCLIENT_TIMERCALLBACK   = 100,
      TIMERCLIENT_SETTIMER        = 110,
      TIMERCLIENT_KILLTIMER       = 120,
      TIMERCLIENT_GETMASTERCLIENT = 130,
      TIMERCLIENT_ONMASTERMUX     = 140,
      TIMERCLIENT_GETDEPPTR       = 150,
      TIMERCLIENT_SETSKIPPED      = 160,
      TIMERCLIENT_GETSKIPPED      = 170,
      TIMERCLIENT_SETTIMERDELAY   = 180,
      TIMERCLIENT_GETTIMERDELAY   = 190,
      TIMERCLIENT_POSTDEFERREDCB  = 200,
      TIMERCLIENT_ONDEFERREDCB    = 210,
      TIMERCLIENT_GETNAME         = 220,
    };
};

inline void TimerClient::timerclient_timerCallback(int id) {
  _voidcall(TIMERCLIENT_TIMERCALLBACK, id);
}

inline int TimerClient::timerclient_setTimer(int id, int ms) {
  return _call(TIMERCLIENT_SETTIMER, 0, id, ms);
}

inline int TimerClient::timerclient_killTimer(int id) {
  return _call(TIMERCLIENT_KILLTIMER, 0, id);
}

inline TimerClient *TimerClient::timerclient_getMasterClient() {
  return _call(TIMERCLIENT_GETMASTERCLIENT, (TimerClient *)NULL);
}

inline void TimerClient::timerclient_onMasterClientMultiplex() {
  _voidcall(TIMERCLIENT_ONMASTERMUX);
}

inline Dependent *TimerClient::timerclient_getDependencyPtr() {
  return _call(TIMERCLIENT_GETDEPPTR, (Dependent *)NULL);
}

inline void TimerClient::timerclient_setSkipped(int s) {
  _voidcall(TIMERCLIENT_SETSKIPPED, s);
}

inline int TimerClient::timerclient_getSkipped() {
  return _call(TIMERCLIENT_GETSKIPPED, 0);
}

inline void TimerClient::timerclient_setTimerDelay(int td) {
  _voidcall(TIMERCLIENT_SETTIMERDELAY, td);
}

inline int TimerClient::timerclient_getTimerDelay() {
  return _call(TIMERCLIENT_GETTIMERDELAY, 0);
}

inline void TimerClient::timerclient_postDeferredCallback(int param1, int param2, int mindelay) {
  _voidcall(TIMERCLIENT_POSTDEFERREDCB, param1, param2, mindelay);
}

inline int TimerClient::timerclient_onDeferredCallback(int param1, int param2) {
  return _call(TIMERCLIENT_ONDEFERREDCB, 0, param1, param2);
}

inline const char *TimerClient::timerclient_getName() {
  return _call(TIMERCLIENT_GETNAME, (const char *)NULL);
}

class COMEXP TimerClientI : public TimerClient {

  public:

    TimerClientI() { skipped = 0; }
    virtual ~TimerClientI() { }

    virtual int timerclient_setTimer(int id, int ms);
    virtual int timerclient_killTimer(int id);
    virtual void timerclient_timerCallback(int id);

    virtual TimerClient *timerclient_getMasterClient() { return NULL; }
    virtual void timerclient_onMasterClientMultiplex() { };
    virtual Dependent *timerclient_getDependencyPtr()=0;
    virtual void timerclient_setSkipped(int s) { skipped = s; }
    virtual int timerclient_getSkipped() { return skipped; }
    virtual void timerclient_setTimerDelay(int td) { timerdelay = td; }
    virtual int timerclient_getTimerDelay() { return timerdelay; }
    virtual void timerclient_postDeferredCallback(int param1, int param2, int mindelay=0);
    virtual int timerclient_onDeferredCallback(int param1, int param2) { return 1; };
    virtual const char *timerclient_getName() { return NULL; }
  
  protected:
    RECVS_DISPATCH;

  private:
    int skipped;
    int timerdelay;
    PtrList<defered_callback> cbs;
};

class COMEXP TimerClientDI : public TimerClientI, public DependentI {
  public:
    Dependent *timerclient_getDependencyPtr() { return this; }
};


#endif
