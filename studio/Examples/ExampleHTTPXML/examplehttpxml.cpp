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
//      File:     ExampleHTTPXML.cpp
//
//!##   Purpose:  This source module that contains the functions for the 
//!##             construction and utilization of the XML Document Display
//!##             sheet of the ExampleHTTPXML project.
//
//      Requires: Please read ExampleB.cpp first.
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//



//
//  GOOD MORNING, CAMPERS!
//

//
//  For ExampleHTTPXML, we're still gonna just ignore most of this file 
//  and play with the ExampleHTTPXMLWnd.cpp file (for the most part).
//
//  As before, there are a handful of things you should know about
//  this file first.  Most specifically, you gotta edit all the
//  "//EDITME" lines.  I'll tag out the info as to what all this
//  stuff does inline with the code there.
//

//
//  Headers, Headers, Headers, Headers.
#include "../../bfc/std.h"  // Always start with std.h
#include "../../common/xlatstr.h"

#include "ExampleHTTPXML.h"     //EDITME
#include "ExampleHTTPXMLWnd.h"  //EDITME

// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"

// ===========================================================================
//
//  Amusing static instantiations.  Your component should live as a static
// data object in your primary module (well, phooey, you can put the damn
// thing anywhere -- just make sure you make one)
static WACNAME wac;
WACPARENT *the = &wac;                     


// ===========================================================================
//
//  *** You MUST use a unique GUID for your WAC files.
//
//  Making it be a local data item makes it easy to centralize.
//  You have to return this value from the Component's getGuid() method.
//
//  Trust me.  This one isn't optional.  Do it, and do it NOW.
// {A2C3EE30-AEB3-45e5-99C5-F71BE8143A22}
GUID EXHTTPXML_guid = 
{ 0xa2c3ee30, 0xaeb3, 0x45e5, { 0x99, 0xc5, 0xf7, 0x1b, 0xe8, 0x14, 0x3a, 0x22 } };
//EDITME (hint: use guidgen.exe)


WACNAME::WACNAME() : WACPARENT(/**/"ExampleHTTPXML Component"/*EDITME*/) {
	// In your component's constructor, you want to register all of the services
	// and attributes you will use.  Some of the basic services are:

  // thinger icon
  registerService(new WndCreateCreatorSingle< CreateBucketItem<ExampleHTTPXMLWnd> >);

  // main menu entry
  registerAutoPopup(getGUID(), getName());

  // main window
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<ExampleHTTPXMLWnd> >);
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return EXHTTPXML_guid;
}

void WACNAME::onCreate() {
  // *** Do startup stuff here that doesn't require you to have a window yet
}

void WACNAME::onDestroy() {
}