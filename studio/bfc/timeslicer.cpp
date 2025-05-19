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

#include "timeslicer.h"

#define TIMER_SLICE 0x7816

TimeSlicer::TimeSlicer(int percent_cpu_usage/* =50 */, int slice_duration/* =-1 */) {
  max_cpu_usage = MIN((float)percent_cpu_usage, 99.0f) / 100.0f;
  duration = slice_duration;
  started = 0;
}

TimeSlicer::~TimeSlicer() {
}

void TimeSlicer::startSlicer() {
  if (started) return;
  started = 1;
  timerclient_setTimer(TIMER_SLICE, duration);
  onSlicerStart();
}

void TimeSlicer::stopSlicer() {
  if (!started) return;
  started = 0;
  timerclient_killTimer(TIMER_SLICE);
  onSlicerStop();
}

void TimeSlicer::onSlicerStop() {
}

void TimeSlicer::onSlicerStart() {
}

int TimeSlicer::isSlicerStarted() {
  return started;
}

void TimeSlicer::timerclient_timerCallback(int id) {
  if (id == TIMER_SLICE) {
    DWORD now = Std::getTickCount();
    runSlice(now, now + (int)((float)duration * max_cpu_usage));
  }
}

void TimeSlicer::runSlice(DWORD start, DWORD stopwhen) {
  onBeginSliceBatch();
  while (Std::getTickCount() < stopwhen) {
    onSlice();
    DWORD after = Std::getTickCount();
  }
  onEndSliceBatch();
}