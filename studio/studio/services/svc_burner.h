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


#include "../../bfc/dispatch.h"

namespace MediaRecorder {

// this represents one session on the cd
// normal audio cds have 1 redbook session, mixed mode has 2 (or more), with
// 1 audio followed by 1 or more data
class Session {	// FUCKO: dispatchable
public:
  virtual int getSessionType()=0;
  enum {
    Session_REDBOOK, Session_DATA, Session_ISO
  };
  virtual int closeSession()=0;
  virtual int getNumEntries()=0;
  virtual const char *enumEntry(int n)=0;
};

// this is the physical device
class Device {	// FUCKO: dispatchable
public:
  virtual Dependent *getDependencyPtr()=0;	// for events

  virtual const char *getDeviceName()=0;	// internal device name
  virtual const char *getDeviceType()=0;	// "CD-R", "CD-RW", "DVD-R" etc
  virtual const char *getDeviceDescription()=0; // user readable string

  virtual int enumDeviceSpeeds(int n)=0;	// in kb/s

  virtual int getMediaSize()=0;			// total space in bytes
  virtual int getMediaFree()=0;			// free space in bytes

  virtual void clearSessions()=0;
  virtual int addSession(Session *session)=0;
  virtual Session *getSession(int num)=0;

  virtual int setRecordSpeed(int kbps)=0; //kbps==0 means max speed
  virtual int setTest(int testmode)=0; // if true, don't really burn
  virtual int setCloseDisc(int closedisc)=0; // if true, close entire disc at end

  virtual int canBurnNow()=0;	// return 1 if everything's ready
  virtual int canCancel()=0;	// return 1 if we can cancel (during burning)

  virtual int begin()=0;
  virtual int end()=0;
  virtual int cancel()=0;

  virtual int getStatus()=0;
  enum {
    Status_IDLE, Status_PREPARING, Status_BURNING, Status_DONE,
  };
  virtual int getProgress()=0;	// # of bytes written
  virtual const char *getStatusText()=0; // like "xx% complete" or something

  virtual const char *getLastError()=0;

  enum {
    Event_BURNBEGIN=100,
    Event_ENTRYCOMPLETE=200,	// param should be the position
    Event_SESSIONCOMPLETE=300,
    Event_BURNEND=400,
    Event_ERROR=500,
    Event_MEDIACHANGE=600,	// user put in a different disc
  };
};

};	// end namespace MediaRecorder

// this should be implemented by a given burning lib
class svc_mediaRecorder { // FUCKO: dispatchable
public:
  static FOURCC getServiceType() { return WaSvc::MEDIARECORDER; }

  virtual int isSessionSupported(MediaRecorder::Session *session)=0;
  virtual int isMediaSupported(const char *medianame)=0;// "CD-R", "DVD-R", etc.

  virtual const char *getName()=0;

  virtual int getNumDevices()=0;
  virtual MediaRecorder::Device *enumDevice(int n)=0;
};
