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


#include "svc_textfeed.h"

#define CBCLASS _svc_textFeed
START_DISPATCH;
  CB(SVCTEXTFEED_HASFEED, hasFeed);
  VCB(SVCTEXTFEED_REGISTERCALLBACK, registerCallback);
  VCB(SVCTEXTFEED_UNREGISTERCALLBACK, unregisterCallback);
END_DISPATCH;
#undef CBCLASS


void svc_textFeedI::registerFeed(const char *feedid) {
  Feed *f = feeds.findItem(feedid);
  if (f != NULL) return;
  f = new Feed(feedid);
  feeds.addItem(f);
}

void svc_textFeedI::sendFeed(const char *feedid, const char *text) {
  Feed *f = feeds.findItem(feedid);
  if (f == NULL) return;
  f->sendFeed(text);
}

int svc_textFeedI::hasFeed(const char *name) {
  return (feeds.findItem(name) != NULL);
}

void svc_textFeedI::registerCallback(const char *feedid, TextFeedCallback *cb) {
  Feed *f = feeds.findItem(feedid);
  if (f == NULL) return;
  f->addCallback(cb);
  cb->textfeed_onReceiveText(f->getLastFeed());
}

void svc_textFeedI::unregisterCallback(const char *feedid, TextFeedCallback *cb) {
  Feed *f = feeds.findItem(feedid);
  if (f == NULL) return;
  f->removeCallback(cb);
}

void Feed::sendFeed(const char *txt) {
  foreach(callbacks) 
    callbacks.getfor()->textfeed_onReceiveText(txt); 
  endfor; 
  last_feed = txt;
}
