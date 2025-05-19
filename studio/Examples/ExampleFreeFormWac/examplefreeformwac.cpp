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

#include "examplefreeformwac.h"
#include "mainwnd.h"

#include "../../studio/api.h"

// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"

static FreeFormComponent FFC;
WAComponentClient *the = &FFC;
FreeFormComponent *wacffc = &FFC;

// {1BA8887F-26AE-4813-A519-81AE7543927A}
static const GUID guid = 
{ 0x1ba8887f, 0x26ae, 0x4813, { 0xa5, 0x19, 0x81, 0xae, 0x75, 0x43, 0x92, 0x7a } };

FreeFormComponent::FreeFormComponent() : WAComponentClient(/**/"ExampleFreeFormWac"/*EDITME*/) {
  // 1) we are going to make a window,
  registerService(new WndCreateCreatorSingle< CreateWndByGuid</**/MainWnd/*EDITME*/> >);

  // 2) and have it be listed in the Thinger,
  registerService(new WndCreateCreatorSingle< CreateBucketItem</**/MainWnd/*EDITME*/> >);

  // 3) and in the main context menu.
  registerAutoPopup(getGUID(), getName());

  // 4) and THIS guy is special.  He uses a skinfile.
  registerSkinFile(/**/"wacs/xml/FreeFormComponentExample/component_skin.xml"/*EDITME*/);
}

GUID FreeFormComponent::getGUID() {
  return guid;
}
