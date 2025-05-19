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
#include "../studio/api.h"
#include "cdamci.h"

MCIERROR MCISendCommand(MCIDEVICEID IDDevice, UINT uMsg, DWORD fdwCommand, DWORD dwParam)
{
  DWORD t=GetCurrentThreadId();
  extern DWORD mainthreadid;
  MCIERROR nErr;
  if(t!=mainthreadid) {
    // need to be called from the main thread
    extern GUID guid;
    mciparams *mp=(mciparams *)api->sysMalloc(sizeof(mciparams));
    mp->IDDevice=IDDevice;
    mp->uMsg=uMsg;
    mp->fdwCommand=fdwCommand;
    mp->dwParam=dwParam;
    int wait=0;
    if(uMsg==MCI_STATUS || uMsg==MCI_PLAY || uMsg==MCI_OPEN || uMsg==MCI_SET) wait=1;
    nErr = api->cmd_postCommand(guid,"mcisendcommand",0,0,mp,sizeof(mciparams),wait);
  } else nErr = mciSendCommand(IDDevice, uMsg, fdwCommand, dwParam);
#if 0
  if (nErr) 
	{
    char szError[256];
    mciGetErrorString(nErr, szError, 256);
    MessageBox(NULL,szError,"MCI CD Error",MB_OK);
  }
#endif
  return nErr;
}

int isMediaPresent(MCIDEVICEID wDeviceID)
{
	MCI_STATUS_PARMS p;
	p.dwItem=MCI_STATUS_MEDIA_PRESENT;
	if (MCISendCommand(wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&p)) return 1;
	return p.dwReturn?1:0;
}

static char g_opened_device=0;
static MCIDEVICEID g_opened_deviceid=0;
static int g_opencd_refcount=0;
BOOL CDOpen(MCIDEVICEID* lpDeviceID, int device)
{
  if(g_opened_device==device) {
    *lpDeviceID=g_opened_deviceid;
    g_opencd_refcount++;
    return TRUE;
  }
  MCI_OPEN_PARMS sMCIOpen;
  MCI_SET_PARMS sMCISet;
  char zDevice[3]={device,':',0};
  DWORD nErr;

  sMCIOpen.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_CD_AUDIO;
  sMCIOpen.lpstrElementName = zDevice;
  nErr = MCISendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_SHAREABLE | 
          MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD)&sMCIOpen);
  if (nErr) 
  {
    nErr = MCISendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | 
          MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD) &sMCIOpen);
	  if (nErr) 
    {
	    return FALSE;
    }
  }
    
	sMCISet.dwTimeFormat = MCI_FORMAT_TMSF;
	MCISendCommand (sMCIOpen.wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD) &sMCISet);
	
  *lpDeviceID = sMCIOpen.wDeviceID;
  
  g_opened_device=device;
  g_opened_deviceid=*lpDeviceID;
  g_opencd_refcount++;

  return TRUE;
}


void CDClose(MCIDEVICEID* lpDeviceID)
{
  if(*lpDeviceID==g_opened_deviceid) {
    g_opencd_refcount--;
    if(g_opencd_refcount) return;
    g_opened_device=0;
    g_opened_deviceid=0;
  }

	MCI_GENERIC_PARMS sMCIGeneric;

  //FUCKO! sMCIGeneric.dwCallback = (DWORD) line.hMainWindow;
  sMCIGeneric.dwCallback = NULL;
  MCISendCommand(*lpDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD) &sMCIGeneric);

  *lpDeviceID = 0;
}


unsigned int CDGetTracks(MCIDEVICEID wDeviceID)
{
  MCI_STATUS_PARMS sMCIStatus;
  sMCIStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
  if (MCISendCommand (wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT,(DWORD) &sMCIStatus)) return -1;
  return sMCIStatus.dwReturn;
}

unsigned int CDGetCurrTrack(MCIDEVICEID wDeviceID)
{
  MCI_STATUS_PARMS sMCIStatus;

  sMCIStatus.dwItem = MCI_STATUS_POSITION;
	MCISendCommand (wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT,(DWORD) &sMCIStatus);

	return ((int) MCI_TMSF_TRACK (sMCIStatus.dwReturn));
}

int CDPlay(MCIDEVICEID wDeviceID, unsigned int nTrack, BOOL bResume, unsigned int nMin, unsigned int nSec, unsigned int endms)
{
	MCI_PLAY_PARMS sMCIPlay;
  unsigned int nActualTrack = nTrack;

  sMCIPlay.dwFrom = MCI_MAKE_TMSF (nActualTrack, nMin, nSec, 0);
  sMCIPlay.dwTo = MCI_MAKE_TMSF (nActualTrack, endms/60000, (endms/1000)%60,0);
  if (!bResume) 
  {
		return MCISendCommand (wDeviceID, MCI_PLAY, MCI_FROM | MCI_TO ,(DWORD) &sMCIPlay);
	}
  else 
  {	
		return MCISendCommand (wDeviceID, MCI_PLAY, MCI_FROM | MCI_TO, (DWORD) (LPVOID) &sMCIPlay);
	}
}


void CDStop(MCIDEVICEID wDeviceID)
{
	MCISendCommand(wDeviceID, MCI_STOP, 0, 0);
}


void CDPause(MCIDEVICEID wDeviceID)
{
	MCISendCommand(wDeviceID, MCI_PAUSE, 0,0);
}

unsigned int CDGetTrackLength(MCIDEVICEID wDeviceID, unsigned int nTrack)
{
  MCI_STATUS_PARMS sMCIStatus;
  int r;

  sMCIStatus.dwItem = MCI_STATUS_POSITION ;
	sMCIStatus.dwTrack = nTrack+1;
  sMCIStatus.dwReturn = 0;
	int ret=MCISendCommand (wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT,(DWORD) &sMCIStatus); 
  if(ret || !sMCIStatus.dwReturn) // if error than last track
  {
    sMCIStatus.dwItem = MCI_STATUS_LENGTH;
	  sMCIStatus.dwTrack = nTrack;
	  if (MCISendCommand (wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT,(DWORD) &sMCIStatus)) return -1000;
    return sMCIStatus.dwReturn;
  }

  r=sMCIStatus.dwReturn;
  sMCIStatus.dwItem = MCI_STATUS_POSITION ;
	sMCIStatus.dwTrack = nTrack;
	if (MCISendCommand (wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK | MCI_WAIT,(DWORD) &sMCIStatus)) return -1000;
	return r-sMCIStatus.dwReturn;
}

