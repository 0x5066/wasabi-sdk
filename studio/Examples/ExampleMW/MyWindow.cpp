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

#include "MyWindow.h"
#include "../../bfc/canvas.h"

// A simple example window which you can instantiate in your xml by using :
//   <svc 
//     id="object_id" 
//     x="0" y="0" 
//     guid="{70DD335C-A5CA-4b93-A78A-8DAAED1C942D}"
//   />
//

// {70DD335C-A5CA-4b93-A78A-8DAAED1C942D}
static const GUID mywindow_guid = 
{ 0x70dd335c, 0xa5ca, 0x4b93, { 0xa7, 0x8a, 0x8d, 0xaa, 0xed, 0x1c, 0x94, 0x2d } };

int MyWindow::onPaint(Canvas *c) {
  MYWINDOW_PARENT::onPaint(c);
  RECT r;
  getClientRect(&r);
//  c->fillRect(&r, 0);
  c->setTextColor(0xFFFFFF);
  c->textOut(r.left+10, r.top+10, "MyWindow");
  return 1;
}

GUID MyWindow::getGuid() {
  return mywindow_guid;
}

int MyWindow::getPreferences(int what) {
  switch (what) {
    case SUGGESTED_W: return 275;
    case SUGGESTED_H: return 232;
    case MINIMUM_W: return 275;
    case MINIMUM_H: return 232;
    case MAXIMUM_W: return 275;
    case MAXIMUM_H: return 232;
    default: return -1;
  }
}
