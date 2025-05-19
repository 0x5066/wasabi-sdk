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

#ifndef _PROFILER_H
#define _PROFILER_H

#include "../bfc/std.h"
#include "../bfc/string.h"
#include "../bfc/ptrlist.h"


#ifdef NO_PROFILING
#define PR_ENTER(msg) 
#define PR_LEAVE()
#else

#define PR_ENTER(msg) { __Profiler __prx(msg) 
#define PR_LEAVE() }

class __ProfilerEntry {
  public: 
    __ProfilerEntry(const char *txt) { text = txt; totaltime = 0; totaln = 0; }
    virtual ~__ProfilerEntry() {}

    void add(float ms) { totaltime += ms; totaln++; }
    float getAverage() { if (totaln == 0) return 0; return totaltime / (float)totaln; }
    float getTotal() { return totaltime; }
    const char *getText() { return text; }

  private:
    float totaltime;
    int totaln;
    String text;
};

class __ProfilerEntrySort {
public:
  static int compareAttrib(const char *attrib, void *item) {
    return STRICMP(attrib, ((__ProfilerEntry*)item)->getText());
  }
  static int compareItem(void *i1, void *i2) {
    return STRICMP(((__ProfilerEntry*)i1)->getText(), ((__ProfilerEntry*)i2)->getText());
  }
};

extern COMEXP PtrListInsertSorted<__ProfilerEntry, __ProfilerEntrySort> __profiler_entries;

class __ProfilerManager {
  public:
  static void log(const char *txt, float ms, float *total, float *average) {
    int pos=-1;
    __ProfilerEntry *e = __profiler_entries.findItem(txt, &pos);
    if (pos < 0 || e == NULL) {
      e = new __ProfilerEntry(txt);
      __profiler_entries.addItem(e);
    }
    if (e != NULL) {
      e->add(ms);
      if (total != NULL) *total = e->getTotal();
      if (average != NULL) *average = e->getAverage();
    }
  }
};

#undef USE_TICK_COUNT

class __Profiler {
public:
  __Profiler(const char *text) {
    str = text;
#ifdef USE_TICK_COUNT
    ts1 = GetTickCount();
#else
    ts1 = Std::getTimeStampMS();
#endif
  }
  ~__Profiler() {
#ifdef USE_TICK_COUNT
    stdtimevalms ts2 = GetTickCount();
#else
    stdtimevalms ts2 = Std::getTimeStampMS();
#endif
    float ms = (float)((ts2 - ts1)
#ifndef USE_TICK_COUNT
*1000.0
#endif
);
    float total=0;
    float average=0;
    __ProfilerManager::log(str, ms, &total, &average);
    char buf[4096];
    sprintf(buf, "Profiler: %s: %0.0f ms (total: %0.0f ms, average: %0.0f ms)\n", str.getValue(), ms, total, average);
    OutputDebugString(buf);
  }
private:
  String str;
  stdtimevalms ts1;
};

#endif//!NO_PROFILING

#endif
