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

#include "timermul.h"
#include "../studio/api.h"
#include "../attribs/attrint.h"
#include "../attribs/cfgitem.h"

// FG> not too sure how to get a callback for attribute change, if anyone wants to change it be my guest ;)
#define RESOLUTION_CHECK_DELAY 2500 // check for resolution changes every 2.5s

// if uioptions CfgItem not found, use this value for resolution
#define DEF_RES 20

// below MAX_TIMER_DELAY, timer are multiplexed using a 'wheel' algorithm (mem used = MAX_TIMER_DELAY/resolution * sizeof(PtrList) + ntimers*sizeof(MultiplexedTimer), but fast (no lookup) )
// above MAX_TIMER_DELAY, resolution drops to MAX_TIMER_DELAY/LOW_RES_DIV and uses ntimers*sizeof(MultiplexedTimer) bytes
#define MAX_TIMER_DELAY 1000  // keep this dividable by LOW_RES_DIV please
#define LOW_RES_DIV        4

TimerMultiplexer::TimerMultiplexer() {
  timerset = 0;
  nslices = 0;
  resolution = -1;
  last_resolution_check = 0;
  server = NULL;
  client = NULL;
  curslice = 0;
  running_timer = NULL;
  uioptions = NULL;
}

TimerMultiplexer::~TimerMultiplexer() {
  doShutdown();
}

void TimerMultiplexer::setClient(TimerMultiplexerClient *_client) {
  client = _client;
}

void TimerMultiplexer::setServer(RootWnd *timerWnd, int timer_id) {
  server = timerWnd;
  server_timer_id = timer_id;

  ASSERTPR(server->isInited(), "Try initing your window before calling TimerMultiplexer::setServer");

  DWORD now = Std::getTickCount();
  checkResolution(now);
}

void TimerMultiplexer::addTimer(int ms, void *data) {
  MultiplexedTimer *t = new MultiplexedTimer(ms, data);
  if (ms >= MAX_TIMER_DELAY) {
    lptimers.addItem(t);
    t->nexttick = Std::getTickCount() + t->ms;
  } else {
    timers.addItem(t);
    if (nslices > 0)
      distribute(t);
  }
}

void TimerMultiplexer::removeTimer(void *data) {

  if (running_timer && running_timer->data == data)
    running_timer = NULL;

  for (int i=0;i<timers.getNumItems();i++) {
    MultiplexedTimer *t = timers.enumItem(i);
    if (t->data == data) {
      removeFromWheel(t);
      timers.removeByPos(i);
      delete t;
      return;
    }
  } 
  for (i=0;i<lptimers.getNumItems();i++) {
    MultiplexedTimer *t = lptimers.enumItem(i);
    if (t->data == data) {
      removeFromLowPrecision(t);
      delete t;
      return;
    }
  }
}

void TimerMultiplexer::setResolution(int ms) {
  resolution = ms;
}

void TimerMultiplexer::shutdown() {
  doShutdown();
}

void TimerMultiplexer::doShutdown() {
  timers.deleteAll();
  wheel.deleteAll();
  lptimers.deleteAll();
  if (timerset) {
    KillTimer(server->gethWnd(), server_timer_id);
    timerset = 0;
  }
}

void TimerMultiplexer::checkResolution(DWORD now) {
  if (last_resolution_check < now - RESOLUTION_CHECK_DELAY) {
    last_resolution_check = now;
    if (uioptions == NULL) {
      // {9149C445-3C30-4e04-8433-5A518ED0FDDE}
      const GUID uioptions_guid = 
      { 0x9149c445, 0x3c30, 0x4e04, { 0x84, 0x33, 0x5a, 0x51, 0x8e, 0xd0, 0xfd, 0xde } };
      uioptions = api->config_getCfgItemByGuid(uioptions_guid);
    }
    int nresolution = uioptions ? _intVal(uioptions, "Multiplexed timers resolution") : DEF_RES;
    nresolution = MAX(5, MIN(MAX_TIMER_DELAY/LOW_RES_DIV, nresolution));
    if (nresolution != resolution) {
      resetTimer(nresolution);
      resolution = nresolution;
      resetWheel();
    }
  }
}

void TimerMultiplexer::resetTimer(int newresolution) {
  #ifdef WIN32

  if (timerset) 
    KillTimer(server->gethWnd(), server_timer_id);

  SetTimer(server->gethWnd(), server_timer_id, newresolution, NULL);
  timerset = 1;

  #else

  #error port me!

  #endif
}

PtrList<MultiplexedTimer> *TimerMultiplexer::getSlice(int n) {
  ASSERT(nslices > 0);
  return wheel.enumItem(n % nslices);
}

void TimerMultiplexer::resetWheel() {

  wheel.deleteAll();

  nslices = MAX_TIMER_DELAY / resolution; 
  
  for (int i=0;i<nslices;i++)
    wheel.addItem(new PtrList< MultiplexedTimer >);

  curslice = 0;
  distributeAll();
}

void TimerMultiplexer::distributeAll() {
  for (int i=0;i<timers.getNumItems();i++) {
    distribute(timers.enumItem(i));
  }
}

void TimerMultiplexer::distribute(MultiplexedTimer *t) {
  ASSERT(t != NULL);

  int delay = t->ms;

  int slice = delay / resolution + curslice;
  PtrList<MultiplexedTimer> *l = getSlice(slice);

  ASSERT(l != NULL);

  l->addItem(t);
}

void TimerMultiplexer::onServerTimer() {
  DWORD now = Std::getTickCount();

  checkResolution(now);

  runCurSlice(now);

  if ((curslice % (nslices/LOW_RES_DIV)) == 0) { // execute low precision timers every MAX_TIMER_DELAY/LOW_RES_DIV
    runLowPrecisionTimers(now);  
  }

  curslice++;
  curslice %= nslices;
}

void TimerMultiplexer::runCurSlice(DWORD now) {
  //OutputDebugString(StringPrintf("Running slice %d\n", curslice));
  PtrList<MultiplexedTimer> *slice = getSlice(curslice);
  ASSERT(slice != NULL);

  // mark them clean
  for (int i=0;i<slice->getNumItems();i++) 
    slice->enumItem(i)->flag = 0;

  // run events
  int n;
  do {
    n = 0;
    for (int i=0;i<slice->getNumItems();i++) {
      MultiplexedTimer *t = slice->enumItem(i);
      //ASSERT (timers.haveItem(t)); //CUT!!!
      if (t->flag == 1) continue;
      t->flag = 1;
      int lastdelay = MAX(0, (int)(now - t->lastmscount));
      t->lastmscount = now;
      t->lastdelay = lastdelay;
      running_timer = t;
      runTimer(t, slice, i);
// -----------------------------------------------------------------------
// WARNING
// 
// below this line, you can no longer assume that t is pointing at valid
// memory, because runTimer can eventually call removeTimer
// -----------------------------------------------------------------------
      n++;
    }
  } while (n > 0);
}

void TimerMultiplexer::runTimer(MultiplexedTimer *t, PtrList<MultiplexedTimer> *slice, int pos) {

  int nextslice = curslice + t->ms / resolution;
  int lost = resolution - t->ms;

  if (lost > 0) {
    t->lost += (float)lost / (float)t->ms;
  }

  PtrList<MultiplexedTimer> *next = getSlice(nextslice);
  ASSERT(next != NULL);

  if (slice == next) {
    nextslice++;
    next = getSlice(nextslice);
  }

  slice->removeByPos(pos);
  next->addItem(t);
  
  int skip = (int)t->lost;
  t->lost -= (int)t->lost;
  if (client) {
    client->onMultiplexedTimer(t->data, skip, t->lastdelay); 
// -----------------------------------------------------------------------
// WARNING
// 
// below this line, you can no longer assume that t is pointing at valid
// memory, because onMultiplexedTimer can eventually call removeTimer
// -----------------------------------------------------------------------
  }
}

void TimerMultiplexer::removeFromWheel(MultiplexedTimer *t) {
  for (int i=0;i<nslices;i++) {
    PtrList<MultiplexedTimer> *slice = getSlice(i);
    for (int j=0;j<slice->getNumItems();j++) {
      if (slice->enumItem(j) == t) {
        slice->removeByPos(j);
        j--;
      }
    }
  }
}

void TimerMultiplexer::removeFromLowPrecision(MultiplexedTimer *t) {
  for (int i=0;i<lptimers.getNumItems();i++) {
    if (lptimers.enumItem(i) == t) {
      lptimers.removeByPos(i);
      i--;
    }
  }
}

void TimerMultiplexer::runLowPrecisionTimers(DWORD now) {
  int restart;
  do {
    restart = 0;
    for (int i=0;i<lptimers.getNumItems();i++) {
      MultiplexedTimer *t = lptimers.enumItem(i);
      if (t->nexttick < now) {
        if (client) {
          running_timer = t;
          int res = (int)(MAX_TIMER_DELAY/LOW_RES_DIV);
          t->lost += (now - t->nexttick) / t->ms;
          int skip = (int)t->lost;
          t->lost -= skip; // remove integer part
          DWORD last = t->lastmscount;
          t->lastdelay = now-last;
          t->lastmscount = now;
          t->nexttick = t->nexttick+(t->ms)*(skip+1);
          client->onMultiplexedTimer(t->data, skip, t->lastdelay);
// -----------------------------------------------------------------------
// WARNING
// 
// below this line, you can no longer assume that t is pointing at valid
// memory, because onMultiplexedTimer can eventually call removeTimer
// -----------------------------------------------------------------------
        }
        if (running_timer == NULL) { // onMultiplexedTimer called removeTimer
          restart =1;
          break;
        }
      }
    }
  } while (restart);
}
