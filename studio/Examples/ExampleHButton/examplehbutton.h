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

#ifndef _EXAMPLEHBUTTON_H
#define _EXAMPLEHBUTTON_H

#include "../../studio/wac.h"
#include "../../studio/skincb.h"
#include "../../common/script/c_script/h_button.h"

class ExampleHButtonHook;

// WE ARE A COMPONENT AND A SKIN CALLBACK!!  WOO HOO!!
class ExampleHButton : public WAComponentClient, public SkinCallbackI {
public:
  ExampleHButton();
  ~ExampleHButton();

  // Methods inherited from WAComponentClient
  virtual const char *getName() { return "Example CButton"; };
  virtual GUID getGUID();
  virtual void onCreate();

  // Methods inherited from SkinCallbackI
  virtual int skincb_onLoaded();

  // Methods which are all our own
  virtual void makeHook();

private:
  ExampleHButtonHook * hook;
};

// And we provide a class for our buttonhook.
class ExampleHButtonHook : public H_Button {
public:
  // Methods inherited from H_Button
  ExampleHButtonHook(ScriptObject *o) : H_Button(o) {}

  virtual void hook_onLeftClick();
};

extern WAComponentClient *the;

#endif //_EXAMPLEHBUTTON_H
