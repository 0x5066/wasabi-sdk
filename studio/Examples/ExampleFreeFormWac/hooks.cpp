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

#include "hooks.h"
#include "examplefreeformwac.h"
#include "../../bfc/msgbox.h"
#include "../../common/script/objcontroller.h"

// Hook onLeftButtonDown for all guiobject wrapped around this class
void MyHooks::hook_onLeftButtonDown(int x, int y) {
  if (getHookedObject() == mainwnd->getLayer()->getScriptObject()) { // if the layer was clicked
    mainwnd->getLayer()->setAlpha(rand()%255);                       // then randomly change its alpha
    return;
  }
}

// Hook onResize for all guiobject wrapped around this class
void MyHooks::hook_onResize(int x, int y, int w, int h) {
  if (getHookedObject() == mainwnd->getGroup()->getScriptObject()) { // if the group was resized
                                                                    // then lets recenter the layer
    C_Group *g = mainwnd->getGroup();
    C_GuiObject *o = mainwnd->getLayer();

    int x = g->getLeft();
    int y = g->getTop();
    int gw = g->getWidth();
    int gh = g->getHeight();
    int lw = o->getWidth();
    int lh = o->getHeight();

    o->resize(x+(gw-lw)/2, y+(gh-lh)/2, lw, lh);

    return;
  }
}

