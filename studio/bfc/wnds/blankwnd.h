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

#ifndef _BLANKWND_H
#define _BLANKWND_H

#include "../../common/common.h"
#include "../virtualwnd.h"

#define BLANKWND_PARENT VirtualWnd

/**
  Class BlankWnd provides blank windows. The initial color can be set in the 
	constructor, with a default of black. There is a method for painting the window from a Canvas.

  @short Blank Window with background color.
  @author Nullsoft
  @ver 1.0
  @see VirtualWnd
*/
class COMEXP BlankWnd : public BLANKWND_PARENT {
public:
  /**
    You can set the background color for the window via an RGB value.
    The RGB value is contructed using RGB(), like so RGB(Red, Green, Blue);
    
    @param color The RGB value of the background color to use.
  */
  BlankWnd(RGB32 color=RGB(0,0,0));
  
  /**
    This event is triggered when the window needs to be repainted.
    Override it to implement your own handling of this event.
    
    @ret 1, If you handle the event;
    @param canvas A pointer to the canvas on which will we paint.
  */
  virtual int onPaint(Canvas *canvas);

private:
  RGB32 color;
};

#endif
