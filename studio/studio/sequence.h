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
#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include "../bfc/dispatch.h"

// abstracted version of a playback order
class ItemSequencer : public Dispatchable {
public:
  const char *getNextPlayItem();
  int getCurrentPlaybackNumber();	// 0-based, -1 if you don't know
  int getNumItems();	// -1 if you don't know
  int rewind();
  int forward();

  int onNotify(int msg, int param1=0, int param2=0);

protected:
  enum {
    GETNEXTPLAYITEM=100,
    GETCURRENTPLAYBACKNUMBER=101,
    GETNUMITEMS=102,
    REWIND=200,
    FORWARD=210,
    ONNOTIFY=300,
  };
};

inline const char *ItemSequencer::getNextPlayItem() {
  return _call(GETNEXTPLAYITEM, (const char *)NULL);
}

inline
int ItemSequencer::getCurrentPlaybackNumber() {
  return _call(GETCURRENTPLAYBACKNUMBER, -1);
}

inline
int ItemSequencer::getNumItems() {
  return _call(GETNUMITEMS, -1);
}

inline int ItemSequencer::rewind() {
  return _call(REWIND, 0);
}

inline int ItemSequencer::forward() {
  return _call(FORWARD, 0);
}

inline int ItemSequencer::onNotify(int msg, int param1, int param2) {
  return _call(ONNOTIFY, 0, msg, param1, param2);
}

#define SEQNOTIFY_ONREGISTER	10
#define SEQNOTIFY_ONDEREGISTER	20
#define SEQNOTIFY_ONNEXTFILE	30
#define SEQNOTIFY_ONTITLECHANGE	40
#define SEQNOTIFY_ONSTARTED	50
#define SEQNOTIFY_ONSTOPPED	60
#define SEQNOTIFY_ONPAUSED	70
#define SEQNOTIFY_ONUNPAUSED	80

// override this one
class ItemSequencerI : public ItemSequencer {
public:
  virtual int rewind()=0;
  virtual int forward()=0;
  virtual const char *getNextPlayItem()=0;
  virtual int getCurrentPlaybackNumber() { return -1; }
  virtual int getNumItems() { return -1; }

  // these are optional callbacks
  virtual int onRegister() { return 0; };
  virtual int onDeregister() { return 0; };
  virtual int onNextFile() { return 0; }// on transition
  virtual int onTitleChange() { return 0; }
  virtual int onStarted() { return 0; }
  virtual int onStopped() { return 0; }
  virtual int onPaused() { return 0; }
  virtual int onUnpaused() { return 0; }

protected:
  // default implementation calls above callback methods based on msg
  virtual int onNotify(int msg, int param1, int param2);

  RECVS_DISPATCH;
};

// also somewhat abstract, but implements playing through some arbitrary
// list. just override the protected stuff
class ListSequencer : public ItemSequencerI {
public:
  virtual const char *getNextPlayItem();
  virtual int rewind();
  virtual int forward();

protected:
  // override these 4 only
  virtual int getNumEntries()=0;
  virtual const char *enumItem(int pos)=0;
  virtual int getCurrent()=0;
  virtual int setCurrent(int cur)=0;

protected:
  virtual int loop() { return 0; }	// override as necessary

private:
  virtual int getNumItems();	// calls getNumEntries()
};

#endif

