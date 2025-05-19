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

//
// Always start with std.h
#include "../../bfc/std.h"
//
#include "exampleoswnd.h"     //EDITME
#include "resource.h"
#include "exampleoswndwnd.h"  //EDITME
// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"


static WACNAME wac;
WACPARENT *the = &wac;                     

//  *** This is MY GUID.  Get your own!
// {208887C7-69C9-40d2-8A14-3ACB1F02E950}
static const GUID guid = 
{ 0x208887c7, 0x69c9, 0x40d2, { 0x8a, 0x14, 0x3a, 0xcb, 0x1f, 0x2, 0xe9, 0x50 } };



WACNAME::WACNAME() : WACPARENT("ExampleOSWnd Component") {
  // So, what did we want to do again?  Oh yes...

  // 1) we are going to make a window,
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<ExampleOSWndWnd> >);

  // 2) and have it be listed in the Thinger,
  registerService(new WndCreateCreatorSingle< CreateBucketItem<ExampleOSWndWnd> >);

  // 3) and in the main context menu.
  registerAutoPopup(getGUID(), getName());
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return guid;
}

void WACNAME::onCreate() {
  // *** Do startup stuff here that doesn't require you to have a window yet
}

void WACNAME::onDestroy() {
}

