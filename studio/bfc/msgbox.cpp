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

#include "msgbox.h"
#include "wnds/skinwnd.h"
#include "../common/guiobjwnd.h"
#include "../common/script/scriptguid.h"
#include "../common/script/c_script/c_container.h"
#include "../common/script/c_script/c_text.h"
#include "../common/script/c_script/c_button.h"

_btnstruct msgboxbtns[] = {
  { "OK", MSGBOX_OK },
  { "Cancel", MSGBOX_CANCEL },
  { "Yes", MSGBOX_YES },
  { "No", MSGBOX_NO },
  { "All", MSGBOX_ALL },
  { ">>", MSGBOX_NEXT },
  { "<<", MSGBOX_PREVIOUS },
};

MsgBox::MsgBox(const char *_text, const char *_title, int _flags, const char *notanymore) {
  text = _text;
  title = _title;
  flags = _flags;
#ifdef WIN32
#ifdef _DEBUG
  OutputDebugString(StringPrintf("msgbox: %s: %s", title, text));
#endif
#endif
  notanymore_id = notanymore;
  sw = NULL;
  if (flags == 0) flags = MSGBOX_OK;
}

MsgBox::~MsgBox() {
  foreach(buttons)
    api->xui_delete(buttons.getfor());
  endfor
}

int MsgBox::run() {

  int _r = -1;
  if (!notanymore_id.isempty()) {
    char txt[4096];
    wsprintf(txt, "msgbox_defaultvalue_%s", notanymore_id);
    if ((GetKeyState(VK_SHIFT) & 0x8000)) 
      api->setIntPublic(txt, -1);
    _r = api->getIntPublic(txt, -1);
  }

  if (_r == -1) {  
    sw = new SkinWnd("msgbox.custom.group", "modal", FALSE, NULL, 1, 1);
    if (!sw->getWindow()) return -1;
    RootWnd *grp = sw->getWindow();
    RootWnd *l = grp->getDesktopParent();

    GuiObject *_p = sw->findObject("msgbox.custom.group");
    RootWnd *p = _p->guiobject_getRootWnd();

    C_Container cont(sw->getContainer());
    cont.setName(title);

    createButtons();
    int min_w = reposButtons();

    GuiObject *go_txt = sw->findObject("msgbox.text");
    if (go_txt != NULL) {
      GuiObject *to = go_txt->guiobject_findObject("wasabi.text");
      GuiObject *gto = sw->findObject("text");
      if (to != NULL && gto != NULL) {
        go_txt->guiobject_setXmlParam("text", text);
        C_GuiObject t(*go_txt);
        int _w = t.getAutoWidth();
        int _h = t.getAutoHeight();

        to->guiobject_setXmlParam("w", StringPrintf("%d", _w));
        to->guiobject_setXmlParam("h", StringPrintf("%d", _h));
        to->guiobject_setXmlParam("relatw", "0");
        to->guiobject_setXmlParam("relath", "0");

        int x, rx, w, rw;
        int y, ry, h, rh;
        int gtow, gtoh;

        go_txt->guiobject_getGuiPosition(&x, &y, &w, &h, &rx, &ry, &rw, &rh);
        if (rw == 1)
          _w += -w; 
        else 
          _w += x*2; 
        if (rh == 1)
          _h += -h; 
        else 
          _h += y*2; 

        gtow = _w;
        gtoh = _h;

        GuiObject *grpo = grp->getGuiObject();
        ASSERT(grpo != NULL);
        gto->guiobject_getGuiPosition(&x, &y, &w, &h, &rx, &ry, &rw, &rh);
        if (rw == 1)
          _w += -w; 
        else 
          _w += x*2; 
        if (rh == 1)
          _h += -h; 
        else 
          _h += y*2; 

        gto->guiobject_setXmlParam("w", StringPrintf("%d", gtow));
        gto->guiobject_setXmlParam("h", StringPrintf("%d", gtoh));

        grpo->guiobject_setXmlParam("w", StringPrintf("%d", _w));
        grpo->guiobject_setXmlParam("h", StringPrintf("%d", _h));
        grpo->guiobject_setXmlParam("lockminmax", "1");
        grpo->guiobject_setXmlParam("propagatesize", "1");
        XmlObject *xl = static_cast<XmlObject *>(l->getInterface(xmlObjectGuid));
        xl->setXmlParam("minimum_h", StringPrintf("%d", _h));
        xl->setXmlParam("minimum_w", StringPrintf("%d", (_w < min_w) ? min_w : _w));
      }
    }

    if (!notanymore_id.isempty()) {
      GuiObject *o = api->xui_new("Wasabi:CheckBox"); // that'll be deleted automatically when our parent group destroys
      if (o != NULL) {
        RootWnd *w = o->guiobject_getRootWnd();
        C_GuiObject go(*o);
        go.init(_p->guiobject_getScriptObject());
        o->guiobject_setXmlParam("text", "Do not show this message anymore");
        o->guiobject_setXmlParam("y", "-50");
        o->guiobject_setXmlParam("relaty", "1");
        o->guiobject_setXmlParam("x", "12");
        o->guiobject_setXmlParam("relatx", "0");
        o->guiobject_setXmlParam("w", "-24");
        o->guiobject_setXmlParam("relatw", "1");
        min_w = MAX(w->getPreferences(SUGGESTED_W), min_w);
      }
    }

    sw->notifyMinMaxChanged();
    reposButtons();
    _r = sw->runModal(1);
    sw->destroy();
  }

    if (!notanymore_id.isempty() && _r != -1 && sw != NULL) {
      GuiObject *o = sw->findObject("checkbox.toggle");
      if (o != NULL) {
        C_Button b(*o);
        if (b.getActivated()) {
          char txt[4096];
          wsprintf(txt, "msgbox_defaultvalue_%s", notanymore_id);
          api->setIntPublic(txt, _r);
        }
      }
    }

  return _r;
}

void MsgBox::addButton(const char *text, int retcode) { 
  GuiObject *o = api->xui_new("Wasabi:Button"); // that wil NOT be deleted automatically when our parent group destroys because we did not init with guiobject
  if (o != NULL) {
    o->guiobject_setXmlParam("action", "endmodal");
    o->guiobject_setXmlParam("retcode", StringPrintf("%d", retcode));
    o->guiobject_setXmlParam("text", text);
    buttons.addItem(o); 
  }
}

void MsgBox::createButtons() {
  GuiObject *_p = sw->findObject("msgbox.custom.group");
  if (!_p) return;

  ASSERT(buttons.getNumItems() == 0);
  buttons.deleteAll();

  for (int i=0;i<sizeof(msgboxbtns)/sizeof(_btnstruct);i++) {
    if (flags & msgboxbtns[i].id) {
      addButton(msgboxbtns[i].txt, msgboxbtns[i].id);
    }
  }

  RootWnd *p = _p->guiobject_getRootWnd();
  
  foreach(buttons) 
    RootWnd *wnd = buttons.getfor()->guiobject_getRootWnd();
    if (wnd != NULL) {
      wnd->setStartHidden(1);
      wnd->setParent(p);
      wnd->init(p);
    }
  endfor;
}

int MsgBox::reposButtons() {
  RECT r;
  GuiObject *_p = sw->findObject("msgbox.custom.group");
  RootWnd *p = _p->guiobject_getRootWnd();
  p->getClientRect(&r);

  int shift = 0;

  int _w = 0;
  int _h = 0;
  for (int i=buttons.getNumItems()-1;i>=0;i--) {
    RootWnd *wnd = buttons.enumItem(i)->guiobject_getRootWnd();
    if (wnd != NULL) {
      int _w = wnd->getPreferences(SUGGESTED_W);
      int _h = wnd->getPreferences(SUGGESTED_H);
      if (_w == AUTOWH) _w = -1;
      int w = max(_w, 64);
      wnd->resize(r.right - w - 16 - shift, r.bottom - 8 - _h, w, _h);
      _w = MAX(w, _w);
      _h = MAX(_h, _h);
      shift += w + 4;
    }
  }

  foreach(buttons) 
    RootWnd *wnd = buttons.getfor()->guiobject_getRootWnd();
    if (wnd != NULL) 
      wnd->setVisible(1);
  endfor;
  return shift;
}
