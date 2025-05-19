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

// the glue!

#include "assert.h"
#include "wac.h"

#include "services/servicei.h"

extern WAComponentClient *the;

ComponentAPI *api;	// we do NOT delete this on shutdown!!!

#ifdef WIN32
HINSTANCE dllhInstance;
#endif

WAComponentClient::WAComponentClient(const char *name) :
  CfgItemI(name) {
  hInstance = NULL;
}

const char *WAComponentClient::getName() {
  return cfgitem_getName();
}

void WAComponentClient::registerServices(ComponentAPI *_api) {
  // save API *
  api = _api;

  // propagate name from component to cfgitemi
  if (cfgitem_getName() == NULL) CfgItemI::setName(WaComponent::getName());

  // register resources
  foreach(resources)
    resources.getfor()->onRegisterServices();
  endfor

  // our CfgItem GUID defaults to our component GUID
  cfgitem_setGUID(getGUID());

  // by default, peg us under "Installed Components" in config
  // {99CFD75C-1CA7-49e5-B8C0-7D78AA443C10}
  const GUID installed_guid = 
  { 0x99cfd75c, 0x1ca7, 0x49e5, { 0xb8, 0xc0, 0x7d, 0x78, 0xaa, 0x44, 0x3c, 0x10 } };
  if (cfgitem_getParentGuid() == INVALID_GUID)
    setParentGuid(installed_guid);

  api->config_registerCfgItem(this);

  // allow overridden component class to do stuff
  onRegisterServices();
}

void WAComponentClient::deregisterServices() {
  foreach(resources)
    resources.getfor()->onDeregisterServices();
  endfor
  api->config_deregisterCfgItem(this);
}

CfgItem *WAComponentClient::getCfgInterface(int n) {
  if (n == 0) return this;
  return NULL;
}

class RegService : public LoadableResource {
public:
  RegService(waServiceFactoryI *sf) : factory(sf) {}

  virtual void onRegisterServices() {
    api->service_register(factory);
  }
  virtual void onDeregisterServices() {
    api->service_deregister(factory);
  }

private:
  waServiceFactoryI *factory;
};

void WAComponentClient::registerService(waServiceFactoryI* service) {
  registerResource(new RegService(service));
}

class SkinPart : public LoadableResource {
public:
  SkinPart(const char *name, int rel) : filename(name), id(-1), relative(rel) {}

  void beforeLoadingSkinElements() {
    ASSERT(!filename.isempty());
    String fn(filename);
    if (relative) fn.prepend(api->path_getComponentPath());
    id = api->loadSkinFile(fn);
  }
  virtual void onDeregisterServices() {
    if (id >= 0 && api) api->unloadSkinPart(id);
    id = -1;
  }

private:
  String filename;
  int id;
  int relative;
};

void WAComponentClient::registerSkinFile(const char *filename, int relative) {
  registerResource(new SkinPart(filename, relative));
}

class AutoPop : public LoadableResource {
public:
  AutoPop(GUID g, const char *str, const char *prefered_layout=NULL, int layout_required=FALSE) : guid(g), desc(str), layout(prefered_layout), layoutflag(layout_required) { groupid.setValue(NULL); }
  AutoPop(const char *group_id, const char *str, const char *prefered_layout=NULL, int layout_required=FALSE) : groupid(group_id), desc(str), layout(prefered_layout), layoutflag(layout_required) { guid = INVALID_GUID; }

  void onRegisterServices() {
    if (guid != INVALID_GUID)
      id = api->autopopup_registerGuid(guid, desc, layout, layoutflag);
    else
      id = api->autopopup_registerGroupId(groupid, desc, layout, layoutflag);
  }
  void onDeregisterServices() {
    api->autopopup_unregister(id);
  }

private:
  GUID guid;
  String desc;
  String groupid;
  String layout;
  int layoutflag;
  int id;
};

void WAComponentClient::registerAutoPopup(GUID guid, const char *description, const char *prefered_layout, int layout_flag) {
  registerResource(new AutoPop(guid, description, prefered_layout, layout_flag));
}

void WAComponentClient::registerAutoPopup(const char *groupid, const char *description, const char *prefered_layout, int layout_flag) {
  registerResource(new AutoPop(groupid, description, prefered_layout, layout_flag));
}


class CfgItemResource : public LoadableResource {
public:
  CfgItemResource(CfgItemI *_cfgitem, int _autodelete) :
    cfgitem(_cfgitem), autodelete(_autodelete) {}

  virtual void onRegisterServices() {
    api->config_registerCfgItem(cfgitem);
  }

  virtual void onDeregisterServices() {
    api->config_deregisterCfgItem(cfgitem);
    if (autodelete) delete cfgitem;
  }

private:
  CfgItemI *cfgitem;
  int autodelete;
};

void WAComponentClient::registerCfgItem(CfgItemI *cfgitem, int autodelete) {
  registerResource(new CfgItemResource(cfgitem, autodelete));
}

class ExtensionResource : public LoadableResource {
public:
  ExtensionResource(const char *extension, const char *description, const char *family) :
    ext(extension), desc(description), fam(family) { }

  virtual void onRegisterServices() {
    if (!ext.isempty() && !desc.isempty()) api->core_registerExtension(ext, desc, fam);
  }
  virtual void onDeregisterServices() {
    if (!ext.isempty() && !desc.isempty()) api->core_unregisterExtension(ext);
  }

private:
  String ext, desc, fam;
};

void WAComponentClient::registerExtension(const char *extension, const char *description, const char *family) {
  registerResource(new ExtensionResource(extension, description, family));
}

void WAComponentClient::registerResource(LoadableResource *res) {
  ASSERT(!resources.haveItem(res));
  resources.addItem(res);
  if (postregisterservices) res->onRegisterServices(); 
}

void WAComponentClient::internal_onDestroy() {
  onDestroy();
  foreach(resources)
    if (resources.getfor()->deleteOnShutdown()) delete resources.getfor();
  endfor
  resources.removeAll();
}

int WAComponentClient::internal_onNotify(int cmd, int param1, int param2, int param3, int param4) {
  switch (cmd) {
    case WAC_NOTIFY_BEFORELOADINGSKINELEMENTS:
      foreach(resources)
        resources.getfor()->beforeLoadingSkinElements();
      endfor
      break;
    case WAC_NOTIFY_SKINLOADED:
      onSkinLoaded();
      break;
  }
  return onNotify(cmd, param1, param2, param3, param4);
}

extern "C" {

__declspec(dllexport) UINT WAC_getVersion(void) {
  return WA_COMPONENT_VERSION;
}

__declspec(dllexport) int WAC_init(HINSTANCE dllInstance) {
  the->sethInstance(dllInstance);
  if (dllhInstance == NULL) dllhInstance = dllInstance;
  return TRUE;
}

__declspec(dllexport) WaComponent *WAC_enumComponent(int n) {
  return the;
}

} // end extern "C"


#define CBCLASS WaComponentI
START_DISPATCH;
  CB(GETNAME, getName);
  CB(GETGUID, getGUID);
  VCB(REGISTERSERVICES, registerServices);
  VCB(DEREGISTERSERVICES, deregisterServices);
  VCB(ONCREATE, onCreate);
  VCB(ONDESTROY, internal_onDestroy);
  CB(CREATEWINDOW, createWindow);
  CB(ONNOTIFY, internal_onNotify);
  CB(ONCOMMAND, onCommand);
  CB(GETCFGINTERFACE, getCfgInterface);
  VCB(SETHINSTANCE, sethInstance);
  CB(GETHINSTANCE, gethInstance);
END_DISPATCH;
#undef CBCLASS
