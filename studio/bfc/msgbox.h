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

#ifndef __MESSAGEBOX_H
#define __MESSAGEBOX_H

#include "string.h"
#include "ptrlist.h"

#define MSGBOX_ABORTED    0 // NOT a flag, only a return code

#define MSGBOX_OK         1
#define MSGBOX_CANCEL     2
#define MSGBOX_YES        4
#define MSGBOX_NO         8
#define MSGBOX_ALL        16
#define MSGBOX_NEXT       32
#define MSGBOX_PREVIOUS   64

class GuiObject;
class SkinWnd;


typedef struct {
  char *txt;
  int id;
} _btnstruct;

class COMEXP MsgBox {
  public:
    
    MsgBox(const char *text, const char *title="Alert", int flags=MSGBOX_OK, const char *notanymore=NULL);
    virtual ~MsgBox();
    virtual int run();


  private:

    void createButtons();
    int reposButtons();
    void addButton(const char *text, int retcode);

    String text, title;
    int flags;
    PtrList<GuiObject> buttons;
    GuiObject *checkbox;
    SkinWnd *sw;
    String notanymore_id;
};

#endif