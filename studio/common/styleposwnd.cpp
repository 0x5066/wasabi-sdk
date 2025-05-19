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

//#include "std.h"
#include "styleposwnd.h"
#include "../bfc/canvas.h"

#include "../studio/api.h"

StylePosWnd::StylePosWnd() : childinfo(), STYLEPOSWND_PARENT() {
  updatechildpos = 0;
}

// Deleting deletes everything.  Make sure you remove everything before you
// let this monkey delete itself if you don't want it to delete your items.
StylePosWnd::~StylePosWnd() {
  for (int i=0;i<childinfo.getNumItems();i++) {
    delete childinfo[i]->child;
  }
  childinfo.deleteAll();
}

int StylePosWnd::addChild(const StylePosWnd::ChildInfo &info) {
  // Copy the given value
  StylePosWnd::ChildInfo *cs = new StylePosWnd::ChildInfo(info);
  // Add the new object to our list.
  childinfo.addItem(cs);

  // If we're already initialized, this code must be run to ensure
  // the new child object is properly resized.
  if (isInited()) {
    cs->child->setParent(this);
    invalidate();
  }

  // Flag that we're in an intermediate state and need to run
  // the position update code at the beginning of onPaint()
  updatechildpos = 1;
  return 1;
}

int StylePosWnd::onInit() {
  STYLEPOSWND_PARENT::onInit();

  for (int i=0; i < childinfo.getNumItems(); i++) {
    StylePosWnd::ChildInfo *cslot = childinfo[i];
    BaseWnd *child = cslot->child;
    if (child->init(this) != 0) {
      child->setParent(this);
    }
  }
  updatePositions();
  return 1;
}

int StylePosWnd::onPaint(Canvas *canvas) {
  if (updatechildpos) {
    updatePositions();
  }
  
  PaintCanvas pc;
  if (canvas == NULL) {
    pc.beginPaint(this);
    canvas = &pc;
  }

  STYLEPOSWND_PARENT::onPaint(canvas);

  RECT r;
  getClientRect(&r); // canvas will have a clipping region set by basewnd if needed

  renderBaseTexture(canvas, r);

  return 1;
}

int StylePosWnd::onResize() {
  updatePositions();
  return 1;
}

int StylePosWnd::deleteChild(BaseWnd *child) {
  StylePosWnd::ChildInfo *childslot = findChild(child);
  // In preparation for removing objects from the interface,
  // we made the childinfo object _NOT_ delete its payload
  // when it, itself, is deleted.
  if (childslot != NULL) {
    // Which means we have to delete the window here
    delete childslot->child;
    // And then remove it from the list
    childinfo.delItem(childslot);
    invalidate();
    return 1;
  }
  invalidate();
  return 0;
}

int StylePosWnd::deleteAll() {
  // Ditto on the comments above.
  for (int i=0;i<childinfo.getNumItems();i++) {
    delete childinfo[i]->child;
  }
  childinfo.deleteAll();
  invalidate();
  return 1;
}

int StylePosWnd::removeChild(BaseWnd *child) {
  StylePosWnd::ChildInfo *childslot = findChild(child);
  // Which, of course, allows us to do this method here.
  if (childslot != NULL) {
    childinfo.delItem(childslot);
    invalidate();
    return 1;
  }
  invalidate();
  return 0;
}

int StylePosWnd::removeAll() {
  // And this method here.
  childinfo.deleteAll();
  invalidate();
  return 1;
}

void StylePosWnd::updatePositions() {
  RECT r;
  getClientRect(&r);

  for (int i=0;i<childinfo.getNumItems();i++) {
    StylePosWnd::ChildInfo *cslot = childinfo[i];
    RECT pos;

    double ratio = 1.0;

    if (!cslot->child->handleRatio()) {
      ratio = getRenderRatio();
    }

    int looseLeft = 0;
    int looseTop = 0;
    int looseRight = 0;
    int looseBottom = 0; // It's fun to have a loose bottom!

    // There are ALOT of code paths here.  This may take awhile to debug, fully. :)

    // Left Edge.
    switch (cslot->edges[LEFT].style) {
      case LOOSE:
        if (cslot->edges[RIGHT].style == LOOSE) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges cannot both be LOOSE.\n");
          pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
        } else {
          looseLeft = 1;
        }
      break;
      case FIXED:
        pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
      break;
      case LOCK_BORDER:
        pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
      break;
      case CENTER:
        if (cslot->edges[RIGHT].style != CENTER) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must both be CENTER.\n");
          pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
        } else {
          if (cslot->edges[LEFT].scalar != cslot->edges[RIGHT].scalar) {
            OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must have equal scalars when CENTER.\n");
            pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
          } else {
            pos.left = (int)((double)r.left + ((r.right - r.left - cslot->edges[LEFT].scalar) / 2) * ratio);
          }
        }
      break;
      case LOCK_LEFT: {
        BaseWnd *who = cslot->edges[LEFT].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.left = (int)((double)target.left + cslot->edges[LEFT].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_LEFT\n");
          pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
        }
      }
      break; 
      case LOCK_TOP:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_TOP for pos.left\n");
        pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
      break;
      case LOCK_RIGHT: {
        BaseWnd *who = cslot->edges[LEFT].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.left = (int)((double)target.right + cslot->edges[LEFT].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_RIGHT\n");
          pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
        }
      }
      break;
      case LOCK_BOTTOM:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_BOTTOM for pos.left\n");
        pos.left = (int)((double)r.left + cslot->edges[LEFT].scalar * ratio);
      break;
    }

    // Top Edge
    switch (cslot->edges[TOP].style) {
      case LOOSE:
        if (cslot->edges[BOTTOM].style == LOOSE) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges cannot both be LOOSE.\n");
          pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
        } else {
          looseTop = 1;
        }
      break;
      case FIXED:
        pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
      break;
      case LOCK_BORDER:
        pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
      break;
      case CENTER:
        if (cslot->edges[BOTTOM].style != CENTER) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must both be CENTER.\n");
          pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
        } else {
          if (cslot->edges[TOP].scalar != cslot->edges[BOTTOM].scalar) {
            OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must have equal scalars when CENTER.\n");
            pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
          } else {
            pos.top = (int)((double)r.top + ((r.bottom - r.top - cslot->edges[TOP].scalar) / 2) * ratio);
          }
        }
      break;
      case LOCK_LEFT:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_LEFT for pos.top\n");
        pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
      break;
      case LOCK_TOP: {
        BaseWnd *who = cslot->edges[TOP].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.top = (int)((double)target.top + cslot->edges[TOP].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_TOP\n");
          pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
        }
      }
      break; 
      case LOCK_RIGHT:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_RIGHT for pos.top\n");
        pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
      break;
      case LOCK_BOTTOM: {
        BaseWnd *who = cslot->edges[TOP].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.top = (int)((double)target.bottom + cslot->edges[TOP].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_BOTTOM\n");
          pos.top = (int)((double)r.top + cslot->edges[TOP].scalar * ratio);
        }
      }
      break;
    }

    // Right Edge.
    switch (cslot->edges[RIGHT].style) {
      case LOOSE:
        if (cslot->edges[LEFT].style == LOOSE) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges cannot both be LOOSE.\n");
          pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
        } else {
          looseRight = 1;
        }
      break;
      case FIXED:
        pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
      break;
      case LOCK_BORDER:
        pos.right = (int)((double)r.right + cslot->edges[RIGHT].scalar * ratio);
      break;
      case CENTER:
        if (cslot->edges[LEFT].style != CENTER) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must both be CENTER.\n");
          pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
        } else {
          if (cslot->edges[RIGHT].scalar != cslot->edges[LEFT].scalar) {
            OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must have equal scalars when CENTER.\n");
            pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
          } else {
            pos.right = (int)((double)r.right - ((r.right - r.left - cslot->edges[RIGHT].scalar) / 2) * ratio);
          }
        }
      break;
      case LOCK_LEFT: {
        BaseWnd *who = cslot->edges[RIGHT].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.right = (int)((double)target.left + cslot->edges[RIGHT].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_LEFT\n");
          pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
        }
      }
      break; 
      case LOCK_TOP:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_TOP for pos.right\n");
        pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
      break;
      case LOCK_RIGHT: {
        BaseWnd *who = cslot->edges[RIGHT].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.right = (int)((double)target.right + cslot->edges[RIGHT].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_RIGHT\n");
          pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
        }
      }
      break;
      case LOCK_BOTTOM:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_BOTTOM for pos.right\n");
        pos.right = (int)((double)r.left + cslot->edges[RIGHT].scalar * ratio);
      break;
    }

    // Bottom Edge
    switch (cslot->edges[BOTTOM].style) {
      case LOOSE:
        if (cslot->edges[TOP].style == LOOSE) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges cannot both be LOOSE.\n");
          pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
        } else {
          looseBottom = 1;
        }
      break;
      case FIXED:
        pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
      break;
      case LOCK_BORDER:
        pos.bottom = (int)((double)r.bottom + cslot->edges[BOTTOM].scalar * ratio);
      break;
      case CENTER:
        if (cslot->edges[TOP].style != CENTER) {
          OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must both be CENTER.\n");
          pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
        } else {
          if (cslot->edges[BOTTOM].scalar != cslot->edges[TOP].scalar) {
            OutputDebugString("StylePosWnd::updatePositions -- Opposite edges must have equal scalars when CENTER.\n");
            pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
          } else {
            pos.bottom = (int)((double)r.bottom - ((r.bottom - r.top - cslot->edges[BOTTOM].scalar) / 2) * ratio);
          }
        }
      break;
      case LOCK_LEFT:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_LEFT for pos.bottom\n");
        pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
      break;
      case LOCK_TOP: {
        BaseWnd *who = cslot->edges[BOTTOM].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.bottom = (int)((double)target.top + cslot->edges[BOTTOM].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_TOP\n");
          pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
        }
      }
      break; 
      case LOCK_RIGHT:
        OutputDebugString("StylePosWnd::updatePositions -- You cannot use LOCK_RIGHT for pos.bottom\n");
        pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
      break;
      case LOCK_BOTTOM: {
        BaseWnd *who = cslot->edges[BOTTOM].who;
        if (who) {
          RECT target;
          who->getClientRect(&target);
          pos.bottom = (int)((double)target.bottom + cslot->edges[BOTTOM].scalar * ratio);
        } else {
          OutputDebugString("StylePosWnd::updatePositions -- You must provide a pointer for LOCK_BOTTOM\n");
          pos.bottom = (int)((double)r.top + cslot->edges[BOTTOM].scalar * ratio);
        }
      }
      break;
    }

    // Loose edges must be calculated after their opposite
    if (looseLeft) {
      pos.left = (int)((double)pos.right - cslot->edges[LEFT].scalar * ratio);
    }
    if (looseTop) {
      pos.top = (int)((double)pos.bottom - cslot->edges[TOP].scalar * ratio);
    }
    if (looseRight) {
      pos.right = (int)((double)pos.left + cslot->edges[RIGHT].scalar * ratio);
    }
    if (looseBottom) {
      pos.bottom = (int)((double)pos.top + cslot->edges[BOTTOM].scalar * ratio);
    }

    // And then we clip to the original parent rect.
    pos.left = max(r.left, pos.left);
    pos.top = max(r.top, pos.top);
    pos.right = min(r.right, pos.right);
    pos.bottom = min(r.bottom, pos.bottom);

    // NOW we have a darn groovy new position for ourselves, don't we?
    cslot->child->resize(&pos);
    if (cslot->invalidate_on_resize) cslot->child->invalidate();
  }

  updatechildpos = 0;
  invalidate();
}


StylePosWnd::ChildInfo * StylePosWnd::findChild(BaseWnd *child) {
  int i, num = childinfo.getNumItems();
  StylePosWnd::ChildInfo *childslot;
  for (i = 0; i < num; i++) {
    childslot = childinfo[i];
    if (childslot->child == child) {
      return childslot;
    }
  }
  return NULL;
}

