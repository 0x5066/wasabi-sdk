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

#include "../bfc/std.h"
#include "../bfc/map.h"
#include "../bfc/cddaenum.h"
#include "cddaplaylist.h"
#include "../pledit/svc_pldir.h"
#include "../pledit/playlisti.h"

// OOOOPS.  I thought actions were persistent global variables.
// I was wrong.  So this now is to be my global.  Boo.  Hiss.
static Map<char, Playlist *> playlists;
// Obviously, if we ever get into a multithreaded environment,
// we'll want to use a threadsafe map object here.

CDDAPlaylist::CDDAPlaylist() {
  registerAction(CDDAPlaylistUPDATE, UPDATE);
  registerAction(CDDAPlaylistREMOVE, REMOVE);
}

CDDAPlaylist::~CDDAPlaylist() { 
}

void CDDAPlaylistI::DoUpdate(const char *devstr) {
  int i;
  char devchar = (char)0x80;

  if (devstr == NULL) {
    // (Shrug) USB CD-Rom drives?  Delete what's there before, the device may have disappeared.
    int n = playlists.getNumItems();
    for (i = 0; i < n; i++) {
      Playlist *playlist = playlists.enumItemByPos(i, NULL);
      if (playlist) {
        // Empty the playlist
        playlist->deleteAll();
      }
    }
  } else {
    // Otherwise, ready just the one we want to update
    devchar = devstr[0];
  }

  // Go through the CDRom drives,
  CDDAEnum cddaenum;
  for (i = 0; i < cddaenum.getNumDrives(); i++ ) {
    CDDADrive *aDrive = cddaenum.enumDrive(i);
    // 
    // If we only want one drive, spin here till we find the drive we want.
    if ((devstr != NULL) && (devchar != aDrive->getDriveChar())) {
      continue;
    }    
    // Find each drive's specific playlist in the map.
    Playlist *playlist = NULL;
    if (playlists.getItem(aDrive->getDriveChar(), &playlist) == 0) {
      // If we have no playlist, make/find it.
      svc_plDir *svc = SvcEnumByGuid<svc_plDir>>();
      if (svc) {
        // The label of our playlist, SEE HOW WE MAKE IT?
        StringPrintf label(CDDAPlaylistPrintf, aDrive->getDriveChar());
        // Anybody else wanting to find this playlist should make it the same way!

        // Go through all the playlists (UGH!@) to find if it is already there.
        int found, j, n = svc->getNumPlaylists();
        for (found = j = 0; j < n; j++) {
          PlaylistHandle h = svc->enumPlaylist(j);
          if (label == svc->getPlaylistLabel(h)) {
            // If we find it,
            found = 1;
            // get the playlist
            playlist = svc->getPlaylist(h);
            // add it to the map (so we only do this once!)
            playlists.addItem(aDrive->getDriveChar(), playlist);
            break;
          }
        }
        // Otherwise, add it to both the pldir and the map.
        if (!found) {
          PlaylistHandle h = svc->insertPlaylist(); // empty blank playlist.  I don't wanna/hafta deal with a filename, boyee.
          if (h != INVALID_PLAYLIST_HANDLE) {
            svc->setPlaylistLabel(h, label);
            svc->setAutoSave(h, 0);// This makes us not get saved at shutdown.
            svc->setHidden(h, 0);  // Set this to one if you want it flagged "hidden" 
                                   // (if you have your own pldir display, you should respect hidden flag!)
            playlist = svc->getPlaylist(h);
            playlist->setPlaylistName(label);
            playlists.addItem(aDrive->getDriveChar(), playlist);
          }
        }
      }
      api->service_release(svc);
    }

    // Okay, so, here, unless we're total screwups, we'll have a playlist pointer which is listed in our map.
    if (playlist) {
      // Empty the playlist (the astute reader will note that sometimes we empty a playlist pointer twice.  sue me.)
      playlist->deleteAll();
      playlist->lock(1);// This makes us not editable.
                        // (if you have your own editor display, you should respect locked flag!)
      // Now fill it with honest playstrings.
      int cdnum = aDrive->getNumTracks();
      for (int j = 0; j < cdnum; j++) {
        String playstring;
        // ask the drive for the OS-specific playstring. woo.
        aDrive->enumCDAPlaystring(j, playstring);
        if (playstring.len()) {
          playlist->addPlaylistItem(playstring);
        }
      }
    }
  }
}

void CDDAPlaylistI::DoRemove(const char *devstr) {
  char devchar = (char)0x80;
  if (devstr != NULL) {
    // Otherwise, ready just the one we want to update
    devchar = devstr[0];
  }

  svc_plDir *svc = SvcEnumByGuid<svc_plDir>>();
  if (svc) {
    int i, n = playlists.getNumItems();
    for (i = 0; i < n; i++) {
      // 
      // If we only want one drive, spin here till we find the drive we want.
      if ( (devstr != NULL) && (devchar != playlists.enumIndexByPos(i, -1)) ) {
        continue;
      }    

      Playlist *playlist = playlists.enumItemByPos(i, NULL);
      if (playlist) {
        svc->removePlaylist(playlist->getHandle());
      }
    }
    api->service_release(svc);
  }
  playlists.deleteAll();
}

int CDDAPlaylist::onActionId(int id, const char *action, const char *param,int,int,void*,int,RootWnd*) {
  switch (id) {
    case UPDATE:
      DoUpdate(param);
    return 1;
    case REMOVE:
      DoRemove(param);
    return 1;
  }
  return 0;
}

