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


#include "cells.h"
#include "../studio/api.h"

#include "textbar.h"
#include "editwnd.h"

TextCell::TextCell(const char *name) {
  textbar = new TextBar();
  setContent(textbar);
  setText(name);
}

void TextCell::setText(const char *txt) {
  textbar->setName(txt);
}

int TextCell::setTextSize(int newsize) {
  return textbar->setTextSize(newsize);
}

void TextCell::setCentered(int center) { 
  textbar->setCenter(center); 
}

ButtonCell::ButtonCell() {
  buttonwnd = new ButtonWnd();
  setContent(buttonwnd);
}

void ButtonCell::setNotifyId(int id) {
  buttonwnd->setNotifyId(id);
}

int ButtonCell::setButtonText(char *text, int size) {
  return buttonwnd->setButtonText(text, size);
}

EditCell::EditCell() {
  editwnd = new EditWnd();
  setContent(editwnd);
}

void EditCell::setBuffer(char *buf, int len) {
  editwnd->setBuffer(buf, len);
}

#include "../attribs/cfgitem.h"

CfgGroupCell::CfgGroupCell(CfgItem *_item, RootWnd *rw, const char *attrname)
  : item(_item) {
  item->onCfgGroupCreate(rw, attrname);
}

CfgGroupCell::~CfgGroupCell() {
  item->onCfgGroupDelete(getRootWnd());
  api->group_destroy(getRootWnd());
}

