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

#ifndef _CORECB_H
#define _CORECB_H

#include "../bfc/dispatch.h"
#include "../bfc/reentryfilter.h"

// don't derive from this
class CoreCallback : public Dispatchable {
protected:
  CoreCallback() {}

public:
  int ccb_notify(int msg, int param1=0, int param2=0) {
    return _call(CCB_NOTIFY, 0, msg, param1, param2);
  }

  // class Dispatchable codes
  enum {
    CCB_NOTIFY = 100,
  };

  // various ccb_notify notifications. these are *not* the Dispatchable codes
  enum {
    REGISTER        = 100,
    DEREGISTER      = 200,
    NEXTFILE        = 300,

    STARTED         = 500,
    STOPPED         = 600,
    PAUSED          = 700,
    UNPAUSED        = 800,
    SEEKED          = 900,

    VOLCHANGE       = 2000,
    EQSTATUSCHANGE  = 2100,
    EQPREAMPCHANGE  = 2200,
    EQBANDCHANGE    = 2300,
    EQAUTOCHANGE    = 2400,
    PANCHANGE       = 2500,
    
    STATUSMSG       = 3000,
    WARNINGMSG      = 3100,
    ERRORMSG        = 3200,
    ERROROCCURED    = 3300,

    TITLECHANGE     = 4000,
    TITLE2CHANGE    = 4100,
    INFOCHANGE      = 4200,
    URLCHANGE       = 4300, 
    LENGTHCHANGE    = 4400,

    NEEDNEXTFILE    = 5100,
    SETNEXTFILE     = 5200,

    ABORTCURRENTSONG= 6000,
    
    ENDOFDECODE     = 7000,
  };
};

// this class does NOT automatically deregister itself when you destruct it
// that is up to you, and obviously, if you fail to do so, you'll crash the
// whole app. and they'll all laugh at you. see class CoreHandle
class CoreCallbackI : public CoreCallback {
protected:
  CoreCallbackI() { } // no instantiating this on its own

public:
  virtual int corecb_onStarted() { return 0; }
  virtual int corecb_onStopped() { return 0; }
  virtual int corecb_onPaused() { return 0; }
  virtual int corecb_onUnpaused() { return 0; }
  virtual int corecb_onSeeked(int newpos) { return 0; }

  virtual int corecb_onVolumeChange(int newvol) { return 0; }
  virtual int corecb_onPanChange(int newpan) { return 0; }

  virtual int corecb_onEQStatusChange(int newval) { return 0; }
  virtual int corecb_onEQPreampChange(int newval) { return 0; }
  virtual int corecb_onEQBandChange(int band, int newval) { return 0; }
  virtual int corecb_onEQAutoChange(int newval) { return 0; }

  virtual int corecb_onStatusMsg(const char *text) { return 0; }
  virtual int corecb_onWarningMsg(const char *text) { return 0; }
  virtual int corecb_onErrorMsg(const char *text) { return 0; }

  virtual int corecb_onTitleChange(const char *title) { return 0; }
  virtual int corecb_onTitle2Change(const char *title, const char *title2) { return 0; }
  virtual int corecb_onInfoChange(const char *info) { return 0; }
  virtual int corecb_onUrlChange(const char *url) { return 0; }
  virtual int corecb_onLengthChange(int newlength) { return 0; }

  virtual int corecb_onNextFile() { return 0; }
  virtual int corecb_onNeedNextFile(int fileid) { return 0; }
  virtual int corecb_onSetNextFile(const char *playstring) { return 0; }

  virtual int corecb_onErrorOccured(int severity, const char *text) { return 0; }

  // return 1 in this callback to make the current callback chain to abort
  virtual int corecb_onAbortCurrentSong() { return 0; }

  virtual int corecb_onEndOfDecode() { return 0; }

  virtual int ccb_notify(int msg, int param1=0, int param2=0);
protected:
  RECVS_DISPATCH;
  ReentryFilterObject filter;
};

#endif
