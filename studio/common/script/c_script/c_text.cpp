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

/* This file was generated by Maki Compiler, do not edit manually */

#include "../../../studio/api.h"
#include "c_text.h"
#include "../objcontroller.h"

C_Text::C_Text(ScriptObject *object) : C_GuiObject(object) {
  inited = 0;
  C_hook(object);
}

C_Text::C_Text() {
  inited = 0;
}

void C_Text::C_hook(ScriptObject *object) {
  ASSERT(!inited);
  ScriptObjectController *controller = object->vcpu_getController();
  obj = controller->cast(object, textGuid);
  if (obj != object && obj != NULL)
    controller = obj->vcpu_getController();
  else
    obj = NULL;

  if (!loaded) {
    loaded = 1;
    settext_id = api->maki_addDlfRef(controller, "setText", this);
    setalternatetext_id = api->maki_addDlfRef(controller, "setAlternateText", this);
    gettext_id = api->maki_addDlfRef(controller, "getText", this);
    gettextwidth_id = api->maki_addDlfRef(controller, "getTextWidth", this);
    ontextchanged_id = api->maki_addDlfRef(controller, "onTextChanged", this);
  }
  inited = 1;
}

C_Text::~C_Text() {
}

ScriptObject *C_Text::getScriptObject() {
  if (obj != NULL) return obj;
  return C_TEXT_PARENT::getScriptObject();
}

void C_Text::setText(const char *txt) {
  ASSERT(inited);
  scriptVar a = MAKE_SCRIPT_STRING(txt);
  scriptVar *params[1] = {&a};
  api->maki_callFunction(getScriptObject(), settext_id, params);
}

void C_Text::setAlternateText(const char *txt) {
  ASSERT(inited);
  scriptVar a = MAKE_SCRIPT_STRING(txt);
  scriptVar *params[1] = {&a};
  api->maki_callFunction(getScriptObject(), setalternatetext_id, params);
}

const char *C_Text::getText() {
  ASSERT(inited);
  return GET_SCRIPT_STRING(api->maki_callFunction(getScriptObject(), gettext_id, NULL));
}

int C_Text::getTextWidth() {
  ASSERT(inited);
  return GET_SCRIPT_INT(api->maki_callFunction(getScriptObject(), gettextwidth_id, NULL));
}

void C_Text::onTextChanged(const char *newtxt) {
  ASSERT(inited);
  scriptVar a = MAKE_SCRIPT_STRING(newtxt);
  scriptVar *params[1] = {&a};
  api->maki_callFunction(getScriptObject(), ontextchanged_id, params);
}

int C_Text::loaded=0;
int C_Text::settext_id=0;
int C_Text::setalternatetext_id=0;
int C_Text::gettext_id=0;
int C_Text::gettextwidth_id=0;
int C_Text::ontextchanged_id=0;
