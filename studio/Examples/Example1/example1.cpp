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

// ===========================================================================
//
//    NULLSOFT WASABI SDK EXAMPLE PROJECTS
//
//      File:     example1.cpp
//
//!##   Purpose:  Setup the Example1 Component class for our example component
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.  Deal.
//
//                Because Example1 is the first of a string of example
//                projects, there are more useless comments in this
//                set of modules than any others.
//


//
//  GOOD MORNING, CAMPERS!
//
//  I'm your uncle mig and I'd like to welcome to you Nullsoft's
//    Winamp3.0 SDK Camp!
//
//  The SDK with a difference!  (Never mind the weather)
//
//  When you play with Winamp, EVERYTHING is better!
//


// ===========================================================================
//
//  *** EXAMPLE1: The Beginning
//
//  Example1 is where we begin.  You have to start somewhere and you've finally
//  stumbled across the starting line.  If you search for "EDITME" you will see
//  all the places where you will need to edit this code in order to make your
//  own WAC do your own thing.  HOWEVER, if you would be so kind as to note the
//  GenericWACTemplate project, also in the Examples directory, you will find
//  it is an almost exact copy of this project, but without the reams and reams
//  of comments that you'd just want to delete anyhow for your own code.
//
//  GenericWACTemplate is a much more suitable project to clone for yourself.
//  

//
// Always start with std.h
#include "../../bfc/std.h"
//
#include "example1.h"     //EDITME
#include "resource.h"
#include "../GenericWACTemplate/genericwnd.h"  //EDITME
//
#include "../../common/xlatstr.h"
// Window creation services includes.
#include "../../studio/services/creators.h"
#include "../../bfc/wndcreator.h"


static WACNAME wac;
WACPARENT *the = &wac;                     

// ===========================================================================
//
//  EXAMPLE1: The GUID
//
//  *** You MUST use a unique GUID for your WAC files.
//  
//  Here's where you input the GUID.  A GUID is simply a REALLY big random
//  number that lets the rest of the world uniquely identify you (oh yah,
//  guess what "GUID" stands for, bucko?)
//  
//  Otherwise, when you run the app, it'll recognize and load all the WAC
//  files in the WACS folder, but if you report the same GUID as another
//  component, the user'll only be able to control one of them.  
//  
//  If you use the same GUID as some other Winamp3.0 developer, 
//  people will throw rocks at you.  And we'll provide the rocks.
//  
//  Trust me.  This one isn't optional.  Do it, and do it NOW.
//  
//  GUIDGEN.EXE is a utility that comes from Microsoft to help you create
//  GUID numerics.   If you're running Visual Studio, you can find it in
//  the "common\tools" folder.  There's also a helpfile in there name of
//  guidgen.hlp which should get you going with it.  If you need more
//  info, try the MSDN site.
//
//  *** This is MY GUID.  Get your own.   {D4F84EAA-4905-4f62-A209-2735769B07A0}
static const GUID guid =  //EDITME (hint: use guidgen.exe)
{ 0xd4f84eaa, 0x4905, 0x4f62, { 0xa2, 0x9, 0x27, 0x35, 0x76, 0x9b, 0x7, 0xa0 } };



// ===========================================================================
//
//  EXAMPLE1: Example1() -- The Component Constructor
//
//  Now, here's our component constructor.
//  This begs the question:  what exactly IS a component?
//
//  The answer is "A component is a collection of registered service instances."
//
//  This, of course, begs the question "what's a service instance?"
//
//  Well, for a more detailed answer, RTFM, but for this early in the Examples process,
//  "service instances" are how you get crap done.  For example, in this Example, we
//  are going to make a window and have it be listed in the Thinger and in the
//  main context menu.  All three of these tasks have associated items to be
//  registered with the system during one's component constructor.
//
//  For those long familiar with the ways of wasabi, I will direct you to note
//  that you now name your component as a parent object construction parameter.
WACNAME::WACNAME() : WACPARENT(/**/"Example1 Component"/*EDITME*/) {
//  Ah, well, yes.  There's one problem with GUIDs.  Nobody will be
//  able to tell that component {6DC73FE8-6D34-4942-950A-FF2370329BB0}
//  is the really kickass toasting-and-breadslicing component you've 
//  written for Winamp3.
//  So you must actually give it at least a SEMI unique name in english, too.

  // *** To simplify things, we'll just use the simple window in the GenericWACTemplate project.

  // So, what did we want to do again?  Oh yes...

  // 1) we are going to make a window,
  registerService(new WndCreateCreatorSingle< CreateWndByGuid</**/GenericWnd/*EDITME*/ > >);

  // 2) and have it be listed in the Thinger,
  registerService(new WndCreateCreatorSingle< CreateBucketItem</**/GenericWnd/*EDITME*/ > >);

  // 3) and in the main context menu.
  registerAutoPopup(getGUID(), getName());

  // Now, some of your components you make aren't going to need windows at all.
  // Still, the way you get anything done at all is through the registration of
  // services with the engine in your WAC constructor.  Think of this as your 
  // ``main()`` function, but the only thing you're allowed to do here is register 
  // things.

  // "How, then," I hear you say with growing shock in your voice, "can this be 
  // my main function?  How in the world can I get things done if I can only call 
  // these registration functions?"

  // Well, for this Example, note the reference to "/**/GenericWnd/*EDITME*/" 
  // twice up there.  All we want to do is make a window.  The window we're going 
  // to make is a class called "GenericWnd" and the code for GenericWnd can be 
  // found in GenericWnd.h and GenericWnd.cpp, so when you get done with this
  // module I'd suggest you go on to them, to see about where you can really
  // start to get your hands dirty and do wonderful things with nullsoft wasabi.
}

WACNAME::~WACNAME() {
}

// ===========================================================================
//
//  *** EXAMPLE1: Example1::getGuid()
//
//  As you might guess, you don't need to do anything with this method.
//  It is simply so the engine can find your GUID for your component.
GUID WACNAME::getGUID() {
  return guid;
}

void WACNAME::onCreate() {
  // *** Do startup stuff here that doesn't require you to have a window yet
}

void WACNAME::onDestroy() {
}

#if 0
// THIS FUNCTION IS DEPRECATED.  DO NOT USE IT.
// ASSIGN YOUR WAC'S NAME STRING THROUGH ITS PARENT CONSTRUCTOR.
const char *WACNAME::getName() {
  return "Example1 Component"; //EDITME
}
// For those who wonder, if a name string is passed as a parent construction param,
// the WACPARENT::getName() virtual method is never called.  If not, this method
// will be called.  The point, however, is to simplify as much functionality as
// possible into the constructor and require as few methods as possible.
#endif

