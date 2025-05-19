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

#include "mainminibrowser.h"
#include "../studio/api.h"
#include "../common/script/scriptguid.h"
#include "../common/script/guiobject.h"

ScriptObject *MainMiniBrowser::getScriptObject() {
  return api->maki_getObjectAtom("browser.main.object");
}

void MainMiniBrowser::back() {
  ScriptObject *so = getScriptObject();
  if (so) {
    C_Browser browser(so);
    browser.back();
  }
}

void MainMiniBrowser::forward(){
  ScriptObject *so = getScriptObject();
  if (so) {
    C_Browser browser(so);
    browser.forward();
  }
}

void MainMiniBrowser::refresh(){
  ScriptObject *so = getScriptObject();
  if (so) {
    C_Browser browser(so);
    browser.refresh();
  }
}

void MainMiniBrowser::stop(){
  ScriptObject *so = getScriptObject();
  if (so) {
    C_Browser browser(so);
    browser.stop();
  }
}

void MainMiniBrowser::home(){
  ScriptObject *so = getScriptObject();
  if (so) {
    C_Browser browser(so);
    browser.home();
  }
}

void MainMiniBrowser::navigateUrl(const char *url){
  ScriptObject *so = getScriptObject();
  if (so) {
    C_Browser browser(so);
    browser.navigateUrl(url);
  }
}

void MainMiniBrowser::popMb(){
  ScriptObject *so = getScriptObject();
  if (so) {
    GuiObject *go = static_cast<GuiObject*>(so->vcpu_getInterface(guiObjectGuid));
    if (go) {
      go->guiobject_popParentLayout();
    }
  }
}

