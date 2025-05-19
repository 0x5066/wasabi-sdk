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
#include "examplevisdata.h"     //EDITME
#include "resource.h"
#include "examplevisdatawnd.h"  //EDITME
//
#include "../../common/xlatstr.h"
// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"


static WACNAME wac;
WACPARENT *the = &wac;                     

//  *** This is MY GUID.  Get your own!
// {2CF2ED18-08EC-4df5-8D28-5BED227E21B2}
static const GUID guid =  //EDITME (hint: use guidgen.exe)
{ 0x2cf2ed18, 0x8ec, 0x4df5, { 0x8d, 0x28, 0x5b, 0xed, 0x22, 0x7e, 0x21, 0xb2 } };



WACNAME::WACNAME() : WACPARENT(/**/"ExampleVisData Component"/*EDITME*/) {
  // So, what did we want to do again?  Oh yes...

  // 1) we are going to make a window,
  registerService(new WndCreateCreatorSingle< CreateWndByGuid</**/ExampleVisDataWnd/*EDITME*/> >);

  // 2) and have it be listed in the Thinger,
  registerService(new WndCreateCreatorSingle< CreateBucketItem</**/ExampleVisDataWnd/*EDITME*/> >);

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

