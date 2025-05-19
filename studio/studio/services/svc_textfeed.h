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

#ifndef _SVC_TEXTFEED_H
#define _SVC_TEXTFEED_H

#include "../../bfc/dispatch.h"
#include "../../bfc/textfeedcb.h"
#include "services.h"
#include "../../bfc/ptrlist.h"
#include "../../bfc/string.h"

class NOVTABLE svc_textFeed : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::TEXTFEED; }
  
  int hasFeed(const char *name);
  void registerCallback(const char *feedid, TextFeedCallback *cb);
  void unregisterCallback(const char *feedid, TextFeedCallback *cb);

  enum {
    SVCTEXTFEED_HASFEED=10,
    SVCTEXTFEED_REGISTERCALLBACK=20,
    SVCTEXTFEED_UNREGISTERCALLBACK=30,
  };

};

inline int svc_textFeed::hasFeed(const char *name) {
  return _call(SVCTEXTFEED_HASFEED, 0, name);
}

inline void svc_textFeed::registerCallback(const char *feedid, TextFeedCallback *cb) {
  _voidcall(SVCTEXTFEED_REGISTERCALLBACK, feedid, cb);
}

inline void svc_textFeed::unregisterCallback(const char *feedid, TextFeedCallback *cb) {
  _voidcall(SVCTEXTFEED_UNREGISTERCALLBACK, feedid, cb);
}

class NOVTABLE _svc_textFeed : public svc_textFeed {
public:
  virtual int hasFeed(const char *name)=0;
  virtual void registerCallback(const char *feedid, TextFeedCallback *cb)=0;
  virtual void unregisterCallback(const char *feedid, TextFeedCallback *cb)=0;

protected:
  RECVS_DISPATCH;
};

class Feed {
  public:
    Feed(const char *id) : feedid(id) {}
    virtual ~Feed() {}

    void addCallback(TextFeedCallback *cb) { callbacks.addItem(cb); }
    void removeCallback(TextFeedCallback *cb) { callbacks.removeItem(cb); }
    const char *getFeedId() { return feedid; }
    int getNumCallbacks() { return callbacks.getNumItems(); }
    TextFeedCallback *enuMCallback(int n) { return callbacks.enumItem(n); }
    void sendFeed(const char *txt);
    const char *getLastFeed() { return last_feed; }

  private:

    String feedid;
    PtrList<TextFeedCallback> callbacks;
    String last_feed;
};

class SortFeeds {
public:
  static int compareItem(Feed *p1, Feed *p2) {
    return STRICMP(p1->getFeedId(), p2->getFeedId());
  }
  static int compareAttrib(const char *attrib, Feed *item) {
    return STRICMP(attrib, item->getFeedId());
  }
};


class NOVTABLE svc_textFeedI : public _svc_textFeed {
public:

  // call this to register your feeds
  void registerFeed(const char *feedid);

  // and call this to send text into a feed, all registered callbacks for it will be called
  void sendFeed(const char *feedid, const char *text);

protected:

  virtual int hasFeed(const char *name);
  virtual void registerCallback(const char *feedid, TextFeedCallback *cb);
  virtual void unregisterCallback(const char *feedid, TextFeedCallback *cb);

  PtrListQuickSorted<Feed, SortFeeds> feeds;
};

#include "../../bfc/svc_enum.h"

class TextFeedEnum : public SvcEnumT<svc_textFeed> {
public:
  TextFeedEnum(const char *_feedid) : feedid(_feedid) {}
protected:
  virtual int testService(svc_textFeed *svc) {
    return (svc->hasFeed(feedid));
  }
private:
  String feedid;
};

#endif
