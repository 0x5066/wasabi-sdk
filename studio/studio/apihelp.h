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

#ifndef _APIHELP_H
#define _APIHELP_H

inline int ComponentAPI::getVersion() {
  return _call(GETVERSION, 0);
}

inline const char *ComponentAPI::main_getVersionString() {
  return _call(GETVERSIONSTRING, "");
}

inline unsigned int ComponentAPI::main_getBuildNumber() {
  return _call(GETBUILDNUMBER, 0);
}

inline GUID ComponentAPI::main_getGUID() {
  return _call(GETGUID, INVALID_GUID);
}

inline HINSTANCE ComponentAPI::main_gethInstance() {
  return _call(GETHINSTANCE, (HINSTANCE)0);
}

inline HICON ComponentAPI::main_getIcon(int _small) {
  return _call(GETICON, (HICON)0, _small);
}

inline RootWnd *ComponentAPI::main_getRootWnd() {
  return _call(GETROOTWND, (RootWnd*)0);
}

inline const char *ComponentAPI::main_getCommandLine() {
  return _call(MAIN_GETCOMMANDLINE, "");
}

inline const char *ComponentAPI::path_getAppPath() {
  return _call(PATH_GETAPPPATH, (const char *)NULL);
}

inline
const char *ComponentAPI::path_getComponentPath(GUID g) {
  return _call(PATH_GETCOMPONENTPATH, (const char *)NULL, g);
}

inline
const char *ComponentAPI::path_getComponentDataPath() {
  return _call(PATH_GETCOMPONENTDATAPATH, (const char *)NULL);
}

inline
const char *ComponentAPI::path_getUserSettingsPath() {
  return _call(PATH_GETUSERSETTINGSPATH, (const char *)NULL);
}

inline void ComponentAPI::main_setWindowTitle(const char *title) {
  _voidcall(MAIN_SETWINDOWTITLE, title);
}

inline void ComponentAPI::setIntPrivate(const char *name, int val) {
  _voidcall(SETINTPRIVATE, name, val);
}

inline int ComponentAPI::getIntPrivate(const char *name, int def_val) {
  return _call(GETINTPRIVATE, def_val, name, def_val);	// hee hee
}

inline void ComponentAPI::setIntArrayPrivate(const char *name, const int *val, int nval) {
  _voidcall(SETINTARRAYPRIVATE, name, val, nval);
}

inline int ComponentAPI::getIntArrayPrivate(const char *name, int *val, int nval) {
  return _call(GETINTARRAYPRIVATE, 0, name, val, nval);
}

inline void ComponentAPI::setStringPrivate(const char *name, const char *str) {
  _voidcall(SETSTRINGPRIVATE, name, str, 0);
}

inline int ComponentAPI::getStringPrivate(const char *name, char *buf, int buf_len, const char *default_str) {
  return _call(GETSTRINGPRIVATE, 0, name, buf, buf_len, default_str);
}

inline
int ComponentAPI::getStringPrivateLen(const char *name) {
  return _call(GETSTRINGPRIVATELEN, -1, name);
}

inline void ComponentAPI::setIntPublic(const char *name, int val) {
  _voidcall(SETINTPUBLIC, name, val);
}

inline int ComponentAPI::getIntPublic(const char *name, int def_val) {
  return _call(GETINTPUBLIC, def_val, name, def_val);
}

inline
void ComponentAPI::setStringPublic(const char *name, const char *str) {
  _voidcall(SETSTRINGPUBLIC, name, str);
}
inline
int ComponentAPI::getStringPublic(const char *name, char *buf, int buf_len, const char *default_str) {
  return _call(GETSTRINGPUBLIC, 0, name, buf, buf_len, default_str);
}

inline
const char *ComponentAPI::core_getSupportedExtensions() {
  return _call(COREGETSUPPORTEDEXTENSIONS, (const char *)0);
}

inline
const char *ComponentAPI::core_getExtSupportedExtensions() {
  return _call(COREGETEXTSUPPORTEDEXTENSIONS, (const char *)0);
}

inline CoreToken ComponentAPI::core_create() {
  return _call(CORECREATE, 0);
}

inline
int ComponentAPI::core_free(CoreToken core) {
  return _call(COREFREE, 0);
}

inline int ComponentAPI::core_setNextFile(CoreToken core, const char *playstring) {
  return _call(CORESETNEXTFILE, 0, core, playstring);
}

inline int ComponentAPI::core_getStatus(CoreToken core) {
  return _call(COREGETSTATUS, 0, core);
}

inline const char *ComponentAPI::core_getCurrent(CoreToken core) {
  return _call(COREGETCURRENT, (const char*)NULL, core);
}

inline int ComponentAPI::core_getCurPlaybackNumber(CoreToken core) {
  return _call(COREGETCURPLAYBACKNUMBER, -1, core);
}

inline int ComponentAPI::core_getPosition(CoreToken core) {
  return _call(COREGETPOSITION, -1, core);
}

inline int ComponentAPI::core_getWritePosition(CoreToken core) {
  return _call(COREGETWRITEPOSITION, -1, core);
}

inline int ComponentAPI::core_setPosition(CoreToken core, int ms) {
  return _call(CORESETPOSITION, 0, core, ms);
}

inline int ComponentAPI::core_getLength(CoreToken core) {
  return _call(COREGETLENGTH, 0, core);
}

inline int ComponentAPI::core_getPluginData(const char *playstring,
  const char *name, char *data, int data_len, int data_type) {
  return _call(COREGETPLUGINDATA, 0, playstring, name, data, data_len, data_type);
}

inline unsigned int ComponentAPI::core_getVolume(CoreToken core) {
  return _call(COREGETVOLUME, 0, core);
}

inline void ComponentAPI::core_setVolume(CoreToken core, unsigned int vol) {
  _voidcall(CORESETVOLUME, core, vol);
}

inline int ComponentAPI::core_getPan(CoreToken core) {
  return _call(COREGETPAN, 0, core);
}

inline void ComponentAPI::core_setPan(CoreToken core, int bal) {
  _voidcall(CORESETPAN, core, bal);
}

inline void ComponentAPI::core_addCallback(CoreToken core, CoreCallback *cb) {
  _voidcall(COREADDCALLBACK, core, cb);
}

inline void ComponentAPI::core_delCallback(CoreToken core, CoreCallback *cb) {
  _voidcall(COREDELCALLBACK, core, cb);
}

inline int ComponentAPI::core_getVisData(CoreToken core, void *dataptr, int sizedataptr) {
  return _call(COREGETVISDATA, 0, core, dataptr, sizedataptr);
}

inline int ComponentAPI::core_getLeftVuMeter(CoreToken core) {
  return _call(COREGETLEFTVUMETER, 0, core);
}

inline int ComponentAPI::core_getRightVuMeter(CoreToken core) {
  return _call(COREGETRIGHTVUMETER, 0, core);
}

inline int ComponentAPI::core_registerSequencer(CoreToken core, ItemSequencer *seq) {
  return _call(COREREGISTERSEQUENCER, 0, core, seq);
}

inline int ComponentAPI::core_deregisterSequencer(CoreToken core, ItemSequencer *seq) {
  return _call(COREDEREGISTERSEQUENCER, 0, core, seq);
}

inline void ComponentAPI::core_userButton(CoreToken core, int button) {
  _voidcall(COREUSERBUTTON, core, button);
}

inline int ComponentAPI::core_getEqStatus(CoreToken core) {
  return _call(COREGETEQSTATUS, 0, core);
}

inline void ComponentAPI::core_setEqStatus(CoreToken core, int enable) {
  _voidcall(CORESETEQSTATUS, core, enable);
}

inline int ComponentAPI::core_getEqPreamp(CoreToken core) {
  return _call(COREGETEQPREAMP, 0, core);
}

inline void ComponentAPI::core_setEqPreamp(CoreToken core, int pre) {
  _voidcall(CORESETEQPREAMP, core, pre);
}

inline int ComponentAPI::core_getEqBand(CoreToken core, int band) {
  return _call(COREGETEQBAND, 0, core, band);
}

inline void ComponentAPI::core_setEqBand(CoreToken core, int band, int val) {
  _voidcall(CORESETEQBAND, core, band, val);
}

inline int ComponentAPI::core_getEqAuto(CoreToken core) {
  return _call(COREGETEQAUTO, 0, core);
}

inline void ComponentAPI::core_setEqAuto(CoreToken core, int enable) {
  _voidcall(CORESETEQAUTO, core, enable);
}

inline void ComponentAPI::core_setCustomMsg(CoreToken core, const char *text) {
  _voidcall(CORESETCUSTOMMSG, core, text);
}

inline void ComponentAPI::core_registerExtension(const char *extensions, const char *extension_name, const char *family) {
  _voidcall(COREREGISTEREXTENSION, extensions, extension_name, family);
}

inline
const char *ComponentAPI::core_getExtensionFamily(const char *extension) {
  return _call(COREGETEXTENSIONFAMILY, (const char *)NULL, extension);
}

inline void ComponentAPI::core_unregisterExtension(const char *extensions) {
  _voidcall(COREUNREGISTEREXTENSION, extensions);
}

inline int ComponentAPI::service_register(waServiceFactory *svc) {
  return _call(SERVICEREGISTER, 0, svc);
}

inline int ComponentAPI::service_deregister(waServiceFactory *svc) {
  return _call(SERVICEDEREGISTER, 0, svc);
}

inline void ComponentAPI::hint_garbageCollect() {
  _voidcall(HINT_GARBAGECOLLECT);
}

inline int ComponentAPI::service_getNumServices(FOURCC svc_type) {
  return _call(SERVICEGETNUMSERVICES, 0, svc_type);
}

inline waServiceFactory *ComponentAPI::service_enumService(FOURCC svc_type, int n) {
  return _call(SERVICEENUMSERVICE, (waServiceFactory*)0, svc_type, n);
}

inline waServiceFactory *ComponentAPI::service_getServiceByGuid(GUID guid) {
  return _call(SERVICEGETSERVICEBYGUID, (waServiceFactory*)0, guid);
}

inline int ComponentAPI::service_lock(waServiceFactory *owner, void *svcptr) {
  return _call(SERVICELOCK, 0, owner, svcptr);
}

inline int ComponentAPI::service_clientLock(void *svcptr) {
  return _call(SERVICECLIENTLOCK, 0, svcptr);
}

inline int ComponentAPI::service_release(void *svcptr) {
  return _call(SERVICERELEASE, 0, svcptr);
}

inline const char *ComponentAPI::service_getTypeName(FOURCC svc_type) {
  return _call(SERVICEGETTYPENAME, "", svc_type);
}

inline GUID ComponentAPI::service_getOwningComponent(void *svcptr) {
  return _call(SERVICEGETOWNINGCOMPONENT, INVALID_GUID, svcptr);
}

inline GUID ComponentAPI::service_getLockingComponent(void *svcptr) {
  return _call(SERVICEGETLOCKINGCOMPONENT, INVALID_GUID, svcptr);
}

inline void ComponentAPI::maki_pushObject(void *o) {
  _voidcall(PUSHOBJECT, o);
}

inline void ComponentAPI::maki_pushInt(int i) {
  _voidcall(PUSHINT, i);
}

inline void ComponentAPI::maki_pushBoolean(int b) {
  _voidcall(PUSHBOOLEAN, b);
}

inline void ComponentAPI::maki_pushFloat(float f) {
  _voidcall(PUSHFLOAT, f);
}

inline void ComponentAPI::maki_pushDouble(double d) {
  _voidcall(PUSHDOUBLE, d);
}

inline void ComponentAPI::maki_pushString(const char *s) {
  _voidcall(PUSHSTRING, s);
}

inline void ComponentAPI::maki_pushVoid() {
  _voidcall(PUSHVOID);
}

inline void ComponentAPI::maki_pushAny(scriptVar v) {
  _voidcall(PUSHANY, v);
}

inline void *ComponentAPI::maki_popObject() {
  return _call(POPOBJECT, (void *)0);
}                       

inline int ComponentAPI::maki_popInt() {
  return _call(POPINT, 0);
}

inline int ComponentAPI::maki_popBoolean() {
  return _call(POPBOOLEAN, 0);
}

inline float ComponentAPI::maki_popFloat() {
  return _call(POPFLOAT, (float)0);
}

inline double ComponentAPI::maki_popDouble() {
  return _call(POPDOUBLE, (double)0);
}

inline const char *ComponentAPI::maki_popString() {
  return _call(POPSTRING, (const char *)NULL);
}

inline scriptVar ComponentAPI::maki_popAny() {
  scriptVar v={SCRIPT_INT, 0};
  return _call(POPANY, v);
}

inline void ComponentAPI::maki_popDiscard() {
  _voidcall(POPDISCARD);
}

inline const char *ComponentAPI::maki_getFunction(int dlfid, int *n, ScriptObjectController **p) {
  return _call(GETMAKIFUNCTION, (const char *)NULL, dlfid, n, p);
}

inline int ComponentAPI::maki_addDlfRef(ScriptObjectController *o, const char *function_name, void *host) {
  return _call(ADDDLFREF, 0, o, function_name, host);
}

inline void ComponentAPI::maki_addDlfClassRef(ScriptObjectController *o, void *host) {
  _voidcall(ADDDLFCLASSREF, o, host);
}

inline void ComponentAPI::maki_remDlfRef(void *host) {
  _voidcall(REMDLFREF, host);
}

inline scriptVar ComponentAPI::maki_callFunction(ScriptObject *o, int dlfid, scriptVar **params) {
  scriptVar v={SCRIPT_INT, 0};
  return _call(CALLFUNCTION, v, o, dlfid, params);
}

inline scriptVar ComponentAPI::maki_triggerEvent(ScriptObject *o, int dlfid, int np, int scriptid) {
  return _call(TRIGGEREVENT, scriptVar(), o, dlfid, np, scriptid);
}

inline int ComponentAPI::maki_getScriptInt(scriptVar v) {
  return _call(GETSCRIPTINT, 0, v);
}

inline int ComponentAPI::maki_getScriptBoolean(scriptVar v) {
  return _call(GETSCRIPTBOOLEAN, 0, v);
}

inline float ComponentAPI::maki_getScriptFloat(scriptVar v) {
  return _call(GETSCRIPTFLOAT, (float)0, v);
}

inline double ComponentAPI::maki_getScriptDouble(scriptVar v) {
  return _call(GETSCRIPTDOUBLE, (double)0, v);
}

inline const char *ComponentAPI::maki_getScriptString(scriptVar v) {
  return _call(GETSCRIPTSTRING, (const char *)0, v);
}

inline ScriptObject *ComponentAPI::maki_getScriptObject(scriptVar v) {
  return _call(GETSCRIPTOBJECT, (ScriptObject *)0, v);
}

inline void ComponentAPI::vcpu_addScriptObject(ScriptObject *o) {
  _voidcall(ADDSCRIPTOBJECT, o);
}

inline void ComponentAPI::vcpu_removeScriptObject(ScriptObject *o) {
  _voidcall(REMOVESCRIPTOBJECT, o);
}

inline scriptVar ComponentAPI::maki_updateDlf(maki_cmd *cmd, int *dlfid, int *linkcount) {
  scriptVar v={SCRIPT_INT, 0};
  return _call(UPDATEDLF, v, cmd, dlfid, linkcount);
}

inline ScriptObject *ComponentAPI::maki_instantiate(GUID classguid) {
  return _call(MAKIINSTANTIATE, (ScriptObject *)NULL, classguid);
}

inline void ComponentAPI::maki_destroy(ScriptObject *o) {
  _voidcall(MAKIDESTROY, o);
}

inline void *ComponentAPI::maki_encapsulate(GUID classguid, ScriptObject *o) {
  return _call(MAKIENCAPSULATE, (ScriptObject *)NULL, classguid, o);
}

inline void ComponentAPI::maki_deencapsulate(GUID classguid, void *o) {
  _voidcall(MAKIDEENCAPSULATE, classguid, o);
}

inline ScriptObjectController *ComponentAPI::maki_getController(GUID scriptclass) {
  return _call(MAKIGETCONTROLLER, (ScriptObjectController *)NULL, scriptclass);
}

inline int ComponentAPI::maki_createOrphan(int type) {
  return _call(CREATEORPHAN, 0, type);
}

inline void ComponentAPI::maki_killOrphan(int id) {
  _voidcall(KILLORPHAN, id);
}

inline void ComponentAPI::maki_setObjectAtom(const char *atomname, ScriptObject *object) {
  _voidcall(MAKISETOBJATOM, atomname, object);
}

inline ScriptObject *ComponentAPI::maki_getObjectAtom(const char *atomname) {
  return _call(MAKIGETOBJATOM, (ScriptObject *)NULL, atomname);
}

inline ScriptObject *ComponentAPI::maki_findObject(const char *name) {
  return _call(MAKIFINDOBJECT, (ScriptObject *)NULL, name);
}

inline int ComponentAPI::vcpu_getCacheCount() {  
  return _call(GETCACHECOUNT, 0);
}

inline int ComponentAPI::vcpu_isValidScriptId(int id) {
  return _call(ISVALIDSCRIPTID, 0, id);
}

inline int ComponentAPI::vcpu_mapVarId(int varid, int scriptid) {
  return _call(MAPVARID, 0, varid, scriptid);
}

inline int ComponentAPI::vcpu_getUserAncestorId(int varid, int scriptid) {
  return _call(GETUSERANCESTORID, 0, varid, scriptid);
}

inline int ComponentAPI::vcpu_getNumEvents() {
  return _call(GETNUMEVENTS, 0);
}

inline int ComponentAPI::vcpu_getEvent(int event, int *dlf, int *script, int *var) {
  return _call(GETEVENT, 0, event, dlf, script, var);
}

inline RootWnd *ComponentAPI::group_create(const char *groupid) {
  return _call(GROUPCREATE, (RootWnd *)0, groupid);
}

inline int ComponentAPI::group_destroy(RootWnd *group) {
  return _call(GROUPDESTROY, 0, group);
}

inline RootWnd *ComponentAPI::group_create_cfg(const char *groupid, CfgItem *cfgitem, const char *attributename) {
  return _call(GROUPCREATECFG, (RootWnd *)0, groupid, cfgitem, attributename);
}

inline RootWnd *ComponentAPI::group_create_layout(const char *groupid) {
  return _call(GROUPCREATELAYOUT, (RootWnd *)0, groupid);
}

inline int ComponentAPI::autopopup_registerGuid(GUID g, const char *desc, const char *prefered_container, int container_flag) {
  return _call(AUTOPOPUP_REGGUID, 0, g, desc, prefered_container, container_flag);
}

inline int ComponentAPI::autopopup_registerGroupId(const char *groupid, const char *desc, const char *prefered_container, int container_flag) {
  return _call(AUTOPOPUP_REGGROUP, 0, groupid, desc, prefered_container, container_flag);
}

inline void ComponentAPI::autopopup_unregister(int id) {
  _voidcall(AUTOPOPUP_UNREG, id);
}

inline int ComponentAPI::syscb_registerCallback(SysCallback *cb, void *param) {
  return _call( SYSCB_REGISTERCALLBACK, (int)0, cb, param );
}

inline int ComponentAPI::syscb_deregisterCallback(SysCallback *cb) {
  return _call( SYSCB_DEREGISTERCALLBACK, (int)0, cb );
}

inline int ComponentAPI::cmd_sendCommand(GUID guid, char *command, int param1, int param2, void *ptr, int ptrlen) {
  return _call( CMD_SENDCOMMAND, (int)0, guid, command, param1, param2, ptr, ptrlen );
}

inline int ComponentAPI::cmd_postCommand(GUID guid, char *command, int param1, int param2, void *ptr, int ptrlen, int waitforanswer) {
  return _call( CMD_POSTCOMMAND, (int)0, guid, command, param1, param2, ptr, ptrlen, waitforanswer );
}

inline void ComponentAPI::cmd_broadcastCommand(char *command, int param1, int param2, void *ptr, int ptrlen) {
  _voidcall( CMD_BROADCASTCOMMAND, command, param1, param2, ptr, ptrlen );
}

inline void ComponentAPI::status_setTrayTipText(const char *text) {
  _voidcall( STATUS_SETTRAYTIPTEXT, text );
}

inline void ComponentAPI::main_enableInput(int enable) {
  _voidcall( MAIN_ENABLEINPUT, enable );
}

inline const char *ComponentAPI::locales_getTranslation(const char *str) {
  return _call( LOCALES_GETTRANSLATION, "", str );
}

inline void ComponentAPI::locales_addTranslation(const char *from, const char *to) {
  _voidcall( LOCALES_ADDTRANSLATION, from, to );
}

inline int ComponentAPI::locales_getNumEntries() {
  return _call(LOCALES_GETNUMENTRIES, 0);
}

inline const char *ComponentAPI::locales_enumEntry(int n) {
  return _call(LOCALES_ENUMENTRY, (const char *)NULL, n);
}

inline const char *ComponentAPI::locales_getBindFromAction(int action) {
  return _call( LOCALES_GETBINDFROMACTION, "", action );
}

inline void ComponentAPI::locales_registerAcceleratorSection(const char *name, RootWnd *wnd, int global) {
  _voidcall( LOCALES_REGISTERACCELERATORSECTION, name, wnd, global );
}

inline int ComponentAPI::messageBox(const char *txt, const char *title, int flags, const char *not_anymore_identifier, RootWnd *parenwnt) {
  return _call( MESSAGEBOX, (int)0, txt, title, flags, not_anymore_identifier, parenwnt );
}

inline RootWnd *ComponentAPI::getModalWnd() {
  return _call( GETMODALWND, (RootWnd *)0  );
}

inline void ComponentAPI::pushModalWnd(RootWnd *w) {
  _voidcall( PUSHMODALWND, w);
}

inline void ComponentAPI::popModalWnd(RootWnd *w) {
  _voidcall( POPMODALWND, w);
}

inline void ComponentAPI::metrics_addCallback(MetricsCallback *mcb) {
  _voidcall( METRICS_ADDCALLBACK, mcb );
}

inline void ComponentAPI::metrics_delCallback(MetricsCallback *mcb) {
  _voidcall( METRICS_DELCALLBACK, mcb );
}

inline int ComponentAPI::metrics_getDelta() {
  return _call( METRICS_GETDELTA, (int)0  );
}

inline int ComponentAPI::metrics_setDelta(int newdelta) {
  return _call( METRICS_SETDELTA, (int)0, newdelta );
}

inline void ComponentAPI::main_navigateUrl(const char *url) {
  _voidcall( MAIN_NAVIGATEURL, url );
}

inline int ComponentAPI::metadb_optimizePlayString(char *playstring) {
  return _call( METADB_OPTIMIZEPLAYSTRING, (int)0, playstring );
}

inline int ComponentAPI::metadb_addNewItem(const char *playstring, const char *initial_name) {
  return _call( METADB_ADDNEWITEM, (int)0, playstring, initial_name);
}

inline
int ComponentAPI::metadb_deleteItem(const char *playstring) {
  return _call(METADB_DELETEITEM, 0, playstring);
}

inline void ComponentAPI::metadb_sync() {
  _voidcall( METADB_SYNC  );
}

inline const char *ComponentAPI::metadb_dupPlaystring(const char *playstring, int tablenum) {
  return _call( METADB_DUPPLAYSTRING, "", playstring, tablenum);
}

inline void ComponentAPI::metadb_releasePlaystring(const char *playstring, int tablenum) {
  _voidcall( METADB_RELEASEPLAYSTRING, playstring, tablenum);
}

inline void ComponentAPI::metadb_getInfo(const char *playstring, char *info, int maxlen) {
  _voidcall( METADB_GETINFO, playstring, info, maxlen );
}

inline int ComponentAPI::metadb_getLength(const char *playstring) {
  return _call( METADB_GETLENGTH, (int)0, playstring );
}
inline int ComponentAPI::metadb_getMetaData(const char *playstring, const char *name, char *data, int data_len, int data_type ) {
  return _call( METADB_GETMETADATA, (int)0, playstring, name, data, data_len, data_type  );
}

inline int ComponentAPI::metadb_getMetaDataType(const char *name) {
  return _call( METADB_GETMETADATATYPE, (int)0, name );
}

inline GUID ComponentAPI::metadb_getMetaDataTable(const char *name) {
  return _call( METADB_GETMETADATATABLE, INVALID_GUID, name );
}

inline int ComponentAPI::metadb_setMetaData(const char *playstring, const char *name, const char *data, int data_len, int data_type ) {
  return _call( METADB_SETMETADATA, (int)0, playstring, name, data, data_len, data_type  );
}

inline int ComponentAPI::metadb_renderData(CanvasBase *cb, RECT &r, void *data, int datatype, int centered) {
  return _call( METADB_RENDERDATA, (int)0, cb, &r, data, datatype, centered);
}

inline int ComponentAPI::metadb_convertToText(void *data, int datatype, char *buf, int buflen) {
  return _call( METADB_CONVERTTOTEXT, (int)0, data, datatype, buf, buflen );
}

inline void ComponentAPI::metadb_deleteMetaData(GUID metadataservice, const char *playitem) {
  _voidcall( METADB_DELETEMETADATA, metadataservice, playitem);
}

inline dbSvcScanner *ComponentAPI::metadb_newScanner(GUID metadataservice) {
  return _call( METADB_NEWSCANNER, (dbSvcScanner *)NULL, metadataservice );
}

inline void ComponentAPI::metadb_deleteScanner(dbSvcScanner *scanner) {
  _voidcall( METADB_DELETESCANNER, scanner);
}

inline void ComponentAPI::startCompleted(int max) {
  _voidcall( STARTCOMPLETED, max );
}

inline void ComponentAPI::incCompleted() {
  _voidcall( INCCOMPLETED );
}

inline void ComponentAPI::endCompleted() {
  _voidcall( ENDCOMPLETED );
}

inline COLORREF ComponentAPI::skin_getColorElement(char *type) {
  return _call( SKIN_GETELEMENTCOLOR, (COLORREF)0, type );
}

inline COLORREF *ComponentAPI::skin_getColorElementRef(char *type) {
  return _call( SKIN_GETELEMENTREF, (COLORREF *)0, type );
}

inline int *ComponentAPI::skin_getIterator() {
  return _call( SKIN_GETITERATOR, (int *)0);
}

inline void ComponentAPI::skin_renderBaseTexture(RootWnd *base, CanvasBase *c, RECT &r, RootWnd *destWnd, int alpha) {
  _voidcall( SKIN_RENDERBASETEXTURE, base, c, &r, destWnd, alpha );
}

inline void ComponentAPI::skin_registerBaseTextureWindow(RootWnd *window, const char *bmp) {
  _voidcall( SKIN_REGISTERBASETEXTUREWINDOW, window, bmp);
}

inline void ComponentAPI::skin_unregisterBaseTextureWindow(RootWnd *window) {
  _voidcall( SKIN_UNREGISTERBASETEXTUREWINDOW, window );
}

inline void ComponentAPI::skin_switchSkin(char *skin_name) {
  _voidcall( SKIN_SWITCHSKIN, skin_name );
}

inline const char *ComponentAPI::getSkinName() {
  return _call( GETSKINNAME, ""  );
}

inline const char *ComponentAPI::getSkinPath() {
  return _call( GETSKINPATH, ""  );
}

inline const char *ComponentAPI::getDefaultSkinPath() {
  return _call( GETDEFAULTSKINPATH, ""  );
}

inline void *ComponentAPI::sysMalloc(int size) {
  return _call(SYSMALLOC, (void *)0, size);
}

inline void ComponentAPI::sysFree(void *ptr) {
  _voidcall(SYSFREE, ptr);
}

inline void *ComponentAPI::sysRealloc(void *ptr, int newsize) {
  return _call(SYSREALLOC, (void *)0, ptr, newsize);
}

inline ARGB32 *ComponentAPI::imgldr_makeBmp(const char *filename, int *has_alpha, int *w, int *h) {
  return _call( IMGLDR_MAKEBMP, (ARGB32 *)0, filename, has_alpha, w, h );
}

inline ARGB32 *ComponentAPI::imgldr_makeBmp(HINSTANCE hInst, int id, int *has_alpha, int *w, int *h, const char *colorgroup) {
  return _call( IMGLDR_MAKEBMP2, (ARGB32 *)0, hInst, id, has_alpha, w, h, colorgroup);
}

inline ARGB32 *ComponentAPI::imgldr_requestSkinBitmap(const char *file, int *has_alpha, int *x, int *y, int *subw, int *subh, int *w, int *h, int cached) {
  return _call( IMGLDR_REQUESTSKINBITMAP, (ARGB32 *)0, file, has_alpha, x, y, subw, subh, w, h, cached);
}

inline void ComponentAPI::imgldr_releaseSkinBitmap(ARGB32 *bmpbits) {
  _voidcall( IMGLDR_RELEASESKINBITMAP, bmpbits );
}

inline RegionServer *ComponentAPI::imgldr_requestSkinRegion(const char *id) {
  return _call(IMGLDR_REQUESTSKINREGION, (RegionServer *)NULL, id);
}

inline void ComponentAPI::imgldr_cacheSkinRegion(const char *id, Region *r) {
  _voidcall(IMGLDR_CACHESKINREGION, id, r);
}

inline COLORREF ComponentAPI::filterSkinColor(COLORREF color, const char *groupname) {
  return _call( FILTERSKINCOLOR, (COLORREF)0, color, groupname );
}

inline void ComponentAPI::reapplySkinFilters() {
  _voidcall( REAPPLYSKINFILTERS  );
}

inline int ComponentAPI::colortheme_getNumColorSets() {
  return _call(GETNUMCOLORSETS, 0);
}

inline const char *ComponentAPI::colortheme_enumColorSet(int n) {
  return _call(ENUMCOLORSET, (const char *)NULL, n);
}

inline int ComponentAPI::colortheme_getNumColorGroups(const char *colorset) {
  return _call(GETNUMCOLORGROUPS, 0, colorset);
}

inline const char *ComponentAPI::colortheme_enumColorGroup(const char *colorset, int n) {
  return _call(ENUMCOLORGROUP, (const char *)NULL, colorset, n);
}

inline void ComponentAPI::colortheme_setColorSet(const char *colorset) {
  _voidcall(SETCOLORSET, colorset);
}

inline const char *ComponentAPI::colortheme_getColorSet() {
  return _call(GETCOLORSET, (const char *)NULL);
}

inline void ComponentAPI::paintset_render(int set, CanvasBase *c, const RECT *r, int alpha) {
  _voidcall( PAINTSET_RENDER, set, c,  r, alpha);
}

inline void ComponentAPI::paintset_renderTitle(char *t, CanvasBase *c, const RECT *r, int alpha) {
  _voidcall( PAINTSET_RENDERTITLE, t, c,  r, alpha);
}

inline void ComponentAPI::drawAnimatedRects(const RECT *r1, const RECT *r2) {
  _voidcall( DRAWANIMATEDRECTS,  r1,  r2 );
}

inline void ComponentAPI::sendDdeCommand(const char *application, const char *command, DWORD minimumDelay) {
  _voidcall( SENDDDECOMMAND, application, command, minimumDelay );
}

inline void ComponentAPI::xmlreader_registerCallback(char *matchstr, XmlReaderCallback *callback) {
  _voidcall( XMLREADER_REGISTERCALLBACK, matchstr, callback );
}

inline void ComponentAPI::xmlreader_unregisterCallback(XmlReaderCallback *callback) {
  _voidcall( XMLREADER_UNREGISTERCALLBACK, callback );
}

inline void ComponentAPI::xmlreader_loadFile(const char *filename, XmlReaderCallback *only_this_object) {
  _voidcall( XMLREADER_LOADFILE, filename, only_this_object);
}

inline void ComponentAPI::wndTrackAdd(RootWnd *wnd) {
  _voidcall( WNDTRACKADD, wnd );
}

inline void ComponentAPI::wndTrackRemove(RootWnd *wnd) {
  _voidcall( WNDTRACKREMOVE, wnd );
}

inline BOOL ComponentAPI::wndTrackDock(RootWnd *wnd, RECT *r, int mask) {
  return _call( WNDTRACKDOCK, (BOOL)0, wnd,  r, mask );
}

inline BOOL ComponentAPI::wndTrackDock(RootWnd *wnd, RECT *r, RECT *or, int mask) {
  return _call( WNDTRACKDOCK2, (BOOL)0, wnd,  r,  or, mask );
}

inline void ComponentAPI::wndTrackStartCooperative(RootWnd * wnd) {
  _voidcall( WNDTRACKSTARTCOOPERATIVE, wnd );
}

inline void ComponentAPI::wndTrackEndCooperative() {
  _voidcall( WNDTRACKENDCOOPERATIVE  );
}

inline int ComponentAPI::wndTrackWasCooperative() {
  return _call( WNDTRACKWASCOOPERATIVE, (int)0  );
}

inline void ComponentAPI::wndTrackInvalidateAll() {
  _voidcall(WNDTRACKINVALIDATEALL, 0);
}

inline int ComponentAPI::getNumComponents() {
  return _call( GETNUMCOMPONENTS, (int)0  );
}

inline GUID ComponentAPI::getComponentGUID(int c) {
  return _call( GETCOMPONENTGUID, GUID(), c );
}

inline const char *ComponentAPI::getComponentName(GUID componentGuid) {
  return _call( GETCOMPONENTNAME, "", componentGuid );
}

inline CfgItem *ComponentAPI::compon_getCfgInterface(GUID componentGuid) {
  return _call(COMPON_GETCFGINTERFACE, (CfgItem*)NULL, componentGuid);
}

inline RootWnd *ComponentAPI::skinwnd_createByGuid(GUID g, const char *prefered_container/*=NULL*/, int container_flag/*=LAYOUT_JUSTAHINT*/, RECT *sourceanimrect/* =NULL */, int transcient/*=0*/, int starthidden/*=0*/) {
  return _call( SKINWND_CREATEBYGUID, (RootWnd *)NULL, g, prefered_container, container_flag, sourceanimrect, transcient, starthidden);
}

inline RootWnd *ComponentAPI::skinwnd_createByGroupId(const char *groupid, const char *prefered_container/*=NULL*/, int container_flag/*=LAYOUT_JUSTAHINT*/, RECT *sourceanimrect/* =NULL */, int transcient/*=0*/, int starthidden/*=0*/) {
  return _call( SKINWND_CREATEBYGROUPID, (RootWnd *)NULL, groupid, prefered_container, container_flag, sourceanimrect, transcient, starthidden);
}

inline int ComponentAPI::skinwnd_toggleByGuid(GUID g, const char *prefered_container/*=NULL*/, int container_flag/*=LAYOUT_JUSTAHINT*/, RECT *sourceanimrect/* =NULL */, int transcient/*=0*/) {
  return _call( SKINWND_TOGGLEBYGUID, 0, g, prefered_container, container_flag, sourceanimrect, transcient);
}

inline int ComponentAPI::skinwnd_toggleByGroupId(const char *groupid, const char *prefered_container/*=NULL*/, int container_flag/*=LAYOUT_JUSTAHINT*/, RECT *sourceanimrect/* =NULL */, int transcient/*=0*/) {
  return _call( SKINWND_TOGGLEBYGROUPID, 0, groupid, prefered_container, container_flag, sourceanimrect, transcient);
}

inline void ComponentAPI::skinwnd_destroy(RootWnd *w, RECT *destanimrect/* =NULL */) {
  _voidcall( SKINWND_DESTROY, w, destanimrect);
}

inline int ComponentAPI::skinwnd_getNumByGuid(GUID g) {
  return _call( SKINWND_GETNUMBYGUID, 0, g);
}

inline RootWnd *ComponentAPI::skinwnd_enumByGuid(GUID g, int n) {
  return _call( SKINWND_ENUMBYGUID, (RootWnd *)NULL, g, n);
}

inline int ComponentAPI::skinwnd_getNumByGroupId(const char *groupid) {
  return _call( SKINWND_ENUMBYGROUPID, 0, groupid);
}

inline void ComponentAPI::skinwnd_attachToSkin(RootWnd *w, int side, int size) {
  _voidcall(SKINWND_ATTACHTOSKIN, w, side, size);
}

inline ScriptObject *ComponentAPI::skin_getContainer(const char *container_name) {
  return _call(SKIN_GETCONTAINER, (ScriptObject *)NULL, container_name);
}

inline ScriptObject *ComponentAPI::skin_getLayout(ScriptObject *container, const char *layout_name) {
  return _call(SKIN_GETLAYOUT, (ScriptObject *)NULL, container, layout_name);
}

inline void ComponentAPI::wndholder_register(WindowHolder *wh) {
  _voidcall(WNDHOLDER_REGISTER, wh);
}

inline void ComponentAPI::wndholder_unregister(WindowHolder *wh) {
  _voidcall(WNDHOLDER_UNREGISTER, wh);
}

inline int ComponentAPI::comp_notifyScripts(const char *s, int i1, int i2) {
  return _call( COMP_NOTIFYSCRIPTS, (int)0, s, i1, i2 );
}

inline RootWnd *ComponentAPI::rootWndFromPoint(POINT *pt) {
  return _call( ROOTWNDFROMPOINT, (RootWnd *)0, pt );
}

inline void ComponentAPI::registerRootWnd(RootWnd *wnd) {
  _voidcall( REGISTERROOTWND, wnd );
}

inline void ComponentAPI::unregisterRootWnd(RootWnd *wnd) {
  _voidcall( UNREGISTERROOTWND, wnd );
}

inline int ComponentAPI::rootwndIsValid(RootWnd *wnd) {
  return _call( ROOTWNDISVALID, (int)0, wnd );
}

inline int ComponentAPI::forwardOnChar(unsigned int c) {
  return _call( FORWARDONCHAR, (int)0, c );
}

inline int ComponentAPI::forwardOnKeyDown(RootWnd *from, int k) {
  return _call( FORWARDONKEYDOWN, (int)0, from, k );
}

inline int ComponentAPI::forwardOnKeyUp(int k) {
  return _call( FORWARDONKEYUP, (int)0, k );
}

inline int ComponentAPI::forwardOnSysKeyDown(RootWnd *from, int k, int kd) {
  return _call( FORWARDONSYSKEYDOWN, (int)0, from, k , kd );
}

inline int ComponentAPI::forwardOnSysKeyUp(int k, int kd) {
  return _call( FORWARDONSYSKEYUP, (int)0, k , kd );
}

inline int ComponentAPI::forwardOnKillFocus() {
  return _call( FORWARDONKILLFOCUS, (int)0  );
}

inline void *ComponentAPI::fileOpen(const char *filename, const char *mode) {
  return _call( FILEOPEN, (void *)0, filename, mode );
}

inline void ComponentAPI::fileClose(void *fileHandle) {
  _voidcall( FILECLOSE, fileHandle );
}

inline int ComponentAPI::fileRead(void *buffer, int size, void *fileHandle) {
  return _call( FILEREAD, (int)0, buffer, size, fileHandle );
}

inline int ComponentAPI::fileWrite(const void *buffer, int size, void *fileHandle) {
  return _call( FILEWRITE, (int)0, buffer, size, fileHandle );
}

inline int ComponentAPI::fileSeek(int offset, int origin, void *fileHandle) {
  return _call( FILESEEK, (int)0, offset, origin, fileHandle );
}

inline int ComponentAPI::fileTell(void *fileHandle) {
  return _call( FILETELL, (int)0, fileHandle );
}

inline int ComponentAPI::fileGetFileSize(void *fileHandle) {
  return _call( FILEGETFILESIZE, (int)0, fileHandle );
}

inline int ComponentAPI::fileExists(const char *filename) {
  return _call( FILEEXISTS, (int)0, filename );
}

inline int ComponentAPI::fileRemove(const char *filename) {
  return _call( FILEREMOVE, (int)0, filename );
}

inline int ComponentAPI::fileMove(const char *filename, const char *destfilename) {
  return _call(FILEMOVE, 0, filename, destfilename);
}

inline void ComponentAPI::font_textOut(CanvasBase *c, int style, int x, int y, int w, int h, const char *txt) {
  _voidcall( FONT_TEXTOUT, c, style, x, y, w, h, txt );
}

inline int ComponentAPI::font_getInfo(CanvasBase *c, const char *font, int infoid, const char *txt, int *w, int *h) {
  return _call( FONT_GETINFO, (int)0, c, font, infoid, txt, w, h );
}

/*
inline void ComponentAPI::hint_garbageCollect() {
  _voidcall( HINT_GARBAGECOLLECT  );
}
*/
inline int ComponentAPI::console_outputString(int severity, const char *string) {
  return _call( CONSOLE_OUTPUTSTRING, 0, severity, string );
}

inline int ComponentAPI::loadSkinFile(const char *xmlfile) {
  return _call( LOADSKINFILE, 0, xmlfile );
}

inline void ComponentAPI::unloadSkinPart(int skinpart) {
  _voidcall( UNLOADSKINPART, skinpart);
}

inline void ComponentAPI::navigateUrl(const char *url) {
  _voidcall( NAVIGATEURL, url );
}

inline int ComponentAPI::parse(const char *str, const char *how) {
  return _call( PARSE, 0, str, how);
}

inline const char *ComponentAPI::varmgr_translate(const char *str) {
  return _call(VARMGRTRANSLATE, (const char *)NULL, str);
}

inline void ComponentAPI::timer_add(TimerClient *client, int id, int ms) {
  _voidcall(TIMERADD, client, id, ms);
}

inline void ComponentAPI::timer_remove(TimerClient *client, int id) {
  _voidcall(TIMERREMOVE, client, id);
}

inline void ComponentAPI::config_registerCfgItem(CfgItem *cfgitem) {
  _voidcall(CONFIG_REGISTERCFGITEM, cfgitem);
}

inline void ComponentAPI::config_deregisterCfgItem(CfgItem *cfgitem) {
  _voidcall(CONFIG_DEREGISTERCFGITEM, cfgitem);
}

inline int ComponentAPI::config_getNumCfgItems() {
  return _call(CONFIG_GETNUMCFGITEMS, (int)0);
}

inline CfgItem *ComponentAPI::config_enumCfgItem(int n) {
  return _call(CONFIG_ENUMCFGITEM, (CfgItem *)NULL, n);
}

inline CfgItem *ComponentAPI::config_getCfgItemByGuid(GUID g) {
  return _call(CONFIG_GETCFGITEMBYGUID, (CfgItem *)NULL, g);
}

inline int ComponentAPI::popupexit_check(RootWnd *w) {
  return _call(POPUPEXIT_CHECK, 0, w);
}

inline void ComponentAPI::popupexit_signal() {
  _voidcall(POPUPEXIT_SIGNAL);
}

inline void ComponentAPI::popupexit_register(PopupExitCallback *cb, RootWnd *watched) {
  _voidcall(POPUPEXIT_REGISTER, cb, watched);
}

inline void ComponentAPI::popupexit_deregister(PopupExitCallback *cb) {
  _voidcall(POPUPEXIT_DEREGISTER, cb);
}

inline void ComponentAPI::preferences_registerGroup(const char *group_id, const char *desc, GUID pref_guid, GUID pref_guid_parent) {
  _voidcall(PREFERENCES_REGISTERGROUP, group_id, desc, pref_guid, pref_guid_parent);
}

inline int ComponentAPI::preferences_getNumGroups(){
  return _call(PREFERENCES_GETNUMGROUPS, 0);
}

inline const char *ComponentAPI::preferences_enumGroup(int n, const char **desc, GUID *this_guid, GUID *parent_guid) {
  return _call(PREFERENCES_ENUMGROUP, (const char *)NULL, n, desc, this_guid, parent_guid);
}

inline void ComponentAPI::appdeactivation_push_disallow(RootWnd *w) {
  _voidcall(APPNODEACTIVATE_PUSH, w);
}

inline void ComponentAPI::appdeactivation_pop_disallow(RootWnd *w) {
  _voidcall(APPNODEACTIVATE_POP, w);
}

inline int ComponentAPI::appdeactivation_isallowed(RootWnd *w) {
  return _call(APPNODEACTIVATE_CHECK, 0, w);
}

inline void ComponentAPI::appdeactivation_setbypass(int i) {
  _voidcall(APPNODEACTIVATE_BYPASS, i);
}

inline GuiObject *ComponentAPI::xui_new(const char *classname) {
  return _call(XUI_NEW, (GuiObject *)NULL, classname);
}

inline void ComponentAPI::xui_delete(GuiObject *o) {
  _voidcall(XUI_DELETE, o);
}

#endif
