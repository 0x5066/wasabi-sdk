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
#ifndef _EDITOR_H
#define _EDITOR_H

#include "../bfc/dispatch.h"
#include "plhand.h"

class Dependent;
class Playlist;

class Editor : public Dispatchable {
public:
  static GUID getInterfaceGuid() {
    // {265947B2-3EDB-453e-B748-EC17890F4FE4}
    const GUID guid = 
    { 0x265947b2, 0x3edb, 0x453e, { 0xb7, 0x48, 0xec, 0x17, 0x89, 0xf, 0x4f, 0xe4 } };
    return guid;
  }

  Dependent *getDependencyPtr() { return _call(GETDEPPTR, (Dependent*)NULL); }
  static const GUID *depend_getClassGuid() {
    // {A48AF8D4-358A-4952-B009-7A92BD1BB802}
    static const GUID retval = 
      { 0xa48af8d4, 0x358a, 0x4952, { 0xb0, 0x9, 0x7a, 0x92, 0xbd, 0x1b, 0xb8, 0x2 } };
    return &retval;
  }

  int setPlaylistByHandle(PlaylistHandle handle, const RECT *src=NULL) {
    return _call(SETPLAYLISTBYHANDLE, 0, handle, src);
  }
  Playlist *getPlaylist() {
    return _call(GETPLAYLIST, (Playlist*)NULL);
  }

  void scrollToY(int y) {
    _voidcall(SCROLLTOY, y);
  }
  int getScrollY() {
    return _call(GETSCROLLY, 0);
  }

  int itemSelected(int pos) {
    return _call(ITEMSELECTED, 0, pos);
  }

  void ensureVisible(int pos) {
    _voidcall(ENSUREVISIBLE, pos);
  }

  // dependency events
  enum {
    Event_REPAINT=27100,
    Event_PLAYLISTLOADED=27200,
    Event_PLAYLISTUNLOADED=27210,
    Event_SHOWPLDIR=27300,
    Event_ENTRYSELECTED=27400,
    Event_ENTRYDESELECTED=27410,
  };

protected:
  enum {
    GETDEPPTR=100,
    SETPLAYLISTBYHANDLE=200,
    GETPLAYLIST=300,
    SCROLLTOY=400,
    GETSCROLLY=500,
    ITEMSELECTED=600,
    ENSUREVISIBLE=700,
  };
};

#endif
