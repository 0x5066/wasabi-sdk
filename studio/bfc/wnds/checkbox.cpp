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

#include "checkbox.h"
#include "../../common/script/c_script/c_text.h"
#include "../../common/script/c_script/c_button.h"
#include "../../common/script/guiobject.h"

// -----------------------------------------------------------------------
CheckBox::CheckBox(const char *_text, const char *_radioid) :
    text(_text), radioid(_radioid), textclicks(NULL), 
    toggleclicks(NULL), use_radioval(0), buttonGuiObj(NULL),
    CHECKBOX_PARENT() { }

CheckBox::~CheckBox() { 
  delete textclicks; 
  delete toggleclicks; 
}

int CheckBox::onInit() {
  int r = CHECKBOX_PARENT::onInit();
  if (radioid.len()) {
    abstract_setContent("wasabi.radiobutton.group", 1);
    // After we set the content for radio, we should register ourselves
    // as a radio button under the radio group we are assigned.
    GuiObject *radioGuiObj = abstract_findObject(radioid); // this will actually go down a level
    if (radioGuiObj != NULL) {
      RootWnd *radioRootWnd = *radioGuiObj; // explicit cast operator :)
      // Now, once we have the rootwnd, we can send our radio group object a message, it will turn off other items as we turn on specific ones
      if (radioRootWnd != NULL) {
        sendAction(radioRootWnd, "REGISTER", NULL, -1, -1, 0, 0, (void *)getGuiObject(), 4);
      }
    }
  } else {
    abstract_setContent("wasabi.checkbox.group", 1);
  }
  return r;
}

void CheckBox::onNewContent() {
  // remove all previous hooks because our content changed
  delete toggleclicks;
  toggleclicks = NULL;

  delete textclicks;
  textclicks = NULL;

  // Capture the clicks on the button
  buttonGuiObj = abstract_findObject("checkbox.toggle");
  if (buttonGuiObj != NULL) {
    toggleclicks = new ToggleClicks(*buttonGuiObj, this);  
  }

  // Capture the clicks on the text
  GuiObject *textGuiObj = abstract_findObject("checkbox.text");
  if (textGuiObj != NULL) {
    textclicks = new TextClicks(*textGuiObj, this);  
  }

  // Set the text object to display the requested text.
  updateText();
}  

int CheckBox::getPreferences(int what) {
  RootWnd *w = abstract_getContentRootWnd();
  if (w != NULL)
    return w->getPreferences(what);
  return CHECKBOX_PARENT::getPreferences(what);
}

void CheckBox::setActivated(int activated, int writetocfg) {
  // this is usually called as a response to onReloadConfig, but could also be called
  // directly by a 3rd party, so if we can, we read the current value and do nothing if
  // it hasn't changed. 
  GuiObject *toggleGuiObj = abstract_findObject("checkbox.toggle");
  if (toggleGuiObj != NULL) {
    C_Button buttonObj(*toggleGuiObj);
    int act = buttonObj.getActivated();
    if (!!act == !!activated) return;
    buttonObj.setActivatedNoCallback(!!activated);
  }
  if (writetocfg) {
    int newVal = activated;
    if (use_radioval) {
      if (activated) getGuiObject()->guiobject_setCfgString(radioval);
    } else {
      getGuiObject()->guiobject_setCfgInt(activated);
    }
  }
}

// This is called by the click catchers
void CheckBox::toggle(int self_switch) {

  if (!buttonGuiObj) return;

  int activated = !!isActivated();
  if (self_switch) activated = !!!activated;

/*  int no_setactive = self_switch;

  if (radioid.len() > 0 && activated) { // but if we're a radiobox, do not toggle if we're already activated
    no_setactive = 1;
  }

  if (self_switch) activated = !!!activated;
*/

  if (!(activated && !radioid.isempty()))
    activated = !activated;

  if (!use_radioval || activated) {
    C_Button b(*buttonGuiObj);
    b.setActivatedNoCallback(activated);
  }

  if (activated) {
    if (use_radioval)
      getGuiObject()->guiobject_setCfgString(radioval);
    else
      getGuiObject()->guiobject_setCfgInt(activated);
  } else 
   if (radioid.len() == 0) getGuiObject()->guiobject_setCfgInt(0); // if we're a radioid being turned off, we shouldn't write our value, as we'll prolly be sharing the same cfgattr with other radioboxes, todo: make that optional

  if (radioid.len() > 0 && activated) {
    GuiObject *radioGuiObj = abstract_findObject(radioid);
    RootWnd *radioRootWnd = *radioGuiObj;
    if (radioRootWnd != NULL) 
      sendAction(radioRootWnd, "TOGGLE", NULL, -1, -1, 0, 0, (void *)getGuiObject(), 4);
  }
}

void CheckBox::setText(const char *_text) {
  text = _text;
  if (isInited()) {
    updateText();
  }
}

void CheckBox::setRadioid(const char *_radioid) {
  radioid = _radioid;
}

void CheckBox::setRadioVal(const char *val, int _use_radioval) {
  radioval = val;
  use_radioval = _use_radioval;
}

int CheckBox::onReloadConfig() {
  String newVal = getGuiObject()->guiobject_getCfgString();
  int checkit = use_radioval ? (newVal == radioval) : ATOI(newVal);
  setActivated(checkit, 0);
  return CHECKBOX_PARENT::onReloadConfig();
}

void CheckBox::updateText() {
  GuiObject *textGuiObj = abstract_findObject("checkbox.text");
  if (textGuiObj != NULL) {
    textGuiObj->guiobject_setXmlParam("text", text);
  }
}
int CheckBox::isActivated() {
  if (!buttonGuiObj) return 0;
  C_Button b(*buttonGuiObj);
  return b.getActivated();
}
