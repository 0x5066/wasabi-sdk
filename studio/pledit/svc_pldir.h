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

#ifndef _SVC_PLDIR_H
#define _SVC_PLDIR_H

// ************* WARNING **************
// this is going to be ported over to use
// svc_objectDir at some point (soon)
// just so you know

#include "../bfc/dispatch.h"
#include "../bfc/depend.h"
#include "../studio/services/services.h"

class Playlist;
class PlaylistEntry;

#define DD_PLDIR "service:svc_plDir"

#include "plhand.h"

class svc_plDir : public Dispatchable {
public:
  static int getServiceType() { return WaSvc::UNIQUE; }
  static const char *dragitem_getDatatype() { return DD_PLDIR; }
  static const GUID getServiceGuid() {
    // {B240D9A9-A39D-410d-BFC7-22856AD9F722}
    const GUID ret = { 0xb240d9a9, 0xa39d, 0x410d, { 0xbf, 0xc7, 0x22, 0x85, 0x6a, 0xd9, 0xf7, 0x22 } };
    return ret;
  }
  static const GUID *depend_getClassGuid() {
    // {F49EF35A-3788-4a73-87E8-E88300F63971}
    static const GUID ret = 
    { 0xf49ef35a, 0x3788, 0x4a73, { 0x87, 0xe8, 0xe8, 0x83, 0x0, 0xf6, 0x39, 0x71 } };
    return &ret;
  }

  Dependent *getDependencyPtr();

  int getNumPlaylists(int skip_hidden=TRUE);
  PlaylistHandle enumPlaylist(int n, int skip_hidden=TRUE);

  Playlist *getPlaylist(PlaylistHandle handle);

  const char *getPlaylistLabel(PlaylistHandle handle);
  int setPlaylistLabel(PlaylistHandle handle, const char *newlabel);

  PlaylistHandle insertPlaylist(const char *filename=NULL, const char *label=NULL, const char *path=NULL, int hidden=FALSE);
  int removePlaylist(PlaylistHandle handle);

  int setAutoSave(PlaylistHandle handle, int autosave);

  void clearAll();

  int setHidden(PlaylistHandle handle, int hidden);
  int getHidden(PlaylistHandle handle);

  void setCurrentlyOpen(PlaylistHandle handle);
  PlaylistHandle getCurrentlyOpen();
  PlaylistHandle getCurrentlyPlaying();

  const char *getPlaystringFromEntry(PlaylistEntry *entry);

  // dependency events
  enum {
    Event_PLAYLIST_ADDED=8000,
    Event_PLAYLIST_REMOVED=8100,
    Event_PLAYLIST_LABELCHANGE=8200,
    Event_PLAYLIST_VISIBILITY=8300,
  };

  // dispatchable codes
  enum {
    GETDEPENDENCYPTR=100,
    //200,300 retired
    GETNUMPLAYLISTS=201,
    ENUMPLAYLIST=301,
    GETPLAYLIST=400,
    GETPLAYLISTLABEL=500,
    SETPLAYLISTLABEL=510,
    //600,601 retired
    INSERTPLAYLIST=602,
    REMOVEPLAYLIST=610,
    CLEARALL=700,
    SETAUTOSAVE=800,
    SETHIDDEN=900,
    GETHIDDEN=1000,
    GETPLAYSTRINGFROMENTRY=1100,
    SETCURRENTLYOPEN=1200,
    GETCURRENTLYOPEN=1210,
    GETCURRENTLYPLAYING=1220,
  };
};

inline
Dependent *svc_plDir::getDependencyPtr() {
  return _call(GETDEPENDENCYPTR, (Dependent*)NULL);
}

inline
int svc_plDir::getNumPlaylists(int skip_hidden) {
  return _call(GETNUMPLAYLISTS, 0, skip_hidden);
}

inline
PlaylistHandle svc_plDir::enumPlaylist(int n, int skip_hidden) {
  return _call(ENUMPLAYLIST, INVALID_PLAYLIST_HANDLE, n, skip_hidden);
}

inline
Playlist *svc_plDir::getPlaylist(PlaylistHandle handle) {
  return _call(GETPLAYLIST, (Playlist*)NULL, handle);
}

inline
const char *svc_plDir::getPlaylistLabel(PlaylistHandle handle) {
  return _call(GETPLAYLISTLABEL, (const char *)NULL, handle);
}

inline
int svc_plDir::setPlaylistLabel(PlaylistHandle handle, const char *newlabel) {
  return _call(SETPLAYLISTLABEL, 0, handle, newlabel);
}

inline
PlaylistHandle svc_plDir::insertPlaylist(const char *filename, const char *label, const char *path, int hidden) {
  return _call(INSERTPLAYLIST, INVALID_PLAYLIST_HANDLE, filename, label, path, hidden);
}

inline
int svc_plDir::removePlaylist(PlaylistHandle handle) {
  return _call(REMOVEPLAYLIST, 0, handle);
}

inline
int svc_plDir::setAutoSave(PlaylistHandle handle, int autosave) {
  return _call(SETAUTOSAVE, 0, handle, autosave);
}

inline
void svc_plDir::clearAll() {
  _voidcall(CLEARALL);
}

inline
int svc_plDir::setHidden(PlaylistHandle handle, int hidden) {
  return _call(SETHIDDEN, 0, handle, hidden);
}

inline
int svc_plDir::getHidden(PlaylistHandle handle) {
  return _call(GETHIDDEN, 0, handle);
}

inline
void svc_plDir::setCurrentlyOpen(PlaylistHandle handle) {
  _voidcall(SETCURRENTLYOPEN, handle);
}

inline
PlaylistHandle svc_plDir::getCurrentlyOpen() {
  return _call(GETCURRENTLYOPEN, INVALID_PLAYLIST_HANDLE);
}

inline
PlaylistHandle svc_plDir::getCurrentlyPlaying() {
  return _call(GETCURRENTLYPLAYING, INVALID_PLAYLIST_HANDLE);
}

inline
const char *svc_plDir::getPlaystringFromEntry(PlaylistEntry *entry) {
  return _call(GETPLAYSTRINGFROMENTRY, (const char *)NULL, entry);
}

class svc_plDirI : public svc_plDir, public DependentI {
public:
  virtual Dependent *getDependencyPtr() { return this; }

  virtual int getNumPlaylists(int skip_hidden=TRUE)=0;
  virtual PlaylistHandle enumPlaylist(int n, int skip_hidden=TRUE)=0;

  virtual Playlist *getPlaylist(PlaylistHandle handle)=0;

  virtual const char *getPlaylistLabel(PlaylistHandle handle)=0;
  virtual int setPlaylistLabel(PlaylistHandle handle, const char *newlabel)=0;

  virtual PlaylistHandle insertPlaylist(const char *filename=NULL, const char *label=NULL, const char *path=NULL, int hidden=FALSE)=0;
  virtual int removePlaylist(PlaylistHandle handle)=0;
  virtual int setAutoSave(PlaylistHandle handle, int autosave)=0;

  virtual void clearAll()=0;

  virtual int setHidden(PlaylistHandle handle, int hidden)=0;
  virtual int getHidden(PlaylistHandle handle)=0;

  virtual void setCurrentlyOpen(PlaylistHandle handle)=0;
  virtual PlaylistHandle getCurrentlyOpen()=0;
  virtual PlaylistHandle getCurrentlyPlaying()=0;

  virtual const char *getPlaystringFromEntry(PlaylistEntry *entry)=0;

protected:
  RECVS_DISPATCH;
};

#endif
