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

#include "std.h"
#include "../studio/assert.h"
#include <mmsystem.h>
#include "tooltip.h"

#define DESKTOPALPHA
#define REFRESH_RATE 25
#define DRAWTIMERID 125

#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
const int WS_EX_LAYERED=0x80000;
const int LWA_ALPHA=2;
#ifndef AC_SRC_ALPHA
const int AC_SRC_ALPHA=1;
#endif
const int LWA_COLORKEY=1;
const int ULW_ALPHA=2;

static int isXPOrGreater() {
  static int isxp=-1;
  if (isxp == -1) {
    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    if(osvi.dwMajorVersion >= 5)
      if(osvi.dwMinorVersion >=1) {
        isxp = 1;
        return 1;
      }
    isxp = 0;
  }
  return isxp;
}

#endif

#include "basewnd.h"
#include "usermsg.h"
#include "tlist.h"
#include "paintcb.h"
#include "../bfc/canvas.h"	//FUCKO: bfc
#include "../common/filename.h"	//FUCKO: bfc
#include "region.h"	
#include "../common/guiobjwnd.h"	
#include "../common/script/scriptguid.h"	
#include "notifmsg.h"
#include "../studio/api.h"
#include "../studio/metricscb.h"

#define UTF8 1
#if UTF8
# include "../bfc/encodedstr.h"
# include "../common/readdir.h"
#endif


#define TIP_TIMER_ID        1601
#define TIP_DESTROYTIMER_ID 1602
#define TIP_AWAY_ID         1603
#define TIP_AWAY_DELAY       100

#define TIP_TIMER_THRESHOLD  350
#define TIP_LENGTH          3000

#define VCHILD_TIMER_ID_MIN	2000
#define VCHILD_TIMER_ID_MAX	2100

#define DEFERREDCB_INVALIDATE   0x201 // move to .h

class DragSet : public PtrList<void>, public Named {};

static void register_wndClass(HINSTANCE);

#define ROOTSTRING "RootWnd"

#define BASEWNDCLASSNAME "BaseWindow_" ROOTSTRING

#define WM_MOUSEWHEEL 0x20A

static RootWnd *stickyWnd;
static RECT sticky;

#ifdef WIN32
static HINSTANCE user32instance;
static void (__stdcall *setLayeredWindowAttributes)(HWND, int, int, int);
static BOOL (__stdcall *updateLayeredWindow)(
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
static int checked_for_alpha_proc;
#endif

/*RootWnd *RootWnd::rootwndFromPoint(POINT &point, int level) {
  RootWnd *wnd;
  wnd = api->rootWndFromPoint(&point);
  return RootWnd::rootwndFromRootWnd(wnd, level, &point);
}

RootWnd *RootWnd::rootwndFromRootWnd(RootWnd *wnd, int level, POINT *point) {

  for (;;) {
    if (wnd == NULL || level < 0) return NULL;
    if (point) {
      RECT r;
      wnd->getWindowRect(&r);
      if (!PtInRect(&r, *point)) return NULL; // PORT ME
    }
    if (level == 0) return wnd; 
    wnd = wnd->getRootWndParent();
    level--;
  }
  // should never get here
}*/

#include <shlobj.h>
#include <shellapi.h>

static HRESULT ResolveShortCut(HWND hwnd, LPCSTR pszShortcutFile, LPSTR pszPath)
{
  HRESULT hres;
  IShellLink* psl;
  char szGotPath[MAX_PATH];
  WIN32_FIND_DATA wfd;

  *pszPath = 0;   // assume failure

  hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (void **) &psl);
  if (SUCCEEDED(hres))
  {
    IPersistFile* ppf;

    hres = psl->QueryInterface(IID_IPersistFile, (void **) &ppf); // OLE 2!  Yay! --YO
    if (SUCCEEDED(hres))
    {
      WORD wsz[MAX_PATH];
      MultiByteToWideChar(CP_ACP, 0, pszShortcutFile, -1, wsz, MAX_PATH);

     hres = ppf->Load(wsz, STGM_READ);
     if (SUCCEEDED(hres))
     {
       hres = psl->Resolve(hwnd, SLR_ANY_MATCH);
       if (SUCCEEDED(hres))
       {
          strcpy(szGotPath, pszShortcutFile);
          hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA *)&wfd,
                 SLGP_SHORTPATH );
		  strcpy(pszPath,szGotPath);
        }
      }
      ppf->Release();
    }
    psl->Release();
  }
  return SUCCEEDED(hres);
}

BaseWnd::BaseWnd() {
  preventcancelcapture = 0;
  deleting = 0;
  hinstance = NULL;
  hwnd = NULL;
  parentWnd = NULL;
  dragging = 0;
  prevtarg = NULL;
  inputCaptured = 0;
  btexture = NULL;
  postoninit = 0;
  inited = 0;
  skipnextfocus = 0;
  ncb=FALSE;

  tooltip = NULL;
  tip_done = FALSE;
  tipshowtimer = FALSE;
  tipawaytimer = FALSE;
  tipdestroytimer = FALSE;
  start_hidden = 0;
  notifyWindow = NULL;
  lastClick[0] = 0;
  lastClick[1] = 0;
  lastClickP[0].x = 0;
  lastClickP[0].y = 0;
  lastClickP[1].x = 0;           
  lastClickP[1].y = 0;
  destroying = FALSE;

  curVirtualChildCaptured = NULL;
  curRealChildFocus = NULL;
  curVirtualChildFocus = NULL;

  virtualCanvas = NULL; virtualCanvasH = virtualCanvasW = 0;
  deferedInvalidRgn = NULL; 

  hasfocus = 0;
  focus_on_click = 1;

  ratio = 1;
  lastratio = 1;
  rwidth=rheight=0;
  skin_id = -1;
  wndalpha = 255;
  activealpha = 255;
  inactivealpha = 255;
  w2k_alpha = 0;
  scalecanvas = NULL;
  clickthrough = 0;

  mustquit = 0;
  returnvalue = 0;
  notifyid = 0;

  subtractorrgn = NULL;
  composedrgn = NULL;
  wndregioninvalid = 1;
  regionop = REGIONOP_NONE;
  rectrgn = 1;
  need_flush_cascaderepaint = 0;
  deferedCascadeRepaintRgn = NULL;
  this_visible = 0;
  renderbasetexture = 0;
  oldCapture = NULL;
  my_guiobject = NULL;
  want_autoresize_after_init = 0;
  resizecount=0;
  suggested_w=320;
  suggested_h=200;
  maximum_w=maximum_h=AUTOWH;
  minimum_w=minimum_h=AUTOWH;
  rx = 0;
  ry = 0;
  rwidth = 0;
  rheight = 0;            
  allow_deactivate = 1;
  minimized = 0;
  inonresize = 0;
}

BaseWnd::~BaseWnd() {
  //ASSERT(virtualChildren.getNumItems() == 0);
  if (api->getModalWnd() == this) api->popModalWnd(this);
  if (hwnd) nreal--;
  if (!nreal) {
    if(gdi32instance) FreeLibrary(gdi32instance);
    gdi32instance = NULL;
    if(user32instance) FreeLibrary(user32instance);
    user32instance = NULL;
  }
  destroying=TRUE;
  curVirtualChildFocus = NULL;
  if (inputCaptured && GetCapture() == gethWnd()) ReleaseCapture();

  if (hwnd != NULL) {
#ifdef URLDROPS
    if (acceptExternalDrops()) RevokeDragDrop(hwnd, &m_target);
#endif
    DestroyWindow(gethWnd());
  }
  deleteFrameBuffer(virtualCanvas);
  virtualCanvas=NULL;
  delete scalecanvas;
  scalecanvas = NULL;

  resetDragSet();

  notifyParent(ChildNotify::DELETED);
  if (tipdestroytimer)
    killTimer(TIP_DESTROYTIMER_ID);
  if (tipshowtimer)
    killTimer(TIP_TIMER_ID);
  if (tipawaytimer)
    killTimer(TIP_AWAY_ID);

  destroyTip();

  delete tooltip;

  if (deferedInvalidRgn)
    delete deferedInvalidRgn;

  delete composedrgn;
  delete subtractorrgn;
  delete deferedCascadeRepaintRgn;

  if (parentWnd != NULL)
    parentWnd->unregisterRootWndChild(this);

  api->unregisterRootWnd(this);
  hwnd = NULL;
}

int BaseWnd::init(RootWnd *parWnd, int nochild) {
  if (parWnd == NULL) return 0;
  HWND phwnd = parWnd->gethWnd();
  ASSERT(phwnd != NULL);
  int ret;
  parentWnd = parWnd;	// set default parent wnd
  ret = init(HINSTANCEfromHWND(phwnd), phwnd, nochild);
  if (!ret) parentWnd = NULL;	// abort
  return ret;
}

int BaseWnd::init(HINSTANCE hInstance, HWND parent, int nochild) {
  RECT r;
  int w, h;

  ASSERTPR(gethWnd() == NULL, "don't you double init you gaybag");

  hinstance = hInstance;

  ASSERT(hinstance != NULL);

  register_wndClass(hinstance);

  if (parent != NULL) {
    GetClientRect(parent, &r);
  } else {
    r.left = 0;
    r.right = getPreferences(SUGGESTED_W);
    r.top = 0;
    r.bottom = getPreferences(SUGGESTED_H);
  }

  w = (r.right - r.left);
  h = (r.bottom - r.top);

  rwidth = w;
  rheight = h;
  rx = r.left;
  ry = r.top;

  hwnd = createWindow(r.left, r.top, w, h, nochild, parent, hinstance);


  //ASSERT(hwnd != NULL); // lets fail nicely, this could happen for some win32 reason, we don't want to fail the whole app for it, so lets just fail the wnd
  if (hwnd == NULL) return 0;

  SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0,
    SWP_NOMOVE|SWP_NOSIZE|SWP_DEFERERASE);

  nreal++;

#ifdef URLDROPS
  if (acceptExternalDrops()) RegisterDragDrop(hwnd, &m_target);
#endif

  this_visible = 0;

  onInit();

  this_visible = !start_hidden;

  onPostOnInit();

  return 1;
}

#define WS_EX_COMPOSITED        0x02000000L

HWND BaseWnd::createWindow(int x, int y, int w, int h, int nochild, HWND parent, HINSTANCE hinstance) {
  // create the window
  RootWnd *rootwnd = (RootWnd *)this;
  int style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  int exstyle=0;
  if (parent == NULL) {
    exstyle |= WS_EX_TOOLWINDOW;
    style |= WS_POPUP;
  } else
    style |= WS_CHILD;

  if (nochild) style=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  if (acceptExternalDrops()) exstyle |= WS_EX_ACCEPTFILES;
/*  if (isXPOrGreater())
    exstyle |= WS_EX_COMPOSITED;*/

  return CreateWindowEx(exstyle, BASEWNDCLASSNAME, NULL, style,
    x, y, w, h, parent, NULL, hinstance, (LPVOID)rootwnd);
}

int (WINAPI *BaseWnd::getRandomRgn)(HDC dc, HRGN rgn, int i)=NULL;
BOOL (WINAPI *BaseWnd::UpdateLayeredWindow)(
  HWND hwnd,             // handle to layered window
  HDC hdcDst,            // handle to screen DC
  POINT *pptDst,         // new screen position
  SIZE *psize,           // new size of the layered window
  HDC hdcSrc,            // handle to surface DC
  POINT *pptSrc,         // layer position
  COLORREF crKey,        // color key
  BLENDFUNCTION *pblend, // blend function
  DWORD dwFlags          // options
)=NULL;


int BaseWnd::nreal=0;
int BaseWnd::grrfailed=0;
HINSTANCE BaseWnd::gdi32instance=NULL;

int BaseWnd::onInit() {

  inited = 1;

  if (getParent())
    getParent()->registerRootWndChild(this);

  api->registerRootWnd(this);

  if (!isDesktopAlphaAvailable()) 
    w2k_alpha = 0;

  if (w2k_alpha) {
    setLayeredWindow(1);
  }

  return 0;
}

int BaseWnd::onPostOnInit() {
  postoninit=1; // from now on, isInited() returns 1;
  if (want_autoresize_after_init) onResize(); 
  else invalidateWindowRegion();
  if (isVisible()) onSetVisible(1); 
  return 0;
}

void BaseWnd::setLayeredWindow(int i) {
  if (!isInited()) return;
  if (i) {
    SetWindowLong(gethWnd(), GWL_EXSTYLE, GetWindowLong(gethWnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
  } else {
    SetWindowLong(gethWnd(), GWL_EXSTYLE, GetWindowLong(gethWnd(), GWL_EXSTYLE) & ~WS_EX_LAYERED);
  }
  setTransparency(-1);
}

int BaseWnd::getCursorType(int x, int y) {
  return BASEWND_CURSOR_POINTER;
}

void BaseWnd::onSetName() {
  if (isInited())
    SetWindowText(gethWnd(), getNameSafe());
  notifyParent(ChildNotify::NAMECHANGED);
}

HWND BaseWnd::gethWnd() {
  return isVirtual() ? getParent()->gethWnd() : hwnd;
}

HINSTANCE BaseWnd::gethInstance() {
  return hinstance;
}

void BaseWnd::timerCallback(int id) { 
  switch (id) {
    case TIP_TIMER_ID:

      tipshowtimer=FALSE;
      tip_done = TRUE;
      killTimer(TIP_TIMER_ID);
  
      POINT p;
      Std::getMousePos(&p.x, &p.y);
      if (api->rootWndFromPoint(&p) == (RootWnd *)this) {
        createTip();
        setTimer(TIP_DESTROYTIMER_ID, TIP_LENGTH);
        tipdestroytimer=TRUE;
      }
      setTimer(TIP_AWAY_ID, TIP_AWAY_DELAY);
      tipawaytimer=TRUE;
      break;
    case TIP_DESTROYTIMER_ID:
      killTimer(TIP_DESTROYTIMER_ID);
      killTimer(TIP_AWAY_ID);
      tipawaytimer=FALSE;
      tipdestroytimer=FALSE;
      destroyTip();
      break;
    case TIP_AWAY_ID:
      onTipMouseMove();
      break;
  }
}

int BaseWnd::isInited() {
  return inited;
}

int BaseWnd::isDestroying() {
  return destroying;
}

int BaseWnd::wantDoubleClicks() {
  return TRUE;
}

int BaseWnd::wantSiblingInvalidations() {
  return FALSE;
}

void BaseWnd::setRSize(int x, int y, int w, int h) {
  rwidth = w;
  rheight = h;
  rx = x;
  ry = y;
}

void BaseWnd::resize(int x, int y, int w, int h, int wantcb) {

  inonresize = 1;

  if (getNumMinMaxEnforcers() > 0) {
    int min_w = getPreferences(MINIMUM_W);
    int min_h = getPreferences(MINIMUM_H);
    int max_w = getPreferences(MAXIMUM_W);
    int max_h = getPreferences(MAXIMUM_H);
    if (w < min_w) w = min_w;
    if (w > max_w) w = max_w;
    if (h < min_h) h = min_h;
    if (h > max_h) h = max_h;
  }

  int noresize = (w == NOCHANGE && h == NOCHANGE);
  int nomove = (x == NOCHANGE && y == NOCHANGE);
  if (x == NOCHANGE) x = rx;
  if (y == NOCHANGE) y = ry;
  if (w == NOCHANGE) w = rwidth;
  if (h == NOCHANGE) h = rheight;

  double thisratio = getRenderRatio();
  int updwndregion = 0;
  int different_ratio = (lastratio != thisratio);
  lastratio = thisratio;

  int noevent = (resizecount > 1 && w == rwidth && h == rheight);
  if (different_ratio == 1 && noevent == 1) {
    HWND w = GetWindow(gethWnd(), GW_CHILD);
    if (w != NULL)
      noevent = 0;
    invalidateWindowRegion();
  }

  RECT oldsize, newsize={x,y,w,h};
  if (hwnd != NULL) 
    BaseWnd::getNonClientRect(&oldsize);

  setRSize(x, y, w, h);
  
  int enlarge=0;
  if (handleRatio() && renderRatioActive()) {
    enlarge=1;
    multRatio(&w, &h);
    if (getParent() != NULL) {
      multRatio(&x, &y);
    }
  }

	if (gethWnd() != NULL) {

  RECT oldsizescaled;
  getWindowRect(&oldsizescaled);
  RECT newsizescaled={x,y,x+w,y+h};
  if (MEMCMP(&newsizescaled, &oldsizescaled, sizeof(RECT))) {
	  SetWindowPos(gethWnd(), NULL, x, y, w, h,
	  SWP_NOZORDER |
	  SWP_NOACTIVATE |
	  (ncb ? SWP_NOCOPYBITS : 0) | 
	  ( nomove ? SWP_NOMOVE : 0) |
	  ( noresize ? SWP_NOSIZE : 0) |
	  0);
  } else {
    //OutputDebugString("BaseWnd::resize optimized\n");
  }

  if (noevent) {
    inonresize = 0;
    return;
  }

	if (wantcb && isPostOnInit()) {
    resizecount = MIN(5, ++resizecount);
    if (!isVirtual())
      invalidateWindowRegion();
    onResize();
  }

	if (ncb)
    invalidate();
  else {
    RECT r;
    if (hwnd != NULL) {
      if (newsize.left == oldsize.left && newsize.top == oldsize.top) {
        if (newsize.right > oldsize.right) { // growing in width
          r.left = oldsize.right;
          r.right = newsize.right;
          r.top = newsize.top;
          r.bottom = newsize.bottom;
          invalidateRect(&r);
          if (newsize.bottom > oldsize.bottom) { // growing in width & height
            r.left = oldsize.left;
            r.right = newsize.right;
            r.top = oldsize.bottom;
            r.bottom = newsize.bottom;
            invalidateRect(&r);
          }
        } else if (newsize.bottom > oldsize.bottom) {
          if (newsize.bottom > oldsize.bottom) { // growing in height
            r.left = oldsize.left;
            r.right = newsize.right;
            r.top = oldsize.bottom;
            r.bottom = newsize.bottom;
            invalidateRect(&r);
          }
        }
      }
    }
  }
 }
  inonresize = 0;
}


int BaseWnd::onResize() {
  if (!isVirtual() || (getRegionOp() != REGIONOP_NONE))
    invalidateWindowRegion();
  // you are not supposed to call onResize until after onInit has returned. If what you wanted was to generate
  // an onResize event to do some custom client coordinates recalculations (ie: to apply on your children)
  // then you don't need to do anything since onResize is going to be called after onInit() is done. If you still want to
  // trigger it because your code might be called by onInit and after onInit, use isPostOnInit() as a test.
  // if what you wanted was to signal a object that you just resized it, then you don't need to do anything beside
  // resize(...), it will generate the event on its own if the window is inited, and will defer to until after onInit
  // if it is not.
  // shortly put: do not call onResize before or inside onInit()
  // if you have any valid reason for doing that, i'd like to know about it so i can make it possible. -FG
#ifdef _DEBUG
  if (!isPostOnInit()) {
    //__asm int 3;
    ASSERTPR(isPostOnInit(), "do not call onResize before or inside onInit()");
  }
#endif
  return FALSE;
}

void BaseWnd::resizeToClient(BaseWnd *wnd) {
  if (wnd != NULL)
    wnd->resize(&clientRect());
}

int BaseWnd::onPostedMove() {
  return FALSE;
}

void BaseWnd::resize(RECT *r, int wantcb) {
  resize(r->left, r->top, r->right-r->left, r->bottom-r->top, wantcb);
}

void BaseWnd::move(int x, int y) {
  setRSize(x, y, rwidth, rheight);
	if (!ncb)
      SetWindowPos(gethWnd(), HWND_TOP, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_DEFERERASE); 
	else
      SetWindowPos(gethWnd(), HWND_TOP, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOACTIVATE|SWP_DEFERERASE);
}

void BaseWnd::bringToFront() {
  SetWindowPos(gethWnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE|SWP_DEFERERASE);
}

void BaseWnd::bringToBack() {
  SetWindowPos(gethWnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE|SWP_DEFERERASE);
}

void BaseWnd::setVisible(int show) {
  if (isVisible(1) && show) return;
  invalidate();
  this_visible = !!show;
  if (!getParent() || getParent() == api->main_getRootWnd() || getParent()->isVisible()) {
    onSetVisible(show);
  }
}

void BaseWnd::onSetVisible(int show) { 
  if (!isVirtual()) if (hwnd != NULL) {
//    SetWindowPos(gethWnd(),NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);
    ShowWindow(gethWnd(), show ? SW_SHOWNA : SW_HIDE);
  }
/*  if (!show)
    postDeferredCallback(0x7849);
  else {*/
    foreach(rootwndchildren)
      RootWnd *w = rootwndchildren.getfor();
      if (w->isVisible(1)) // check internal flag only
        w->onSetVisible(show);
    endfor;
    dependent_sendEvent(RootWnd::depend_getClassGuid(), Event_SETVISIBLE, show);
  //}
/*  if (getDesktopParent() == this) {
    cascadeRepaint(0);
  }*/

/*#ifdef WIN32 // os-specific non virtual child wnd support
  if (!isVirtual()) {
    HWND w = GetWindow(gethWnd(), GW_CHILD);
    while (w != NULL) {
      RootWnd *rootwnd = (RootWnd*)GetWindowLong(w, GWL_USERDATA);
      if (rootwnd && rootwnd != this)
        if (rootwnd->isInited())
          rootwnd->onSetVisible(show);
      w = GetWindow(w, GW_HWNDNEXT);
    }
  }
#endif*/
}

void BaseWnd::enable(int en) {
  EnableWindow(gethWnd(), en);
  onEnable(en);
}

int BaseWnd::getEnabled() {
  return IsWindowEnabled(hwnd);
}

int BaseWnd::onEnable(int en) {
  return 0;
}

void BaseWnd::setFocus(int skipnextautochildfocus) {
  skipnextfocus = skipnextautochildfocus;
  SetFocus(gethWnd());
}

void BaseWnd::setFocusOnClick(int f) {
  focus_on_click = f;
}

Region *BaseWnd::getDeferedInvalidRgn() {
  return deferedInvalidRgn;
}

void BaseWnd::deferedInvalidate() {
  if (!hasVirtualChildren() || !isVisible(1)) return; 
  RECT r={0,0,0,0};
  getNonClientRect(&r);
  deferedInvalidateRect(&r);
}

void BaseWnd::deferedInvalidateRect(RECT *r) {
  if (!hasVirtualChildren()) return; 
  RegionI h(r);
  deferedInvalidateRgn(&h);
}

void BaseWnd::deferedInvalidateRgn(Region *h) {
  if (!hasVirtualChildren()) return; 
  if (!deferedInvalidRgn) {
    deferedInvalidRgn = new RegionI();
  }

  deferedInvalidRgn->add(h);
}

void BaseWnd::deferedValidate() {
  if (!hasVirtualChildren() || !isVisible(1)) return; 
  RECT r={0,0,0,0};
  getNonClientRect(&r);
  deferedValidateRect(&r);
}

void BaseWnd::deferedValidateRect(RECT *r) {
  if (!hasVirtualChildren()) return; 
  Region *h = new RegionI(r);
  deferedValidateRgn(h);
  delete r;
}

void BaseWnd::deferedValidateRgn(Region *h) {
  if (!hasVirtualChildren()) return; 
  if (!deferedInvalidRgn) return;

  deferedInvalidRgn->subtractRgn(h);
}

int BaseWnd::hasVirtualChildren() {
  return 1; //virtualChildren.getNumItems() > 0;
}

void BaseWnd::invalidate() {
  invalidateFrom(this);
}

void BaseWnd::invalidateFrom(RootWnd *who) {
  if (hasVirtualChildren()) deferedInvalidate();
  if (hwnd != NULL && isVisible(1)) InvalidateRect(gethWnd(), NULL, FALSE);
}

void BaseWnd::invalidateRectFrom(RECT *r, RootWnd *who) {
  if (hasVirtualChildren()) deferedInvalidateRect(r);
  RegionI *rg = new RegionI(r);
  invalidateRgnFrom(rg, who);
  delete rg;
}

void BaseWnd::invalidateRgn(Region *r) {
  invalidateRgnFrom(r, this);
}

void BaseWnd::invalidateRect(RECT *r) {
  invalidateRectFrom(r, this);
}

void BaseWnd::invalidateRgnFrom(Region *r, RootWnd *who) {
  static int inhere=0;
  if (hwnd != NULL && isVisible(1)) {
    if (hasVirtualChildren()/* && !inhere*/) {
      inhere=1;
      Region *_r = r->clone();
      int j = virtualChildren.searchItem(who);
      for (int i=0;i<virtualChildren.getNumItems();i++) {
        RootWnd *w = virtualChildren[i];
        if (w != who && w->wantSiblingInvalidations())
          w->onSiblingInvalidateRgn(_r, who, j, i);
      }

      inhere=0;
	  
      deferedInvalidateRgn(_r);
      physicalInvalidateRgn(_r);
      r->disposeClone(_r);
    } else {
      deferedInvalidateRgn(r);
      physicalInvalidateRgn(r);
    }
  }
}

void BaseWnd::physicalInvalidateRgn(Region *r) {
  if (hwnd != NULL && isVisible(1)) {
    if (ABS(getRenderRatio()-1.0) > 0.01) {
      Region *clone = r->clone();
      clone->scale(getRenderRatio(), getRenderRatio(), TRUE);
      InvalidateRgn(gethWnd(), clone->getOSHandle(), FALSE);
      r->disposeClone(clone);
    } else
      InvalidateRgn(gethWnd(), r->getOSHandle(), FALSE);
  }
}

void BaseWnd::validate() {
  if (hwnd != NULL) ValidateRect(gethWnd(), NULL);
}

void BaseWnd::validateRect(RECT *r) {
  if (hwnd != NULL) {
    if (ABS(getRenderRatio()-1.0) > 0.01) {
      RECT r2 = *r;
      r2.left = (int)((double)r2.left * getRenderRatio());
      r2.top = (int)((double)r2.top * getRenderRatio());
      r2.right = (int)((double)r2.right * getRenderRatio());
      r2.bottom = (int)((double)r2.bottom * getRenderRatio());
      ValidateRect(gethWnd(), &r2);
    } else
    ValidateRect(gethWnd(), r);
  }
}

void BaseWnd::validateRgn(Region *reg) {
  if (hwnd != NULL) {
    if (ABS(getRenderRatio()-1.0) > 0.01) {
      Region *clone = reg->clone();
      clone->scale(getRenderRatio(), getRenderRatio(), TRUE);
      ValidateRgn(gethWnd(), clone->getOSHandle());
      reg->disposeClone(clone);
    } else
      ValidateRgn(gethWnd(), reg->getOSHandle());
  }
}

void BaseWnd::repaint() {
/*	if (hasVirtualChildren())	{
	  Region *h = new Region();
	  int s = GetUpdateRgn(gethWnd(), h->getHRGN(), FALSE);
	  if (s != NULLREGION && s != ERROR) {
  		virtualDrawRgn(h);
	  }
	  delete h;
	}*/
  if (hwnd != NULL) UpdateWindow(gethWnd());
}

void BaseWnd::getClientRect(RECT *rect) {
/*  rect->left = rx;
  rect->right = rx + rwidth;
  rect->top = ry;
  rect->bottom = ry + rheight;*/
  //ASSERT(hwnd != NULL);
  if (hwnd == NULL) {
    MEMSET(rect, 0, sizeof(RECT));
    return;
  }
  GetClientRect(gethWnd(), rect);
  rect->right = rect->left + rwidth;
  rect->bottom = rect->left + rheight;
}

RECT BaseWnd::clientRect() {
  RECT ret;
  getClientRect(&ret);
  return ret;
}

void BaseWnd::getNonClientRect(RECT *rect) {
//  ASSERT(hwnd != NULL);
  if (!hwnd) getClientRect(rect);
  else {
    if (getRenderRatio() == 1.0)
      GetClientRect(gethWnd(), rect);
    else {
      GetClientRect(gethWnd(), rect);
      rect->right = rect->left + rwidth;
      rect->bottom = rect->left + rheight;
    }
  }
/*  rect->left = rx;
  rect->right = rx + rwidth;
  rect->top = ry;
  rect->bottom = ry + rheight;*/
}

RECT BaseWnd::nonClientRect() {
  RECT ret;
  getNonClientRect(&ret);
  return ret;
}

void BaseWnd::getWindowRect(RECT *rect) {
#ifdef WIN32
  ASSERT(hwnd != NULL);
  GetWindowRect(gethWnd(), rect);
#else
#error port me
#endif
}

// get position relative to parent (same coordinate system for basewnd & virtualwnd)
void BaseWnd::getPosition(POINT *pt) {
  pt->x = rx;
  pt->y = ry;
}

void *BaseWnd::dependent_getInterface(const GUID *classguid) {
  HANDLEGETINTERFACE(RootWnd);
  HANDLEGETINTERFACE(RootWnd);
  return NULL;
}

RECT BaseWnd::windowRect() {
  RECT ret;
  getWindowRect(&ret);
  return ret;
}


void BaseWnd::clientToScreen(int *x, int *y) {
  POINT p;
  p.x = x ? *x : 0;
  p.y = y ? *y : 0;
  if (ABS(getRenderRatio()-1.0) > 0.01) {
    p.x = (int)((double)p.x * getRenderRatio());
    p.y = (int)((double)p.y * getRenderRatio());
  }
  ClientToScreen(gethWnd(), &p);
  if (x) *x = p.x;
  if (y) *y = p.y;
}

void BaseWnd::clientToScreen(RECT *r) {
  clientToScreen((int*)&r->left, (int*)&r->top);
  clientToScreen((int*)&r->right, (int*)&r->bottom);
}

void BaseWnd::clientToScreen(POINT *p) {
  clientToScreen((int *)&p->x, (int *)&p->y);
}

void BaseWnd::screenToClient(int *x, int *y) {
  POINT p;
  p.x = x ? *x : 0;
  p.y = y ? *y : 0;
  ScreenToClient(gethWnd(), &p);
  if (ABS(getRenderRatio()-1.0) > 0.01) {
    p.x = (int)((double)p.x / getRenderRatio());
    p.y = (int)((double)p.y / getRenderRatio());
  }
  if (x) *x = p.x;
  if (y) *y = p.y;
}

void BaseWnd::screenToClient(RECT *r) {
  screenToClient((int*)&r->left, (int*)&r->top);
  screenToClient((int*)&r->right, (int*)&r->bottom);
}

void BaseWnd::screenToClient(POINT *p) {
  screenToClient((int *)&p->x, (int *)&p->y);
}


void BaseWnd::setParent(RootWnd *newparent) {
  ASSERTPR(newparent != NULL, "quit being a weeny");
  ASSERTPR(parentWnd == NULL || newparent == parentWnd, "can't reset parent");
  parentWnd = newparent;
  if (isInited()) {
    HWND w1 = gethWnd();
    HWND w2 = newparent->gethWnd();
    if (w1 != w2)
      SetParent(w1, w2);
  }
}

int BaseWnd::reparent(RootWnd *newparent) {
  if (!isVirtual()) {
    if (isInited()) {
      RootWnd *old = getParent();
      if (!old && newparent) {
        ::SetParent(gethWnd(), newparent->gethWnd());
        SetWindowLong(gethWnd() , GWL_STYLE, GetWindowLong(gethWnd(), GWL_STYLE) & ~WS_POPUP);
        SetWindowLong(gethWnd() , GWL_STYLE, GetWindowLong(gethWnd(), GWL_STYLE) | WS_CHILD);
      } else if (old && !newparent) {
        SetWindowLong(gethWnd() , GWL_STYLE, GetWindowLong(gethWnd(), GWL_STYLE) & ~WS_CHILD);
        SetWindowLong(gethWnd() , GWL_STYLE, GetWindowLong(gethWnd(), GWL_STYLE) | WS_POPUP);
        ::SetParent(gethWnd(), NULL);
      } else {
        ::SetParent(gethWnd(), newparent ? newparent->gethWnd() : NULL);
      }
    }
  }

  parentWnd = newparent;
  onSetParent(newparent);
  return 1;
}

RootWnd *BaseWnd::getParent() {
  return parentWnd;
}

RootWnd *BaseWnd::getRootParent() {
  return this;
}

RootWnd *BaseWnd::getDesktopParent() {
  // NONPORTABLE
  HWND w = gethWnd();
  if (!w) return NULL;
  HWND p = w;
  while (p && !(GetWindowLong(p, GWL_STYLE) & WS_POPUP)) 
    p = GetParent(p);
  if (p) return (RootWnd*)GetWindowLong(p, GWL_USERDATA);
  return NULL;
}

int BaseWnd::notifyParent(int msg, int param1, int param2) {
  RootWnd *notifywnd = getNotifyWindow();
  if (getParent() == NULL && notifywnd == NULL) return 0;
  if (notifywnd == NULL) notifywnd = getParent();
  ASSERT(notifywnd != NULL);
  return notifywnd->childNotify(this, msg, param1, param2);
}

int BaseWnd::passNotifyUp(RootWnd *child, int msg, int param1, int param2) {
  // Same code as above to decide for whom we should notify.
  RootWnd *notifywnd = getNotifyWindow();
  if (getParent() == NULL && notifywnd == NULL) return 0;
  if (notifywnd == NULL) notifywnd = getParent();
  ASSERT(notifywnd != NULL);
  // And here we just change the RootWnd pointer.
  return notifywnd->childNotify(child, msg, param1, param2);
}

void BaseWnd::broadcastNotify(RootWnd *wnd, int msg, int p1, int p2) {
  for (int i=0;i<getNumRootWndChildren();i++)
    enumRootWndChildren(i)->childNotify(wnd, msg, p1, p2);

  for (i=0;i<getNumRootWndChildren();i++)
    enumRootWndChildren(i)->broadcastNotify(wnd, msg, p1, p2);
}


void BaseWnd::setNotifyId(int id) {
  notifyid = id;
}

int BaseWnd::getNotifyId() {
  return notifyid;
}

DragInterface *BaseWnd::getDragInterface() {
  return this;
}

RootWnd *BaseWnd::rootWndFromPoint(POINT *pt) {
  // pt is in client coordinates
  int x = (int)((double)pt->x / getRenderRatio());
  int y = (int)((double)pt->y / getRenderRatio());

  RootWnd *ret = findRootWndChild(x, y);
  if (ret == NULL) ret = this;
  return ret;
}

int BaseWnd::getSkinId() {
  return skin_id;
}

int BaseWnd::onMetricChange(int metricid, int param1, int param2) {
  switch (metricid) {
    case Metric::TEXTDELTA:
      return setFontSize(-1);
  }
  return 0;
}

int BaseWnd::rootwnd_onPaint(CanvasBase *canvas, Region *r) {
  BaseCloneCanvas c(canvas);
  return onPaint(&c, r);
}

int BaseWnd::rootwnd_paintTree(CanvasBase *canvas, Region *r) {
  BaseCloneCanvas c(canvas);
  return paintTree(&c, r);
}

const char *BaseWnd::getRootWndName() {
  return getName();
}

const char *BaseWnd::getId() {
  return NULL;
}

void BaseWnd::setSkinId(int id) {
  skin_id = id;
}

void BaseWnd::setPreferences(int what, int v) {
  switch (what) {
    case MAXIMUM_W: maximum_w = v; break;
    case MAXIMUM_H: maximum_h = v; break;
    case MINIMUM_W: minimum_w = v; break;
    case MINIMUM_H: minimum_h = v; break;
    case SUGGESTED_W: suggested_w = v; break;
    case SUGGESTED_H: suggested_h = v; break;
  }
}

int BaseWnd::getPreferences(int what) {
  if (getNumMinMaxEnforcers() > 0) {

    int min_x=minimum_w, min_y=minimum_h, max_x=maximum_w, max_y=maximum_h, sug_x=suggested_w, sug_y=suggested_h;
  
    for (int i=0;i<getNumMinMaxEnforcers();i++) {
  
      int tmin_x, tmin_y, tmax_x, tmax_y, tsug_x, tsug_y;
  
      RootWnd *w = enumMinMaxEnforcer(i);
  
      if (w) {

        tmin_x = w->getPreferences(MINIMUM_W);
        tmin_y = w->getPreferences(MINIMUM_H);
        tmax_x = w->getPreferences(MAXIMUM_W);
        tmax_y = w->getPreferences(MAXIMUM_H);
        tsug_x = w->getPreferences(SUGGESTED_W);
        tsug_y = w->getPreferences(SUGGESTED_H);

        if (tmin_x == -1) tmin_x = AUTOWH;
        if (tmin_y == -1) tmin_y = AUTOWH;
        if (tmax_x == -1) tmax_x = AUTOWH;
        if (tmax_y == -1) tmax_y = AUTOWH;
        if (tsug_x == -1) tsug_x = AUTOWH;
        if (tsug_y == -1) tsug_y = AUTOWH;

#ifndef DISABLE_SYSFONTSCALE
        GuiObject *o = static_cast<GuiObject *>(getInterface(guiObjectGuid));
        if (o != NULL) {
          if (o->guiobject_getAutoSysMetricsW()) {
            if (tmin_x != AUTOWH) tmin_x = (int)((float)tmin_x*Canvas::getSystemFontScale());
            if (tmax_x != AUTOWH) tmax_x = (int)((float)tmax_x*Canvas::getSystemFontScale());
            if (tsug_x != AUTOWH) tsug_x = (int)((float)tsug_x*Canvas::getSystemFontScale());
          }
          if (o->guiobject_getAutoSysMetricsH()) {
            if (tmin_y != AUTOWH) tmin_y = (int)((float)tmin_y*Canvas::getSystemFontScale());
            if (tmax_y != AUTOWH) tmax_y = (int)((float)tmax_y*Canvas::getSystemFontScale());
            if (tsug_y != AUTOWH) tsug_y = (int)((float)tsug_y*Canvas::getSystemFontScale());
          }
        }
#endif

        RECT cor;
        w->getNonClientRect(&cor);
        RECT wr;
        getNonClientRect(&wr);

        int xdif = (wr.right-wr.left)-(cor.right-cor.left);
        int ydif = (wr.bottom-wr.top)-(cor.bottom-cor.top);
        if (tmin_x != AUTOWH) tmin_x += xdif;
        if (tmin_y != AUTOWH) tmin_y += ydif;
        if (tmax_x != AUTOWH) tmax_x += xdif;
        if (tmax_y != AUTOWH) tmax_y += ydif;
        if (tsug_x != AUTOWH) tsug_x += xdif;
        if (tsug_y != AUTOWH) tsug_y += ydif;
      }

      if (min_x != AUTOWH) min_x = (tmin_x != AUTOWH) ? MAX(min_x, tmin_x) : min_x; else min_x = tmin_x;
      if (max_x != AUTOWH) max_x = (tmax_x != AUTOWH) ? MAX(max_x, tmax_x) : max_x; else max_x = tmax_x;
      if (min_y != AUTOWH) min_y = (tmin_y != AUTOWH) ? MAX(min_y, tmin_y) : min_y; else min_y = tmin_y;
      if (max_y != AUTOWH) max_y = (tmax_y != AUTOWH) ? MAX(max_y, tmax_y) : max_y; else max_y = tmax_y;
      if (sug_x != AUTOWH) sug_x = (tsug_x != AUTOWH) ? MAX(sug_x, tsug_x) : sug_x; else sug_x = tsug_x;
      if (sug_y != AUTOWH) sug_y = (tsug_y != AUTOWH) ? MAX(sug_y, tsug_y) : sug_y; else sug_y = tsug_y;
    }

    if (min_x != AUTOWH && min_x == max_x) sug_x = min_x;
    if (min_y != AUTOWH && min_y == max_y) sug_y = min_y;

    switch (what) {
      case MINIMUM_W: return min_x;
      case MINIMUM_H: return min_y;
      case MAXIMUM_W: return max_x;
      case MAXIMUM_H: return max_y;
      case SUGGESTED_W: return sug_x;
      case SUGGESTED_H: return sug_y;
    }
  }

  switch (what) {
    case SUGGESTED_W: return suggested_w;
    case SUGGESTED_H: return suggested_h;
    case MAXIMUM_W: return maximum_w;
    case MAXIMUM_H: return maximum_h;
    case MINIMUM_W: return minimum_w;
    case MINIMUM_H: return minimum_h;
  }

  return AUTOWH;
}

void BaseWnd::setStartHidden(int wtf) {
  start_hidden = wtf;
}

LRESULT BaseWnd::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

  if (!isDestroying()) switch (uMsg) {
    case WM_CREATE:
      hwnd = hWnd;
    break;

    case WM_CLOSE:
      return 0;

    case WM_PAINT: {
      ASSERT(hwnd != NULL);
      if (!isVisible(1) || IsIconic(gethWnd())) break;
      RECT r;
      if (GetUpdateRect(hWnd, &r, FALSE)) {
	      if (virtualOnPaint()) { return 0; }
      }
    }
    break;

    case WM_NCPAINT: return 0;
    case WM_SYNCPAINT: return 0;

    case WM_SETCURSOR:
     if (checkModal()) return TRUE;
     if (hWnd == (HWND)wParam) {
        int ct = BASEWND_CURSOR_POINTER;
        int _x, _y;
        Std::getMousePos(&_x, &_y);
        screenToClient(&_x, &_y);
        if (!handleVirtualChildMsg(WM_SETCURSOR,_x,_y, &ct)) {
          ct = getCursorType(_x, _y);
        }
        char *wincursor = NULL;
        switch (ct) {
          case BASEWND_CURSOR_USERSET: /* do nothing */ break;
          default:
          case BASEWND_CURSOR_POINTER:
            wincursor = IDC_ARROW;
          break;
          case BASEWND_CURSOR_NORTHSOUTH:
            wincursor = IDC_SIZENS;
          break;
          case BASEWND_CURSOR_EASTWEST:
            wincursor = IDC_SIZEWE;
          break;
          case BASEWND_CURSOR_NORTHWEST_SOUTHEAST:
            wincursor = IDC_SIZENWSE;
          break;
          case BASEWND_CURSOR_NORTHEAST_SOUTHWEST:
            wincursor = IDC_SIZENESW;
          break;
          case BASEWND_CURSOR_4WAY:
            wincursor = IDC_SIZEALL;
          break;
          case BASEWND_CURSOR_EDIT:
            wincursor = IDC_IBEAM;
          break;
        }
        if (wincursor != NULL) SetCursor(LoadCursor(NULL, wincursor));
      }
    return TRUE;

    case WM_TIMER:
      timerCallback(wParam);
    return 0;

    case WM_SETFOCUS:
      if (isInited()) onGetFocus();
    break;

    case WM_KILLFOCUS:
      if(api) api->forwardOnKillFocus(); // resets the keyboard active keys buffer
      onKillFocus();
    break;

    // dragging and dropping

    case WM_LBUTTONDOWN: {
      api->popupexit_check(this);
      if (checkModal()) return 0;
      abortTip();
      int xPos = (signed short)LOWORD(lParam);
      int yPos = (signed short)HIWORD(lParam);
      xPos = (int)((float)xPos / getRenderRatio());
      yPos = (int)((float)yPos / getRenderRatio());
			if (!getCapture() && hasVirtualChildren())
			  if (handleVirtualChildMsg(WM_LBUTTONDOWN,xPos,yPos)) 
			    return 0;
      if (!dragging) {
        if (!isVirtual()) 
          setFocus();
        int r = onLeftButtonDown(xPos, yPos);
        if (checkDoubleClick(uMsg, xPos, yPos)) if (onLeftButtonDblClk(xPos, yPos)) return 0;
        return r;
      }
		}
    break;

    case WM_RBUTTONDOWN: {
      api->popupexit_check(this);
      if (checkModal()) return 0;
      abortTip();
      int xPos = (signed short)LOWORD(lParam);
      int yPos = (signed short)HIWORD(lParam);        
      xPos = (int)((float)xPos / getRenderRatio());
      yPos = (int)((float)yPos / getRenderRatio());
			if (!getCapture() && hasVirtualChildren())
			  if (handleVirtualChildMsg(WM_RBUTTONDOWN,xPos,yPos)) 
			    return 0;
      if (!dragging) {
        int r = onRightButtonDown(xPos, yPos);
        if (checkDoubleClick(uMsg, xPos, yPos)) if (onRightButtonDblClk(xPos, yPos)) return 0;
        return r;
      }
		}
    break;

    case WM_MOUSEMOVE: {
      if (checkModal()) return 0;
      int xPos = (signed short)LOWORD(lParam);
      int yPos = (signed short)HIWORD(lParam);        
      xPos = (int)((float)xPos / getRenderRatio());
      yPos = (int)((float)yPos / getRenderRatio());
      if (dragging) {
        POINT pt={xPos, yPos};
        clientToScreen(&pt);
        RootWnd *targ;
        int candrop = 0;
        // find the window the mouse is over

        targ = NULL;
        if (stickyWnd) {
          RECT wr;
          GetWindowRect(stickyWnd->gethWnd(), &wr);
          if (pt.x >= wr.left-sticky.left &&
              pt.x <= wr.right + sticky.right &&
              pt.y >= wr.top - sticky.top &&
              pt.y <= wr.bottom + sticky.bottom) targ = stickyWnd;
          else stickyWnd = NULL;
        }

        if (targ == NULL && api) targ = api->rootWndFromPoint(&pt); // FG> not to self, check

        DI prevtargdi(prevtarg);
        DI targdi(targ);

        if (prevtarg != targ) {	// window switch
          if (prevtarg != NULL) prevtargdi.dragLeave(this);
          if (targ != NULL) targdi.dragEnter(this);
        }
        if (targ != NULL)
          candrop = targdi.dragOver(pt.x, pt.y, this);
        if (targ == NULL || !candrop)
          SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_NO)));
        else
          SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_APPSTARTING)));
        prevtarg = targ;
      } else {
        tipbeenchecked=FALSE;
        if (!getCapture() && hasVirtualChildren()) {
          if (handleVirtualChildMsg(WM_MOUSEMOVE, xPos, yPos))
            return 0;
        }
        if (getCapture()) {
          if (onMouseMove(xPos, yPos)) 
            return 0;
        }
        if (!tipbeenchecked) onTipMouseMove();
        return 0;
      }
    }
    break;

    case WM_LBUTTONUP: {
      if (checkModal()) return 0;
      int xPos = (signed short)LOWORD(lParam);
      int yPos = (signed short)HIWORD(lParam);        
      xPos = (int)((float)xPos / getRenderRatio());
      yPos = (int)((float)yPos / getRenderRatio());
      abortTip();
      if (!dragging && !getCapture() && hasVirtualChildren()) {
        if (handleVirtualChildMsg(WM_LBUTTONUP,xPos,yPos)) 
          return 0;
      }
      if (dragging) {
        clientToScreen(&xPos, &yPos);
        int res = 0;
        if (prevtarg != NULL) {
          res = DI(prevtarg).dragDrop(this, xPos, yPos);
        }

        // inform source what happened
        dragComplete(res);

        resetDragSet();
        prevtarg = NULL;
        stickyWnd = NULL;
        suggestedTitle = NULL;
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
        ReleaseCapture();
        dragging = 0;
      } else {
        if (onLeftButtonUp(xPos, yPos)) return 0;
      }
    }
    break;

    case WM_RBUTTONUP: {
      if (checkModal()) return 0;
      abortTip();
      int xPos = (signed short)LOWORD(lParam);
      int yPos = (signed short)HIWORD(lParam);        
      xPos = (int)((float)xPos / getRenderRatio());
      yPos = (int)((float)yPos / getRenderRatio());
      if (!getCapture() && hasVirtualChildren()) {
        if (handleVirtualChildMsg(WM_RBUTTONUP,xPos,yPos))
          return 0;
      }
      if (!dragging) if (onRightButtonUp(xPos, yPos)) return 0;
    }
    break;

    case WM_CONTEXTMENU: {
      if (checkModal()) return 0;
      ASSERT(hWnd != NULL);
      int xPos = (signed short)LOWORD(lParam);
      int yPos = (signed short)HIWORD(lParam);        
      if (hWnd == gethWnd()) {
        if (onContextMenu(xPos, yPos)) return 0;
      } else if (GetParent(hWnd) == gethWnd()) {
        if (onChildContextMenu(xPos, yPos)) return 0;
      }
    }
    break;

    case WM_ERASEBKGND:
      return (onEraseBkgnd((HDC)wParam));

    case WM_MOUSEWHEEL: {
      abortTip();
      
      int l,a,t=LOWORD(wParam);;
      l=(short)HIWORD(wParam)/120;
      a=(short)HIWORD(wParam);
      if (!l)
        if (a > 0) l=1;
      else if (a < 0)l=0;
      a=l>=0?l:-l;
      if (GetAsyncKeyState(VK_MBUTTON)&0x8000) {
        if (l>=0) l=0; // Fast Forward 5s
        else l=1; // Rewind 5s
      } else {
        if (l>=0) l=2; // Volume up
        else l=3; // Volume down
      }

      if (l & 1)
        return onMouseWheelDown(!(BOOL)(l&2), a);
      else
        return onMouseWheelUp(!(BOOL)(l&2), a);
    }

    case WM_WA_RELOAD: {
      if (wParam == 0)
          freeResources();
        else
          reloadResources();
      return 0;
      }

    case WM_WA_GETFBSIZE: {
      SIZE *s = (SIZE *)wParam;
      s->cx = rwidth;
      s->cy = rheight;
      return 0;
      }
      
    case WM_USER+8976: // wheel in tip, delete tip
      abortTip();
      return 0;

    case WM_CHAR:
      if (curVirtualChildFocus == NULL) {
        if (onChar(((TCHAR) wParam))) return 0;
      } else {
        if (curVirtualChildFocus->onChar(((TCHAR) wParam))) return 0;
      }
      if (api && api->forwardOnChar((TCHAR) wParam)) return 0;
      break;

    case WM_KEYDOWN:
      if (curVirtualChildFocus == NULL) {
        if (onKeyDown((int) wParam)) return 0;
      } else {
        if (curVirtualChildFocus->onKeyDown((int)wParam)) return 0;
      }
      if (api && api->forwardOnKeyDown(this, (int) wParam)) return 0;
      break;

    case WM_KEYUP:
      if (curVirtualChildFocus == NULL) {
        if (onKeyUp((int) wParam)) return 0;
      } else {
        if (curVirtualChildFocus->onKeyUp((int)wParam)) return 0;
      }
      if (api && api->forwardOnKeyUp((int) wParam)) return 0;
      break;

    case WM_SYSKEYDOWN:
      if (curVirtualChildFocus == NULL) {
        if (onSysKeyDown((int) wParam, lParam)) return 0;
      } else {
        if (curVirtualChildFocus->onSysKeyDown((int)wParam, lParam)) return 0;
      }
      if (api && api->forwardOnSysKeyDown(this, (int) wParam, lParam)) return 0;
      break;

    case WM_SYSKEYUP:
      if (curVirtualChildFocus == NULL) {
        if (onSysKeyUp((int) wParam, lParam)) return 0;
      } else {
        if (curVirtualChildFocus->onSysKeyUp((int)wParam, lParam)) return 0;
      }
      if (api && api->forwardOnSysKeyUp((int) wParam, lParam)) return 0;
      break;
      
    case WM_MOUSEACTIVATE: {
      if (checkModal())
        return MA_NOACTIVATE;
      SetFocus(gethWnd());
      return MA_ACTIVATE;
      }

    case WM_ACTIVATEAPP:
      if (wParam == FALSE) {
        api->popupexit_signal();
        return 0;
      }
      
    case WM_ACTIVATE: {
      int fActive = LOWORD(wParam);
      if (fActive == WA_ACTIVE || fActive == WA_CLICKACTIVE)  {
        api->popupexit_check(this);
        onActivate();
      } else {
        onDeactivate();
      }
    }
    return 0;

    case WM_NCACTIVATE:
      if (allowDeactivation())
        return TRUE;
      return FALSE;

    case WM_WINDOWPOSCHANGED: {
      
      WINDOWPOS *lpwp = (WINDOWPOS *)lParam; // points to size and position data 
      if (lpwp->flags & SWP_HIDEWINDOW) {
        minimized = 1;
        onMinimize();
      } else if (lpwp->flags & SWP_SHOWWINDOW) {
        minimized = 0;
        onRestore();
      }

      if (!inonresize) {
        int w = rwidth;
        int h = rheight;
        multRatio(&w, &h);
        if (lpwp->cx != w || lpwp->cy != h) {
          OutputDebugString("external onResize\n");
          w = lpwp->cx;
          h = lpwp->cy;
          divRatio(&w, &h);
          setRSize(rx, ry, w, h);
          onResize();
        }
      }
      
      onPostedMove();
      return 0;
    }

    case WM_DROPFILES: {
      if (checkModal()) break;
      api->pushModalWnd();
      onExternalDropBegin();
      HDROP h = (HDROP)wParam;
      POINT dp;
      DragQueryPoint(h, &dp);
      clientToScreen(&dp);
      // build a file list
      char buf[WA_MAX_PATH];
      PtrList<FilenamePS> keep;
      
      SetCursor(LoadCursor(NULL, IDC_WAIT));

//CUT #if UTF8
//CUT       // doesn't really need UTF8, the "buf" is never written to.
//CUT       // made to be NULL to enforce this concept.
      int nfiles = DragQueryFile(h, 0xffffffff, NULL, 0);
//CUT #else
//CUT       int nfiles = DragQueryFile(h, 0xffffffff, buf, sizeof(buf));
//CUT #endif

      // convert them all to PlayItem *'s
      String buf8;
      for (int i = 0; i < nfiles; i++) {
#if UTF8
        if (Std::encodingSupportedByOS(SvcStrCnv::UTF16)) {
          // CODE FOR NT COMPATIBLE OS'S
          wchar_t bufW[WA_MAX_PATH];
          // bufW contains the next file we want to process.
          DragQueryFileW(h, i, bufW, sizeof(bufW));          
          // And convert so buf8 is carrying the package.
          EncodedStr fname16enc(SvcStrCnv::UTF16, bufW, (WSTRLEN(bufW)+1)*2, 0/*no delete*/);
          int retcode = fname16enc.convertToUTF8(buf8);
          if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
            DragQueryFile(h, i, buf, sizeof(buf));
            buf8 = buf;
          }
        } else {
          // CODE FOR 9x COMPATIBLE OS'S
          DragQueryFile(h, i, buf, sizeof(buf));
          // And convert so buf8 is carrying the package.
          EncodedStr fnameOSenc(SvcStrCnv::OSNATIVE, buf, (STRLEN(buf)+1), 0/*no delete*/);
          int retcode = fnameOSenc.convertToUTF8(buf8);
          if (retcode == SvcStrCnv::ERROR_UNAVAILABLE) {
            DragQueryFile(h, i, buf, sizeof(buf));
            buf8 = buf;
          }
        }
        addDroppedFile(buf8, &keep);	// recursive
#else
        DragQueryFile(h, i, buf, sizeof(buf));
        addDroppedFile(buf, &keep);	// recursive
#endif
      }
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      dragging = 1;
      if (dragEnter(this)) {
        if (dragOver(dp.x, dp.y, this)) dragDrop(this, dp.x, dp.y);
      } else {
        dragLeave(this);
      }
      dragging = 0;

      // remove data
      keep.deleteAll();
      resetDragSet();

      onExternalDropEnd();
      api->popModalWnd();
    }
    return 0;	// dropfiles

  case WM_CAPTURECHANGED: {
    if (preventcancelcapture) break;
    inputCaptured = 0;
    if (curVirtualChildCaptured != NULL) {
      RootWnd *w = curVirtualChildCaptured;
      curVirtualChildCaptured = NULL;
      w->onCancelCapture();
    } else
      onCancelCapture();
    return 0;
    }
  }

  if (uMsg >= WM_USER) {
    int ret;
    if (onUserMessage(uMsg, wParam, lParam, &ret))
      return ret;
    return 0;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int BaseWnd::onUserMessage(int msg, int w, int l, int *r) {
  return 0;
}

BaseWnd::checkDoubleClick(int b, int x, int y) {

  DWORD now = Std::getTickCount();

  switch (b) {
    case WM_LBUTTONDOWN:
      if (lastClick[0] > now - Std::getDoubleClickDelay()) {
        lastClick[0] = 0;
        if (abs(lastClickP[0].x - x) > Std::getDoubleClickX() || abs(lastClickP[0].y - y) > Std::getDoubleClickY()) return 0;
        return 1;
      }
      lastClick[0] = now;
      lastClickP[0].x = x;
      lastClickP[0].y = y;
      break;

    case WM_RBUTTONDOWN:
      if (lastClick[1] > now - Std::getDoubleClickDelay()) {
        lastClick[1] = 0;
        if (abs(lastClickP[1].x - x) > Std::getDoubleClickX() || abs(lastClickP[1].y - y) > Std::getDoubleClickY()) return 0;
        return 1;
      }
      lastClick[1] = now;
      lastClickP[1].x = x;
      lastClickP[1].y = y;
      break;
    }

  return 0;
}

BaseWnd::onMouseWheelUp(int click, int lines) {
  return 0;
}

BaseWnd::onMouseWheelDown(int click, int lines) {
  return 0;
}

int BaseWnd::onContextMenu(int x, int y) {
  return 0;
}

int BaseWnd::onChildContextMenu(int x, int y) {
  return 0;
}

int BaseWnd::onDeferredCallback(int param1, int param2) {
  switch (param1) {
    case DEFERREDCB_FLUSHPAINT:
      do_flushPaint();
      break;
    case DEFERREDCB_INVALIDATE:
      if (isPostOnInit())
        invalidate();
      break;
    case 0x7849/*DEFERREDCB_ONHIDE*/: {
      foreach(rootwndchildren)
        RootWnd *w = rootwndchildren.getfor();
        if (w->isVisible(1)) // check internal flag only
          w->onSetVisible(0);
      endfor;
      dependent_sendEvent(RootWnd::depend_getClassGuid(), Event_SETVISIBLE, 0);
      break;
    }
  }
  return 0;
}

int BaseWnd::onPaint(Canvas *canvas) {
#if 0
  // example:
  PaintCanvas c;
  if (!c.beginPaint(this)) return 0;
  (do some painting)
  c will self-destruct on return
#endif
  if (renderbasetexture) {
    PaintCanvas paintcanvas;
    if (canvas == NULL) {
      if (!paintcanvas.beginPaint(this)) return 0;
      canvas = &paintcanvas;
    }
    RECT r;
    getNonClientRect(&r);
    RenderBaseTexture(canvas, r);
  }
  return 0;
}

int BaseWnd::onPaint(Canvas *canvas, Region *h) {

  if (!canvas) return onPaint(canvas);

  int sdc = SaveDC(canvas->getHDC());

  canvas->selectClipRgn(h);

	int rs = onPaint(canvas);

  RestoreDC(canvas->getHDC(), sdc);

	return rs;
}

int BaseWnd::getTransparency() {
  return wndalpha;
}

void BaseWnd::setTransparency(int amount) {

  if (wndalpha == amount) return;

  if (amount != -1) wndalpha = amount; else amount = wndalpha;

  if (!isDesktopAlphaAvailable() && amount < 255) {
    wndalpha = 255;
    return;
  } 

  if (w2k_alpha) {
    invalidate(); 
    return;
  }

#ifdef WIN32

  if (!isInited()) return;
  if (isVirtual()) return;

  if (amount < -1) amount = 0;
  else if (amount > 255) amount = 255;

  DWORD dwLong = GetWindowLong(hwnd, GWL_EXSTYLE);
  if (amount==255) {
    if (dwLong & WS_EX_LAYERED)
      SetWindowLong(hwnd, GWL_EXSTYLE, dwLong & ~WS_EX_LAYERED);
  } else {
    if (!(dwLong & WS_EX_LAYERED))
      SetWindowLong(hwnd, GWL_EXSTYLE, dwLong | WS_EX_LAYERED);
    setLayeredWindowAttributes(hwnd, RGB(0,0,0), amount, LWA_ALPHA);
  }
#endif
}

int BaseWnd::beginCapture() {
  if (!getCapture()) {
    curVirtualChildCaptured = NULL;
/*    oldCapture = */SetCapture(gethWnd());
/*    if (oldCapture) {
      OutputDebugString("Stolen capture detected, this may be ok, but try to avoid it if possible. Saving old capture\n");
    }*/
    inputCaptured = 1;
  }
  return 1;
}

int BaseWnd::endCapture() {
  preventcancelcapture = 1;
  if (!inputCaptured) return 0;
  ReleaseCapture();
/*  if (oldCapture) { 
    OutputDebugString("Restoring old capture\n");
    SetCapture(oldCapture); 
    oldCapture = NULL; 
  }*/
  inputCaptured = 0;
  preventcancelcapture = 0;
  return 1;
}

int BaseWnd::getCapture() {
  if (inputCaptured && GetCapture() == gethWnd() && curVirtualChildCaptured == NULL) return 1;
  return 0;
}

void BaseWnd::cancelCapture() {
  if (curVirtualChildCaptured != NULL) {
    curVirtualChildCaptured->cancelCapture();
    return;
  } 
  if (getCapture()) endCapture();
  onCancelCapture();
}

int BaseWnd::onMouseMove(int x, int y) { 
  onTipMouseMove();
  return 0;
}

void BaseWnd::onTipMouseMove() {
  POINT p;

  if (dragging) return;

  tipbeenchecked = TRUE;

  Std::getMousePos(&p.x, &p.y);

  if (api->rootWndFromPoint(&p) != (RootWnd *)this) { // leaving area
    tip_done = FALSE;
    if (tipawaytimer)
      killTimer(TIP_AWAY_ID);
    tipawaytimer = FALSE;
    if (tipshowtimer) 
      killTimer(TIP_TIMER_ID);
    tipshowtimer = FALSE;
    destroyTip();
  } else { // moving in area
    const char *t = getTip();
    if (!tipshowtimer && !tip_done && t != NULL && *t != 0) { //entering area & need tip
      setTimer(TIP_TIMER_ID, TIP_TIMER_THRESHOLD);
      tipshowtimer=TRUE;
    } else if (tipshowtimer) {
      killTimer(TIP_TIMER_ID);
      setTimer(TIP_TIMER_ID, TIP_TIMER_THRESHOLD);
    }
  }
}

int BaseWnd::onLeftButtonDblClk(int x, int y) {
  return 0;
}

int BaseWnd::onRightButtonDblClk(int x, int y) {
  return 0;
}

int BaseWnd::onGetFocus() {	// return TRUE if you override this
//  if (curVirtualChildFocus == NULL && hasVirtualChildren()) {
    if (!wantFocus() && !skipnextfocus) {
      skipnextfocus = 0;

      // if a real child already told us to give him back the focus when we get it, do so
      if (curRealChildFocus != NULL) {
        curRealChildFocus->setFocus();
      } else {
        // otherwise, look if we have any real children windows, if so, give focus to the first one
        HWND child = GetWindow(gethWnd(), GW_CHILD);
        if (child) {
          SetFocus(child);
        } else {
          // we had no real child, find the first virtual child that wants focus and give it
          RootWnd *w = getNextVirtualFocus(NULL);
          if (w) w->setFocus();
        }
      }
    }
  skipnextfocus = 0;
//  }
  hasfocus = 1;
  notifyParent(ChildNotify::GOTFOCUS);
  return 1;
}

int BaseWnd::onKillFocus() {	// return TRUE if you override this
  hasfocus = 0;
  if (curVirtualChildFocus)
    curVirtualChildFocus->onKillFocus();
  else
    notifyParent(ChildNotify::KILLFOCUS);
  curVirtualChildFocus = NULL;
  return 1;
}

int BaseWnd::childNotify(RootWnd *child, int msg, int p1, int p2) {
  return 0;
}

int BaseWnd::addDragItem(const char *droptype, void *item) {
  ASSERT(droptype != NULL);
  if (item == NULL) return -1;
  DragSet *set;
  int pos = dragCheckData(droptype);
  if (pos == -1) {
    set = new DragSet();
    set->setName(droptype);
    dragsets.addItem(set);
    pos = dragsets.getNumItems() - 1;
  } else set = dragsets[pos];
  set->addItem(item);
  return pos;
}

int BaseWnd::handleDrag() {
  abortTip();
  if (dragsets.getNumItems() == 0) return 0;

  SetCapture(hwnd);
  SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_APPSTARTING)));

  dragging = 1;
  stickyWnd = NULL;

  return 1;
}

int BaseWnd::resetDragSet() {
  dragsets.deleteAll();
  return 1;
}

int BaseWnd::dragEnter(RootWnd *sourceWnd) {
  RootWnd *rw = getParent(); //RootWnd::rootwndFromRootWnd(getParent()); //FG> note to self, check!
  if (rw) return DI(rw).dragEnter(sourceWnd);
  return 0;
}

int BaseWnd::dragSetSticky(RootWnd *wnd, int left, int right, int up, int down){
  ASSERT(dragging);
  stickyWnd = wnd;

  if (left < 0) left = 0;
  if (right < 0) right = 0;
  if (up < 0) up = 0;
  if (down < 0) down = 0;

  Std::setRect(&sticky, left, up, right, down);

  return 1;
}

void BaseWnd::setSuggestedDropTitle(const char *title) {
  ASSERT(title != NULL);
  suggestedTitle = title;
}

const char *BaseWnd::dragGetSuggestedDropTitle() {
  return suggestedTitle;	// can be NULL
}

int BaseWnd::dragCheckData(const char *type, int *nitems) {
  for (int i = 0; i < dragsets.getNumItems(); i++) {
    if (STRCASEEQL(type, dragsets[i]->getName())) {
      if (nitems != NULL) *nitems = dragsets[i]->getNumItems();
      return i;
    }
  }
  return -1;
}

void *BaseWnd::dragGetData(int slot, int itemnum) {
  if (slot < 0 || slot >= dragsets.getNumItems()) return 0;
  if (itemnum < 0 || itemnum >= dragsets[slot]->getNumItems()) return 0;
  return dragsets[slot]->enumItem(itemnum);
}

void BaseWnd::addDroppedFile(const char *filename, PtrList<FilenamePS> *plist) {
#ifdef RECURSE_SUBDIRS_ON_DROP
  const char *slash = filename + STRLEN(filename) - 1;
  for (; slash > filename; slash--) if (*slash == '/' || *slash == '\\') break;
  if (STREQL(slash+1, ".") || STREQL(slash+1, "..")) return;

  char buf[WA_MAX_PATH];
  STRCPY(buf, filename);
  // try to resolve shortcuts
  char *ext = buf + STRLEN(buf) - 1;
  for (; ext > buf; ext--) if (*ext == '.' || *ext == '\\' || *ext == '/') break;
  if (!STRICMP(ext, ".lnk")) {
    char buf2[MAX_PATH];
    if (ResolveShortCut(gethWnd(), buf, buf2)) STRCPY(buf, buf2);
  }

  int isdir = 0;

  // handle root dir specially?
#if UTF8
  ReadDir testForDir(buf, NULL, 0/*don't skip dots*/); // using ReadDir to test for dir?
  if (testForDir.next()) {
    // the first file you get if you don't skip dots is your own self if you're a dir.
    isdir = testForDir.isDir();
  }
#else  
  WIN32_FIND_DATA data;
  HANDLE r = FindFirstFile(buf, &data);
  if (!r) return;
  FindClose(r);
  if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) isdir = 1;
#endif

  if (isdir) {
    onExternalDropDirScan(buf);
#if UTF8
    ReadDir dirFiles(buf); // default params means do the whole directory, recursively
    while (dirFiles.next()) {
      addDroppedFile(StringPrintf("%s%s%s", buf, DIRCHARSTR, dirFiles.getFilename()), plist);
    }
#else
    // enumerate that dir
    char search[WA_MAX_PATH];
    wsprintf(search, "%s\\*.*", buf);
    HANDLE files = FindFirstFile(search, &data);
    if (files == INVALID_HANDLE_VALUE) return;	// nothin' in it
    for (;;) {
      char obuf[WA_MAX_PATH];
      wsprintf(obuf, "%s\\%s", buf, data.cFileName);
      addDroppedFile(obuf, plist);
      if (!FindNextFile(files, &data)) {
        FindClose(files);
        return;
      }
    }
#endif
    // should never get here
  } else {
    addDragItem(DD_FILENAME, plist->addItem(new FilenamePS(StringPrintf("file:%s", buf))));
  }
#else
    addDragItem(DD_FILENAME, plist->addItem(new FilenamePS(StringPrintf("file:%s", filename))));
#endif
}

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  // fetch out the RootWnd *
  if (uMsg == WM_CREATE) {
    CREATESTRUCT *cs = (CREATESTRUCT *)lParam;
    ASSERT(cs->lpCreateParams != NULL);
    // stash pointer to self
    SetWindowLong(hWnd, GWL_USERDATA, (LONG)cs->lpCreateParams);
  }

  // pass the messages into the BaseWnd
  RootWnd *rootwnd = (RootWnd*)GetWindowLong(hWnd, GWL_USERDATA);
  if (rootwnd == NULL) {
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  } else {
    int r = rootwnd->wndProc(hWnd, uMsg, wParam, lParam);
    rootwnd->performBatchProcesses();
    return r;
  }
}

static void register_wndClass(HINSTANCE hInstance) {

  WNDCLASS wc;
  if (GetClassInfo(hInstance, BASEWNDCLASSNAME, &wc)) return;

  // regiszter pane class
  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC)wndProc;
  wc.cbClsExtra		= 0;
  wc.cbWndExtra		= 0;
  wc.hInstance		= hInstance;
  wc.hIcon		= NULL;
  wc.hCursor		= NULL;
  wc.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
  wc.lpszMenuName	= NULL;
  wc.lpszClassName	= BASEWNDCLASSNAME;

  int r = RegisterClass(&wc);
  if (r == 0) {
    int res = GetLastError();
    if (res != ERROR_CLASS_ALREADY_EXISTS) {
      char florp[WA_MAX_PATH];
      wsprintf(florp, "Failed to register class, err %d", res);
      ASSERTPR(0, florp);
    }
  }
}

BOOL BaseWnd::getNoCopyBits(void) {
  return ncb;
}

void BaseWnd::setNoCopyBits(BOOL newncb) {
  ncb = newncb;
}

int BaseWnd::onEraseBkgnd(HDC dc) {
  return 1; 
}

void BaseWnd::setIcon(HICON icon, int _small) {
  SendMessage(gethWnd(), WM_SETICON, _small ? ICON_SMALL : ICON_BIG, (int)icon);
}

const char *BaseWnd::getTip() {
  return tip;
}

void BaseWnd::setTip(const char *_tooltip) {
  tip = _tooltip;
  abortTip();
}

int BaseWnd::getStartHidden() {
  return start_hidden;
}

void BaseWnd::abortTip() {
  if (tipshowtimer)
    killTimer(TIP_TIMER_ID);
  tipshowtimer=FALSE;
  if (tipawaytimer)
    killTimer(TIP_AWAY_ID);
  tipawaytimer=FALSE;
  if (tipdestroytimer)
    killTimer(TIP_DESTROYTIMER_ID);
  tipdestroytimer=FALSE;
  destroyTip();
  tip_done=FALSE;
  RECT r;
  if (gethWnd() && GetUpdateRect(gethWnd(), &r, FALSE) != 0) // FG> avoids xoring over disapearing tip
    repaint();
}

int BaseWnd::isVisible(int within) {
  if (!isVirtual() && !gethWnd()) return 0;
  if (!this_visible) return 0; 

  if (within) return this_visible; // whatever, local

  if (isVirtual()) // virtual, global
    if (getParent())
      return getParent()->isVisible();
    else
      return 0;
  
  // non virtual, global
  if (GetWindowLong(gethWnd(), GWL_STYLE) & WS_POPUP) return this_visible;
  if (getParent()) return getParent()->isVisible(); // not a popup, check its parent or fail
  return 0;
}

void BaseWnd::registerRootWndChild(RootWnd *child) {
  rootwndchildren.addItem(child);
	if (child->isVirtual())
    virtualChildren.addItem(child);
}

void BaseWnd::unregisterRootWndChild(RootWnd *child) {
  rootwndchildren.removeItem(child);
  if (curVirtualChildCaptured == child) {
    setVirtualChildCapture(NULL);
  }
  if (curVirtualChildFocus == child)
    curVirtualChildFocus = NULL;
  virtualChildren.removeItem(child);
  //api->timer_remove(this, -1);
  if (isPostOnInit())
    postDeferredCallback(DEFERREDCB_INVALIDATE, 0);
}

int BaseWnd::isVirtual() { 
  return 0; 
}

inline int isInRect(RECT *r,int x,int y) {
	if(x>=r->left&&x<=r->right&&y>=r->top&&y<=r->bottom) return 1;
	return 0;
}

int BaseWnd::ensureVirtualCanvasOk() {
	RECT ncr;

  if (isVirtual() && getParent()) return 1;

	int size_w = rwidth;
	int size_h = rheight;
                    
	if(!size_w||!size_h) return 0;

  if(!virtualCanvas || virtualCanvasH!=size_h || virtualCanvasW!=size_w) {
    if(virtualCanvas) {
      deleteFrameBuffer(virtualCanvas);
      virtualCanvas=NULL;
    }
    delete scalecanvas;
    scalecanvas = NULL;
    virtualCanvas = createFrameBuffer(size_w, size_h);
    prepareFrameBuffer(virtualCanvas, size_w, size_h);
		virtualCanvas->setBaseWnd(this);
		virtualCanvasH=size_h; virtualCanvasW=size_w;
		BaseWnd::getNonClientRect(&ncr);
		deferedInvalidateRect(&ncr);
	}
  return 1;
}

Canvas *BaseWnd::createFrameBuffer(int w, int h) {
	return new BltCanvas(w,h);      
}

void BaseWnd::prepareFrameBuffer(Canvas *canvas, int w, int h) {
  RECT r={0,0,w, h};
  RegionI reg(&r);
  virtualBeforePaint(&reg);
  canvas->selectClipRgn(NULL);
  canvas->fillRect(&r, 0);
  virtualAfterPaint(&reg);
}

void BaseWnd::deleteFrameBuffer(Canvas *canvas) {
  delete canvas;
}

// paints the client content, followed by the virtual child tree. recursive
int BaseWnd::paintTree(Canvas *canvas, Region *r) {

  onPaint(canvas, r);

  if (isVirtual() && !hasVirtualChildren()) return 0;

  Region *hostrgn = NULL;
  Region *update = r;

  if (!(hwnd != NULL && getParent() == NULL)) {
    hostrgn = getRegion();
    update = r->clone();
    if (hostrgn && !isRectRgn()) {
      RECT ncr;
      getClientRect(&ncr);
      Region *hostclone = hostrgn->clone();
      hostclone->offset(ncr.left, ncr.top);
      update->and(hostclone);
      hostrgn->disposeClone(hostclone);
    }
  }

  RegionI *client_update = new RegionI();
  for (int i=0;i<virtualChildren.getNumItems();i++) {
    if (!virtualChildren[i]->isVisible(1)) continue;
    RECT rChild;
    RootWnd *w = virtualChildren[i];
    w->getNonClientRect(&rChild);
    if ((rChild.right != rChild.left) && (rChild.bottom != rChild.top))
    if (update->intersectRect(&rChild, client_update)) {
      w->paintTree(canvas, client_update);
    }
  }
  delete client_update;
  if (update != r) r->disposeClone(update);

  return 1;
}

void BaseWnd::setVirtualCanvas(Canvas *c) {
  virtualCanvas = c;
}

int BaseWnd::pointInWnd(POINT *p) {
  RECT r;
  if (!isVisible(1)) return 0;
  getWindowRect(&r);
  if (!Std::pointInRect(&r, *p))
    return 0;
  for(int i=0;i<getNumRootWndChildren();i++) {
    RootWnd *c = enumRootWndChildren(i);
    if (!c->isVisible(1)) continue;
    RECT rChild;
    c->getWindowRect(&rChild);
    if (Std::pointInRect(&rChild, *p))
      return 0;
  }
  //NONPORTABLE
/*  HWND child = GetWindow(gethWnd(), GW_CHILD);
  while (child != NULL) {
    if (IsWindowVisible(child)) {
      RECT r2;
      GetWindowRect(child, &r2);
      if (Std::pointInRect(&r2, *p))
        return 0;
    }
    child = GetWindow(child, GW_HWNDNEXT);
  }*/
  return 1;
}

int BaseWnd::virtualOnPaint() {

  RECT cr;
  getNonClientRect(&cr);
  if (cr.left == cr.right || cr.top == cr.bottom) return 0;

  if (gdi32instance == NULL && !grrfailed) {
    gdi32instance=LoadLibrary("GDI32.dll");
    if (gdi32instance != NULL) {
      getRandomRgn = (int (WINAPI *)(HDC,HRGN,int)) GetProcAddress(gdi32instance,"GetRandomRgn");
      if (getRandomRgn == NULL) {
        grrfailed=1;
        FreeLibrary(gdi32instance);
        gdi32instance=NULL;
      }
    } else {
      grrfailed=1;
    }
  }

  if (!ensureVirtualCanvasOk()) return 0;

  RegionI *r = new RegionI();

  GetUpdateRgn(gethWnd(), r->getOSHandle(), FALSE);

  virtualBeforePaint(r);

  PaintCanvas paintcanvas;
  if (!paintcanvas.beginPaint(this)) { virtualAfterPaint(r); return 0; }

  // DO NOT DELETE - This looks like it does nothing, but it actually makes the GDI call us again with WM_PAINT if some window
  // moves over this one between BeginPaint and EndPaint. We still use GetUpdateRgn so we don't have to check for
  // the version of Windows. See doc. If this function is not available (should be here in 95/98/NT/2K, but we never know)
  // then we use the rcPaint rect... less precise, but still works.
/*  if (getRandomRgn) {
    Region *zap = new Region();
    getRandomRgn(paintcanvas.getHDC(), zap->getHRGN(), SYSRGN);
    delete zap;
  } else { 
    RECT z;
    paintcanvas.getRcPaint(&z);
    r->setRect(&z);
  }*/

  // -------------

/*// for debug
  HDC dc = GetDC(gethWnd());
  InvertRgn(dc, r->getHRGN());
  InvertRgn(dc, r->getHRGN());
  ReleaseDC(gethWnd(), dc);*/

  RECT rcPaint;
  r->getBox(&rcPaint);

  double ra = getRenderRatio();

  if (deferedInvalidRgn) {
    Region *nr=NULL;
    if (ABS(ra-1.0) > 0.01) {
      nr=r->clone();
      double d=1.0/ra;
      nr->scale(d,d,TRUE); 
    }
    RegionI *i = new RegionI();
    if (deferedInvalidRgn->intersectRgn(nr?nr:r, i)) { // some deferednvalidated regions needs to be repainted
      paintTree(virtualCanvas, i);
      deferedValidateRgn(i);
  	}
    delete i;
    if (nr) r->disposeClone(nr);
  }


  virtualAfterPaint(r);
  virtualCanvasCommit(&paintcanvas, &rcPaint, ra);

  delete r;
  return 1;
}

RootWnd *BaseWnd::enumVirtualChild(int _enum) {
  return virtualChildren[_enum];
}

int BaseWnd::getNumVirtuals() {
  return virtualChildren.getNumItems();
}

RootWnd *BaseWnd::enumRootWndChildren(int n) {
  return rootwndchildren.enumItem(n);
}

int BaseWnd::getNumRootWndChildren() {
  return rootwndchildren.getNumItems();
}

RootWnd *BaseWnd::findRootWndChild(int x,int y, int only_virtuals) {
  for(int i=getNumRootWndChildren()-1;i>-1;i--) {
    RECT r;
    RootWnd *child = enumRootWndChildren(i);
//    OutputDebugString(StringPrintf("findRootWndChild = entering = %s\n", child->getId()));
    if (only_virtuals && !child->isVirtual()) continue;
    child->getNonClientRect(&r);
    int _x = x;
    int _y = y;
    if (!child->isVirtual()) {
      POINT pt;
      child->getPosition(&pt);
      _x -= pt.x;
      _y -= pt.y;
    } 
    int iv = child->isVisible(1);
    //int gpa = child->getPaintingAlpha();
    if (iv /*&& gpa > 0*/ && _x>=r.left&&_x<=r.right&&_y>=r.top&&_y<=r.bottom) { // GROUP
      RootWnd *z = child->findRootWndChild(_x, _y);
      if (z) return z; 
    }
/*gpa > 0 &&*/
    /*if (iv && _x>=r.left&&_x<=r.right&&_y>=r.top&&_y<=r.bottom && !child->isClickThrough() && child->ptInRegion(_x, _y)) { 
      return child;
    }*/
  }
  return (!isClickThrough() && ptInRegion(x, y)) ? this : NULL;
}

int BaseWnd::handleVirtualChildMsg(UINT uMsg, int x, int y, void *p) {

  RootWnd *child=NULL;

  if(curVirtualChildCaptured) 
    child=curVirtualChildCaptured;
  else 
    child=findRootWndChild(x, y, 1); // warning, can return this which is not virtual

//  ASSERT(child != NULL);	// BU this came up and I don't know why, looks like it should never happen
// FG> actually it can happen when coming back from a popup menu when cpu usage is high, the window might be
// hidden (destroying) and ptInRegion returns false.
  if (!child) return 0;

  int isvirtual = child->isVirtual();

  if (child) child = child->getForwardWnd(); 

  if (child) {
    switch(uMsg) {
      case WM_LBUTTONDOWN:
        if (isvirtual && child != curVirtualChildFocus)
          focusVirtualChild(child);
        child->onLeftButtonDown(x, y);
        if (child->checkDoubleClick(uMsg, x, y))
          child->onLeftButtonDblClk(x, y);
      return 1;
      case WM_RBUTTONDOWN:
        if (isvirtual && child != curVirtualChildFocus)
          focusVirtualChild(child);
        child->onRightButtonDown(x, y);
        if (child->checkDoubleClick(uMsg, x, y))
          child->onRightButtonDblClk(x, y);
      return 1;
      case WM_LBUTTONUP:
        child->onLeftButtonUp(x, y);
      return 1;
      case WM_RBUTTONUP:
        child->onRightButtonUp(x, y);
      return 1;
      case WM_MOUSEMOVE:{
        if (curVirtualChildCaptured == child || (curVirtualChildCaptured == NULL)) {
          child->onMouseMove(x, y);
          return 1;
        }
        return 0;
      }
      case WM_SETCURSOR:
        if (!p) return 0;
        *(int *)p = child->getCursorType(x, y);
        return 1;
    }
  }
  return 0;
}

int BaseWnd::onLeftButtonDown(int x, int y) {
 if (focus_on_click) setFocus();
 return 1;
}

void BaseWnd::setVirtualChildCapture(RootWnd *child) {
  if (child) {
    if (!inputCaptured) {
      beginCapture();
    }
  } else {
    endCapture();
  }
  curVirtualChildCaptured = child;
}

RootWnd *BaseWnd::getVirtualChildCapture() {
  if (inputCaptured && GetCapture() == gethWnd())
    return curVirtualChildCaptured;
  else
    if (inputCaptured) inputCaptured=0;
  return NULL;
}

RootWnd *BaseWnd::getBaseTextureWindow() {
  // return our base texture window if we have it
  if (btexture)
    return btexture;
  // return our parent's if they have it
  if (getParent())
    return getParent()->getBaseTextureWindow();
  else
    return NULL;
}

void BaseWnd::renderBaseTexture(CanvasBase *c, RECT &r, int alpha) {
  api->skin_renderBaseTexture(getBaseTextureWindow(), c, r, this, alpha);
}

void BaseWnd::setBaseTextureWindow(RootWnd *w) {
  btexture = w;
}

void BaseWnd::setNotifyWindow(RootWnd *newnotify) {
  notifyWindow = newnotify;
}

RootWnd *BaseWnd::getNotifyWindow() {
  return destroying ? NULL : notifyWindow;
}

int BaseWnd::gotFocus() {
  return hasfocus && curVirtualChildFocus == NULL;
}

int BaseWnd::isActive() {
  HWND h = hwnd;
  if (h == NULL) {
    RootWnd *par = getParent();
    if (par == NULL) return 0;
    h = par->gethWnd();
  }
  if (h == NULL) return 0;
  return (GetActiveWindow() == h);
}

int BaseWnd::onChar(unsigned int c) {
/*  switch (c) {
    case 9: // TAB
        focusNextSibbling(1);
      return 1;
  }*/
  return 0;
}

int BaseWnd::focusVirtualChild(RootWnd *child) {
  if (!gotFocus()) setFocus();
  if (!child->wantFocus()) return 0;
  setVirtualChildFocus(child);
  return 1;
}

int BaseWnd::wantFocus() {
  return 1;
}

// Return 1 if there is a modal window that is not this
int BaseWnd::checkModal() {
  if (bypassModal()) return 0;
  RootWnd *w = api->getModalWnd();
  if (w && w != (RootWnd*)this && w != getDesktopParent()) {
    return 1;
  }
  return 0;
}

int BaseWnd::cascadeRepaintFrom(RootWnd *who, int pack) {
  RECT r;
  BaseWnd::getNonClientRect(&r);
  return BaseWnd::cascadeRepaintRect(&r, pack);
}

int BaseWnd::cascadeRepaint(int pack) {
  return cascadeRepaintFrom(this, pack);
}

int BaseWnd::cascadeRepaintRgn(Region *r, int pack) {
  return cascadeRepaintRgnFrom(r, this, pack);
}

int BaseWnd::cascadeRepaintRect(RECT *r, int pack) {
  return cascadeRepaintRectFrom(r, this, pack);
}

int BaseWnd::cascadeRepaintRectFrom(RECT *r, RootWnd *who, int pack) {
  RegionI *reg = new RegionI(r);
  int rt = cascadeRepaintRgnFrom(reg, who, pack);
  delete reg;
  return rt; 
}

void BaseWnd::_cascadeRepaintRgn(Region *rg) {
  if (!ensureVirtualCanvasOk()) return;

  virtualBeforePaint(rg);

  deferedInvalidateRgn(rg);
  paintTree(virtualCanvas, rg);

  DCCanvas *paintcanvas;
  HDC dc = GetDC(gethWnd());
  paintcanvas = new DCCanvas();
  paintcanvas->cloneDC(dc, this);

  virtualAfterPaint(rg);

  double ra = getRenderRatio();

  RECT rcPaint;
  rg->getBox(&rcPaint);

  RECT rc;
  getClientRect(&rc); //JF> this gets it in virtual (non-scaled) coordinates,
                      // so we need to do these comparisons before scaling.
  rcPaint.bottom = MIN((int)rc.bottom, (int)rcPaint.bottom);
  rcPaint.right = MIN((int)rc.right, (int)rcPaint.right);
 
  if (ABS(ra-1.0) > 0.01) // probably faster than scaling the clone
  {
    rcPaint.left=(int)((rcPaint.left-1)*ra);
    rcPaint.top=(int)((rcPaint.top-1)*ra);
    rcPaint.right=(int)(rcPaint.right*ra + 0.999999);
    rcPaint.bottom=(int)(rcPaint.bottom*ra + 0.999999);
  }
  rcPaint.left = MAX(0, (int)rcPaint.left);
  rcPaint.top = MAX(0, (int)rcPaint.top);
  rcPaint.right = MIN((int)rcPaint.right, (int)(rwidth*ra));
  rcPaint.bottom  = MIN((int)rcPaint.bottom, (int)(rheight*ra));

  virtualCanvasCommit(paintcanvas, &rcPaint, ra);

	ReleaseDC(gethWnd(), dc);
	delete paintcanvas;
}

void BaseWnd::packCascadeRepaintRgn(Region *rg) {
  if (!deferedCascadeRepaintRgn) deferedCascadeRepaintRgn = new RegionI;
  deferedCascadeRepaintRgn->add(rg);
  need_flush_cascaderepaint = 1;
}

int BaseWnd::cascadeRepaintRgnFrom(Region *_rg, RootWnd *who, int pack) {

  Region *rg = _rg->clone();

  int j = virtualChildren.searchItem(who);
  for (int i=0;i<virtualChildren.getNumItems();i++) {
    RootWnd *w = virtualChildren[i];
    if (w != who && w->wantSiblingInvalidations()) 
      w->onSiblingInvalidateRgn(rg, who, j, i);
  }

  if (!pack) {
    _cascadeRepaintRgn(rg);
  } else {
    packCascadeRepaintRgn(rg);
  }

  _rg->disposeClone(rg);
  return 1;
}

void BaseWnd::setDesktopAlpha(int a) {
  if (a && !isDesktopAlphaAvailable()) return;
  if (a == w2k_alpha) return;
  w2k_alpha = a;
  if (!a && scalecanvas) {
    delete scalecanvas;
    scalecanvas = NULL;
  }
  setLayeredWindow(w2k_alpha);
  onSetDesktopAlpha(a);
}

void BaseWnd::onSetDesktopAlpha(int a) { };

void BaseWnd::virtualCanvasCommit(Canvas *paintcanvas, RECT *r, double ra) {

  if (w2k_alpha && isDesktopAlphaAvailable()) {
   	BLENDFUNCTION blend= {AC_SRC_OVER, 0, wndalpha, AC_SRC_ALPHA };
    POINT pt={0,0};
    RECT spr;
    getWindowRect(&spr);
    POINT sp={spr.left,spr.top};
    SIZE ss={spr.right-spr.left, spr.bottom-spr.top};
    SysCanvas c;

    if (handleRatio() && renderRatioActive()) {
      // eek slow!
      RECT r;
      getWindowRect(&r);
      int w=r.right-r.left;
      int h=r.bottom-r.top;
      if (!scalecanvas) scalecanvas = new BltCanvas(w, h);
      virtualCanvas->stretchblit(0, 0, (int)((double)virtualCanvasW * 65536.0), (int)((double)virtualCanvasH * 65536.0), scalecanvas, 0, 0, w, h);
    }

    updateLayeredWindow(hwnd, c.getHDC(), &sp, &ss, (scalecanvas ? scalecanvas : virtualCanvas)->getHDC(), &pt, 0, &blend, ULW_ALPHA);
  } else {

    if (ABS(ra-1.0) <= 0.01) {
  	  virtualCanvas->blit(r->left,r->top,paintcanvas,r->left,r->top,r->right-r->left,r->bottom-r->top);
    } else {
      RECT tr=*r;

      double invra=65536.0/ra;
      int lp=tr.left;
      int tp=tr.top;
      int w=tr.right-tr.left;
      int h=tr.bottom-tr.top;

      int sx=(int)((double)lp * invra);
      int sy=(int)((double)tp * invra);
      int sw=(int)((double)w * invra);
      int sh=(int)((double)h * invra);
   
  	  virtualCanvas->stretchblit(
        sx,sy,sw,sh,
        paintcanvas, lp, tp, w, h);
    }
  }
}

void BaseWnd::flushPaint() {
  postDeferredCallback(DEFERREDCB_FLUSHPAINT, 0);
}

void BaseWnd::do_flushPaint() {
  if (!deferedInvalidRgn || deferedInvalidRgn->isEmpty()) return;
  Region *r = deferedInvalidRgn->clone();
  cascadeRepaintRgn(r);
  deferedInvalidRgn->disposeClone(r);
  deferedInvalidRgn->empty();
}

int BaseWnd::isMouseOver(int x, int y) {
  POINT pt={x, y};
  clientToScreen(&pt);

  return (api->rootWndFromPoint(&pt) == this);
}

void BaseWnd::freeResources() {
}

void BaseWnd::reloadResources() {
  invalidate(); 
}

double BaseWnd::getRenderRatio() {
  if (!handleRatio()) return 1.0;
  return getParent() ? getParent()->getRenderRatio() : ratio;
}

void BaseWnd::setRenderRatio(double r) {
  // "snap" to 1.0
  if (ABS(r - 1.0) <= 0.02f) r = 1.0;
  if (scalecanvas) {
    delete scalecanvas;
    scalecanvas = NULL;
  }
  if (isInited() && r != ratio && !isVirtual() && !getParent()) {
    // must scale size & region accordingly
    RECT rc;
    BaseWnd::getWindowRect(&rc);
    rc.right = rc.left + rwidth;
    rc.bottom = rc.top + rheight;
    ratio = r;
    resize(&rc);

/*    Region *reg = new Region;
    GetWindowRgn(gethWnd(), reg->getHRGN());
    if (!reg->isEmpty()) {
      reg->scale(getRenderRatio());
      SetWindowRgn(gethWnd(), reg->getHRGN(), TRUE);
    }
    delete reg;*/
    invalidate();
  }
}

int BaseWnd::renderRatioActive() {
  return ABS(getRenderRatio() - 1.0) > 0.01f;
}

void BaseWnd::multRatio(int *x, int *y) {
  double rr = getRenderRatio();
  if (x) *x = (int)((double)(*x) * rr);
  if (y) *y = (int)((double)(*y) * rr);
}

void BaseWnd::multRatio(RECT *r) {
  double rr = getRenderRatio();
  r->left = (int)((double)(r->left) * rr);
  r->right = (int)((double)(r->right) * rr);
  r->top = (int)((double)(r->top) * rr);
  r->bottom = (int)((double)(r->bottom) * rr);
}

void BaseWnd::divRatio(int *x, int *y) {
  double rr = getRenderRatio();
  if (x) *x = (int)((double)(*x) / rr);
  if (y) *y = (int)((double)(*y) / rr);
}

void BaseWnd::divRatio(RECT *r) {
  double rr = getRenderRatio();
  r->left = (int)((double)(r->left) / rr);
  r->right = (int)((double)(r->right) / rr);
  r->top = (int)((double)(r->top) / rr);
  r->bottom = (int)((double)(r->bottom) / rr);
}

void BaseWnd::bringVirtualToFront(RootWnd *w) {
  changeChildZorder(w, 0);
}

void BaseWnd::bringVirtualToBack(RootWnd *w) {
  changeChildZorder(w, virtualChildren.getNumItems());
}

void BaseWnd::bringVirtualAbove(RootWnd *w, RootWnd *b) {
  ASSERT(b->isVirtual());
  int p = virtualChildren.searchItem(b);
  if (p == -1) return;
  changeChildZorder(w, p);
}

void BaseWnd::bringVirtualBelow(RootWnd *w, RootWnd *b) {
  ASSERT(b->isVirtual());
  int p = virtualChildren.searchItem(b);
  if (p == -1) return;
  changeChildZorder(w, p+1);
}

void BaseWnd::changeChildZorder(RootWnd *w, int newpos) {
  int p = newpos;
  p = MAX(p, (int)0);
  p = MIN(p, virtualChildren.getNumItems());
  RECT cr;
  w->getClientRect(&cr);

  PtrList<RootWnd> l;
  for (int i=0;i<virtualChildren.getNumItems();i++)
    if (virtualChildren[i] != w)
      l.addItem(virtualChildren[i]);

  p = virtualChildren.getNumItems() - newpos -1;
  virtualChildren.removeAll();

  int done = 0;

  for (i=0;i<l.getNumItems();i++)
    if (i==p && !done) {
      virtualChildren.addItem(w);
      i--;
      done++;
    } else {
      RECT dr;
      RECT intersection;
      l.enumItem(i)->getClientRect(&dr);
      if (IntersectRect(&intersection, &dr, &cr))
        l[i]->invalidateRect(&intersection);
      virtualChildren.addItem(l.enumItem(i));
    }
  if (i==p && !done) 
    virtualChildren.addItem(w);
  w->invalidate();
}

int BaseWnd::onActivate() {
  if(hasVirtualChildren()) {
    int l=getNumVirtuals();
    for(int i=0;i<l;i++) {
      RootWnd *r=enumVirtualChild(i);
      r->onActivate();
    }
  }
  return 0;
}

int BaseWnd::onDeactivate() {
  if(hasVirtualChildren()) {
    int l=getNumVirtuals();
    for(int i=0;i<l;i++) {
      RootWnd *r=enumVirtualChild(i);
      r->onDeactivate();
    }
  }
  return 0;
}

int BaseWnd::getDesktopAlpha() {
  return w2k_alpha;
}

Region *BaseWnd::getRegion() {
  return NULL;  
}

int BaseWnd::isDesktopAlphaAvailable() {
  if (user32instance == NULL) user32instance = LoadLibrary("USER32.DLL");
  if (user32instance != NULL) {

    if (setLayeredWindowAttributes == NULL || updateLayeredWindow == NULL) {
      if (!checked_for_alpha_proc) {
        setLayeredWindowAttributes=(void (__stdcall *)(HWND,int,int,int))GetProcAddress(user32instance ,"SetLayeredWindowAttributes");
        updateLayeredWindow=(BOOL (__stdcall *)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD))GetProcAddress(user32instance ,"UpdateLayeredWindow");
        checked_for_alpha_proc = 1;
      }
    }
  }
  return (updateLayeredWindow != NULL);
}

int BaseWnd::handleTransparency() {
  return 1; // by default all windows handle transparency, only windows blitting directly on the SCREEN (if you blit directly on the DC it's still ok), 
}           // for instance, a vis or a video using overlay should return 0, this will let the layout auto manage its alpha as that window is shown/hiden

void BaseWnd::setAlpha(int active, int inactive) {
  int oldactivealpha = activealpha;
  int oldinactivealpha = inactivealpha;
  if (active != activealpha) {
    activealpha = active;
    if (isActive()) {
      invalidate();
      if ((oldactivealpha == 0 || activealpha == 0) && (oldactivealpha != 0 || activealpha != 0))
        invalidateWindowRegion();
    }
  }
  if (inactive == -1) inactive = active;
  if (inactive != inactivealpha) {
    inactivealpha = inactive;
    if (!isActive()) {
      invalidate();
      if ((oldactivealpha == 0 || activealpha == 0) && (oldactivealpha != 0 || activealpha != 0))
        invalidateWindowRegion();
    }
  }
}

void BaseWnd::getAlpha(int *active, int *inactive) {
  if (active) *active = activealpha;
  if (inactive) *inactive = inactivealpha;
}

int BaseWnd::getPaintingAlpha(void) {
  return isActive() ? activealpha : inactivealpha;
}

void BaseWnd::setClickThrough(int ct) {
  clickthrough = ct;
}

int BaseWnd::isClickThrough() {
  return clickthrough;
}

int BaseWnd::handleRatio() {
  return 1;
}
#include "../common/script/c_script/c_rootobj.h"

int BaseWnd::createTip() {
  destroyTip();
  tooltip = new Tooltip(getTip());
  return -1;
}

void BaseWnd::destroyTip() {
  // this is to avoid pb if destroytip() is being called by a time while destroying tip
  Tooltip *tt = tooltip;
  tooltip = NULL;
  delete tt;
}

int BaseWnd::runModal() {

  RootWnd *dp = getDesktopParent();
  if (dp && dp != this)
    return dp->runModal();

  MSG msg;

  SetCapture(NULL);
  SetFocus(gethWnd());

  api->pushModalWnd(this);
  returnvalue = 0;
  mustquit = 0;

   // Main message loop:
  while (!mustquit) {
    mustquit = !GetMessage(&msg, NULL, 0, 0);
    if (!msg.hwnd || !TranslateAccelerator(msg.hwnd, NULL, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  api->popModalWnd(this);
  SetCapture(NULL);
  return returnvalue;
}

void BaseWnd::endModal(int ret) {
  RootWnd *dp = getDesktopParent();
  if (dp && dp != this) {
    dp->endModal(ret);
    return;
  }
  returnvalue = ret;
  mustquit = 1;
}

int BaseWnd::wantAutoContextMenu() { 
  return 1; 
}

void BaseWnd::onCancelCapture() {
}

RootWnd *BaseWnd::getNextVirtualFocus(RootWnd *w) {
  return NULL;
}

void BaseWnd::setVirtualChildFocus(RootWnd *w) {
  ASSERT(w && w->isVirtual());
  if (curVirtualChildFocus == w) return;
  if (curVirtualChildFocus)
    curVirtualChildFocus->onKillFocus();
  curVirtualChildFocus = w;
  curVirtualChildFocus->onGetFocus();
}

int BaseWnd::ptInRegion(int x, int y) {
  RECT cr;
  getNonClientRect(&cr);
  POINT pt={x-cr.left,y-cr.top};
  Region *reg = getRegion();
  if (isRectRgn()) 
    return (x >= cr.left && x <= cr.right && y >= cr.top && y <= cr.bottom);
  return reg ? reg->ptInRegion(&pt) : 0;
}

Region *BaseWnd::getComposedRegion() {
  ensureWindowRegionValid();
  return composedrgn;
}

Region *BaseWnd::getSubtractorRegion() {
  ensureWindowRegionValid();
  return subtractorrgn;  
}

int BaseWnd::ensureWindowRegionValid() {
  if (!isInited()) return 0;
  if (wndregioninvalid) {
    computeComposedRegion();
    return 1;
  }
  return 0;
}

void BaseWnd::invalidateWindowRegion() {
  wndregioninvalid = 1;
  if (getParent()) getParent()->invalidateWindowRegion();
}

void BaseWnd::computeComposedRegion() {
  if (!isPostOnInit()) return;

  wndregioninvalid = 0;

  RECT r;
  getNonClientRect(&r);

  Region *reg = getRegion();
  RegionI *_reg = NULL;

  if (!reg) {
    _reg = new RegionI;
    reg = _reg;
    if (isRectRgn())
      reg->setRect(&r);
  } else
    if (isRectRgn())
      reg->setRect(&r);

  Region *wr = reg->clone();

  if (!subtractorrgn) subtractorrgn = new RegionI();
  subtractorrgn->empty();
  if (!composedrgn) composedrgn = new RegionI;
  composedrgn->empty();

  RegionI *subme = NULL;
  RegionI *andme = NULL;
  RegionI *orme = NULL;

  // if subregion is now empty, we need to only use our region
  RECT gr;
  getNonClientRect(&gr);
  for (int i=0;i<virtualChildren.getNumItems();i++) {
    RootWnd *srw = virtualChildren.enumItem(i);
    if (!srw->isVisible(1) || srw->getPaintingAlpha() == 0) continue;
    if (srw->getRegionOp() != REGIONOP_NONE) {
      Region *sr = srw->getComposedRegion();
      if (sr) {
        Region *osr = sr->clone();
        RECT r;
        srw->getNonClientRect(&r);
        r.left -= gr.left;
        r.top -= gr.top;
        osr->offset(r.left, r.top);
/*        sr->debug();
        osr->debug();*/
        if (srw->getRegionOp() == REGIONOP_OR) {
          if (!orme) orme = new RegionI;
          orme->add(osr);
        } else if (srw->getRegionOp() == REGIONOP_AND) {
          if (!andme) andme = new RegionI;
          andme->add(osr);
        } else if (srw->getRegionOp() == REGIONOP_SUB) {
          if (!subme) subme = new RegionI;
          subme->add(osr);
        } else if (srw->getRegionOp() == REGIONOP_SUB2) {
          if (!subme) subme = new RegionI;
          subtractorrgn->add(osr);
        }
        sr->disposeClone(osr);
      }
    }
    Region *sr = srw->getSubtractorRegion();
    if (sr != NULL && !sr->isEmpty()) {
      Region *osr = sr->clone();
      RECT r;
      srw->getNonClientRect(&r);
      r.left -= gr.left;
      r.top -= gr.top;
      osr->offset(r.left, r.top);
      subtractorrgn->add(osr);
      sr->disposeClone(osr);
    }
  }

  if (andme) {
    wr->and(andme);
    delete andme;
  }
  if (orme) {
    wr->add(orme);
    delete orme;
  }
  if (subme) {
    wr->subtractRgn(subme);
    delete subme;
  }

  composedrgn->add(wr);
  reg->disposeClone(wr);
  delete _reg;
}

void BaseWnd::updateWindowRegion() {
  if (!isPostOnInit() || isVirtual()) return;
  if (getDesktopAlpha()) { // if desktopalpha is on, we can't use regions (thanks MS), we have to rely on the framebuffer correctness
    SetWindowRgn(gethWnd(), NULL, FALSE);
    return;
  }
  Region *_r = getComposedRegion();
  Region *_s = getSubtractorRegion();
  ASSERT(_r != NULL && _s != NULL);

  Region *z = _r->clone();
  z->subtractRgn(_s);
                      
  assignWindowRegion(z);
  _r->disposeClone(z);
}

// wr is NOT scaled!!!
void BaseWnd::assignWindowRegion(Region *wr) {
  ASSERT(wr != NULL);

  if (!isPostOnInit()) return;

  int isrect = wr->isRect();

  RECT r;
  BaseWnd::getWindowRect(&r);
  r.right -= r.left;
  r.left = 0;
  r.bottom -= r.top;
  r.top = 0;

  RECT z;
  wr->getBox(&z);
  z.left = 0;
  z.top = 0;

  if (renderRatioActive()) {
    double i = getRenderRatio();
    wr->scale(i, i, TRUE);
  }

  RECT sz;
  wr->getBox(&sz);
  sz.right -= sz.left;
  sz.bottom -= sz.top;
  sz.left = 0;
  sz.top = 0;

  if (isrect && 
      ((z.right == rwidth && z.bottom == rheight) || 
       (sz.left == r.left && sz.right == r.right && sz.top == r.top && sz.bottom == r.bottom) ||
       (0)
      )
     )
    SetWindowRgn(gethWnd(), NULL, TRUE); 
  else {
    //OutputDebugString(StringPrintf("setting region, rwidth = %d, rheight = %d, z.right = %d, z.bottom = %d\n", rwidth, rheight, z.right, z.bottom));
    SetWindowRgn(gethWnd(), wr->makeWindowRegion(), TRUE);
  }
}

void BaseWnd::performBatchProcesses() {
  // recompute the window region if needed and apply it to the HWND
  if (wndregioninvalid && !isVirtual()) 
    if (ensureWindowRegionValid())
      updateWindowRegion();
  if (need_flush_cascaderepaint) {
    _cascadeRepaintRgn(deferedCascadeRepaintRgn);
    deferedCascadeRepaintRgn->empty();
    need_flush_cascaderepaint = 0;
  }
}

int BaseWnd::getRegionOp() {
  return regionop; 
}

void BaseWnd::setRegionOp(int op) {
  if (regionop != op) {
    regionop = op;
    invalidateWindowRegion();
  }
}

int BaseWnd::isRectRgn() {
  return rectrgn;
}

void BaseWnd::setRectRgn(int i) {
  rectrgn = i;
}

TimerClient *BaseWnd::timerclient_getMasterClient() {
  if (!isVirtual()) return this;
  RootWnd *w = getParent();
  if (w) {
    TimerClient *tc = w->getTimerClient();
    if (tc)
      return tc->timerclient_getMasterClient();
  }
  return NULL;
}

void BaseWnd::timerclient_onMasterClientMultiplex() {
  performBatchProcesses();
}

TimerClient *BaseWnd::getTimerClient() {
  return this;
}

Dependent *BaseWnd::rootwnd_getDependencyPtr() {
  return this;
}

Dependent *BaseWnd::timerclient_getDependencyPtr() {
  return this;
}

void BaseWnd::addMinMaxEnforcer(RootWnd *w) {
  minmaxEnforcers.addItem(w);
  signalMinMaxEnforcerChanged();  
}

void BaseWnd::removeMinMaxEnforcer(RootWnd *w) {
  minmaxEnforcers.removeItem(w);
  signalMinMaxEnforcerChanged();
}

void BaseWnd::signalMinMaxEnforcerChanged(void) {
  RootWnd *w = getDesktopParent();
  if (w == NULL || w == this) onMinMaxEnforcerChanged();
  else w->signalMinMaxEnforcerChanged();
}

int BaseWnd::getNumMinMaxEnforcers() {
  return minmaxEnforcers.getNumItems();
}

RootWnd *BaseWnd::enumMinMaxEnforcer(int n) {
  return minmaxEnforcers.enumItem(n);
}

int BaseWnd::onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source) {
  return 1;
}

int BaseWnd::sendAction(RootWnd *target, const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen) {
  ASSERT(target != NULL);
  return target->onAction(action, param, x, y, p1, p2, data, datalen, this);
}

int BaseWnd::virtualBeforePaint(Region *r) {
  if (!virtualCanvas) return 0;
  PaintCallbackInfoI pc(virtualCanvas, r);
  dependent_sendEvent(RootWnd::depend_getClassGuid(), Event_ONPAINT, PaintCallback::BEFOREPAINT, &pc);
  return 1;
}

int BaseWnd::virtualAfterPaint(Region *r) {
  if (!virtualCanvas) return 0;
  PaintCallbackInfoI pc(virtualCanvas, r);
  dependent_sendEvent(RootWnd::depend_getClassGuid(), Event_ONPAINT, PaintCallback::AFTERPAINT, &pc);
  return 1;
}

int BaseWnd::timerclient_onDeferredCallback(int p1, int p2) {
  TimerClientI::timerclient_onDeferredCallback(p1, p2);
  return onDeferredCallback(p1, p2);
}

void BaseWnd::timerclient_timerCallback(int id) {
  TimerClientI::timerclient_timerCallback(id);
  timerCallback(id);
}

int BaseWnd::setTimer(int id, int ms) {
/*  FILE *out = fopen("c:\\timerlogs.txt", "a+t");
  if (out) {
    fprintf(out, StringPrintf("setTimer for %08X(%08X) (%s)\n", this, getDependencyPtr(), getId()));
    fclose(out);
  }*/
  return timerclient_setTimer(id, ms);
}

int BaseWnd::killTimer(int id) {
  return timerclient_killTimer(id);
}

void BaseWnd::postDeferredCallback(int p1, int p2, int mindelay) {
/*  FILE *out = fopen("c:\\timerlogs.txt", "a+t");
  if (out) {
    fprintf(out, StringPrintf("postDeferredCallback for %08X(%08X) (%s)\n", this, getDependencyPtr(), getId()));
    fclose(out);
  }*/
  timerclient_postDeferredCallback(p1, p2, mindelay);
}

int BaseWnd::bypassModal() {
  return 0;
}

void BaseWnd::setRenderBaseTexture(int r) {
  renderbasetexture = r;
  if (isInited()) invalidate();
}

int BaseWnd::getRenderBaseTexture() {
  return renderbasetexture;
}

GuiObject *BaseWnd::getGuiObject() {
  if (my_guiobject == NULL) {
    my_guiobject = static_cast<GuiObject *>(getInterface(guiObjectGuid));
  }
  return my_guiobject;
}

int BaseWnd::getFlag(int flag) {
/*  switch (flag) {
  }*/
  return 0;
}

int BaseWnd::triggerEvent(int event, int p1, int p2) {
  switch (event) {
    case WNDEVENT_ONRESIZE:
      if (isPostOnInit())
        onResize();
      break;
  }
  return 0;
}

void BaseWnd::registerAcceleratorSection(const char *name, int global) {
  api->locales_registerAcceleratorSection(name,this, global);
}

int BaseWnd::onAcceleratorEvent(const char *name) {
  for (int i=0;i<getNumRootWndChildren();i++)
    if(enumRootWndChildren(i)->onAcceleratorEvent(name))
      return 1;
  return 0;
}

int BaseWnd::allowDeactivation() {
  return allow_deactivate & api->appdeactivation_isallowed(this); 
}

void BaseWnd::onMinimize() {
}

void BaseWnd::onRestore() {
  if (getDesktopParent() == this) {
    cascadeRepaint(1);
  }
}

RootWnd *BaseWnd::findWindow(const char *id) {
  RootWndFinder find_object;
  find_object.reset();
  find_object.setFindId(id);
  RootWnd *ret = findWindowChain(&find_object);
  if (ret == NULL) 
    OutputDebugString(StringPrintf("findWindow : window not found by id ! %s \n", id));
  return ret;
}

RootWnd *BaseWnd::findWindowByInterface(GUID interface_guid) {
  RootWndFinder find_object;
  find_object.reset();
  find_object.setFindGuid(interface_guid);
  RootWnd *ret = findWindowChain(&find_object);
  char str[256];
  nsGUID::toChar(interface_guid, str);
  if (ret == NULL) 
    OutputDebugString(StringPrintf("findWindow : object not found by guid ! %s \n", str));
  return ret;
}

RootWnd *BaseWnd::findWindowByCallback(FindObjectCallback *cb) {
  RootWnd *ret = findWindowChain(cb);
  if (ret == NULL) 
    OutputDebugString("findWindow : object not found by callback!\n");
  return ret;
}

RootWnd *BaseWnd::findWindowChain(FindObjectCallback *cb, RootWnd *wcaller) {

  if (!cb) return NULL;

  // first lets not look in subdirectories

  for (int i=0;i<getNumRootWndChildren();i++) {
    RootWnd *child = enumRootWndChildren(i);
    if (!child || child == wcaller) continue;
    if (cb->findobjectcb_matchObject(child))
      return child;
  }

  // ok so it wasn't in our content, lets try to find it as a grandchildren

  for (i=0;i<getNumRootWndChildren();i++) {
    RootWnd *child = enumRootWndChildren(i);
    if (child->getNumRootWndChildren() > 0) { 
      RootWnd *ret = child->findWindowChain(cb, this);
      if (ret) return ret;
    }
  }

  // so it wasnt one of our children, we'll hop the tree up one level and ask our parent to find it
  // for us. of course, our parents are smart, they won't ask us back when asking our sibblings
  RootWnd *p = getParent();
  if (p != NULL && wcaller != p) {
    return p->findWindowChain(cb, this);
  }

  return NULL;
}


const char *BaseWnd::timerclient_getName() {
  tcname = StringPrintf("name=\"%s\", id=\"%s\"", getRootWndName(), getId());
  return tcname;
}

