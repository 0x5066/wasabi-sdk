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

#ifndef _ROOTWND_H
#define _ROOTWND_H

#include "std.h"

#include "dispatch.h"

class CanvasBase;
class Region;
class TimerClient;
class Dependent;
class GuiObject;
class FindObjectCallback;

// only these methods are safe across dll boundaries
// this is the pointer you find in the GWL_USERDATA of a window

class DragInterface;

enum WndPreferences {
  SUGGESTED_X,
  SUGGESTED_Y,
  SUGGESTED_W,
  SUGGESTED_H,
  MINIMUM_W,
  MINIMUM_H,
  MAXIMUM_W,
  MAXIMUM_H,
};

class COMEXP NOVTABLE RootWnd : public Dispatchable {
protected:
  RootWnd() {}	// protect constructor
public:
  static const GUID *depend_getClassGuid() {
    // {11981849-61F7-4158-8283-DA7DD006D732}
    static const GUID ret = 
{ 0x11981849, 0x61f7, 0x4158, { 0x82, 0x83, 0xda, 0x7d, 0xd0, 0x6, 0xd7, 0x32 } };
    return &ret;
  }
#ifdef WIN32

  // this passes thru to the windows WndProc, if there is one -- NONPORTABLE
  virtual LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)=0;

  // get the HWND (if there is one) -- NONPORTABLE
  HWND gethWnd();
#endif
  void performBatchProcesses(); // this is called after wndProc is called (under win32) to recompute batch operations such as calculating window regions, cascaderepainting, etc. this prevents N children from independently calling repaintTree for the whole gui on overlaping zones of the framebuffer. under OSes other than win32, this should be called after you've executed all your window events for this poll
  TimerClient *getTimerClient();

  const char *getRootWndName();
  const char *getId();

  int init(RootWnd *parent, int nochild=FALSE);
  int isInited();	// are we post init() ? USE THIS INSTEAD OF gethWnd()==1
  int isPostOnInit();	// are we post onInit() ? USE THIS INSTEAD OF ISINITED TO KNOW IF YOU CAN CALL ONRESIZE (if a function can be called by onInit and other parts of the code at the same time)

  int isClickThrough();
  int onMouseMove(int x, int y);
  int onLeftButtonUp(int x, int y);
  int onRightButtonUp(int x, int y);
  int onLeftButtonDown(int x, int y);
  int onRightButtonDown(int x, int y);
  int onLeftButtonDblClk(int x, int y);
  int onRightButtonDblClk(int x, int y);

  // fetch the DragInterface of the RootWnd here, can be NULL
  DragInterface *getDragInterface();

  int getCursorType(int x, int y);

  // returns deepest child for point or yourself if no child there
  RootWnd *rootWndFromPoint(POINT *pt);

  void getClientRect(RECT *);
  void getNonClientRect(RECT *rect);
  // the onscreen coordinates
  void getWindowRect(RECT *r);

  void setVisible(int show);
  void onSetVisible(int show);
  int isVisible(int within=0);
  int getSkinId();

  void *getInterface(GUID interface_guid);
  int onMetricChange(int metricid, int param1=0, int param2=0);

  // painting stuff
  void invalidate();
  void invalidateRect(RECT *r);
  void invalidateRgn(Region *r);
  void invalidateFrom(RootWnd *who);
  void invalidateRectFrom(RECT *r, RootWnd *who);
  void invalidateRgnFrom(Region *r, RootWnd *who);
  void validate();
  void validateRect(RECT *r);
  void validateRgn(Region *reg);
  int onPaint(CanvasBase *canvas, Region *r);
  int paintTree(CanvasBase *canvas, Region *r);
  RootWnd *getParent();
  RootWnd *getRootParent();
  RootWnd *getDesktopParent();
  void setParent(RootWnd *newparent);
  int onSiblingInvalidateRgn(Region *r, RootWnd *who, int who_idx, int my_idx);
  int wantSiblingInvalidations();
  int wantAutoContextMenu();
  int cascadeRepaintFrom(RootWnd *who, int pack=1);
  int cascadeRepaintRgnFrom(Region *reg, RootWnd *who, int pack=1);
  int cascadeRepaintRectFrom(RECT *r, RootWnd *who, int pack=1);
  int cascadeRepaint(int pack=1);
  int cascadeRepaintRgn(Region *reg, int pack=1);
  int cascadeRepaintRect(RECT *r, int pack=1);
  void repaint();
  RootWnd *getBaseTextureWindow();
  int onDeferredCallback(int param1, int param2);
  int childNotify(RootWnd *child, int msg, int p1, int p2);
  void broadcastNotify(RootWnd *wnd, int msg, int p1, int p2);
  int getPreferences(int what);
  void setPreferences(int what, int v);
  Region *getRegion();
  void invalidateWindowRegion();
  Region *getComposedRegion();
  Region *getSubtractorRegion();
  int getRegionOp();
  void setRegionOp(int op);
  int isRectRgn();
  void setRectRgn(int rrgn);
  int getDesktopAlpha();
  void setStartHidden(int sh);
  void setClickThrough(int ct);

  double getRenderRatio();
  void setRenderRatio(double r);
  int handleRatio();
  void resize(int x, int y, int w, int h, int wantcb=1);
  inline void resizeToRect(RECT *r) {
    resize(r->left, r->top, r->right - r->left, r->bottom - r->top);
  }
  void move(int x, int y);
  void getPosition(POINT *pt);
  RootWnd *getForwardWnd();

  // children registration/enumeration
  void registerRootWndChild(RootWnd *child);
  void unregisterRootWndChild(RootWnd *child);
  RootWnd *findRootWndChild(int x, int y, int only_virtuals=0);
  RootWnd *enumRootWndChildren(int _enum);
  int getNumRootWndChildren();

  // virtual child stuff
  int isVirtual();
  void bringVirtualToFront(RootWnd *w);
  void bringVirtualToBack(RootWnd *w);
  void bringVirtualAbove(RootWnd *w, RootWnd *b);
  void bringVirtualBelow(RootWnd *w, RootWnd *b);
  int checkDoubleClick(int button, int x, int y);

  int beginCapture();
  int endCapture();
  int getCapture();	// returns 1 if this window has mouse/keyb captured
  void onCancelCapture();
  void cancelCapture();
  void setVirtualChildCapture(RootWnd *child);
  RootWnd *getVirtualChildCapture();

  int ptInRegion(int x, int y);

  void clientToScreen(int *x, int *y); // so rootWndFromPoint can map ratio
  void screenToClient(int *x, int *y); // ..	
  
  int getNotifyId();

  int onActivate();
  int onDeactivate();
  int isActive();
  int handleTransparency();
  int handleDesktopAlpha();
  void enable(int e);
  int getEnabled();
  int getPaintingAlpha();           
  void getAlpha(int *activealpha=NULL, int *inactivealpha=NULL);
  void setAlpha(int activealpha, int inactivealpha=-1); // -1 means same as activealpha
  void setTip(const char *tip);

  int runModal();
  void endModal(int retcode);

  void bringToFront();
  void bringToBack();
  void setFocus(int skipnextautochildfocus=0);
  int gotFocus();
  RootWnd *getNextVirtualFocus(RootWnd *w);
  void setVirtualChildFocus(RootWnd *w);
  int wantFocus();


  int onAcceleratorEvent(const char *name);
  int onChar(unsigned int c);
  int onKeyDown(int keycode);
  int onKeyUp(int keycode);
  int onSysKeyDown(int keyCode, int keyData);
  int onSysKeyUp(int keyCode, int keyData);
  int onKillFocus();
  int onGetFocus();

  Dependent *getDependencyPtr();
  void addMinMaxEnforcer(RootWnd *w);
  void removeMinMaxEnforcer(RootWnd *w);
  RootWnd *enumMinMaxEnforcer(int n);
  int getNumMinMaxEnforcers();
  void signalMinMaxEnforcerChanged();

  int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);

  int bypassModal();
  void setRenderBaseTexture(int r);
  int getRenderBaseTexture();

  GuiObject *getGuiObject(); // not guaranteed non null
  int getFlag(int flag);
  int triggerEvent(int event, int p1=0, int p2=0);

  int allowDeactivation();
  void setAllowDeactivation(int allow);

  RootWnd *findWindow(const char *id);
  RootWnd *findWindowByInterface(GUID interface_guid);
  RootWnd *findWindowByCallback(FindObjectCallback *cb);
  RootWnd *findWindowChain(FindObjectCallback *cb, RootWnd *wcaller);

  enum {
    Event_SETVISIBLE=100,	// param 2 is 1 or 0
    Event_ONPAINT=200, // param 2 is PaintCallback::BEFOREPAINT or PaintCallback::AFTERPAINT
  };

  enum {
    BATCHPROCESSES = 50,
    GETHWND		= 100,
    GETROOTWNDNAME	= 110,
    GETID = 120,
    GETDRAGINTERFACE	= 200,
    FROMPOINT		= 300,
    GETWINDOWRECT	= 400,
    ISVISIBLE		= 501,
    GETSKINID		= 600,
    ONMETRICCHANGE	= 700,
    ONPAINT		= 800,
    PAINTTREE = 900,
    GETPARENT = 1000,
    GETROOTPARENT = 1001,
    SETPARENT = 1002,
    GETDESKTOPPARENT = 1003,
    ONDEFERREDCALLBACK = 1100,
    CHILDNOTIFY = 1200,
    BROADCASTNOTIFY = 1210,
    GETPREFERENCES = 1300,
    SETPREFERENCES = 1310,
    BEGINCAPTURE = 1400,
    ENDCAPTURE = 1401,
    GETCAPTURE = 1402,
    CLICKTHROUGH = 1500,
    GETFORWARDWND = 1501,
    SETCLICKTHROUGH = 1502,
    CLIENTSCREEN = 1600,
    SCREENCLIENT = 1601,
    INIT = 1700,
    ISINITED = 1701,
    ISPOSTONINIT = 1702,
    GETCURSORTYPE = 1800,
    GETINTERFACE = 1850,
    GETCLIENTRECT = 1900,
    GETNONCLIENTRECT = 1901,
    SETVISIBLE = 2000,
    ONSETVISIBLE = 2001,
    INVALIDATE = 2100,
    INVALIDATERECT = 2101,
    INVALIDATERGN = 2102,
    INVALIDATEFROM = 2103,
    INVALIDATERECTFROM = 2104,
    INVALIDATERGNFROM = 2105,
    VALIDATE = 2200,
    VALIDATERECT = 2201,
    VALIDATERGN = 2202,
    ONSIBINVALIDATE = 2300,
    WANTSIBINVALIDATE = 2301,
    CASCADEREPAINTFROM = 2400,
    CASCADEREPAINTRECTFROM = 2401,
    CASCADEREPAINTRGNFROM = 2402,
    CASCADEREPAINT = 2403, 
    CASCADEREPAINTRECT = 2405,
    CASCADEREPAINTRGN = 2406,
    REPAINT = 2500,
    GETTEXTUREWND = 2600,
    ONACTIVATE = 2700,
    ONDEACTIVATE = 2800,
    ISACTIVATED = 2801,
    GETPOSITION = 2900,
    GETREGION = 3000,
    GETREGIONOP = 3001,
    INVALWNDRGN = 3002,
    GETCOMPOSEDRGN = 3003,
    GETSUBTRACTORRGN = 3010,
    SETREGIONOP = 3004,
    SETRECTRGN = 3005,
    ISRECTRGN = 3006,
    GETDESKTOPALPHA = 3100,
    //SETPARAM = 3200, // deprecated, use the xmlobject interface
    HANDLETRANSPARENCY = 3300,
    HANDLEDESKTOPALPHA = 3400,
    SETSTARTHIDDEN = 3500,
    GETRENDERRATIO = 3600,
    SETRENDERRATIO = 3610,
    HANDLERATIO = 3620,
    //_RESIZE = 3700, // deprecated, cut
    _RESIZE = 3701,
    _MOVE = 3710,
    CHECKDBLCLK = 3800,
    REGISTERROOTWNDCHILD = 3960,
    UNREGISTERROOTWNDCHILD = 3965,
    FINDROOTWNDCHILD= 3970,
    ENUMROOTWNDCHILDREN = 3975,
    GETNUMROOTWNDCHILDREN = 3980,
    ISVIRTUAL = 3950,
    BRINGVTOFRONT = 4000,
    BRINGVTOBACK = 4010,
    BRINGVABOVE = 4020,
    BRINGVBELOW = 4030,
    SETVCAPTURE = 4100,
    GETVCAPTURE = 4110,
    SETVTIMER = 4200,
    KILLVTIMER = 4210,
    PTINREGION = 4400,
    ONLBDBLCLK = 4500,
    ONRBDBLCLK = 4510,
    ONLBUP = 4520,
    ONRBUP = 4530,
    ONLBDOWN = 4540,
    ONRBDOWN = 4550,
    ONMOUSEMOVE = 4560,
    CLIENTTOSCREEN = 4600,
    SCREENTOCLIENT = 4610,
    GETNOTIFYID = 4700,
    ENABLE = 4800,
    GETENABLED = 4810,
    SETALPHA = 4900,
    GETALPHA = 4910,
    GETPAINTALPHA = 4911,
    SETTOOLTIP = 4920,
    RUNMODAL = 5000,
    ENDMODAL = 5010,
    WANTAUTOCONTEXTMENU = 5100,
    ONCANCELCAPTURE = 5200,
    CANCELCAPTURE = 5210,
    BRINGTOFRONT = 5300,
    BRINGTOBACK = 5310,
    SETFOCUS = 5400,
    GOTFOCUS = 5410,
    GETNEXTVFOCUS = 5420,
    SETVFOCUS = 5430,
    ONKILLFOCUS = 5440,
    ONGETFOCUS = 5450,
    WANTFOCUS = 5460,
    ONKEYDOWN = 5500,
    ONKEYUP = 5510,
    ONCHAR = 5520,
    ONACCELERATOREVENT = 5530,
    GETTIMERCLIENT = 5600,
    GETDEPENDENCYPTR = 6000,
    ADDMINMAXENFORCER = 6400,
    REMOVEMINMAXENFORCER = 6410,
    GETNUMMINMAXENFORCERS = 6420,
    ENUMMINMAXENFORCER = 6430,
    SIGNALMINMAXCHANGED = 6440,
    ROOTWNDONACTION = 6300,
    BYPASSMODAL = 6500,
    SETRENDERBASETEXTURE = 6600,
    GETRENDERBASETEXTURE = 6610,
    GETGUIOBJECT = 6620,
    ONSYSKEYDOWN = 6630,
    ONSYSKEYUP = 6640,
    GETFLAG = 6650,
    TRIGGEREVENT = 6660,
    SETALLOWDEACTIVATE = 6700,
    ALLOWDEACTIVATE=6710,
    FINDWND_BYID=6800,
    FINDWND_BYGUID=6810,
    FINDWND_BYCB=6820,
    FINDWNDCHAIN=6830,
  };
};

class COMEXP NOVTABLE RootWndI : public RootWnd {
protected:
  RootWndI() {}	// protect constructor

public:
#ifdef WIN32
  virtual HWND gethWnd()=0;
#endif
  virtual void performBatchProcesses()=0;
  virtual TimerClient *getTimerClient()=0;
  virtual const char *getRootWndName()=0;
  virtual const char *getId()=0;
  virtual int init(RootWnd *parent, int nochild)=0;
  virtual int isInited();
  virtual int isPostOnInit();

  virtual int isClickThrough()=0;
  virtual int onMouseMove(int x, int y)=0;
  virtual int onLeftButtonUp(int x, int y)=0;
  virtual int onRightButtonUp(int x, int y)=0;
  virtual int onLeftButtonDown(int x, int y)=0;  
  virtual int onRightButtonDown(int x, int y)=0;
  virtual int onLeftButtonDblClk(int x, int y)=0;
  virtual int onRightButtonDblClk(int x, int y)=0;

  virtual DragInterface *getDragInterface()=0;

  virtual int getCursorType(int x, int y)=0;
  virtual RootWnd *rootWndFromPoint(POINT *pt)=0;

  virtual void getClientRect(RECT *r)=0;
  virtual void getNonClientRect(RECT *rect)=0;
  virtual void getWindowRect(RECT *r)=0;

  virtual void setVisible(int show)=0;
  virtual void onSetVisible(int show)=0;
  virtual int isVisible(int within=0)=0;
  virtual int getSkinId()=0;

  virtual void *getInterface(GUID interface_guid)=0;
  virtual int onMetricChange(int metricid, int param1, int param2)=0;

  virtual void invalidate()=0;
  virtual void invalidateRect(RECT *r)=0;
  virtual void invalidateRgn(Region *r)=0;
  virtual void invalidateFrom(RootWnd *who)=0;
  virtual void invalidateRectFrom(RECT *r, RootWnd *who)=0;
  virtual void invalidateRgnFrom(Region *r, RootWnd *who)=0;
  virtual void validate()=0;
  virtual void validateRect(RECT *r)=0;
  virtual void validateRgn(Region *reg)=0;

  virtual int rootwnd_onPaint(CanvasBase *canvas, Region *r)=0;
  virtual int rootwnd_paintTree(CanvasBase *canvas, Region *r)=0;

  virtual RootWnd *getParent()=0;
  virtual RootWnd *getRootParent()=0;
  virtual RootWnd *getDesktopParent()=0;
  virtual void setParent(RootWnd *parent)=0;

  virtual int onSiblingInvalidateRgn(Region *r, RootWnd *who, int who_idx, int my_idx)=0;
  virtual int wantSiblingInvalidations()=0;

  virtual int cascadeRepaintFrom(RootWnd *who, int pack=1)=0;
  virtual int cascadeRepaintRgnFrom(Region *reg, RootWnd *who, int pack=1)=0;
  virtual int cascadeRepaintRectFrom(RECT *r, RootWnd *who, int pack=1)=0;
  virtual int cascadeRepaint(int pack=1)=0;
  virtual int cascadeRepaintRgn(Region *reg, int pack=1)=0;
  virtual int cascadeRepaintRect(RECT *r, int pack=1)=0;
  virtual void repaint()=0;

  virtual void setClickThrough(int ct)=0;
  virtual RootWnd *getBaseTextureWindow()=0;

  virtual int onDeferredCallback(int param1, int param2)=0;
  virtual int childNotify(RootWnd *child, int msg, int p1, int p2)=0;
  virtual void broadcastNotify(RootWnd *wnd, int msg, int p1, int p2)=0;
  virtual int getPreferences(int what)=0;
  virtual void setPreferences(int what, int v)=0;

  virtual Region *getRegion()=0;
  virtual int getRegionOp()=0;
  virtual void setRegionOp(int op)=0;
  virtual int isRectRgn()=0;
  virtual void setRectRgn(int rrgn)=0;
  virtual void invalidateWindowRegion()=0;
  virtual Region *getComposedRegion()=0;
  virtual Region *getSubtractorRegion()=0;

  virtual int getDesktopAlpha()=0;

  virtual void setStartHidden(int sh)=0;
  virtual double getRenderRatio()=0;
  virtual void setRenderRatio(double r)=0;
  virtual int handleRatio()=0;
  virtual void resize(int x, int y, int w, int h, int wantcb=1)=0;
  virtual void move(int x, int y)=0;

  virtual void getPosition(POINT *pt)=0;
  virtual RootWnd *getForwardWnd()=0;

  virtual void registerRootWndChild(RootWnd *child)=0;
  virtual void unregisterRootWndChild(RootWnd *child)=0;
  virtual RootWnd *findRootWndChild(int x, int y, int only_virtuals=0)=0;
  virtual RootWnd *enumRootWndChildren(int _enum)=0;
  virtual int getNumRootWndChildren()=0;

  virtual int isVirtual()=0;

  virtual void bringVirtualToFront(RootWnd *w)=0;
  virtual void bringVirtualToBack(RootWnd *w)=0;
  virtual void bringVirtualAbove(RootWnd *w, RootWnd *b)=0;
  virtual void bringVirtualBelow(RootWnd *w, RootWnd *b)=0;
  virtual int checkDoubleClick(int button, int x, int y)=0;
  
  virtual int beginCapture()=0;
  virtual int endCapture()=0;
  virtual int getCapture()=0;
  virtual void onCancelCapture()=0;
  virtual void cancelCapture()=0;

  virtual void clientToScreen(int *x, int *y)=0; 
  virtual void screenToClient(int *x, int *y)=0; 

  virtual void setVirtualChildCapture(RootWnd *child)=0;
  virtual RootWnd *getVirtualChildCapture()=0;

  virtual int ptInRegion(int x, int y)=0;

  virtual int onActivate()=0;
  virtual int onDeactivate()=0;
  virtual int isActive()=0;
  virtual int handleTransparency()=0;
  virtual int handleDesktopAlpha()=0;

  virtual int getNotifyId()=0;
  virtual void enable(int en)=0;
  virtual int getEnabled()=0;

  virtual int getPaintingAlpha()=0; 
  virtual void getAlpha(int *active=NULL, int *inactive=NULL)=0; 
  virtual void setAlpha(int activealpha, int inactivealpha=-1)=0; 

  virtual void setTip(const char *tip)=0;
  virtual int runModal()=0;
  virtual void endModal(int retcode)=0;
  virtual int wantAutoContextMenu()=0;

  virtual void bringToFront()=0;
  virtual void bringToBack()=0;
  virtual void setFocus(int skipnextautochildfocus=0)=0;
  virtual int gotFocus()=0;
  virtual RootWnd *getNextVirtualFocus(RootWnd *w)=0;
  virtual void setVirtualChildFocus(RootWnd *w)=0;
  virtual int wantFocus()=0;

  virtual int onAcceleratorEvent(const char *name)=0;

  virtual int onChar(unsigned int c)=0;
  virtual int onKeyDown(int keycode)=0;
  virtual int onKeyUp(int keycode)=0;
  virtual int onSysKeyDown(int keyCode, int keyData)=0;
  virtual int onSysKeyUp(int keyCode, int keyData)=0;
  virtual int onKillFocus()=0;
  virtual int onGetFocus()=0;
  virtual Dependent *rootwnd_getDependencyPtr()=0;
  virtual void addMinMaxEnforcer(RootWnd *w)=0;
  virtual void removeMinMaxEnforcer(RootWnd *w)=0;
  virtual RootWnd *enumMinMaxEnforcer(int n)=0;
  virtual int getNumMinMaxEnforcers()=0;
  virtual void signalMinMaxEnforcerChanged()=0;

  virtual int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL)=0;

  virtual int bypassModal()=0;

  virtual void setRenderBaseTexture(int r)=0;
  virtual int getRenderBaseTexture()=0;
  virtual GuiObject *getGuiObject()=0; // not guaranteed non null

  virtual int getFlag(int flag)=0;
  virtual int triggerEvent(int event, int p1, int p2)=0;

  virtual int allowDeactivation();
  virtual void setAllowDeactivation(int allow);

  virtual RootWnd *findWindow(const char *id)=0;
  virtual RootWnd *findWindowByInterface(GUID interface_guid)=0;
  virtual RootWnd *findWindowByCallback(FindObjectCallback *cb)=0;
  virtual RootWnd *findWindowChain(FindObjectCallback *cb, RootWnd *wcaller)=0;

protected:
  RECVS_DISPATCH;
};

#endif
