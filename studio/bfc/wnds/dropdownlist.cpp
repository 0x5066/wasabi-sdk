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

#include "dropdownlist.h"
#include "../../common/script/guiobject.h"
#include "../../common/script/scriptguid.h"
#include "../../common/script/c_script/c_text.h"
#include "../../studio/api.h"
#include "../popexitcb.h"
#include "../notifmsg.h"
#include "../svc_enum.h"
#include "../paramparser.h"

#define DDL_CLOSELISTCB 0x0721

// -----------------------------------------------------------------------
DropDownList::DropDownList() {
  selected = -1;
  //abstract_setAllowDeferredContent(1);
  clicks_button = NULL;
  clicks_text = NULL;
  list_key = NULL;
  height = 128;
  noitemtext = "";
  list_group = NULL;
  trap_click = 0;
  disable_cfg_event = 0;
  feed = NULL;

  getScriptObject()->vcpu_setInterface(dropDownListGuid, (void *)static_cast<DropDownList *>(this));
  getScriptObject()->vcpu_setClassName("DropDownList"); // this is the script class name
  getScriptObject()->vcpu_setController(dropDownListController);

  myxuihandle = newXuiHandle();
                         
  addParam(myxuihandle, "items", DROPDOWNLIST_SETITEMS, XUI_ATTRIBUTE_IMPLIED);
  addParam(myxuihandle, "feed", DROPDOWNLIST_SETFEED, XUI_ATTRIBUTE_IMPLIED);
  addParam(myxuihandle, "select", DROPDOWNLIST_SELECT, XUI_ATTRIBUTE_IMPLIED);

  registerAcceleratorSection("popup", 1);
}

DropDownList::~DropDownList() { 
  closeFeed();
  doCloseList(0);
  delete clicks_text;
  delete clicks_button;
  delete list_key;
}

int DropDownList::onAcceleratorEvent(const char *name) {
  int r = DROPDOWNLIST_PARENT::onAcceleratorEvent(name);
  if (STRCASEEQLSAFE(name, "exit")) {
    escapeCallback();
    return 1;
  }
  return r;
}

// -----------------------------------------------------------------------
int DropDownList::setXuiParam(int xuihandle, int xmlattributeid, const char *xmlattributename, const char *value) {
  if (xuihandle != myxuihandle)
    return DROPDOWNLIST_PARENT::setXuiParam(xuihandle, xmlattributeid, xmlattributename, value);

  switch (xmlattributeid) {
    case DROPDOWNLIST_SETITEMS:
      setItems(value);
      break;
    case DROPDOWNLIST_SETFEED:
      setFeed(value);
      break;
    case DROPDOWNLIST_SELECT:
      selectItem(findItem(value));
      break;
    default:
      return 0;
  }
  return 1;
}

int DropDownList::onInit() {
  int rt = DROPDOWNLIST_PARENT::onInit();
  abstract_setContent(dropdownlist_getMainGroupId());
  return rt;
}

void DropDownList::abstract_onNewContent() {
  DROPDOWNLIST_PARENT::abstract_onNewContent();
  trapControls();
  updateTextInControl(getSelectedText());
}  

int DropDownList::onReloadConfig() {
  int r = DROPDOWNLIST_PARENT::onReloadConfig();
  disable_cfg_event = 1;
  updateTextFromConfig(); // triggers onSelect
  disable_cfg_event = 0;
  return r;
}

void DropDownList::updateTextFromConfig() { 
  const char *val = getGuiObject()->guiobject_getCfgString();
  const char *old = getSelectedText();
  if (old && val && STRCASEEQL(val, old)) return;

  if (val != NULL) {
    int id = findItem(val);
    if (id != -1)
      selectItem(id);
  }
}

void DropDownList::trapControls() {
  delete clicks_button;
  delete clicks_text;

  clicks_button = NULL;
  clicks_text = NULL;

  if (wantTrapText()) {
    GuiObject *textGuiObj = getGuiObject()->guiobject_findObject(dropdownlist_getTextId());
    if (textGuiObj) clicks_text = new DDLClicksCallback(*textGuiObj, this);
  }

  if (wantTrapButton()) {
    GuiObject *butGuiObj = getGuiObject()->guiobject_findObject(dropdownlist_getButtonId());
    if (butGuiObj) clicks_button = new DDLClicksCallback(*butGuiObj, this);
  }
}


void DropDownList::clickCallback() {
  if (list_group != NULL)
    closeList();
  else
    openList();
}

void DropDownList::escapeCallback() {
  if (isListOpen())
    closeList();
}

void DropDownList::openList() {
  onPreOpenList();
  api->appdeactivation_push_disallow(this);
  list_group = api->group_create_layout(dropdownlist_getListGroupId());
  group_dep = list_group->getDependencyPtr();
  viewer_addViewItem(group_dep);
  if (list_group == NULL) return;
  list_group->setStartHidden(1);
  list_group->setParent(api->main_getRootWnd());
  trap_click = 0;
  list_group->init(this, TRUE);
  setListParams();
  RECT r;
  getWindowRect(&r);
  int w = r.right-r.left;
  r.top = r.bottom;
  r.bottom = r.top + height;
  divRatio(&r);
  list_group->resize(r.left, r.top, r.right-r.left, r.bottom-r.top);
  list_group->setVisible(1);
  api->popupexit_register(this, this); // this will call us back whenever someone clicks outside us
  trap_click = 1;
  RootWnd *list = list_group->findWindowByInterface(listGuid);
  if (list != NULL)
    list_key = new DDLKeyCallback(list->getGuiObject()->guiobject_getScriptObject(), this);
  dropdownlist_onOpenList();
}

void DropDownList::dropdownlist_onOpenList() {
  SetCapture(NULL); // NONPORTABLE, the goal is to cancel any capture some of our content guiobject might have so as to let the click down + slide in list transfer mouse capture
}

void DropDownList::dropdownlist_onCloseList() {
}

void DropDownList::closeList() {
  if (list_group != NULL) {
    onPreCloseList();
    postDeferredCallback(DDL_CLOSELISTCB, 0);
  }
}

void DropDownList::doCloseList(int cb) {
  if (cb) dropdownlist_onCloseList();
  if (list_group) {
    trap_click = 0;
    api->popupexit_deregister(this);
    api->group_destroy(list_group);
    list_group = NULL;
    group_dep = NULL;
    action_list = NULL;
    delete list_key;
    list_key = NULL;
    api->appdeactivation_pop_disallow(this);
  }
}

void DropDownList::setListParams() {
  ASSERT(list_group != NULL);
  GuiObject *go = static_cast<GuiObject *>(list_group->getInterface(guiObjectGuid));
  if (go != NULL) {
    dropdownlist_onConfigureList(go);
  }
}

void DropDownList::dropdownlist_onConfigureList(GuiObject *go) {
  XmlObject *o = NULL;
  if (go != NULL) {
    GuiObject *list = go->guiobject_findObject(dropdownlist_getListId());
    action_list = list->guiobject_getRootWnd();
    if (list != NULL) {
      o = static_cast<XmlObject *>(list->guiobject_getScriptObject()->vcpu_getInterface(xmlObjectGuid));
    }
  }
  String s;
  foreach(items) 
    if (foreach_index > 0) s += ";";
    s += items.getfor()->getText();
  endfor;
  o->setXmlParam("multiselect", "0");
  o->setXmlParam("hoverselect", "1");
  o->setXmlParam("selectonupdown", "0");
  o->setXmlParam("sort", StringPrintf("%d", wantAutoSort()));
  o->setXmlParam("items", s);
  if (selected != -1) o->setXmlParam("select", getSelectedText());
}

int DropDownList::onAction(const char *action, const char *param, int x, int y, int p1, int p2, void *data, int datalen, RootWnd *source) {
  if (STRCASEEQLSAFE(action, "set_selection")) {
    int p = findItem(param);
    selectItem(p);
    return p;
  }
  if (STRCASEEQLSAFE(action, "get_selection")) {
    if (source)
      sendAction(source, "set_selection", getSelectedText());
  }
  return DROPDOWNLIST_PARENT::onAction(action, param, x, y, p1, p2, data, datalen, source);
}

int DropDownList::addItem(const char *text) {
  DDLEntry *e = new DDLEntry(text);
  items.setSorted(wantAutoSort());
  items.addItem(e);
  return e->getId();
}

void DropDownList::selectDefault() {
  onReloadConfig();
}

void DropDownList::delItem(int id) {
  foreach(items)
    if (items.getfor()->getId() == id) {
      delete items.getfor();
      items.removeByPos(foreach_index);
      break;
    }
  endfor;
  if (list_group != NULL)
    setListParams();
}

void DropDownList::selectItem(int id, int hover) {
  //FG> DO NOT PUT THIS TEST BACK: if (selected == id) return;
  selected = id;
  onSelect(selected, hover);
}

void DropDownList::onSelect(int id, int hover) {
  updateTextInControl(getSelectedText());
  if (!disable_cfg_event && !hover) {
    if (selected == -1)
      getGuiObject()->guiobject_setCfgString("");
    else
      getGuiObject()->guiobject_setCfgString(getSelectedText());
  }
}

const char *DropDownList::getItemText(int id) {
  foreach(items)
    if (items.getfor()->getId() == id)
      return items.getfor()->getText();
  endfor;
  return NULL;
}

int DropDownList::findItem(const char *text) {
  int pos=-1;
  items.findItem(text, &pos);
  if (pos < 0) return -1;
  return items[pos]->getId();
}

void DropDownList::updateTextInControl(const char *txt) {
  GuiObject *content = getContent();
  if (content != NULL) {
    if (wantTrapText()) {
      GuiObject *text = content->guiobject_findObject(dropdownlist_getTextId());
      if (text != NULL) {
        C_Text t(*text);
        t.setText(txt);
      }
    }
  }
}

void DropDownList::setNoItemText(const char *txt) {
  noitemtext = txt;
  if (selected == -1)
    updateTextInControl(getSelectedText());
}

int DropDownList::popupexitcb_onExitPopup() {
  closeList();
  return 1;
}

int DropDownList::childNotify(RootWnd *child, int msg, int param1, int param2) {
  if (msg == ChildNotify::LISTWND_ITEMSELCHANGED && param2 && trap_click) {
    sendAction(action_list, "get_selection");
    closeList();
  }
  return DROPDOWNLIST_PARENT::childNotify(child, msg, param1, param2);
}

int DropDownList::onDeferredCallback(int p1, int p2) {
  if (p1 == DDL_CLOSELISTCB)
    doCloseList();
  return DROPDOWNLIST_PARENT::onDeferredCallback(p1, p2);
}

int DropDownList::viewer_onItemDeleted(Dependent *item) {
  if (item == group_dep) {
    api->popupexit_deregister(this);
    trap_click = 0;
    list_group = NULL;
    group_dep = NULL;
    action_list = NULL;
  }
  return 1;
}

void DropDownList::deleteAllItems() {
  items.deleteAll();
  selected = -1;
}

int DropDownList::onKeyDown(int keyCode) {
  if (keyCode == VK_DELETE && isListOpen()) {
    closeList();
    return 1;
  }
  return DROPDOWNLIST_PARENT::onKeyDown(keyCode);
}

// -----------------------------------------------------------------------
void DropDownList::openFeed(const char *feedid) {
  if (STRCASEEQL(feedid, last_feed)) return;
  feed = TextFeedEnum(feedid).getFirst();
  if (feed != NULL)
    feed->registerCallback(feedid, this);
  last_feed = feedid;
}

// -----------------------------------------------------------------------
void DropDownList::closeFeed() {
  if (feed)
    SvcEnum::release(feed);
  feed = NULL;
  last_feed = "";
}

// -----------------------------------------------------------------------
void DropDownList::textfeed_onReceiveText(const char *text) {
  setItems(text);  
}

// -----------------------------------------------------------------------
void DropDownList::setItems(const char *value) {
  deleteAllItems();
  ParamParser pp(value);
  for (int i=0;i<pp.getNumItems();i++) {
    addItem(pp.enumItem(i));
  }
  selectDefault();
}

// -----------------------------------------------------------------------
void DropDownList::setFeed(const char *feedid) {
  closeFeed();
  openFeed(feedid);
}

int __id = 0;


// -----------------------------------------------------------------------
// Script Object

DropDownListScriptController _dropDownListController;
DropDownListScriptController *dropDownListController = &_dropDownListController;

// -- Functions table -------------------------------------
function_descriptor_struct DropDownListScriptController::exportedFunction[] = {
  {"getItemSelected",            0, DropDownListScriptController::DropDownList_getItemSelected},
};
                                      
ScriptObject *DropDownListScriptController::instantiate() {
  DropDownList *ddl = new DropDownList;
  ASSERT(ddl != NULL);
  return ddl->getScriptObject();
}

void DropDownListScriptController::destroy(ScriptObject *o) {
  DropDownList *ddl= static_cast<DropDownList *>(o->vcpu_getInterface(dropDownListGuid));
  ASSERT(ddl != NULL);
  delete ddl;
}

void *DropDownListScriptController::encapsulate(ScriptObject *o) {
  return NULL; // no encapsulation for DropDownlist yet
}

void DropDownListScriptController::deencapsulate(void *o) {
}

int DropDownListScriptController::getNumFunctions() { 
  return sizeof(exportedFunction) / sizeof(function_descriptor_struct); 
}

const function_descriptor_struct *DropDownListScriptController::getExportedFunctions() { 
  return exportedFunction; 
}


scriptVar DropDownListScriptController::DropDownList_getItemSelected(SCRIPT_FUNCTION_PARAMS, ScriptObject *o) {
  SCRIPT_FUNCTION_INIT
  DropDownList *ddl = static_cast<DropDownList*>(o->vcpu_getInterface(dropDownListGuid));
  const char *p="";
  if (ddl) p = ddl->getSelectedText();
  return MAKE_SCRIPT_STRING(p);
}

