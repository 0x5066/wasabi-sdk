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

#include "console.h"

#include "resource.h"

#include "../studio/api.h"
#include "../common/svc_enum.h"
#include "../common/bitmap.h"
#include "../studio/services/servicei.h"
#include "../studio/services/creators.h"	// WndCreateCreator
#include "../bfc/wndcreator.h"		// CreateWndByGuid
#include "../common/servicewnd.h"		// ServiceWndHolder
#include "debugwnd.h"


static WACConsole Console;
WAComponentClient *the = &Console;
WACConsole *consolewac = &Console;

// Console's GUID
// {EAB8435F-FEA9-43c2-9AEF-C62C668B552F}
static const GUID guid = 
{ 0xeab8435f, 0xfea9, 0x43c2, { 0x9a, 0xef, 0xc6, 0x2c, 0x66, 0x8b, 0x55, 0x2f } };

WACConsole::WACConsole() : WAComponentClient("The Debug Console Example Project") {
  // thinger icon
  registerService(new WndCreateCreatorSingle< CreateBucketItem<DebugWnd> >);
  // main menu entry
  registerAutoPopup(getGUID(), getName());
  // main window
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<DebugWnd> >);
}

WACConsole::~WACConsole() {
}

GUID WACConsole::getGUID() {
  return guid;
}

void WACConsole::onCreate() {
}

void WACConsole::onDestroy() {
// be sure to delete all your windows etc HERE, not in the destructor
// because the API pointer might be invalid in the destructor

  // delete control wnd
  delete list;
}

