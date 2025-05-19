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

#ifndef _CONTWND_H // Generic container window
#define _CONTWND_H

#include "common.h"
#include "../bfc/basewnd.h"
#include "labelwnd.h"
#include "../bfc/tlist.h"

#define CONTWND_PARENT LabelWnd

class ChildSlot {
public:
  friend class ContWnd;
  ChildSlot(BaseWnd *_child, RECT &_pos, int inval, int _delchild) {
    child = _child;
    pos = _pos;
    invalidate_on_resize = inval;
    delchild = _delchild;
  }
  ~ChildSlot() {
    if(delchild) delete child;
  }

protected:
  RECT pos;
  BOOL invalidate_on_resize;
  BaseWnd *child;
  int delchild;
};


class COMEXP ContWnd : public CONTWND_PARENT {
public:
  ContWnd();
  virtual ~ContWnd();

  virtual int onInit();
  virtual int onPaint(Canvas *canvas);
  virtual int onResize();

  int addChild(BaseWnd *child, int x, int y, int w, int h, int invalidate_on_resize=0);
  int deleteChild(BaseWnd *child);
  int deleteAll();

  void setDeleteChilds(int on) { delchilds=on; }

protected:
  ChildSlot * findChild(BaseWnd *child);
  int delchilds;

private:
  PtrList<ChildSlot> children;
};

#endif
