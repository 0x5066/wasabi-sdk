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


//
// Always start with std.h
#include "../../bfc/std.h"
//
#include "exampleprefsgroup.h"
#include "../../attribs/attrbool.h"
#include "../../attribs/attrstr.h"
#include "../../attribs/attrint.h"
//


static WACNAME wac;
WACPARENT *the = &wac;                     

//  *** This is MY GUID.  Get your own!
// {27C266EF-E6D1-471d-9AFB-62208182FB0A}
static const GUID guid = 
{ 0x27c266ef, 0xe6d1, 0x471d, { 0x9a, 0xfb, 0x62, 0x20, 0x81, 0x82, 0xfb, 0xa } };

// If you don't know how to play with attribs, go read ExampleAttrib
_string myExampleStringAttrib("StringAttrib", "DefaultValue");
_bool myExampleBoolAttrib("BoolAttrib", FALSE);
_int myExampleIntAttrib("IntAttrib", 128);

WACNAME::WACNAME() : WACPARENT(/**/"ExamplePrefsGroup Component"/*EDITME*/) {

  // So you want to put a box up with the prefrences like the rest of the kids, eh?

  // Okay, so, your preferences "box" is actually a Group that you load from an
  // XML file.  If you don't know what groups are, please refer to the NSDN site
  // and read the skinning tutorial for what groups are and how to make them.

  // If you've got a good feel for groups, go open up the XML file that we register:
  registerSkinFile("wacs/xml/exampleprefsgroup/exampleprefsgroup.xml");

  // Got it open?  Good.  Now look at the first groupdef: "ExamplePrefsGroup.Attribs.Content"
  // In it, there's a single checkbox object.  This object has a cfgattrib= parameter.
  // The value to that parameter should be the config-item pair for the attrib that
  // you want to control with the checkbox.  For this example, that would be our GUID
  // and our attrib name: "{27C266EF-E6D1-471d-9AFB-62208182FB0A};BoolAttrib"
  
  // But, of course, before the XML can touch it, we must register it.
  registerAttribute(&myExampleIntAttrib);
  registerAttribute(&myExampleBoolAttrib);
  registerAttribute(&myExampleStringAttrib);

  // If you're following orders like a good little soldier, you see in the XML there's
  // a slider bound to the integer attrib, and an editbox bound to the string atrrib.
}

WACNAME::~WACNAME() {
}

GUID WACNAME::getGUID() {
  return guid;
}

void WACNAME::onCreate() {
  // Okay, so, AFTER everything gets created and our skin file containing our group
  // definitions has been properly parsed and loaded, we want to load it into the API.
  
  // This, of course, is WHY we're in onCreate(), because the API pointer does not
  // exist for use until your onCreate() is called.

  // However, once you get into the onCreate, simply register your group.
  api->preferences_registerGroup("TheNameOfTheGroupToLoad", "ExamplePrefsGroup Title", getGUID());

  // And if you run the app and go into the Prefs screen, you'll see your happy happy
  // preferences group right there alongside everybody else.  And won't that be nice?
}

void WACNAME::onDestroy() {
}

