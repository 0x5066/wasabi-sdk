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

//PORTABLE
#ifndef _TEXTBAR_H
#define _TEXTBAR_H

#include "common.h"
#include "../bfc/virtualwnd.h"
#include "../bfc/autobitmap.h"

class CheckWnd;

#define TEXTBAR_PARENT VirtualWnd
/**
  TextBar uses the BaseWnd name field of the object as the text 
  to be displayed.

  @short TextBar control.
  @author Nullsoft
  @ver 1.0
  @see LabelWnd
*/
class COMEXP TextBar : public VirtualWnd {
public:
  /**
    Sets the default flags of the TextBar. Defaults to 16px fonts, 
    no background texture, left justified text, shadowed text in 
    the bgcolor, no outline, not box around the text.
  */
  TextBar();

  /**
    Event is triggered when the window requires a repaint.
    Override this to implement your own behavior.
    
    Paints the bitmap on canvas according to current 
    options (centering, tiling, stretching, title).

    @ret 0 for failure, 1 for success
    @param canvas The canvas on which to paint.
  */
  virtual int onPaint(Canvas *canvas);

  /**
    Event is triggered when the name of the window is changed.
    Override this to implement your own behavior.
    
    @see BaseWnd::setName()
  */
  virtual void onSetName();
  
  /**
    Set the text to be displayed to an ascii representation of a numeric value.
    
    @ret 1.
    @param i The numeric value to be displayed.
  */
  int setInt(int i);

  /**
    Set the size of the text for the textbar.
    
    @ret 1, success; 0, failure.
    @param newsize The new text size, range is from 1 to 72 pixels.
  */
  int setTextSize(int newsize);

  /**
    Get the width of the text displayed, in pixels.
    
    @ret Width of the displayed text (in pixels).
  */
  int getTextWidth();
  
  /**
    Get the height of the text displayed, in pixels.
    
    @ret Height of the displayed text.
  */
  int getTextHeight();
  
  /**
    Use the base texture when rendering the TextBar?
    If the base texture is used, it will be rendered as 
    the background of the textbar.
    
    @param u !0, Use base texture; 0, Do not use base texture;
  */
  void setUseBaseTexture(int u);
  
  /**
    Event is triggered when the left mouse button is pressed while 
    the textbar has focus. Override this to implement your
    own behavior.
    
    @param x X coordinate of the mouse pointer.
    @param y Y coordinate of the mouse pointer.
  */
  virtual int onLeftButtonDown(int x, int y);

  /**
    Center the text in the textbar? If not, 
    it will be left justified by default.
    
    @param center !0, Center text; 0, Do not center text;
  */
  void setCenter(int center);
  
  /**
    Get the center text flag.
    
    @see setCenter()    
    @ret TRUE, Text is being centered; FALSE, No centering (left justified);
  */
  BOOL getCentered();

  // The following three options have ascending overriding priority --

  /**
    Sets the shadowed text flag. If enabled, the text will be shadowed
    with the "bgcolor" value.
    
    @see getTextShadowed()
    @param settextshadowed !0, Shadow the text; 0, Do not shadow the text;
  */
  void setTextShadowed(int settextshadowed) { 
    textshadowed = !!settextshadowed;
  }
  
  /**
    Get the shadowed text flag. If enabled, the text will be shadowed
    with the "bgcolor" value.
    
    @see setTextShadowed()
    @ret !0, Shadow the text; 0, Do not shadow the text;
  */
  int getTextShadowed() {
    return textshadowed;
  }
  
  /**
    Sets the outline text flag. If enabled, the text will be
    outlined with the "bgcolor" value.
    
    @param settextoutlined !0, Outline the text; 0, Do not outline the text;
  */
  void setTextOutlined(int settextoutlined) { 
    textoutlined = !!settextoutlined;
  }
  
  /**
    Get the outline text flag. If enabled, the text will be
    outlined with the "bgcolor" value.
    
    @ret !0, Outline the text; 0, Do not outline the text;
  */
  int getTextOutlined() {
    return textoutlined;
  }
 
  /**
    Set the drawbox flag. If true, the drawbox flag will cause
    a box to be drawn around the text in the textbar.
    
    @param setdrawbox !0, Drawbox around the text; 0, No drawbox;
  */
  void setDrawBox(int setdrawbox) {
    drawbox = !!setdrawbox;
  }
  
  /**
    Get the drawbox flag. If true, the drawbox flag will cause
    a box to be drawn around the text in the textbar.
    
    @ret !0, Drawbox around the text; 0, No drawbox;
  */
  int getDrawBox() {
    return drawbox;
  }
  
  /**
    Associate a checkbox with the textbar. When a textbar is linked
    to a checkbox, it will toggle the checkbox when it receives
    left clicks.
    
    @param target A pointer to the CheckWnd to link.
  */
  void setAutoToggleCheckWnd(CheckWnd *target) {
    checkwndtarget = target;
  }


private:
  int size;
  int centered;
  int usebt;

  int textshadowed; // display a shadow of the text in bgcolor.  default: on
  int textoutlined; // draw an outline of the text in bgcolor.  default: off
  int drawbox;      // draw a box of bgcolor the size of the boundsrect.  default: off

  AutoSkinBitmap bgbitmap;
  CheckWnd *checkwndtarget;
};

const int TEXTBAR_LEFTMARGIN = 2;

#endif
