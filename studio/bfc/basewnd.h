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

//NONPORTABLE
#ifndef _BASEWND_H
#define _BASEWND_H

#include "std.h"
#include "../common/common.h"
#include "rootwnd.h"
#include "drag.h"
#include "../bfc/ptrlist.h"
#include "../bfc/tlist.h"
#include "named.h"
#include "timerclient.h"
#include "findobjectcb.h"

class BaseWnd;
class BltCanvas;
class FilenamePS;
class Canvas;
class CanvasBase;
class RegionI;
class DragSet;
class VirtualWnd;
class Tooltip;
class svc_toolTipsRenderer;

using namespace wasabi;

// it is safe to use anything higher than this for your own funky messages
#define BASEWND_NOTIFY_USER	NUM_NOTIFY_MESSAGES

#define BASEWND_CURSOR_USERSET			-1// window will do own setting
#define BASEWND_CURSOR_POINTER			1
#define BASEWND_CURSOR_NORTHSOUTH		2
#define BASEWND_CURSOR_EASTWEST			3
#define BASEWND_CURSOR_NORTHWEST_SOUTHEAST	4
#define BASEWND_CURSOR_NORTHEAST_SOUTHWEST	5
#define BASEWND_CURSOR_4WAY			6
#define BASEWND_CURSOR_EDIT			7

// Our own defined window messages

#define WM_WA_CONTAINER_TOGGLED WM_USER+0x1338
#define WM_WA_COMPONENT_TOGGLED WM_USER+0x1339
#define WM_WA_RELOAD            WM_USER+0x133A
#define WM_WA_GETFBSIZE         WM_USER+0x133B

#define DEFERREDCB_FLUSHPAINT   0x200

#define SYSRGN  4

#define REGIONOP_NONE 0
#define REGIONOP_OR   1
#define REGIONOP_AND  2
#define REGIONOP_SUB  -1
#define REGIONOP_SUB2 -2

#define WNDEVENT_ONRESIZE 0x1000

class COMEXP virtualChildTimer {
  public:
    virtualChildTimer(int _id, RootWnd *_child) : id(_id), child(_child) { }
    virtual ~virtualChildTimer() {}
		int id;
		RootWnd *child;
};

class RootWndFinder : public FindObjectCallbackI {
  public:

    RootWndFinder() {  findobject_guid = INVALID_GUID; }
    virtual ~RootWndFinder() {  }

    void reset() { findobject_id = ""; findobject_guid = INVALID_GUID; }
    void setFindId(const char *id) { findobject_id = id; }
    void setFindGuid(GUID guid) { findobject_guid = guid; }

    virtual int findobjectcb_matchObject(RootWnd *w) {
      if (!findobject_id.isempty()) {
        if (w != NULL) {
          const char *id = w->getId();
          return STRCASEEQLSAFE(id, findobject_id);
        }
      } else if (findobject_guid != INVALID_GUID) {
        return (w->getInterface(findobject_guid) != NULL);
      }
      return 0;
    }

  private:

    String findobject_id;
    GUID findobject_guid;
};


// base class
#define BASEWND_PARENT RootWndI
class COMEXP NOVTABLE BaseWnd :
  public RootWndI,
  public DragInterface,
  public Named ,
  public DependentI,
  public TimerClientI {

  friend VirtualWnd;

protected:
  // override constructor to init your data, but don't create anything yet
  BaseWnd();
public:
  virtual ~BaseWnd();

//INITIALIZATION
  // these actually create the window
  // try REALLY hard to not have to override these, and if you do,
  // override the second one
  virtual int init(RootWnd *parent, int nochild=FALSE);
  virtual int init(HINSTANCE hInstance, HWND parent, int nochild=FALSE);// for the root window
  virtual int isInited();	// are we post init() ? USE THIS INSTEAD OF gethWnd()==1
  virtual int isPostOnInit() { return postoninit; }

  // if at all possible put your init stuff in this one, and call up the
  // heirarchy BEFORE your code
  virtual int onInit();
  virtual int onPostOnInit(); // this one is called after onInit so you get a chance to do something after your inheritor override

  // use this to return the cursor type to show
  virtual int getCursorType(int x, int y);

// WINDOW SIZING/MOVEMENT/CONTROL
  virtual int getFontSize() { return 0; }
  virtual int setFontSize(int size) { return -1; }

  // if you override it, be sure to call up the heirarchy
  virtual void resize(int x, int y, int w, int h, int wantcb=1);	// resize yourself
  void resize(RECT *r, int wantcb=1);
  void resizeToRect(RECT *r) { resize(r); }//helper

  // called after resize happens, return TRUE if you override
  virtual int onResize();
  void resizeToClient(BaseWnd *wnd);	// resize a window to match you
  virtual int onPostedMove(); // whenever WM_WINDOWPOSCHANGED happens, use mainly to record positions when moved by the window tracker, avoid using for another prupose, not portable

  // move only, no resize
  virtual void move(int x, int y);

  // puts window on top of its siblings
  virtual void bringToFront();
  // puts window behind its siblings
  virtual void bringToBack();

  // fired when a sibbling invalidates a region. you can change the region before returning, use with care, can fuck up everything if not used well
  virtual int onSiblingInvalidateRgn(Region *r, RootWnd *who, int who_idx, int my_idx) { return 0; }

  // set window's visibility
  virtual void setVisible(int show);
  virtual void onSetVisible(int show);	// override this one

  // enable/disable window for input
  virtual void enable(int en);
  // grab the keyboard focus
  virtual void setFocus(int skipnextautochildfocus=0);
  void setFocusOnClick(int f);

  virtual int pointInWnd(POINT *p);

  // repaint yourself
  virtual void invalidate();	// mark entire window for repainting
  virtual void invalidateRect(RECT *r);// mark specific rect for repainting
  virtual void invalidateRgn(Region *r);// mark specific rgn for repainting
  virtual void invalidateFrom(RootWnd *who);	// mark entire window for repainting
  virtual void invalidateRectFrom(RECT *r, RootWnd *who);// mark specific rect for repainting
  virtual void invalidateRgnFrom(Region *r, RootWnd *who);// mark specific rgn for repainting
  virtual void validate(); // unmark the entire window from repainting
  virtual void validateRect(RECT *r); // unmark specific rect from repainting
  virtual void validateRgn(Region *reg); // unmark specific region from repainting

  // no virtual on these please
  void deferedInvalidateRgn(Region *h);
  void deferedInvalidateRect(RECT *r);
  void deferedInvalidate();
  void deferedValidateRgn(Region *h); 
  void deferedValidateRect(RECT *r);
  void deferedValidate();
  Region *getDeferedInvalidRgn();
  int hasVirtualChildren();
  virtual void setVirtualChildFocus(RootWnd *w);
  virtual RootWnd *getNextVirtualFocus(RootWnd *w);


private: 
  virtual int focusVirtualChild(RootWnd *child);
  virtual void physicalInvalidateRgn(Region *r); 
protected:
  virtual int ensureVirtualCanvasOk();
  virtual void setVirtualCanvas(Canvas *c);
  virtual void setRSize(int x, int y, int w, int h);

public:

  virtual void repaint();	// repaint right now!

  // override this to add decorations
  virtual void getClientRect(RECT *);
  RECT clientRect();	// helper
  virtual void getNonClientRect(RECT *);
  RECT nonClientRect();	// helper
  virtual void getWindowRect(RECT *);	// windows coords in screen system
  RECT windowRect();	// helper
  virtual void getPosition(POINT *pt); // window coord relative to os window (instead of rootparent)

  virtual void *getInterface(GUID interface_guid) { return NULL; }
  virtual void *dependent_getInterface(const GUID *classguid);

  virtual void clientToScreen(int *x, int *y);		// convenience fn
  virtual void screenToClient(int *x, int *y);		// convenience fn
  virtual void clientToScreen(POINT *pt);		// convenience fn
  virtual void screenToClient(POINT *pt);		// convenience fn
  virtual void clientToScreen(RECT *r);		// convenience fn
  virtual void screenToClient(RECT *r);		// convenience fn

  void setIcon(HICON icon, int _small);
  virtual void setSkinId(int id);

  virtual int getPreferences(int what);
  virtual void setPreferences(int what, int v);
  virtual void setStartHidden(int wtf);


// from RootWnd
protected:
  virtual DragInterface *getDragInterface();
  virtual RootWnd *rootWndFromPoint(POINT *pt);
  virtual int getSkinId();
  virtual int onMetricChange(int metricid, int param1, int param2);
  virtual int rootwnd_onPaint(CanvasBase *canvas, Region *r);
  virtual int rootwnd_paintTree(CanvasBase *canvas, Region *r);

public:
#ifdef WIN32
  // override for custom processing (NONPORTABLE!)
  virtual LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
  virtual void performBatchProcesses(); // this is called after wndProc is called (under win32) to recompute batch operations such as calculating window regions, cascaderepainting, etc. this prevents N children from independently calling repaintTree for the whole gui on overlaping zones of the framebuffer. under OSes other than win32, this should be called after you've executed all your window events for this poll, try not to use it, or use a dirty bit to cut down on your job
  virtual const char *getRootWndName();
  virtual const char *getId();
// end from RootWnd

  // or override these, they're more portable
  virtual int onContextMenu(int x, int y);	// always return 1 if accept msg
  virtual int onChildContextMenu(int x, int y);	// always return 1 if accept msg

  // called on WM_PAINT by onPaint(Canvas, Region *), if the canvas is null, create your own, if not, it will have update regions clipped for you
  virtual int onPaint(Canvas *canvas);
  void setTransparency(int amount);	// 0..255
  int getTransparency();

  // override those two
  virtual void timerCallback(int id);
  virtual int onDeferredCallback(int p1, int p2);

  int setTimer(int id, int ms);
  int killTimer(int id);
  void postDeferredCallback(int p1, int p2=0, int mindelay=0);

  // from timerclient
  virtual int timerclient_onDeferredCallback(int p1, int p2);
  virtual void timerclient_timerCallback(int id);
  virtual TimerClient *timerclient_getMasterClient();
  virtual void timerclient_onMasterClientMultiplex();
  virtual TimerClient *getTimerClient();
  virtual const char *timerclient_getName();

private:
  virtual int onPaint(Canvas *canvas, Region *r);

public:  
  // click-drag. FYI, the drag-drop handling overrides these
  virtual int onLeftButtonDown(int x, int y); 
  virtual int onRightButtonDown(int x, int y) { return 0; }
  virtual int onMouseMove(int x, int y);	// only called when mouse captured
  virtual int onLeftButtonUp(int x, int y) { return 0; }
  virtual int onRightButtonUp(int x, int y) { return 0; }
  virtual int onMouseWheelUp(int click, int lines);
  virtual int onMouseWheelDown(int click, int lines);
  virtual int beginCapture();
  virtual int endCapture();
  virtual int getCapture();	// returns 1 if this window has mouse/keyb captured
  virtual void onCancelCapture(); // called when someone steals the capture from you
  virtual void cancelCapture();

  // these will not be called in the middle of a drag operation
  virtual int onLeftButtonDblClk(int x, int y);
  virtual int onRightButtonDblClk(int x, int y);

  virtual int onGetFocus();	// you have the keyboard focus
  virtual int onKillFocus();	// you lost the keyboard focus
  virtual int gotFocus();
  virtual int isActive();
  virtual int onActivate();
  virtual int onDeactivate();
  virtual int onEnable(int en);
  virtual int getEnabled();

  virtual void registerAcceleratorSection(const char *name, int global=0);
  virtual int onAcceleratorEvent(const char *name);

  virtual int onChar(unsigned int c);
  virtual int onKeyDown(int keyCode) { return 0; }
  virtual int onKeyUp(int keyCode) { return 0; }
  virtual int onSysKeyDown(int keyCode, int keyData) { return 0; }
  virtual int onSysKeyUp(int keyCode, int keyData) { return 0; }

  virtual int onEraseBkgnd(HDC dc);// override and return 0 to authorize bkg painting, 1 to avoid it (default to 1)
  virtual int onUserMessage(int msg, int w, int l, int *r);

//CHILD->PARENT MESSAGES
  // feel free to override for your own messages, but be sure to call up the
  // chain for unhandled msgs
  // children should not call this directly if they don't have to; use
  // notifyParent on yourself instead
  // message ids should be put in notifmsg.h to avoid conflicts
  virtual int childNotify(RootWnd *child, int msg,
                          int param1=0, int param2=0);

  // call this to send a notification to all the children (and grandchildren (and grand-grand-children, (and... you get it)))
  virtual void broadcastNotify(RootWnd *wnd, int msg, int p1, int p2);

  // don't try to override these
  void setParent(RootWnd *newparent);
  RootWnd *getParent();
  virtual RootWnd *getRootParent(); 
  virtual RootWnd *getDesktopParent();
  
  // avoid overriding this one if you can
  virtual int reparent(RootWnd *newparent);

  // override this one
  virtual void onSetParent(RootWnd *newparent) {}

  virtual Region *getRegion(); // override to return your client region
  virtual void setRegionOp(int op); // 0 = none, 1 == or, 2=and, 3=xor, -1 = sub, -2 = sub thru groups
  virtual void setRectRgn(int i); // set to 1 if you don't want your region to clip your clicks

  virtual void invalidateWindowRegion(); // call this when your region has changed

  Region *getComposedRegion(); // returns the result of and/or/subing your children regions with your region
  Region *getSubtractorRegion(); // returns the composed subtracting region, that region is automatically subtracted from the desktop root parent's region
  int ptInRegion(int x, int y); // handled automatically if you override getRegion and isRectRgn, but you can still override it if you need
  virtual int getRegionOp(); //
  virtual int isRectRgn();

  // call this to notify your parent via its childNotify method
  virtual int notifyParent(int msg, int param1=0, int param2=0);

  // call this when you have received a childNotify and wish to
  // defer the notification to your own notify object.
  virtual int passNotifyUp(RootWnd *child, int msg,
                          int param1=0, int param2=0);

  // This allows you to set a custom integer ID for any object you control,
  // such that you can use its ID in a switch statement by calling getNotifyId()
  // which is dispatched through RootWnd.
  void setNotifyId(int id);
  virtual int getNotifyId();

  // from class Named
  virtual void onSetName();

  virtual const char *getTip();
  virtual void setTip(const char *tooltip);
  virtual int getStartHidden();
  virtual void abortTip();
  virtual int isVisible(int within=0);

	// Virtual windows functions
  virtual Canvas *createFrameBuffer(int w, int h);
  virtual void prepareFrameBuffer(Canvas *canvas, int w, int h);
  virtual void deleteFrameBuffer(Canvas *canvas);

  virtual void registerRootWndChild(RootWnd *child);
  virtual void unregisterRootWndChild(RootWnd *child);
  virtual RootWnd *findRootWndChild(int x, int y, int only_virtuals=0);
  virtual RootWnd *enumRootWndChildren(int _enum);
  virtual int getNumRootWndChildren();

  virtual int isVirtual();
  virtual RootWnd *enumVirtualChild(int _enum);
  virtual int getNumVirtuals();

  virtual int handleVirtualChildMsg(UINT uMsg,int x, int y, void *p=NULL);
  virtual void setVirtualChildCapture(RootWnd *child);
  virtual RootWnd *getVirtualChildCapture();

  virtual int cascadeRepaintFrom(RootWnd *who, int pack=1);
  virtual int cascadeRepaintRgnFrom(Region *reg, RootWnd *who, int pack=1);
  virtual int cascadeRepaintRectFrom(RECT *r, RootWnd *who, int pack=1);
  virtual int cascadeRepaint(int pack=1);
  virtual int cascadeRepaintRgn(Region *reg, int pack=1);
  virtual int cascadeRepaintRect(RECT *r, int pack=1);
  virtual void flushPaint();

  virtual void onMinimize();
  virtual void onRestore();

  virtual void freeResources();
  virtual void reloadResources();
  virtual int getDesktopAlpha();
  virtual int handleDesktopAlpha() { return isVirtual(); }
  virtual int getPaintingAlpha();           // this one is a hint for painting, it returns either activealpha or inactivealpha
  virtual void setAlpha(int activealpha, int inactivealpha=-1); // -1 means same as activealpha
  virtual void getAlpha(int *activealpha=NULL, int *inactivealpha=NULL);
  virtual int getFlag(int flag);
  virtual int triggerEvent(int event, int p1, int p2);

protected:
  virtual void do_flushPaint();
  virtual int paintTree(Canvas *canvas, Region *r);
  virtual int virtualBeforePaint(Region *r);
  virtual int virtualAfterPaint(Region *r);
  virtual int virtualOnPaint();
  virtual void virtualCanvasCommit(Canvas *canvas, RECT *r, double ratio);
  virtual void setDesktopAlpha(int do_alpha);
  virtual void onSetDesktopAlpha(int a);

public:
	
#ifdef WIN32
  virtual HWND gethWnd();
  //NONPORTABLE--avoid prolonged use
  virtual HINSTANCE gethInstance();//FUCKO CUT, use HINSTANCEfromHWND
#endif

public:
	
  BOOL getNoCopyBits(void);
  void setNoCopyBits(BOOL ncb);
  BltCanvas *scalecanvas;

protected:
  virtual int checkDoubleClick(int button, int x, int y);

//MISC
public:
  virtual int isDestroying();	// in the middle of dying

//DRAGGING AND DROPPING -- (derived from DragInterface)

  // derived windows should call this if they detect a drag beginning
  // call once per datum per type of data being exposed. order is maintained
  int addDragItem(const char *droptype, void *item);
  // returns TRUE if drop was accepted
  int handleDrag();
  int resetDragSet();	// you don't need to call this

  // (called on dest) when dragged item enters the winder
  virtual int dragEnter(RootWnd *sourceWnd);
  // (called on dest) during the winder
  // FG> x/y are in screen corrdinates because target is a rootwnd
  virtual int dragOver(int x, int y, RootWnd *sourceWnd) { return 0; }
  // (called on src)
  virtual int dragSetSticky(RootWnd *wnd, int left, int right, int up, int down);
  // (called on dest) when dragged item leaves the winder
  virtual int dragLeave(RootWnd *sourceWnd) { return 0; }

  // when it finally is dropped:
 
  // called on destination window
  // FG> x/y are in screen corrdinates because target is a rootwnd
  virtual int dragDrop(RootWnd *sourceWnd, int x, int y) { return 0; }
  // called on source window
  virtual int dragComplete(int success) { return 0; }
  // in that order
  // must be called right before handleDrag();		(sender)
  void setSuggestedDropTitle(const char *title);

  // must be called from dragDrop();			(receiver)
  virtual const char *dragGetSuggestedDropTitle(void);
  virtual int dragCheckData(const char *type, int *nitems=NULL);
  virtual void *dragGetData(int slot, int itemnum);
  virtual int dragCheckOption(int option) { return 0; }

  // return TRUE if you support any of the datatypes this window is exposing
  virtual int checkDragTypes(RootWnd *sourceWnd) { return 0; }

// external drops 
  // override this and return 1 to receive drops from the OS
  virtual int acceptExternalDrops() { return 0; }

  virtual void onExternalDropBegin() {}
  virtual void onExternalDropDirScan(const char *dirname) {}
  virtual void onExternalDropEnd() {}

  virtual int bypassModal();

  virtual RootWnd *findWindow(const char *id);
  virtual RootWnd *findWindowByInterface(GUID interface_guid);
  virtual RootWnd *findWindowByCallback(FindObjectCallback *cb);
  virtual RootWnd *findWindowChain(FindObjectCallback *cb, RootWnd *wcaller=NULL);

private:
  void addDroppedFile(const char *filename, PtrList<FilenamePS> *plist); // recursive
  void setLayeredWindow(int i);

public:
//FG> alternate notify window
  virtual void setNotifyWindow(RootWnd *newnotify);
  virtual RootWnd *getNotifyWindow();

  virtual double getRenderRatio();
  virtual void setRenderRatio(double r);
  virtual int handleRatio();
  int renderRatioActive();
  void multRatio(int *x, int *y=NULL);
  void multRatio(RECT *r);
  void divRatio(int *x, int *y=NULL);
  void divRatio(RECT *r);
  virtual int isClickThrough();
  virtual void setClickThrough(int ct);
  virtual RootWnd *getForwardWnd() { return this; }

// DERIVED WINDOW BEHAVIORAL PREFERENCES
protected:
  virtual int wantDoubleClicks();
  // return 1 to get onMouseOver even if mouse isn't captured
  virtual int wantSiblingInvalidations();

  virtual int wantFocus();
  virtual int wantAutoContextMenu();

protected: 

  void onTipMouseMove();
  void renderBaseTexture(CanvasBase *c, RECT &r, int alpha=255);

public:
  virtual RootWnd *getBaseTextureWindow();
  void setBaseTextureWindow(RootWnd *w);
  virtual int isMouseOver(int x, int y);

  virtual void bringVirtualToFront(RootWnd *w);
  virtual void bringVirtualToBack(RootWnd *w);
  virtual void bringVirtualAbove(RootWnd *w, RootWnd *b);
  virtual void bringVirtualBelow(RootWnd *w, RootWnd *b);
  void changeChildZorder(RootWnd *w, int newpos);

  static int isDesktopAlphaAvailable();
  virtual int handleTransparency(); // return 0 if you use overlay mode to render your stuff
  virtual int runModal();
  virtual int exec() { return runModal(); }
  virtual void endModal(int ret);

  Dependent *rootwnd_getDependencyPtr();
  Dependent *timerclient_getDependencyPtr();

  virtual void signalMinMaxEnforcerChanged(void);
  virtual void onMinMaxEnforcerChanged(void) {}
  virtual void addMinMaxEnforcer(RootWnd *w);
  virtual void removeMinMaxEnforcer(RootWnd *w);
  virtual RootWnd *enumMinMaxEnforcer(int n);
  virtual int getNumMinMaxEnforcers();
  virtual int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);
  virtual int sendAction(RootWnd *target, const char *action, const char *param=NULL, int x=0, int y=0, int p1=0, int p2=0, void *data=NULL, int datalen=0);

  virtual void setRenderBaseTexture(int r);
  virtual int getRenderBaseTexture();

  virtual GuiObject *getGuiObject();

  void setAutoResizeAfterInit(int tf) { want_autoresize_after_init = tf; }
  virtual void setAllowDeactivation(int allow) { allow_deactivate = allow; }
  virtual int allowDeactivation();

private:
  // ATTENTION: note the capitalization on these -- so as not to mix up with
  // wndProc()'s hWnd
  HINSTANCE hinstance;
  HWND hwnd;

  RootWnd *parentWnd;

  int inputCaptured;

  void onTab();
  HWND createWindow(int x, int y, int w, int h, int nochild, HWND parent, HINSTANCE hinstance);

protected:

  int checkModal();
  void hintDestroying() { destroying=TRUE; } // call that in your destructor if you'd otherwise generate virtual calls after your destructor

  int dragging;

	Canvas *virtualCanvas;
  static int (WINAPI *getRandomRgn)(HDC dc, HRGN rgn, int i);
  static BOOL (WINAPI *BaseWnd::UpdateLayeredWindow)(
  HWND hwnd,             // handle to layered window
  HDC hdcDst,            // handle to screen DC
  POINT *pptDst,         // new screen position
  SIZE *psize,           // new size of the layered window
  HDC hdcSrc,            // handle to surface DC
  POINT *pptSrc,         // layer position
  COLORREF crKey,        // color key
  BLENDFUNCTION *pblend, // blend function
  DWORD dwFlags          // options
  );

private:

  void _cascadeRepaintRgn(Region *rg);
  void packCascadeRepaintRgn(Region *rg);
  int createTip();       
  void destroyTip();     // called in destructor, do not override
  PtrList<DragSet> dragsets;
  RootWnd *prevtarg;
  String suggestedTitle;

  //FG
  int start_hidden;
  svc_toolTipsRenderer *tipsvc;
  String tip;
  int tipid;
  BOOL tip_done;
  BOOL tipawaytimer;
  BOOL tipshowtimer;
  BOOL tipbeenchecked;
  BOOL tipdestroytimer;

  //FG
  BOOL ncb;
  DWORD lastClick[2];
  POINT lastClickP[2];

  //FG
  RootWnd *btexture;

  //FG
  RootWnd *notifyWindow;
  BOOL destroying;

	//CT:Virtual children
	PtrList<RootWnd> virtualChildren;
	PtrList<RootWnd> rootwndchildren; //FG
	int virtualCanvasHandled;
	int virtualCanvasH,virtualCanvasW;
	int rwidth, rheight, rx, ry;
	RootWnd *curVirtualChildCaptured;

	//FG>
	RegionI *deferedInvalidRgn;
	static int nreal;
	static HINSTANCE gdi32instance;
  static int grrfailed;

  HWND oldCapture;
  
	int hasfocus;
	RootWnd *curVirtualChildFocus;
  RootWnd *curRealChildFocus;

	double ratio;
  int skin_id;
  int wndalpha;
  int w2k_alpha;
  int curframerate;

  int notifyid;
  int activealpha;
  int inactivealpha;
  int clickthrough;
  int triedtipsvc;
  int mustquit;
  int returnvalue;
  int postoninit;
  int inited;
  int skipnextfocus;
  RegionI *subtractorrgn, *composedrgn;
  void setWindowRegion(Region *r);
  void computeComposedRegion();
  int ensureWindowRegionValid();
  void updateWindowRegion();
  void assignWindowRegion(Region *wr);
  int wndregioninvalid;
  int rectrgn, regionop;
  RegionI *deferedCascadeRepaintRgn;
  int need_flush_cascaderepaint;
  Tooltip *tooltip;
  PtrList<RootWnd> minmaxEnforcers;
  int this_visible;
  int renderbasetexture;
  GuiObject *my_guiobject;
  int want_autoresize_after_init;
  int resizecount;
  double lastratio;
  int suggested_w, suggested_h;
  int maximum_w, maximum_h;
  int minimum_w, minimum_h;
  int allow_deactivate;
  int minimized;
  int deleting;
  int preventcancelcapture;
  int inonresize;
  String tcname;
  int focus_on_click;
};

#ifdef WIN32
__inline HINSTANCE HINSTANCEfromHWND(HWND wnd) {
  if (wnd == NULL) return NULL;
  return (HINSTANCE)GetWindowLong(wnd, GWL_HINSTANCE);
}
#endif

#endif
