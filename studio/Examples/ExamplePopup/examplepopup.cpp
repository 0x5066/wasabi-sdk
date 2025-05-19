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

// ===========================================================================
//
//    NULLSOFT WASABI SDK EXAMPLE PROJECT
//
//      File:     ExamplePopup.cpp
//
//!##   Purpose:  This source module that contains the functions for the 
//!##             construction and utilization of the XML Document Display
//!##             sheet of the ExamplePopup project.
//
//      Requires: Please read Example1.cpp first.
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//

#include "../../bfc/std.h"  // Always start with std.h
#include "../../common/xlatstr.h"

#include "examplepopup.h"       //EDITME
#include "examplepopupwnd.h"    //EDITME
#include "resource.h"

// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"

static WACNAME wac;
WACPARENT *the = &wac;                     

// ===========================================================================
//
//  *** You MUST use a unique GUID for your WAC files.
//
// {99CAD29A-8819-4cf9-944E-B96A329EA1A0}
GUID expopup_guid = 
{ 0x99cad29a, 0x8819, 0x4cf9, { 0x94, 0x4e, 0xb9, 0x6a, 0x32, 0x9e, 0xa1, 0xa0 } };
//EDITME (hint: use guidgen.exe)

WACNAME::WACNAME() : WACPARENT(/**/"ExamplePopup Component"/*EDITME*/) {
	// In your component's constructor, you want to register all of the services
	// and attributes you will use.  Some of the basic services are:

  // thinger icon
  registerService(new WndCreateCreatorSingle< CreateBucketItem<ExamplePopupWnd> >);

  // main menu entry
  registerAutoPopup(getGUID(), getName());

  // main window
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<ExamplePopupWnd> >);
}

WACNAME::~WACNAME() {
}

// ===========================================================================
//
GUID WACNAME::getGUID() {
  return expopup_guid;
}

void WACNAME::onCreate() {
  WACPARENT::onCreate();
}

void WACNAME::onDestroy() {
  WACPARENT::onDestroy();
}
