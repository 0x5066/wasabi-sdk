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
//      File:     ExampleAttrib.cpp
//
//!##   Purpose:  This source module that contains the functions for the 
//!##             construction and utilization of the XML Document Display
//!##             sheet of the ExampleAttrib project.
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

#include "ExampleAttrib.h"       //EDITME
#include "ExampleAttribWnd.h"    //EDITME
#include "ExampleAttribHandler.h"//EDITME

// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"

static WACNAME wac;
WACPARENT *the = &wac;                     

// ===========================================================================
//
//  *** You MUST use a unique GUID for your WAC files.
//
// {7D64B092-DA4F-48bc-8051-4F71967DA593}
GUID exattrib_guid = 
{ 0x7d64b092, 0xda4f, 0x48bc, { 0x80, 0x51, 0x4f, 0x71, 0x96, 0x7d, 0xa5, 0x93 } };
//EDITME (hint: use guidgen.exe)

// ===========================================================================
//
//    Attribute Handler.  Ya gotta have it somewhere.
//    You _must_ allocate it, because the system will delete it.
//
ExampleAttribHandler *attr = NULL;
// *** EVEN THOUGH WE ALLOCATE THE ATTRIB HANDLER, WE DO NOT DELETE IT.
// IT WILL BE DELETED FOR US (which means don't make it a static global, heh)

WACNAME::WACNAME() : WACPARENT(/**/"ExampleAttrib Component"/*EDITME*/) {
	// In your component's constructor, you want to register all of the services
	// and attributes you will use.  Some of the basic services are:

  // thinger icon
  registerService(new WndCreateCreatorSingle< CreateBucketItem<ExampleAttribWnd> >);

  // main menu entry
  registerAutoPopup(getGUID(), getName());

  // main window
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<ExampleAttribWnd> >);

  // But guess what!  We're not done yet!
  // In order to have attributes, you have to register them with 
  // the engine similarily as you do for your window creation.  
  // Here's the attrib registration code:

  // *** EVEN THOUGH WE ALLOCATE THE ATTRIB HANDLER, WE DO NOT DELETE IT.
  // IT WILL BE DELETED FOR US (which means don't make it a static global, heh)
  if (attr == NULL) { // it better be,
    attr = new ExampleAttribHandler;
  }
  // To register your attributes, just bind them through the AttribHandler.
  attr->bindComponent(this);
  attr->registerAttribute(&attr->intAttr, ExampleAttribWnd::INT);
  attr->registerAttribute(&attr->boolAttr, ExampleAttribWnd::BOOL);
  attr->registerAttribute(&attr->floatAttr, ExampleAttribWnd::FLOAT);
  attr->registerAttribute(&attr->stringAttr, ExampleAttribWnd::STRING);
  // Because our attrib handler will be calling back to our window, I defined
  // the ID's in the window object.  Again, it doesn't matter, they could also
  // be defined in the handler.
}

WACNAME::~WACNAME() {
  // *** VERY IMPORTANT!!!

  // EVEN THOUGH WE ALLOCATE THE ATTRIB HANDLER, WE DO NOT DELETE IT.
}

GUID WACNAME::getGUID() {
  return exattrib_guid;
}

void WACNAME::onCreate() {
}

void WACNAME::onDestroy() {
}
