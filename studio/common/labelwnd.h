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

#ifndef _LABELWND_H
#define _LABELWND_H

#include "common.h"
#include "guiobjwnd.h"

#define LABELWND_PARENT GuiObjectWnd
class COMEXP LabelWnd : public LABELWND_PARENT {
protected:
  LabelWnd();
public:

  virtual void getClientRect(RECT *);
  virtual int onResize();
  virtual int onPaint(Canvas *canvas);
  virtual int onGetFocus();
  virtual int onKillFocus();
  virtual void invalidateLabel();
  virtual int wantFocus();
  virtual int wantRenderBaseTexture() { return 1; }

  // override & return 1 to force painting label with focus all the time
  virtual int forceFocus() { return 0; }

  virtual void onSetName();
  virtual void setMargin(int newmargin);

  virtual int setFontSize(int size);

//CUT  virtual int childNotify(RootWnd *child, int msg, int param1=0, int param2=0);

  int showLabel(int show);
  int getLabelHeight();

  void reloadResources();

private:
  int show_label, labelsize;
  int labelHeight;
  int margin;
};

// use this if you want a generic labelwnd (but try not to)
class COMEXP LabelWndI : public LabelWnd { };

#endif
