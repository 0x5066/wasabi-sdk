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

#include "pathpicker.h"
#include "../../common/script/guiobject.h"
#include "../../common/script/scriptguid.h"
#include "../../common/script/c_script/c_text.h"
#include "../../studio/api.h"
#include "../../common/selectfile.h"

#define PATHPICKER_MAIN_GROUP "wasabi.pathpicker.main.group"
#define PATHPICKER_BUTTON "pathpicker.button"
#define PATHPICKER_TEXT "pathpicker.text"

// -----------------------------------------------------------------------
PathPicker::PathPicker() {
  abstract_setAllowDeferredContent(1);
  abstract_setContent(PATHPICKER_MAIN_GROUP);
  clicks_button = NULL;
  disable_cfg_event = 0;
}

PathPicker::~PathPicker() { 
}

int PathPicker::onInit() {
  int rt = PATHPICKER_PARENT::onInit();
  return rt;
}

void PathPicker::abstract_onNewContent() {
  PATHPICKER_PARENT::abstract_onNewContent();
  trapControls();
  updatePathInControl();
}  

int PathPicker::onReloadConfig() {
  int r = PATHPICKER_PARENT::onReloadConfig();
  disable_cfg_event = 1;
  updatePathFromConfig(); // triggers onSelect
  disable_cfg_event = 0;
  return r;
}

void PathPicker::updatePathFromConfig() { 
  const char *val = getGuiObject()->guiobject_getCfgString();
  const char *old = getPath();
  if (old && val && STRCASEEQL(val, old)) return;

  setPath(val);
}

void PathPicker::trapControls() {
  delete clicks_button;
  clicks_button = NULL;

  GuiObject *butGuiObj = getGuiObject()->guiobject_findObject(PATHPICKER_BUTTON);
  if (butGuiObj) clicks_button = new PPClicksCallback(*butGuiObj, this);
}

void PathPicker::clickCallback() {
	SelectFile sf(this,0,0);
	sf.setDefaultDir(getPath());
	if (sf.runSelector("directory",0,0)) {
    String p = sf.getDirectory();
    if (p[STRLEN(p-1)] != '/' && p[STRLEN(p-1)] != '\\')
      p += "\\";
    setPath(p);
	}
}

void PathPicker::setDefault() {
  onReloadConfig();
}

void PathPicker::setPath(const char *path) {
  if (STRCASEEQLSAFE(curpath, path)) return;
  curpath = path;
  onPathChanged(path);
}

void PathPicker::onPathChanged(const char *newpath) {
  updatePathInControl();
  if (!disable_cfg_event) {
    if (newpath == NULL)
      getGuiObject()->guiobject_setCfgString("");
    else
      getGuiObject()->guiobject_setCfgString(newpath);
  }
}

void PathPicker::updatePathInControl() {
  GuiObject *content = getContent();
  if (content != NULL) {
    GuiObject *text = content->guiobject_findObject(PATHPICKER_TEXT);
    if (text != NULL) {
      C_Text t(*text);
      if (curpath.isempty())
        t.setText("");
      else
        t.setText(curpath);
    }
  }
}

