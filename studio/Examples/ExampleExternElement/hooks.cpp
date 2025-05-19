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
#include "exampleexternelement.h"
#include "../../bfc/msgbox.h"
#include "../../common/script/objcontroller.h"
#include "../../common/selectfile.h"

// Hook onLeftButtonDown for all guiobject wrapped around this class
void MyHooks::hook_onLeftButtonDown(int x, int y) {
  if (getHookedObject() == mainwnd->getLayer()->getScriptObject()) { // if the layer was clicked
    setExternElement();
    // Hopefully, this will work soon.  But not at the moment.
    mainwnd->setName("Changed Name!");
    // force invalidation after the name change
    mainwnd->getDesktopParent()->invalidate();
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

void MyHooks::setExternElement() {
  //
  //  Want some cut and paste code to "go grab some files from the OS, please" ??
  //
  String filepath;
  String extlist="PNGs (*.png)|*.png|JPGs (*.jpg)|*.jpg|XML Snippets (*.xml)|*.xmljpg|All files (*.*)|*.*|";
  extlist.changeChar('|', '\0');
  SelectFile sf(api->main_getRootWnd());
  sf.setIdent("ExternElement");
  sf.runSelector("files", FALSE, extlist);
  if (sf.getNumFiles() > 0) {
    filepath = sf.enumFilename(0);
  }

  //
  //  Okay, then if we have a real file.
  //
  if (filepath.len()) {
    // if we've already loaded a skinpart, give it back.
    if (currentSkinpartId != -1) {
      // change back to the old logo, to free the use of the element in our skinpart,
      mainwnd->getLayer()->setXmlParam("image","eee.logo"); 
      api->unloadSkinPart(currentSkinpartId);    // right now, this is crashy!  poo poo!
      currentSkinpartId = -1;
    }
    String buffer = filepath;
    String ext = buffer.rSpliceChar('.');
    // If we didn't pick an XML file,
    if (ext != "xml") {
      // We create a new buffer of XML to feed in as the eee.newlogo element.
      buffer = String("buf:<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><WinampAbstractionLayer version=\"0.6\">") +
                "<elements><bitmap id=\"eee.newlogo\" file=\"" + filepath + "\"/></elements></WinampAbstractionLayer>";

      // So check this slick trick out -- you can now (as of 482, I hope) use a filename of "buf:...." for
      // anything that takes an XML file and feed it a buffer version of whatever XML you want to create.

      // A very simplified way of covering alot of dynamic content without resorting to a provider service.

    } else {
      // Otherwise, it's just the name of the xml file to load and hope it has the element created.
      buffer = filepath;
    }
    currentSkinpartId = api->loadSkinFile(buffer);
  }

  mainwnd->getLayer()->setXmlParam("image","eee.newlogo"); // then change to the "new" logo.
}
