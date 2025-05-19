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
//    NULLSOFT WASABI SDK EXAMPLE PROJECTS
//
//      File:     examplecanvas.cpp
//
//!##   Purpose:  Setup the ExampleCanvas Component class for our example component
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.  Deal.
//
//


// ===========================================================================
//
//  *** EXAMPLECANVAS: The Beginning
//
//  For those who remember the pre-launch SDK, all the Canvas tricks from
//  the old Example1 are moved to here, ExampleCanvas.   Example1 pretty
//  much ONLY shows you how to setup your component object and get whatever
//  you need registered.
//
//  GenericWACTemplate is still a much more suitable project to clone for yourself.  :)
//  

//
// Always start with std.h
#include "../../bfc/std.h"
//
#include "examplecanvas.h"     //EDITME
#include "resource.h"
#include "examplecanvaswnd.h"  //EDITME
//
#include "../../common/xlatstr.h"
// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"


static WACNAME wac;
WACPARENT *the = &wac;                     

// ===========================================================================
//
//  EXAMPLECANVAS: The GUID
//
//  *** You MUST use a unique GUID for your WAC files.
//  
//  *** This is MY GUID.  Get your own.   
// {353AF806-70EC-44db-ABAF-4BA73503DCA9}
static const GUID guid =  //EDITME (hint: use guidgen.exe)
{ 0x353af806, 0x70ec, 0x44db, { 0xab, 0xaf, 0x4b, 0xa7, 0x35, 0x3, 0xdc, 0xa9 } };

// ===========================================================================
//
//  EXAMPLECANVAS: ExampleCanvas() -- The Component Constructor
//
WACNAME::WACNAME() : WACPARENT(/**/"ExampleCanvas Component"/*EDITME*/) {

  // So, what did we want to do again?  Oh yes...

  // 1) we are going to make a window,
  registerService(new WndCreateCreatorSingle< CreateWndByGuid</**/ExampleCanvasWnd/*EDITME*/ > >);

  // 2) and have it be listed in the Thinger,
  registerService(new WndCreateCreatorSingle< CreateBucketItem</**/ExampleCanvasWnd/*EDITME*/ > >);

  // 3) and in the main context menu.
  registerAutoPopup(getGUID(), getName());
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return guid;
}

