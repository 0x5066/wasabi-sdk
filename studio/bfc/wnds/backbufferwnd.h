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

#ifndef __BBWND_H
#define __BBWND_H

#include "abstractwndhold.h"
#include "../autobitmap.h"

#define BBWND_PARENT AbstractWndHolder

/**
  class BackBufferWnd
  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP BackBufferWnd : public BBWND_PARENT {
  
  public:

    BackBufferWnd();
    virtual ~BackBufferWnd();

    virtual int onPaint(Canvas *c); 

    /**
      BackBufferWnd method wantBackBuffer .
      
      @ret 0
      @param None
    */
    virtual int wantBackBuffer() { return 0; }
    virtual BltCanvas *getBackBuffer();
    virtual int onSiblingInvalidateRgn(Region *r, RootWnd *who, int who_idx, int my_idx);
    
    /**
      BackBufferWnd method wantSiblingInvalidations .
    
      @ret 0
      @param None
    */
    virtual int wantSiblingInvalidations() { return wantBackBuffer(); }

  private:

    int backbuffer;
    BltCanvas *back_buffer;
    int canvas_w, canvas_h;
};

#endif
