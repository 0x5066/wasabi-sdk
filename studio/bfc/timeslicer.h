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

#ifndef __TIMESLICER_H
#define __TIMESLICER_H

// TimeSlicer allows you to create a background job to perform while not blocking
// the main GUI thread. You give it the max percentage of CPU it should use, call star()
// and it'll start calling onSlice as many times as it can without using more cpu than requested
//
// To use this class, you need to break down your job into multiple tiny chunks that
// you perform in onSlice. Typical uses include adding files to or filtering entries from
// the database, driving state machines, etc.
// 
// onSlice will be called multiple times per timer.

#include "timerclient.h"

enum {
  GRANULARITY_EXTRALOW  =   20,
  GRANULARITY_LOW       =   50,
  GRANULARITY_MEDIUM    =  100,
  GRANULARITY_HIGH      =  250,
  GRANULARITY_EXTRAHIGH = 1000,
};

class COMEXP TimeSlicer : public TimerClientI {
  public:
    
    TimeSlicer(int percent_cpu_usage=25, int slice_duration=GRANULARITY_LOW); 
    virtual ~TimeSlicer();

    virtual void timerclient_timerCallback(int id);

    void startSlicer();
    void stopSlicer();
    int isSlicerStarted();

    virtual void onSlicerStart();
    virtual void onSlicerStop();
    virtual void onBeginSliceBatch() {}
    virtual void onEndSliceBatch() {}
    Dependent *timerclient_getDependencyPtr() { return timeslicer_getDependencyPtr(); }
    virtual Dependent *timeslicer_getDependencyPtr()=0;


    // override this to do your work
    virtual void onSlice() {  }

  private:

    virtual void runSlice(DWORD start, DWORD stopwhen);
    float max_cpu_usage;
    int duration;
    int started;
};

class COMEXP TimeSlicerD : public TimeSlicer, public DependentI {
  public:
    virtual Dependent *timeslicer_getDependencyPtr() { return this; }    
};

#endif