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

#include "xuiwac.h"
#include "myobj.h"
#include "../../bfc/blending.h"

#include "resource.h"
#include "../../studio/api.h"

static WACXuiObject xuiobject;
WAComponentClient *the = &xuiobject;
WACXuiObject *xuiobjectwac = &xuiobject;

// This is your component GUID, you should generate a new one using guidgen.exe
// before publishing your component.

// {7BCB4F93-842A-4ab4-A188-523E15774028}
static const GUID guid = 
{ 0x7bcb4f93, 0x842a, 0x4ab4, { 0xa1, 0x88, 0x52, 0x3e, 0x15, 0x77, 0x40, 0x28 } };

#include "../../studio/services/creators.h"	

WACXuiObject::WACXuiObject() {
  registerService(new XuiObjectCreator<MyClassXuiSvc>);
  // If you have some implied elements (colors, bitmaps, fonts), you need to register
  // your default xml resource files here, preferably one for each of your xui object
  //registerSkinFile("Wacs/xml/yourwac/yourobject/yourobject.xml"); 
}

GUID WACXuiObject::getGUID() {
  return guid;
}

void WACXuiObject::onCreate() {
  Blenders::init();
}

void WACXuiObject::onDestroy() {
// be sure to delete all your windows etc HERE, not in the destructor
// because the API pointer might be invalid in the destructor
}

