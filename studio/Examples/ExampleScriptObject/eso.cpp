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


#include "eso.h"
#include "esosvc.h"

#include "resource.h"
#include "../../studio/api.h"

static WACExampleScriptObject eso;
WAComponentClient *the = &eso;
WACExampleScriptObject *esowac = &eso;

// this is the WAC guid, not the script object, look in esosvc.h for scriptobject related stuff
// {C7C66786-5D19-4c38-8A7A-E5DE4B7BF604} // change this for your own component, using GUIDGEN (it comes with your VC++ install)
static const GUID guid = 
{ 0xc7c66786, 0x5d19, 0x4c38, { 0x8a, 0x7a, 0xe5, 0xde, 0x4b, 0x7b, 0xf6, 0x4 } };

#include "../../studio/services/creators.h"	

WACExampleScriptObject::WACExampleScriptObject() {
  registerService(new ScriptObjectCreator<ExampleScriptObjectSvc>);
}

GUID WACExampleScriptObject::getGUID() {
  return guid;
}

void WACExampleScriptObject::onCreate() {
}

void WACExampleScriptObject::onDestroy() {
// be sure to delete all your windows etc HERE, not in the destructor
// because the API pointer might be invalid in the destructor
}

