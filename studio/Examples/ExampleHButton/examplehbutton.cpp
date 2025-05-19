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

//!## How to hook a H_Button object when you have no context to it.

#include "examplehbutton.h"

#include "../../studio/api.h"

// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"

static ExampleHButton ECB;
WAComponentClient *the = &ECB;

// {FFCA7812-2D49-4075-9C40-9F623CB0366F}
static const GUID guid = 
{ 0xffca7812, 0x2d49, 0x4075, { 0x9c, 0x40, 0x9f, 0x62, 0x3c, 0xb0, 0x36, 0x6f } };

ExampleHButton::ExampleHButton() : WAComponentClient("ExampleHButton") {
  // Initialize member data;
  hook = NULL;

  // So, what we want to do is hook a button without having an actual context near the button.
  // In order to show you how to do that, I should put a button out in the system that we can
  // play with.  This is why we are registering this skin file.  This is NOT needed for anything
  // but to get that button out there to begin with, eh?
  registerSkinFile("wacs/xml/ExampleHButton/component_skin.xml");

  // Okay, after we do that, then we can get to the business of hooking the button.

  // Jump down to onCreate();
}

GUID ExampleHButton::getGUID() {
  return guid;
}

void ExampleHButton::onCreate() {  
  // So, all the skinpieces have been loaded when we get here.  So here is the 
  // first place where we know the button probably exists and we can hook it.
  makeHook();

  // The big issue we're going to have to deal with is making sure we get called at
  // the right point in the system to insert our hooks to a button in the skin if the
  // skin ever changes out from under us (because all the objects get thrown away then
  // recreated, so the ScriptObject pointer found in makeHook is no longer valid then.

  // This is done by registering a skin callback and overriding the virtual method for
  // onLoaded.  Luckily enough, our component object just happens to inherit from
  // SkinCallbackI too, so we can register ourselves as a skin callback. :)
  api->syscb_registerCallback(this);

  // This means, whenever things about the skin change, our virtual methods will get called.
  // We only care about getting told that a new skin has completed loading, skip to skincb_onLoaded();
}

ExampleHButton::~ExampleHButton() {
  // We're going to make it, we have to delete it.
  if (hook) {
    delete hook;
    hook = NULL;
  }
}

// So this gets called when a new skin has completed loading.
int ExampleHButton::skincb_onLoaded() {
  makeHook();
  return 1;
}

// Methods which are all our own
void ExampleHButton::makeHook() {
  // Get rid of whatever was there before.
  if (hook) {
    delete hook;
    hook = NULL;
  }

  // In here, we need to find our button (if possible).  To do this, we query the API
  // to find a script object for us.  We have to do this from a skin callback, because
  // this is when the ID's are available for maki_findObject to query through.
  ScriptObject *ourButtonScriptObj = api->maki_findObject("TheIDOfTheButtonWeWannaHook");

  // What if the skin being loaded doesn't have this object?  Check your pointers!
  if (ourButtonScriptObj != NULL) {
    // So, that's the object to hook.  How do we hook it?
    hook = new ExampleHButtonHook(ourButtonScriptObj);
  }
}

// So, if we did EVERYTHING right, we will be able to detect when a "completely unrelated"
// button (ignore the fact that this component registered the skinfile for the button, too)
// that is living somewhere "out there in the skin" ever gets clicked.
void ExampleHButtonHook::hook_onLeftClick() {
  OutputDebugString("WOO HOO, WE GOT CLICKED AND WE NOTICED IT!!");
}

