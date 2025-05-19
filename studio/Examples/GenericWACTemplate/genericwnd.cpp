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

#include "../../bfc/std.h"

#include "generic.h"
#include "genericwnd.h"
#include "resource.h"

#include "../../bfc/canvas.h"
#include "../../common/xlatstr.h"


//  Methods required by Window Creation Services
const char *GenericWnd::getWindowTypeName() { return /**/"Generic Window"/*EDITME*/; }
GUID GenericWnd::getWindowTypeGuid() { return the->getGUID(); }
void GenericWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}


int GenericWnd::onInit() {
  int retval = GENERICWND_PARENT::onInit();

  // Do your startup stuff here.
  
  return retval;
}


int GenericWnd::onPaint(Canvas *canvas) {

  PaintBltCanvas pbc;
  if (canvas == NULL) {
    pbc.beginPaint(this);
    canvas = &pbc;
  }

  GENERICWND_PARENT::onPaint(canvas);

  RECT crect = clientRect();

  canvas->fillRect(&crect, RGB(0xF0, 0xF0, 0xF0)); // RGB: #F0F0F0 for all you HTML-heads
  canvas->setTextColor(RGB(0, 0, 0));   // Black pen.
  canvas->setTextSize(48);              // 48 point text.
  canvas->textOutCentered(&crect, _("Hello, World!"));

  return 1;
}