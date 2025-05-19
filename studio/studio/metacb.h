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

#ifndef _METACB_H
#define _METACB_H

#include "syscb.h"

namespace MetaCallback {
  enum {
    NOP=0,
    ITEMADD=10,		// when DB row is created
    ITEMDEL=20,		// when DB row is deleted
    ITEMREAD=30,	// when DB row is filled in first time
    ITEMCHANGE=30,	// when a single column changes in a row
    POSTSCAN=40,
  };
};

#define METACALLBACKI_PARENT SysCallbackI
class MetaCallbackI : public SysCallbackI {
public:
  virtual FOURCC syscb_getEventType() { return SysCallback::DB; }

protected:
  // when items added (will be called w/ NULL after all items in)
  virtual void metacb_onItemAdd(const char *) {}
  // when items changed during a session
  virtual void metacb_onItemChange(const char *, const char *tag) {}
  // when items deleted during a session
  virtual void metacb_onItemDel(const char *) {}
  // after db is ready to query
  virtual void metacb_onPostScan() {}
  // NOTE: NO callback on system shutdown! (yet)

private:
  virtual int syscb_notify(int msg, int param1=0, int param2=0) {
    using namespace MetaCallback;
    switch (msg) {
      case ITEMADD:
        metacb_onItemAdd(reinterpret_cast<const char *>(param1));
      break;
      case ITEMCHANGE:
        metacb_onItemChange(reinterpret_cast<const char *>(param1), reinterpret_cast<const char *>(param2));
      break;
      case ITEMDEL:
        metacb_onItemDel(reinterpret_cast<const char *>(param1));
      break;
      case POSTSCAN:
        metacb_onPostScan();
      break;
      default:
        return 0;
    }
    return 1;
  }

};

#endif
