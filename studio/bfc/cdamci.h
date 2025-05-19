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

// Just stuffing some of Christophe's MCI utils into a more centralized package.
// The Crossplatform interface for this stuff is cddaenum.h

#include <mmsystem.h>

MCIERROR MCISendCommand(MCIDEVICEID IDDevice, UINT uMsg, DWORD fdwCommand, DWORD dwParam);
int isMediaPresent(MCIDEVICEID wDeviceID);
BOOL CDOpen(MCIDEVICEID* lpDeviceID, int device);
void CDClose(MCIDEVICEID* lpDeviceID);
unsigned int CDGetTracks(MCIDEVICEID wDeviceID);
unsigned int CDGetCurrTrack(MCIDEVICEID wDeviceID);
int CDPlay(MCIDEVICEID wDeviceID, unsigned int nTrack, BOOL bResume, unsigned int nMin, unsigned int nSec, unsigned int endms);
void CDStop(MCIDEVICEID wDeviceID);
void CDPause(MCIDEVICEID wDeviceID);
unsigned int CDGetTrackLength(MCIDEVICEID wDeviceID, unsigned int nTrack);

typedef struct {
  MCIDEVICEID IDDevice;
  UINT uMsg;
  DWORD fdwCommand;
  DWORD dwParam;
} mciparams;
