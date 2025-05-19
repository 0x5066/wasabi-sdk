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

//NONPORTABLE - HWND
#ifndef _WAC_H
#define _WAC_H

#include "api.h"//CUT

#include "../common/compdb.h"//CUT
#include "../bfc/ptrlist.h"
#include "../attribs/cfgitemi.h"	// for CfgItemI

class CanvasBase;	// see ../common/canvas.h
class CfgItem;		// see ../attribs/cfgitem
class CfgItemI;
class ComponentAPI;	// see api.h
class RootWnd;		// see ../bfc/rootwnd.h
class LoadableResource;	// see below

// this tells wasabi.dll what version of the SDK we're compiled with
// this number will be incremented every once in a while. when it is, you'll
// have to recompile
// 5 -> build #471
// 6 -> build rc1	(most recent)
// 7 -> build 484 (rc5)
#define WA_COMPONENT_VERSION	7

#define DB_DENY  0
#define DB_ALLOW 1

// commands for onNotify below

#define WAC_NOTIFY_NOP			0

#define WAC_NOTIFY_ACTIVATE		10
#define WAC_NOTIFY_DEACTIVATE		20

#define WAC_NOTIFY_FULLSCREEN		30
#define WAC_NOTIFY_RESTORE		40

#define WAC_NOTIFY_ENTERRESIZE		50
#define WAC_NOTIFY_LEAVERESIZE		51

#define WAC_NOTIFY_SYSTRAYDONE		60
#define WAC_NOTIFY_POPUPDONE  		61

#define WAC_NOTIFY_BASETEXTUREWINDOW	78

#define WAC_NOTIFY_MENU_COMMAND		10000

// this message is sent when service runlevel notifies are sent
// param1 = msg
// param2 = param1
// param3 = param2
#define WAC_NOTIFY_SERVICE_NOTIFY	100

#define WAC_NOTIFY_SKINUNLOADING             0xE300 // before unloading current skin
#define WAC_NOTIFY_SWITCHINGSKIN             0xE301 // right after unloading current skin and right before loading new one, getSkinName/Path points to new skin
#define WAC_NOTIFY_SKINELEMENTSLOADED	       0xE302 // after skin elements definitions are loaded
#define WAC_NOTIFY_BEFORELOADINGSKINELEMENTS 0xE303 // before skin elements definitions are loaded, trap this to load your elements/groups and let people override them
#define WAC_NOTIFY_SKINGUILOADED	           0xE304 // after skin GUI objects are loaded from xml
#define WAC_NOTIFY_SKINLOADED	               0xE305 // after skin is fully loaded

// -----------

#include "../bfc/dispatch.h"

class NOVTABLE WaComponent : public Dispatchable {
public:
  // provide your component name & other info here
  const char *getName() { return _call(GETNAME, (char *)NULL); }
  GUID getGUID() { return _call(GETGUID, INVALID_GUID); }

  void registerServices(ComponentAPI *a) { _voidcall(REGISTERSERVICES, a); }
  void deregisterServices() { _voidcall(DEREGISTERSERVICES); }

  void onCreate() { _voidcall(ONCREATE); }
  void onDestroy() { _voidcall(ONDESTROY); }
//DEPRECATED: will go away
  RootWnd *createWindow(int n, RootWnd *parentWnd) { return _call(CREATEWINDOW, (RootWnd*)NULL, n, parentWnd); }

  int onNotify(int cmd, int param1=0, int param2=0, int param3=0, int param4=0){
    return _call(ONNOTIFY, 0, cmd, param1, param2, param3, param4);
  }
  // everything after cmd is for future expansion
  int onCommand(const char *cmd, int param1, int param2, void *ptr, int ptrlen){
    return _call(ONCOMMAND, 0, cmd, param1, param2, ptr, ptrlen);
  }

  // gets the CfgItem * from WAComponentClient
  CfgItem *getCfgInterface(int n) {
    return _call(GETCFGINTERFACE, (CfgItem*)NULL, n);
  }

  // saves the HINSTANCE of your DLL for you	(NONPORTABLE)
  void sethInstance(HINSTANCE hInst) {
    _voidcall(SETHINSTANCE, hInst);
  }
  HINSTANCE gethInstance() {
    return _call(GETHINSTANCE, (HINSTANCE)0);
  }

  enum {
    GETNAME=100,
    GETGUID=120,
    REGISTERSERVICES=200,
    ONCREATE=400,
    ONDESTROY=410,
    CREATEWINDOW=420,
    ONNOTIFY=500,
    ONCOMMAND=600,
    GETCFGINTERFACE=800,
    SETHINSTANCE=900,
    GETHINSTANCE=910,
    DEREGISTERSERVICES=1000,
  };
};

// hides the dispatchable interface
class NOVTABLE WaComponentI : public WaComponent {
protected:
  WaComponentI() {}	// protect constructor
public:
  virtual const char *getName()=0;
  virtual GUID getGUID()=0;

  virtual void registerServices(ComponentAPI *)=0;
  virtual void deregisterServices()=0;

  virtual void onCreate()=0;
  virtual void onDestroy()=0;

  virtual RootWnd *createWindow(int n, RootWnd *parentWnd)=0;	//DEPRECATED

  virtual int onNotify(int cmd, int param1, int param2, int param3, int param4)=0;
  virtual int onCommand(const char *cmd, int param1, int param2, void *ptr, int ptrlen)=0;

  virtual CfgItem *getCfgInterface(int n)=0;

  virtual void sethInstance(HINSTANCE hInst)=0;
  virtual HINSTANCE gethInstance()=0;

private:
  virtual void internal_onDestroy()=0;
  virtual int internal_onNotify(int cmd, int param1, int param2, int param3, int param4)=0;	// calls thru to onNotify after processing
  RECVS_DISPATCH;
};

class waServiceFactoryI;

class NOVTABLE WAComponentClient : public WaComponentI, public CfgItemI {
protected:
  WAComponentClient(const char *name=NULL);
public:
  virtual ~WAComponentClient() {}

  // provide your component name & other info here
  virtual const char *getName(); //OVERRIDE ME (or just set name in constructor)
  virtual GUID getGUID()=0;	// be sure to override this one	//OVERRIDE ME

  void registerServices(ComponentAPI *);	// don't override
  void deregisterServices();			// don't override

  // override these to receive notifications
  virtual void onRegisterServices() {}// register extra services here
  virtual void onCreate() {}	// init stuff
  virtual void onDestroy() {}	// destroy everything here, not in ~
//DEPRECATED: will go away
// this is moving to svc_wndcreate
  virtual RootWnd *createWindow(int n, RootWnd *parentWnd) { return NULL; }

  // OVERRIDE ME for various events
  virtual int onNotify(int cmd, int param1, int param2, int param3, int param4) { return 0; }
  // everything after cmd is for future expansion
  virtual int onCommand(const char *cmd, int param1, int param2, void *ptr, int ptrlen) { return 0; }

  // config ptr... you can override this and provide a different *
  // otherwise 'this' will be returned
  virtual CfgItem *getCfgInterface(int n);

  // saves the HINSTANCE of your DLL for you	(NONPORTABLE)
  void sethInstance(HINSTANCE hInst) { hInstance = hInst; };
  HINSTANCE gethInstance() { return hInstance; };

  // various options to register during your constructor, like services,
  // skin parts, and autopop guids. just register them and forget them

  void registerService(waServiceFactoryI* service);
  void registerSkinFile(const char *filename, int relative=FALSE);
  void registerAutoPopup(GUID guid, const char *description, const char *prefered_container=NULL, int container_required=FALSE);
  void registerAutoPopup(const char *groupid, const char *description, const char *container_layout=NULL, int container_required=FALSE);
  void registerCfgItem(CfgItemI *cfgitem, int autodelete=FALSE);
  void registerExtension(const char *extension, const char *description, const char *family=NULL);
  // or register your own resource types here
  void registerResource(LoadableResource *res);

  virtual void onSkinLoaded() {}

private:
  virtual void internal_onDestroy();
  virtual int internal_onNotify(int cmd, int param1, int param2, int param3, int param4);
  HINSTANCE hInstance;
  int postregisterservices;	// if 1, onRegisterServices has been called
  PtrList<LoadableResource> resources;
};

extern "C" {
  typedef void (*WACINIT)(HINSTANCE);
  typedef UINT (*WACGETVERSION)(void); // returns the interface version component was compiled with
  typedef WaComponent *(*WACENUMCOMPONENT)(int n);
};

class LoadableResource {
public:
  virtual ~LoadableResource() {}

  virtual int deleteOnShutdown() { return TRUE; } // return FALSE if you're static

  virtual void onRegisterServices() {}
  virtual void onDeregisterServices() {}	// unload during this one
  virtual void beforeLoadingSkinElements() {}
};

#endif
