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

#ifndef _CELLS_H
#define _CELLS_H

#include "common.h"
#include "formwnd.h"

#include "../bfc/wnds/buttwnd.h" // for DEFAULT_BUTTON_TEXT_SIZE

class TextBar;
class EditWnd;
class CfgItem;
class RootWnd;

// these are pre-made cells to drop into dialog-box type formwnds

// the cell just sets a text label
class COMEXP TextCell : public FormCell {
public:
  TextCell(const char *txt=NULL);

  void setText(const char *txt);
  int setTextSize(int newsize);

  void setCentered(int center);

private:
  TextBar *textbar;
};

// yes, I am this razy! --BU
class CenteredTextCell : public TextCell {
public:
  CenteredTextCell(const char *txt=NULL) : TextCell(txt) {
    setCentered(TRUE);
  }
};

// this cell type implements a button with callback to parent form
class COMEXP ButtonCell : public FormCell {
public:
  ButtonCell();

  void setNotifyId(int id);	// id will be param1 in child notify msgs
  int setButtonText(char *text, int size=DEFAULT_BUTTON_TEXT_SIZE);

private:
  ButtonWnd *buttonwnd;
};

class COMEXP EditCell : public FormCell {
public:
  EditCell();

  void setBuffer(char *buf, int len);
  EditWnd *getEditWnd() { return editwnd; }

private:
  EditWnd *editwnd;
};

class COMEXP CfgGroupCell : public FormCell {
public:
  CfgGroupCell(CfgItem *item=NULL, RootWnd *rw=NULL, const char *attrname=NULL);
  virtual ~CfgGroupCell();

private:
  CfgItem *item;
};


#endif
