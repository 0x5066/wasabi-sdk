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

#include "rootcomp.h"

#include "../bfc/canvas.h"

RootCompWnd::RootCompWnd() {
  child = NULL;
}

RootCompWnd::~RootCompWnd() {
  	delete child;
}

int RootCompWnd::setChild(BaseWnd *_child) {
  child = _child;
	return 1;
}

int RootCompWnd::onInit() {

  ROOTCOMPWND_PARENT::onInit();

  if (!child) return 1;

	RECT r;
	getClientRect(&r);

  child->setParent(this);
  child->init(this);

  onResize();

  return 1;
}

int RootCompWnd::onPaint(Canvas *canvas) {

  PaintCanvas pc;
  if (canvas == NULL) {
    pc.beginPaint(this);
    canvas = &pc;
  }

  // don't return before creation of canvas bcause if for any reason this window doesn't clip children,
  // then returning immediatly will generate more WM_PAINT, having a canvas and having it destroyed
  // clears the update region
  if (child) return 1;

  ROOTCOMPWND_PARENT::onPaint(canvas);

  RECT cr;
  getClientRect(&cr); 

  canvas->fillRect(&cr, RGB(Std::random()%255,Std::random()%255,Std::random()%255));

  return 1;
}

int RootCompWnd::onResize() {
  if (!child) return 1;

  RECT r;
	getClientRect(&r);

  child->resize(&r);
  return 1;
}
