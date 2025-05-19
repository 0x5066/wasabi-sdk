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

#include "callbackconsole.h"

#include "resource.h"

#include "../../studio/api.h"
#include "../../bfc/svc_enum.h"
//#include "../../bfc/bitmap.h"

// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"

#include "callbackwnd.h"

WACNAME wac;
WACPARENT *the = &wac;

// ExampleCoreCallback GUID
// {18B318A1-849A-46a1-A9F4-DD73F7436B6C}
static const GUID guid = 
{ 0x18b318a1, 0x849a, 0x46a1, { 0xa9, 0xf4, 0xdd, 0x73, 0xf7, 0x43, 0x6b, 0x6c } };


WACNAME::WACNAME() : WACPARENT("Example Core Callback Display") {
	// In your component's constructor, you want to register all of the services
	// and attributes you will use.  Some of the basic services are:

  // thinger icon
  registerService(new WndCreateCreatorSingle< CreateBucketItem<CallbackWnd> >);

  // main menu entry
  registerAutoPopup(getGUID(), getName());

  // main window
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<CallbackWnd> >);
}

WACNAME::~WACConsole() {
}

GUID WACNAME::getGUID() {
  return guid;
}

void WACNAME::onCreate() {
  WACPARENT::onCreate();
}

void WACNAME::onDestroy() {
  WACPARENT::onDestroy();
}