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

#ifndef __CDDAENUM_H
#define __CDDAENUM_H

#include "ptrlist.h"
#include "string.h"

//
// Portable header, write your OS specific modules to it. 
// (No OS specific types in header, runs versionless in local DLL space only)
//
class CDDADrive;

// A simple class to enumerate and handle audio CDs in the user's drives.  
// (repeated in every OS specific module, no OS specific data members)
class CDDAEnum {
public:
  CDDAEnum();
  ~CDDAEnum();

  // Methods for enumerating the drives in the system.
  int getNumDrives();
  CDDADrive *enumDrive(int which);

protected:
  PtrList<CDDADrive> drives;
};

// The baseclass for the OS specific CDDA drive implementation 
// (assumes you will need to subclass for OS specific data members)
class CDDADrive {
public:
  CDDADrive() {}
  virtual ~CDDADrive() {}

  // Methods for determining the CDA tracks on the current disc.
  virtual int getNumTracks() = 0;
  virtual int hasCD() = 0;
  virtual void enumCDAPlaystring(int track, String &playstring) = 0;
  virtual char getDriveChar() = 0;// A single character to represent the drive to the system.  'D' or '1' or whatever.
                                  // Used by other parts of the system to make unique strings.
};


#endif//__CDDAENUM_H                                 
  