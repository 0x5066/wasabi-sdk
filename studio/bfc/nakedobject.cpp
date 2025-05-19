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

#include "nakedobject.h"

NakedObject::NakedObject() {
  reentry_onresize = 0;
  reentry_onsetvisible = 0;
}

int NakedObject::getPreferences(int what) {
  return 0;
}

int NakedObject::onResize() {
  int rt = NAKEDOBJECT_PARENT::onResize();
  RECT r;
  getClientRect(&r);
  if (!reentry_onresize && r.left != r.right || r.top != r.bottom) {
    reentry_onresize = 1;
    resize(r.left, r.top, 0, 0);
    reentry_onresize = 0;
  }
  return rt;
}

void NakedObject::onSetVisible(int i) {
  NAKEDOBJECT_PARENT::onSetVisible(i);
  if (!i) return;
  if (!reentry_onsetvisible) {
    reentry_onsetvisible = 1;
    setVisible(0);
    reentry_onsetvisible = 0;
  }
}

