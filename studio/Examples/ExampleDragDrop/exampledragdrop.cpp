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

//=====================================
//  Project Includes
//=====================================
#include "exampledragdrop.h"      //WACexampledragdrop class.
#include "exampledragdropwnd.h"   //ExampleDragDropWnd, DropWnd, DropWndItem class.

//=====================================
//  Wasabi Includes
//=====================================
#include "../../studio/services/creators.h" //WndCreateCreatorSingle, CreateWndByGuid class.
#include "../../bfc/wndcreator.h"           //WndCreateCreatorSingle, CreateWndByGuid class.

//=====================================
//  Globals
//=====================================
WACNAME wacexampledragdrop;           //Required!!!
WACPARENT *the = &wacexampledragdrop; //Required!!!

// {DF44CEC8-37D2-472c-BC10-86A302102309} //Our components GUID.
static const GUID exampledragdrop_guid = 
{ 0xdf44cec8, 0x37d2, 0x472c, { 0xbc, 0x10, 0x86, 0xa3, 0x2, 0x10, 0x23, 0x9 } };


WACNAME::WACNAME() : WACPARENT("Example Drag And Drop Component") {
  //Register a Window Creation Service for our ExampleDragDropWnd (our main window).
  registerService(new WndCreateCreatorSingle< CreateWndByGuid<ExampleDragDropWnd> >);
  //Register our window for Auto-Pop (so we're in the window list).
  registerAutoPopup(ExampleDragDropWnd::getWindowTypeGuid(), ExampleDragDropWnd::getWindowTypeName());
  
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
  return exampledragdrop_guid;  //Return our components guid.
}