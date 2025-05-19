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

#ifndef __COMBOBOX_H
#define __COMBOBOX_H

#include "dropdownlist.h"
#include "../../common/script/c_script/h_edit.h"
#include "../../common/script/c_script/c_edit.h"

#define COMBOBOX_PARENT DropDownList

class XmlObject;
class HEBKeysCallback;

class COMEXP ComboBox : public COMBOBOX_PARENT {

  public:

    ComboBox();
    virtual ~ComboBox();

    virtual int wantTrapButton() { return 1; }
    virtual int wantTrapText() { return 0; }
    virtual int wantTrapEdit() { return 1; }

    virtual void abstract_onNewContent();
    virtual void trapControls();

    virtual const char *dropdownlist_getMainGroupId() { return "wasabi.combobox.main.group"; }
    virtual const char *dropdownlist_getListGroupId() { return "wasabi.combobox.list.group"; }
    virtual const char *dropdownlist_getButtonId() { return "combobox.button"; }
    virtual const char *dropdownlist_getListId() { return "combobox.list"; }

    virtual const char *combobox_getEditId() { return "combobox.edit"; }
                        
    virtual const char *embeddedxui_getEmbeddedObjectId() { return combobox_getEditId(); }

    virtual void dropdownlist_onCloseList();
    virtual void dropdownlist_onOpenList();

    virtual void setText(const char *text, int hover=0); // use this to set the content of the edit box
    virtual const char *getText(int fromcontrol=0); // use this one to ask for the currently displayed entry

    virtual const char *getCustomText() { return NULL; }

    virtual void dropdownlist_onConfigureList(GuiObject *o);
    virtual void onSelect(int id, int hover);
    virtual void enter();

    void selectEditor();
    virtual int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);

    virtual void onEditKeyDown(int vk);
    virtual void onEditKeyUp(int vk);
    virtual void onEditEnter(const char *txt);
    virtual void onEditChar(const char *c);

    virtual int wantTransferDownToList() { return 1; }
    virtual int wantTransferUpToList() { return 1; }
    virtual int wantTransferHomeToList() { return 1; }
    virtual int wantTransferEndToList() { return 1; }
    virtual int wantTransferPgUpToList() { return 1; }
    virtual int wantTransferPgDnToList() { return 1; }
    virtual int wantTransferEnterToList() { return 1; }
    virtual int wantDownOpenList() { return 1; }
    virtual int wantCloseListOnChar() { return 1; }
    virtual int wantEnterOnSelect() { return 1; }

    virtual void listDown();
    virtual void listUp();
    virtual void listHome();
    virtual void listEnd();
    virtual void listPageDown();
    virtual void listPageUp();
    virtual void listSelect();

    virtual void onPreCloseList();

  private:
    
    virtual void updateTextInControl(const char *text);

    HEBKeysCallback *keys_edit;
    GuiObject *lastlist;
    String curtxt;

    int savedidle, savedautoenter;
    int disable_getselection;
};

class COMEXP HEBKeysCallback : public H_Edit {
  public:
    
    HEBKeysCallback(ScriptObject *trap, ComboBox *_callback) :
        callback(_callback), H_Edit(trap), o(trap) {
    }

  virtual void hook_onKeyDown(int vk) {
    callback->onEditKeyDown(vk);
  }

  virtual void hook_onKeyUp(int vk) {
    callback->onEditKeyUp(vk);
  }

  virtual void hook_onEnter() {
    C_Edit e(o);
    callback->onEditEnter(e.getText());
  }


  virtual void hook_onChar(const char *c) {
    callback->onEditChar(c);
  } 

  private:
    ComboBox *callback;
    ScriptObject *o;
};


#endif
