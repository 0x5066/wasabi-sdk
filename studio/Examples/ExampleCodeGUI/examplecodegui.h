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
//      File:     ExampleCodeGUI.h
//
//!##   Purpose:  Define the ComponentClient class for our example component
//
//      Requires: Please read Example1 first.
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


#ifndef _EXAMPLECODEGUI_H //EDITME
#define _EXAMPLECODEGUI_H //EDITME

#include "../../studio/wac.h"

#define WACNAME WACExampleCodeGUI //EDITME
#define WACPARENT WAComponentClient

class WACNAME : public WACPARENT {
public:
  WACNAME();
  virtual ~WACNAME();

  virtual GUID getGUID();
  virtual void onCreate();
  virtual void onDestroy();
};

extern WACPARENT *the;

#endif
