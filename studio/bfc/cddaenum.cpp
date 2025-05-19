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

#include "std.h"
#include "cddaenum.h"
#include "cdamci.h"


//
//  NOT PORTABLE.  Based off of portable header.
//

class CDDADriveWin32 : public CDDADrive {
public:
  CDDADriveWin32(int _driveletteridx);
  virtual ~CDDADriveWin32();

  virtual int getNumTracks();
  virtual int hasCD();
  virtual void enumCDAPlaystring(int track, String &playstring);
  virtual char getDriveChar();

protected:
  int driveletteridx;
  MCIDEVICEID dev;
};

CDDADriveWin32::CDDADriveWin32(int _driveletteridx) :
  driveletteridx(_driveletteridx), CDDADrive() {

}

CDDADriveWin32::~CDDADriveWin32() {
}

int CDDADriveWin32::getNumTracks() {
  // Try to open the device under MCI
  CDOpen(&dev, 'A' + driveletteridx);
  // Apparently, it's BAD to leave this device open for long periods of time.
  int retval = CDGetTracks(dev);
  CDClose(&dev);
  return retval;
}

int CDDADriveWin32::hasCD() {
  // Try to open the device under MCI
  CDOpen(&dev, 'A' + driveletteridx);
  // Apparently, it's BAD to leave this device open for long periods of time.
  int retval = isMediaPresent(dev);
  CDClose(&dev);
  return retval;
}

void CDDADriveWin32::enumCDAPlaystring(int track, String &playstring) {
  playstring = StringPrintf("cda://%c,%d", 'A' + driveletteridx, track + 1);
}

char CDDADriveWin32::getDriveChar() {
  return 'A' + driveletteridx;
}

CDDAEnum::CDDAEnum() {
  // Scan and record all the CDRom drives available.
  DWORD drivebits=GetLogicalDrives();
  for (int drivemask = 0; drivemask < 32; drivemask++) {
    if (drivebits&(1<<drivemask)) {
      if (GetDriveType(StringPrintf("%c:\\",'A'+drivemask))==DRIVE_CDROM) {
        drives.addItem( new CDDADriveWin32(drivemask));
      }
    }
  }
}

CDDAEnum::~CDDAEnum() {
  drives.deleteAll();
}

  // Methods for enumerating the drives in the system.
int CDDAEnum::getNumDrives() {
  return drives.getNumItems();
}

CDDADrive *CDDAEnum::enumDrive(int which) {
  return drives.enumItem(which);
}


