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

//=================================
//  Project Includes
//=================================
#include "examplepl.h"
#include "exampleplwnd.h"

//=================================
//  Wasabi Includes
//=================================
#include "../../studio/api.h"         //api pointer.
#include "../../bfc/wndcreator.h"     //CreateWndByGuid, WndCreateCreatorSingle

#include "../../common/metatags.h"    //metadb defines

#include "../../bfc/wnds/listwnd.h"   //ListWnd

#include "../../pledit/svc_pldir.h"   //svc_plDir

//=================================
//  Globals
//=================================

// {D99950FD-7083-416a-B7BD-3F42C945C2A8} //Example Pl GUID
static const GUID examplepl_guid = 
{ 0xd99950fd, 0x7083, 0x416a, { 0xb7, 0xbd, 0x3f, 0x42, 0xc9, 0x45, 0xc2, 0xa8 } };

static WACNAME examplepl;             //Required!!!
WAComponentClient *the = &examplepl;  //Required!!!

//========================================================================  
//  WACNAME CLASS
//========================================================================
WACNAME::WACNAME() {
  //Register a Window Creation Service for our ExampleDragDropWnd (our main window).
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<ExamplePlWnd> >);
  
  //Register our window for Auto-Pop (so we're in the window list).
  registerAutoPopup(ExamplePlWnd::getWindowTypeGuid(), ExamplePlWnd::getWindowTypeName());
}

WACNAME::~WACNAME() {
  //Destructo do nothingo.
}

void WACNAME::onCreate() {
  //Anything that needs the API pointer needs to go here.
}

void WACNAME::onDestroy() {
  //Anything that needs the API pointer needs to go here.
}

GUID WACNAME::getGUID() {
  return examplepl_guid;  //Return our components guid.
}