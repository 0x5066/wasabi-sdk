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


#include "svc_coreadmin.h"

#define CBCLASS svc_coreAdminI
START_DISPATCH;
  CB(CREATECORE,createCore)
  CB(NAMETOTOKEN,nameToToken)
  CB(FREECOREBYTOKEN,freeCoreByToken)
  CB(FREECOREBYNAME,freeCoreByName)
  CB(VERIFYTOKEN,verifyToken)
  
  CB(GETSUPPORTEDEXTENSIONS,getSupportedExtensions)
  CB(GETEXTSUPPORTEDEXTENSIONS,getExtSupportedExtensions)
  VCB(REGISTEREXTENSION,registerExtension)
  CB(GETEXTENSIONFAMILY, getExtensionFamily);
  VCB(UNREGISTEREXTENSION,unregisterExtension)
  
  CB(SETNEXTFILE,setNextFile)
  CB(SETNEXTFILEOLD,setNextFile)

  CB(GETSTATUS,getStatus)
  CB(GETCURRENT,getCurrent)
  CB(GETCURPLAYBACKNUMBER,getCurPlaybackNumber)
  CB(GETNUMTRACKS, getNumTracks);
  CB(GETPOSITION,getPosition)
  CB(GETWRITEPOSITION,getWritePosition)
  CB(GETLENGTH,getLength)
  CB(GETPLUGINDATA,getPluginData)
  CB(GETVOLUME,getVolume)
  CB(GETPAN,getPan)
  CB(GETVISDATA,getVisData)
  CB(GETLEFTVUMETER,getLeftVuMeter)
  CB(GETRIGHTVUMETER,getRightVuMeter)
  CB(GETEQSTATUS,getEqStatus)
  CB(GETEQPREAMP,getEqPreamp)
  CB(GETEQBAND,getEqBand)
  CB(GETEQAUTO,getEqAuto)
  CB(GETMUTE,getMute)
    
  CB(SETPOSITION,setPosition)
  VCB(SETVOLUME,setVolume)
  VCB(SETPAN,setPan)
  VCB(SETEQSTATUS,setEqStatus)
  VCB(SETEQPREAMP,setEqPreamp)
  VCB(SETEQBAND,setEqBand)
  VCB(SETEQAUTO,setEqAuto)
  VCB(SETMUTE,setMute)

  VCB(ADDCALLBACK,addCallback)
  VCB(DELCALLBACK,delCallback)

  CB(REGISTERSEQUENCER,registerSequencer)
  CB(DEREGISTERSEQUENCER,deregisterSequencer)
    
  VCB(USERBUTTON,userButton)
  
  VCB(SETCUSTOMMSG, setCustomMsg)
  
  VCB(SETPRIORITY, setPriority)
  CB(GETPRIORITY, getPriority)
END_DISPATCH;
#undef CBCLASS

