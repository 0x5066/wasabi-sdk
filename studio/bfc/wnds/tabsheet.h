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

#ifndef _TABSHEET_H
#define _TABSHEET_H

#include "grouptgbutton.h"
#include "buttwnd.h"
#include "../../common/common.h"
#include "../../common/guiobjwnd.h"
#include "../ptrlist.h"

class ButtonWnd;
class SkinBitmap;
class TabButtonBase;
class ButtBar;
class TabSheetBar;

#define TABSHEET_GROUPS -2
#define TABSHEET_NOTABS -3

#define TABSHEET_PARENT GuiObjectWnd
/**
  class TabSheet

  @short A TabSheet Control.
  @author Nullsoft
  @ver 1.0
  @see 
  @cat SDK
*/
class COMEXP TabSheet : public TABSHEET_PARENT {
public:
  /**
    TabSheet constructor
  
    @see ~TabSheet()
    @param bbtype The type of button bar to use in the tabsheet.
  */
  TabSheet(int bbtype=-1);
  
  /**
    TabSheet destructor 

    @see TabSheet()
  */
  virtual ~TabSheet();

  /**
    TabSheet method onInit
  
    @ret 1
  */
  virtual int onInit();
  
  /**
    TabSheet method getClientRect
  
    @param r A pointer to the RECT that will be filled.
  */
  virtual void getClientRect(RECT *);
  
  /**
    TabSheet method onPaint
  
    @ret 1
    @param canvas The canvas upon which we'll paint ourself.
  */
  virtual int onPaint(Canvas *canvas);

  /**
    TabSheet method onResize
  
    @ret 1
  */
  virtual int onResize();
  
  /**
    TabSheet method setButtonType .
  
    @param type The button type.
  */
  void setButtonType(int type);

  /**
    TabSheet method setTabRowMargin
  
    @assert newmargin is non-negative
    @param newmargin The new margin width in pixels.
  */
  void setTabRowMargin(int pixels);
  
  /**
    TabSheet method addChild
  
    @ret One less than the number of tabs
    @param newchild A pointer to the new child window to add.
    @param tip The tooltip for the button associated with this child.
  */
  int addChild(BaseWnd *newchild, const char *tooltip=NULL);
  
  /**
    TabSheet method activateChild
  
    @see addChild()
    @see killChildren()
    @ret None
    @param newactive A pointer to the child window to render active.
  */
  virtual void activateChild(BaseWnd *activechild);
  
  /**
    TabSheet method killChildren .
  */
  virtual void killChildren();

  /**
    TabSheet method childNotify .

    @ret Returns 1 when complete.
    @param child  The child that's being notified.
    @param msg    The message.
    @param param1 Custom parameter 1.
    @param param2 Custom parameter 2.
  */
  virtual int childNotify(RootWnd *child, int msg,
                          int param1=0, int param2=0);

  void setContentMarginLeft(int cm);
  void setContentMarginTop(int cm);
  void setContentMarginRight(int cm);
  void setContentMarginBottom(int cm);
  
  /**
    TabSheet method enumChild
    @short 
    @author Nullsoft
    @ver 1.0
    @see 
    @assert 
    @algorithm 
    @ret The base window of the specified tab button, or NULL if there is none.
    @except 
    @param child 
  */
  BaseWnd *enumChild(int child);
  
  /**
    TabSheet method getNumChild
    @short 
    @author Nullsoft
    @ver 1.0
    @see 
    @ret The number of tabs
    @except 
    @param None
  */
  int getNumChild();

  BaseWnd *getActiveChild() { return active; }
  virtual void onSetPage(int n) { lastpage = n; }
  int getCurPage() { return lastpage; }
  void setCurPage(int page);
  int getNumPages() { return tabs.getNumItems(); }
  void nextPage() { int n = getCurPage()+1; if (n >= getNumPages()) n = 0; setCurPage(n); }
  void previousPage() { int n = getCurPage()-1; if (n < 0) n = getNumPages()-1; setCurPage(n); }
  int onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source);

protected:
  /**
    TabSheet method enumButton
    @short 
    @author Nullsoft
    @ver 1.0
    @see 
    @ret The specified tab, or NULL if there is none.
    @except 
    @param i 
  */
  TabButtonBase *enumButton(int button);

public:
  /**
    TabSheet method setBackgroundBmp
  
    @param name The name of the bitmap to use.
  */
  void setBackgroundBmp(const char *name); //FG
  SkinBitmap *getBackgroundBitmap(); //FG

protected:
  // you can set these in your constructor, they will be deleted for you
  ButtonWnd *leftscroll, *rightscroll;
  SkinBitmap *background;

private:
  int tabrowheight, tabrowwidth, tabrowmargin;
  PtrList<TabButtonBase> tabs;

  ButtBar *bb;
  TabSheetBar *tsb;
  GuiObjectWnd *contentwnd;

  int tilex, tilew;

  BaseWnd *active;
  int type;
  int content_margin_left, content_margin_top, content_margin_right, content_margin_bottom;
  int lastpage;
};

class TabButtonBase {
  public:
    TabButtonBase(BaseWnd *linkWnd, TabSheet *par, const char *tip=NULL);
    virtual ~TabButtonBase();

    BaseWnd *getBaseWnd() const { return linked; }
    void setNoDeleteLinked(int i) { nodeletelinked = i; }

    virtual void btn_setHilite(int tf)=0;
    virtual void btn_setText(const char *txt)=0;

  protected:
    BaseWnd *linked;
    TabSheet *parent;
    int nodeletelinked;
};

#define TABBUTTON_PARENT ButtonWnd

/**
  Class TabButton
  
  @short 
  @author Nullsoft
  @ver 1.0
  @see TabButtonBase
  @cat SDK
*/
class TabButton : public TABBUTTON_PARENT, public TabButtonBase {
public:
  /**
    TabButton constructor 
  
    @assert The BaseWnd passed to this method must previously be linked.
    @param linkwnd The window to associate with this button.
    @param par A pointer to the parent tabsheet.
    @param tip The tooltip for the window associated with this button.
  */
  TabButton(BaseWnd *linkWnd, TabSheet *par, const char *tip=NULL) : TabButtonBase(linkWnd, par, tip) {
    if (tip != NULL) setTip(tip);
  }
  
  /**
    TabButton method onInit

    @ret 1
  */
  virtual int onInit();
  
  /**
    TabButton method onLeftPush .

    @assert parent and linked both exist.
    @param x The X position, of the mouse pointer, in the client screen.
    @param y The Y position, of the mouse pointer, in the client screen.
  */
  virtual void onLeftPush(int x, int y);
  virtual void btn_setHilite(int tf);
  virtual void btn_setText(const char *text);
};


#define GROUPTABBUTTON_PARENT GroupToggleButton
class GroupTabButton : public GROUPTABBUTTON_PARENT, public TabButtonBase {
public:
  GroupTabButton(BaseWnd *linkWnd, TabSheet *par, const char *tip=NULL) : TabButtonBase(linkWnd, par, tip) {   
    setGroups("wasabi.tabsheet.button.selected.group", "wasabi.tabsheet.button.unselected.group");
  }
  virtual int wantFullClick() { return 0; }
  virtual int wantAutoToggle() { return 0; }
  virtual int onInit();
  virtual void grouptoggle_onLeftPush();
  virtual void btn_setHilite(int tf);
  virtual void btn_setText(const char *text);
};

#endif
