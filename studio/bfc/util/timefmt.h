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

//PORTABLE
#ifndef _TIMEFMT_H
#define _TIMEFMT_H

/**
  Simple time formatting. Can format into a minutes:seconds style
  display based on count in seconds only. 
  
  Can also format a timestamp into human readable format.
  
  @author Nullsoft
  @ver 1.0
*/
class TimeFmt {
public:
  /**
    Formats a time value in seconds to minute:seconds.
    
    If the buffer is too small, the string will be
    truncated.
    
    @param seconds  Time value to convert.
    @param buf      Buffer to receive formatted string.
    @param buflen   Length of the buffer.
  */
  static void printMinSec(int seconds, char *buf, int buflen);
  
  /**
    Formats a time value (from unix timestamp) to
    human readable format.

    If the buffer is too small, the string will be
    truncated.
    
    Example of formatted output:
      Tue Sep 10 18:34:42 PDT 2002
      
    @param buf      Buffer to receive the formatted string.
    @param bufsize  Length of the buffer.
    @param ts       The timestamp to use.
  */
  static void printTimeStamp(char *buf, int bufsize, int ts);
};

#endif
