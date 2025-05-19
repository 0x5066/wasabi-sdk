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
//      File:     examplecanvaswnd.h
//
//!##   Purpose:  Define the ExampleCanvasWnd class for our example component
//
//      Requires: Please read ExampleCanvas.cpp first.
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.
//


#ifndef _EXAMPLECANVASWND_H
#define _EXAMPLECANVASWND_H

#include "../../bfc/virtualwnd.h"
#include "../../bfc/wnds/buttwnd.h"
#include "../../common/nsGUID.h"

#define EXAMPLECANVASWND_PARENT VirtualWnd

class ExampleCanvasWnd : public EXAMPLECANVASWND_PARENT {
public:
  // This public enum allows us to decide at runtime if the object that we're
  // instantiating will paint itself in a boring or exciting manner.
  typedef enum {
    TOGGLE = -1,
    BORING = 0,
    EXCITING = 1
  } BlitStyle;

public:
  // And make a "pair" of constructors that let us set the default blitstyle value
  // or specify it upon allocation & construction.  Toggling will flip the window
  // style back and forth each time it is created to toggle.
  ExampleCanvasWnd( BlitStyle style = TOGGLE ) : EXAMPLECANVASWND_PARENT() {  
    if (style == TOGGLE) {
      myBlitStyle = staticStyle;
      staticStyle = static_cast<BlitStyle>(!staticStyle); // dumbass compiler doesn't like the ! operator for enums, heh
    } else {
      myBlitStyle = style;
    }
    setName("ExampleCanvasWnd");
  }

  // =========================================================================
  //
  //  Methods required by Window Creation Services
  //
  static const char *getWindowTypeName();
  static GUID getWindowTypeGuid();
  static void setIconBitmaps(ButtonWnd *button);    

  // =========================================================================
  //
  // And, then, here are the virtual methods we'll be overriding to get our
  // little window to be painting prettily on its little canvas.  Woot!
  //
  virtual void timerCallback(int id);
  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  // We'll cover these methods as we implement them in the CPP file.

protected:
  // How did I want to draw myself today?
  BlitStyle myBlitStyle;

  static BlitStyle staticStyle;
};

#endif _EXAMPLECANVASWND_H
