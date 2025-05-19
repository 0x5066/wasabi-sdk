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

#include "mainwnd.h" 
#include "exampleexternelement.h" 
#include "resource.h"
#include "../../studio/api.h" 
#include "../../bfc/canvas.h" 
#include "../../bfc/bitmap.h" 
#include "../../bfc/wnds/buttwnd.h" 
#include "../../bfc/msgbox.h"
#include "../../common/script/c_script/c_guiobject.h"
#include "../../common/script/c_script/c_group.h"
#include "../../common/script/scriptguid.h"

// this window simply instantiates an xml group called 'eee.main.window'
// and places it in its client area

MainWnd::MainWnd() {
  groupwnd = NULL;
  group = NULL;
  obj = NULL;

  // The setName call sets what the titlebar of your window will show.
  setName("XML Based Content");
}

MainWnd::~MainWnd() {
  if (groupwnd) api->group_destroy(groupwnd); 
  hooks.deleteAll();
  delete obj; obj = NULL;
  delete group; group = NULL;
}

int MainWnd::onInit() {
  MAINWND_PARENT::onInit();
  createGroup();
  return 1;
}

void MainWnd::createGroup() {
  ASSERT(groupwnd == NULL);
  setContent("eee.main.window");
  groupwnd = getContentRootWnd();
  if (!groupwnd) return;
  hookControls();
}

void MainWnd::hookControls() {

  hooks.deleteAll();
  delete group; group = NULL;
  delete obj; obj = NULL;

  // Get the ScriptObject pointer out of the RootWnd pointer
  ScriptObject *_group = reinterpret_cast<ScriptObject *>(groupwnd->getInterface(scriptObjectGuid));

  if (!_group) {
    api->messageBox("Group \"eee.main.window\" not found, can't continue", "ExternElement Component Example", MSGBOX_OK, NULL, NULL);
    return;
  }

  // the skinned group is now created, lets wrap a helper around it to get our layer
  group = new C_Group(_group);
  ScriptObject *layer = group->getObject("eee.layer1");

  if (!layer) {
    api->messageBox("Could not find layer \"eee.layer1\" in group \"eee.main.window\", can't continue", "ExternElement Component Example", MSGBOX_OK, NULL, NULL);
    delete group; group = NULL;
    return;
  }

  // ok we have the layer, we need a helper around it too so we can...
  obj = new C_GuiObject(layer);

  // ... talk to it
  obj->setAlpha(128);
  
  // now lets monitor some objects events
  hooks.addItem(new MyHooks(obj->getScriptObject(), this));
  hooks.addItem(new MyHooks(group->getScriptObject(), this));
}

C_GuiObject *MainWnd::getLayer() {
  return obj;
}

C_Group *MainWnd::getGroup() {
  return group;
}

//  Methods required by Window Creation Services
const char *MainWnd::getWindowTypeName() { return /**/"ExampleExternElement Window"/*EDITME*/; }
GUID MainWnd::getWindowTypeGuid() { return the->getGUID(); }
void MainWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}


