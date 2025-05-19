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

#ifndef _SVC_PLAYLIST_H
#define _SVC_PLAYLIST_H

#include "../../bfc/dispatch.h"
#include "services.h"

class Playlist;

class NOVTABLE svc_playlistReader : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::PLAYLISTREADER; }

  const char *getExtension();
  int testFilename(const char *filename);

  int readPlaylist(const char *filename);

  const char *getLabel();
  int getNumEntries();
  const char *enumEntry(int n);

protected:
  enum {
    GETEXTENSION=0, READPLAYLIST=1, GETLABEL=2, GETNUMENTRIES=3, ENUMENTRY=4,
    TESTFILENAME=100,
  };
};

inline
const char *svc_playlistReader::getExtension() {
  return _call(GETEXTENSION, "");
}

inline
int svc_playlistReader::testFilename(const char *filename) {
  return _call(TESTFILENAME, -1, filename);
}

inline
int svc_playlistReader::readPlaylist(const char *filename) {
  return _call(READPLAYLIST, 0, filename);
}

inline
const char *svc_playlistReader::getLabel() {
  return _call(GETLABEL, (const char *)0);
}

inline
int svc_playlistReader::getNumEntries() {
  return _call(GETNUMENTRIES, 0);
}

inline
const char *svc_playlistReader::enumEntry(int n) {
  return _call(ENUMENTRY, (const char *)NULL, n);
}

class NOVTABLE svc_playlistReaderI : public svc_playlistReader {
public:
  virtual const char *getExtension()=0;
  virtual int testFilename(const char *filename) { return -1; }

  virtual int readPlaylist(const char *filename)=0;

  virtual const char *getLabel()=0;
  virtual int getNumEntries()=0;
  virtual const char *enumEntry(int n)=0;

private:
  RECVS_DISPATCH;
};

class NOVTABLE svc_playlistWriter : public Dispatchable {
public:
  static FOURCC getServiceType() { return WaSvc::PLAYLISTWRITER; }

  const char *getExtension();
  const char *getDescription();

  int writePlaylist(const char *filename, Playlist *pl, int full_data, int first, int last);

// old-style, DEPRECATED
  int beginWrite(const char *filename, int n, const char *label);
  void writeEntry(const char *playstring);
  void endWrite();

protected:
  enum {
    GETEXTENSION=0,
    GETDESCRIPTION=1,
    BEGINWRITE=2,
    WRITEENTRY=3,
    ENDWRITE=4,
    WRITEPLAYLIST=100,
  };
};

inline
const char *svc_playlistWriter::getExtension() {
  return _call(GETEXTENSION, (const char *)0);
}

inline
const char *svc_playlistWriter::getDescription() {
  return _call(GETDESCRIPTION, (const char *)0);
}

inline
int svc_playlistWriter::writePlaylist(const char *filename, Playlist *pl, int full_data, int first, int last) {
  return _call(WRITEPLAYLIST, -1, filename, pl, full_data, first, last);
}

inline
int svc_playlistWriter::beginWrite(const char *filename, int n, const char *label) {
  return _call(BEGINWRITE, 0, filename, n, label);
}

inline
void svc_playlistWriter::writeEntry(const char *playstring) {
  _voidcall(WRITEENTRY, playstring);
}

inline
void svc_playlistWriter::endWrite() {
  _voidcall(ENDWRITE);
}

class NOVTABLE svc_playlistWriterI : public svc_playlistWriter {
public:
  virtual const char *getExtension()=0;
  virtual const char *getDescription()=0;

  virtual int writePlaylist(const char *filename, Playlist *pl, int full_data, int first, int last) { return -1; }

// old-style, DEPRECATED
  virtual int beginWrite(const char *filename, int n, const char *label) { return 0; }
  virtual void writeEntry(const char *playstring) { }
  virtual void endWrite() { }

protected:
  RECVS_DISPATCH;
};

#include "servicei.h"

template <class T>
class PlaylistReaderCreator : public waServiceFactoryT<svc_playlistReader, T> {};

template <class T>
class PlaylistWriterCreator : public waServiceFactoryT<svc_playlistWriter, T> {};

#include "../../bfc/svc_enum.h"
#include "../../bfc/string.h"

class PlaylistReaderEnum : public SvcEnumT<svc_playlistReader> {
public:
  PlaylistReaderEnum(const char *filename) : fn(filename) {}

protected:
  virtual int testService(svc_playlistReader *svc) {
    int r = svc->testFilename(fn);
    if (r == -1) return STRCASEEQL(svc->getExtension(), Std::extension(fn));
    return r;
  }

private:
  String fn;
};

class PlaylistWriterEnum : public SvcEnumT<svc_playlistWriter> {
public:
  PlaylistWriterEnum(const char *filename) :
    ext(Std::extension(filename)) {}
protected:
  virtual int testService(svc_playlistWriter *svc) {
    return STRCASEEQL(svc->getExtension(), ext);
  }

private:
  String ext;
};

#endif
