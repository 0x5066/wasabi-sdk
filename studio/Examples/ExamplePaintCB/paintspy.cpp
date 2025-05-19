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

#include "paintspy.h"
#include "spywnd.h"
#include "../../bfc/blending.h"

#include "resource.h"
#include "../../studio/api.h"

static WACXuiObject xuiobject;
WAComponentClient *the = &xuiobject;
WACXuiObject *xuiobjectwac = &xuiobject;

// {13834B4C-3D51-440b-8C1F-0812274352D5}
static const GUID guid = 
{ 0x13834b4c, 0x3d51, 0x440b, { 0x8c, 0x1f, 0x8, 0x12, 0x27, 0x43, 0x52, 0xd5 } };

#include "../../studio/services/creators.h"	

WACXuiObject::WACXuiObject() {
  registerService(new XuiObjectCreator<SpyWndXuiSvc>);
  registerSkinFile("Wacs/xml/paintspy/paintspy.xml"); 
  registerAutoPopup("paintspy.main.group", "PaintSpy Example");
}

GUID WACXuiObject::getGUID() {
  return guid;
}
