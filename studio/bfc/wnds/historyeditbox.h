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

#ifndef __HISTORYEDITBOX_H
#define __HISTORYEDITBOX_H

#include "combobox.h"
#include "../ptrlist.h"
#include "../string.h"

#define HISTORYEDITBOX_PARENT ComboBox

class COMEXP HistoryEditBox : public HISTORYEDITBOX_PARENT {

  public:

    HistoryEditBox();
    virtual ~HistoryEditBox();

    virtual const char *dropdownlist_getMainGroupId() { return "wasabi.historyeditbox.main.group"; }
    virtual const char *dropdownlist_getListGroupId() { return "wasabi.historyeditbox.list.group"; }
    virtual const char *dropdownlist_getButtonId() { return "historyeditbox.button"; }
    virtual const char *dropdownlist_getListId() { return "historyeditbox.list"; }

    virtual const char *combobox_getEditId() { return "historyeditbox.edit"; }

    void onEditKeyDown(int vk);
    void onEditEnter(const char *txt);

    virtual int wantAutoSort() { return 0; }

    virtual int wantDownOpenList() { return history_pos == 0; }
    virtual void dropdownlist_onCloseList();
    virtual void onPreOpenList();
    virtual int onInit();
    virtual void addHistory(const char *txt);

    virtual int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);

    void history_back();
    void history_forward();

    enum {
      HISTORYEDITBOX_SETNAVBUTTONS=0,
    };

  private:

    void saveHistory();
    void loadHistory(int refill=1);

    PtrList<String> history;
    int history_pos;
    int xuihandle;
};

#endif
