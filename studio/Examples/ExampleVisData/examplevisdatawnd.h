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
//      File:     examplevisdatawnd.h
//
//!##   Purpose:  Define the ExampleVisDataWnd class for our example component
//
//      Requires: Please read ExampleVisData.cpp first.
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


#ifndef _EXAMPLEVISDATAWND_H
#define _EXAMPLEVISDATAWND_H

#include "../../bfc/virtualwnd.h"
#include "../../bfc/wnds/buttwnd.h"

#define EXAMPLEVISDATAWND_PARENT VirtualWnd

class ExampleVisDataWnd : public EXAMPLEVISDATAWND_PARENT {

public:
  ExampleVisDataWnd() :
    EXAMPLEVISDATAWND_PARENT() {  
    setName("ExampleVisDataWnd");
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
  //  Just like Ex1, we're just going to be doing some direct canvas blits.
  //
  virtual void timerCallback(int id);
  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
};

#endif _EXAMPLEVISDATAWND_H
