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

#ifndef _PLAYLIST_H
#define _PLAYLIST_H

#include "../bfc/dispatch.h"
#include "plhand.h"

#define DD_PLAYLIST "DD_Playlist v1"

class PlaylistEntry;
class Dependent;

class Playlist : public Dispatchable {
protected:
  Playlist() {}	// protect constructor
  ~Playlist() {}	// protect destructor
public:
  static const char *dragitem_getDatatype() { return DD_PLAYLIST; }
  static const GUID *depend_getClassGuid() {
    // {C11CD373-2D92-4cbb-BF0C-D3CA36151C21}
    static const GUID ret = 
    { 0xc11cd373, 0x2d92, 0x4cbb, { 0xbf, 0xc, 0xd3, 0xca, 0x36, 0x15, 0x1c, 0x21 } };
    return &ret;
  }

  inline Dependent *getDependencyPtr() {
    return _call(GETDEPENDENCYPTR, (Dependent*)NULL);
  }

  inline const char *getPlaylistName() {
    return _call(GETPLAYLISTNAME, (const char *)NULL);
  }
  inline void setPlaylistName(const char *newname) {
    _voidcall(SETPLAYLISTNAME, newname);
  }

  inline int getNumEntries() {
    return _call(GETNUMENTRIES, -1);
  }
  inline PlaylistEntry *enumEntry(int which) {
    return _call(ENUMENTRY, (PlaylistEntry*)NULL, which);
  }
  inline const char *enumItem(int pos) {
    return _call(ENUMITEM, (const char *)NULL, pos);
  }
  inline const char *getPlayStringFromEntry(PlaylistEntry *entry) {
    return _call(GETPLAYSTRINGFROMENTRY, (const char *)NULL, entry);
  }

  inline PlaylistHandle getHandle() {
    return _call(GETHANDLE, INVALID_PLAYLIST_HANDLE);
  }

  inline int getPositionForEntry(PlaylistEntry *entry) {
    return _call(GETPOSITIONFORENTRY, -1, entry);
  }

  // if autoread==TRUE it will load the info if it's not already cached (can be slow)
  inline const char *getCachedNameForEntry(PlaylistEntry *entry, int autoread=TRUE) {
    return _call(GETCACHEDNAMEFORENTRY, (const char *)NULL, entry, autoread);
  }
  inline int getCachedLengthForEntry(PlaylistEntry *entry, int autoread=TRUE) {
    return _call(GETCACHEDLENGTHFORENTRY, -1, entry, autoread);
  }

  // sorting
  inline void randomize() { _voidcall(RANDOMIZE); }
  inline void removeDups() { _voidcall(REMOVEDUPS); }
  inline void reverse() { _voidcall(REVERSE); }
  inline void sortBy(const char *field) { _voidcall(SORTBY, field); }

  // locking -- just a hint to not allow the user to modify this playlist
  inline void lock(int locked) { _voidcall(LOCK, locked); }
  inline int isLocked() { return _call(ISLOCKED, FALSE); }
  inline int isModified() { return _call(ISMODIFIED, FALSE); }

  // queueing
  inline void markNextFile(int pos, int insert_at_front=FALSE) {
    _voidcall(MARKNEXTFILE, pos, insert_at_front);
  }
  inline void unmarkFile(int pos) {
    _voidcall(UNMARKFILE, pos);
  }
  inline const char *getNextQueuedFile(int *pos=NULL) {
    return _call(GETNEXTQUEUEDFILE, (const char *)NULL, pos);
  }
  inline int getQueuePos(int pos) {
    return _call(GETQUEUEPOS, -1, pos);
  }

  // adding, deleting
  inline int addPlaylistItem(const char *playstring, int after=APPEND) {
    return _call(ADDPLAYLISTITEM, 0, playstring, after);
  }
  inline int removeByEntry(PlaylistEntry *entry) {
    return _call(REMOVEBYENTRY, 0, entry);
  }
  inline int removeByPos(int nitem) {
    return _call(REMOVEBYPOS, 0, nitem);
  }
  inline void deleteAll() {
    _voidcall(DELETEALL);
  }

  // all editors for this list should show this item
  inline void ensureVisible(int pos) {
    _voidcall(ENSUREVISIBLE, pos);
  }

  inline void deleteCachedInfo() { _voidcall(DELETECACHEDINFO); }

  inline int setCurrent(int pos) {
    return _call(SETCURRENT, 0, pos);
  }
  inline int getCurrent() {
    return _call(GETCURRENT, -1);
  }

  virtual void onNextFile()=0;	// from sequencer

  // i/o
  inline int writeToFile(const char *filename, int full_data=FALSE) {
    return _call(WRITETOFILE, 0, filename, full_data);
  }
  inline int readFromFile(const char *filename) {
    return _call(READFROMFILE, 0, filename);
  }

  // playback
  inline int startPlayback(int position, int start=TRUE) {
    return _call(STARTPLAYBACK, 0, position, start);
  }

  enum {
    BEGINNING=-1,
    APPEND=-2,
  };

  // events
  enum {
    Event_NAMECHANGE=100,
    Event_REPAINT=110,
    Event_ITEMS_ADDED=200,
    Event_ITEMS_DELETED=210,
    Event_RELOAD=220,	// order changed
    Event_QUEUE_ADDITION=300,
    Event_QUEUE_DELETION=305,
    Event_QUEUE_EMPTY=310,
    Event_SETCURRENT=400,
    Event_LOCK=410,	// param is lock status
    Event_ENSUREVISIBLE=420,	// param is which item
  };

  // dispatch
  enum { 
    GETDEPENDENCYPTR=100,
    GETPLAYLISTNAME=200,
    SETPLAYLISTNAME=300,
    GETNUMENTRIES=400,
    ENUMENTRY=500,
    ENUMITEM=600,
    GETPLAYSTRINGFROMENTRY=700,
    GETHANDLE=800,
    GETPOSITIONFORENTRY=900,
    GETCACHEDNAMEFORENTRY=905,
    GETCACHEDLENGTHFORENTRY=910,
    RANDOMIZE=1000,
    REMOVEDUPS=1100,
    REVERSE=1200,
    SORTBY=1300,
    LOCK=1400,
    ISLOCKED=1500,
    ISMODIFIED=1600,
    MARKNEXTFILE=1700,
    UNMARKFILE=1800,
    GETNEXTQUEUEDFILE=1900,
    GETQUEUEPOS=2000,
    ADDPLAYLISTITEM=2100,
    REMOVEBYENTRY=2200,
    REMOVEBYPOS=2300,
    DELETEALL=2400,
    DELETECACHEDINFO=2500,
    SETCURRENT=2600,
    GETCURRENT=2700,
    WRITETOFILE=2800,
    READFROMFILE=2900,
    STARTPLAYBACK=3000,
    ENSUREVISIBLE=3100,
  };
};

#endif
