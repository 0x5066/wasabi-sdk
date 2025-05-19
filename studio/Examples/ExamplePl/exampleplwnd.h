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

#ifndef _EXAMPLEPLWND_H
#define _EXAMPLEPLWND_H

//=================================
//  Wasabi Includes
//=================================
#include "../../common/framewnd.h"    //FrameWnd
#include "../../bfc/appcmds.h"        //AppCmdsI
#include "../../studio/runlevelcb.h"  //RunlevelCallbackI

//=================================
//  Forward References
//=================================
class ListWnd;
class svc_plDir;

//=================================
//  
//=================================
#define EXAMPLEPLWND_PARENT FrameWnd

class ExamplePlWnd : public EXAMPLEPLWND_PARENT, public AppCmdsI, public RunlevelCallbackI {
public:
  ExamplePlWnd();           //El Constructor.
  virtual ~ExamplePlWnd();  //El Destructor.

  //Our Window Name.
  static const char *getWindowTypeName() { return "ExamplePl Window"; }
  
  //Our Window GUID.  
  static GUID getWindowTypeGuid() { 
    GUID examplepl_guid = 
    { 0xd99950fd, 0x7083, 0x416a, { 0xb7, 0xbd, 0x3f, 0x42, 0xc9, 0x45, 0xc2, 0xa8 } };
    return examplepl_guid; 
  }
  
  //We want to do something upon initialisation of our window.
  virtual int onInit();

  //Override for our AppCmdsI Buttons we're using.
  virtual void appcmds_onCommand(int id, const RECT *buttonRect); //from AppCmdsI.

  //Override to trap the applications shutdown so we can properly dispose of our service.
  virtual void runlevelcb_onShutdown(); //from RunlevelCallbackI.

  //An enum for the commands for our AppCmdsI buttons.
  enum {
    PCMD_CLEARALL = 100,
    PCMD_GLUEALL,
    PCMD_READCURRENT,
    PCMD_READLIST
  };

protected:
  ListWnd   *rightpane, *leftpane;  //Left and right pane.
  svc_plDir *pldir; //Playlist directory service pointer.

};

#endif