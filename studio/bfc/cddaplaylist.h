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

#ifndef __CDDAPLAYLIST_H
#define __CDDAPLAYLIST_H

#include "../studio/services/svc_action.h"
#include "../studio/runlevelcb.h"

//
//  This class handles refresh of named playlists based on a global action.
//

class Playlist;

#define CDDAPlaylistUPDATE "CDDAPlaylist:UPDATE"
#define CDDAPlaylistREMOVE "CDDAPlaylist:REMOVE"

#define CDDAPlaylistPrintf "CDROM %c"  // Used with CDDADrive::getDriveChar() for fully platform neutral joy!

// Actions are NOT global instantiates!
class CDDAPlaylist : public svc_actionI {
public:
  enum {
    UPDATE = 0,
    REMOVE
  };

  // You can use these static inlines to fire off the actions this guy exposes.
  // Other objects can subclass from this object and will be allowed to handle the actions, too,
  // IF you send the action using these statics.
  static inline void Update(const char devchar = (char)0x80) {
    ActionEnum aenum(CDDAPlaylistUPDATE);
    svc_action *action = aenum.getFirst();
    while (action) {
      const char *param = NULL;
      if ((int)devchar != -128) {
        param = StringPrintf("%c",devchar);
      }
      action->onAction(CDDAPlaylistUPDATE, param);
      SvcEnum::release(action);
      action = aenum.getNext();
    }
  }
  static inline void Remove(const char devchar = (char)0x80) {
    ActionEnum aenum(CDDAPlaylistREMOVE);
    svc_action *action = aenum.getFirst();
    while (action) {
      const char *param = NULL;
      if ((int)devchar != -128) {
        param = StringPrintf("%c",devchar);
      }
      action->onAction(CDDAPlaylistREMOVE, param);
      SvcEnum::release(action);
      action = aenum.getNext();
    }
  }

  CDDAPlaylist();
  virtual ~CDDAPlaylist();

  // These are the actual methods that will get called for the two actions we expose.
  virtual void DoUpdate(const char *devstr = NULL) = 0;
  virtual void DoRemove(const char *devstr = NULL) = 0;

  // Methods from svc_actionI
  virtual int onActionId(int id, const char *action, const char *param, int, int, void *, int, RootWnd *);
  static const char *getServiceName() { return "CDDAPlaylist Actions Service"; }
};

// Make a subclass like this one if you also want to handle these actions.
class CDDAPlaylistI : public CDDAPlaylist {
  // These are the actual methods that will get called for the two actions we expose.
  virtual void DoUpdate(const char *devstr = NULL);
  virtual void DoRemove(const char *devstr = NULL);
};

class CDDAPlaylistCallback : public RunlevelCallbackI {
  virtual void runlevelcb_onStartup() {
  }
  virtual void runlevelcb_onAppRunning() {
    // The playlists come alive during onStartup,
    // so I have to hook in onAppRunning.  Phew.
    CDDAPlaylist::Update();
  }
  virtual void runlevelcb_onShutdown() {
/*
    (sigh)

    The pldir service already uses onShutdown, so it's not safe to use this
    for playlist-oriented shutdown needs.


    CDDAPlaylist::Remove();
*/
  }
};


#endif//__CDDAPLAYLIST_H
