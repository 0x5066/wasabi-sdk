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

#include "../std.h"
#include "../string.h"
#include <time.h>
#include "timefmt.h"

void TimeFmt::printMinSec(int sec, char *buf, int buflen) {
  int minutes, seconds;
  int negative = sec < 0;

  if (buf == NULL) return;

  if (sec == -1) {
    *buf = 0;
    return;
  }

  seconds = sec % 60;
  sec /= 60;
  minutes = sec;

  StringPrintf sp("%s%d:%02d", (minutes == 0 && negative) ? "-" : "", minutes, ABS(seconds));
  STRNCPY(buf, sp, buflen);
}

void TimeFmt::printTimeStamp(char *buf, int bufsize, int ts) {
  if (ts == 0) {
    STRNCPY(buf, "Never", bufsize);	// FUCKO: load from lang pack
    return;
  }

  struct tm *tm_now;
  tm_now = localtime((const long *)&ts);
  if (tm_now == NULL) {
    *buf = 0;
    return;
  }
  strftime(buf, bufsize, "%a %b %Y %d %I:%M:%S %p", tm_now);
}
