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

//-----------------------------------------------------------------------------
// Multi Window Example 
//
// This component provides 2 icons to the component bucket, one of them toggles
// a single instance window, and the other instantiate a group inside itself.
// In addition to that, an xml multiple instance svcwindow GUID is provided,
// and the component creates as many of these as required by the system.
//
//
//  ExampleMW.cpp    : Component
//  WndCreateSvc.cpp : Window Creation Service, add your new bucket icons and GUID wnd instantiation/deletion here
//  BucketGroup.cpp  : Overrides BucketItem to hold a group
//  MyWindow.cpp     : Implements instantiable window class
//
//  13/09/2001 : Francis Gastellu  - 1st version 
//-----------------------------------------------------------------------------

#include <windows.h>

#include "../../common/msgbox.h"
#include "../../studio/api.h"
#include "../../studio/services/servicei.h"
#include "ExampleMW.h"
#include "WndCreateSvc.h"
#include "MyWindow.h"
#include "resource.h"

// The Wac

static WACExampleMW examplemw;
WAComponentClient *the = &examplemw;
WACExampleMW *examplemwwac = &examplemw;

// This is your component GUID. 
// {15A3CAEE-09B0-458a-A68D-6710575C2A24}

static const GUID guid = 
{ 0x15a3caee, 0x9b0, 0x458a, { 0xa6, 0x8d, 0x67, 0x10, 0x57, 0x5c, 0x2a, 0x24 } };

// Window creation service

typedef waServiceT<svc_windowCreate, ExampleMWWndCreateSvc> ExampleMW_wndcreateSvc;

//-----------------------------------------------------------------------------

WACExampleMW::WACExampleMW() : WAComponentClient("Multi Window Example") {
#pragma SELF("mig", "cmdbar_createButton doesn't work right now.  plz2fix after!")
//  api->cmdbar_createButton("Blah", "blah");
  registerAutoPopup(MyWindow::getGuid(), "ExampleMW"); // register this window GUID in the default popup menu
  registerSkinFile("Wacs\\xml\\ExampleMW\\ExampleMW.xml");
  registerService(new ExampleMW_wndcreateSvc); // register our window creation service
}

GUID WACExampleMW::getGUID() {
  return guid;
}

#pragma SELF("mig", "cmdbar_createButton doesn't work right now.  plz2fix after!")
/*
int WACExampleMW::onCommand(const char *cmd, int param1, int param2, void *ptr, int ptrlen) {
  if (STREQL(cmd, "blah")) {
    api->messageBox("blah", "blah", MSGBOX_OK, NULL, staticWnd);
  }
	return 0;
}
*/