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


#include "objectdir.h"

ObjectDir::ObjectDir(const char *_config_prefix, const char *_dirname)
    : config_prefix(_config_prefix), dir_name(_dirname)
{
  id_gen = INVALID_OBJECT_HANDLE+1;
  api->syscb_registerCallback(this);
}

ObjectDir::~ObjectDir() {
  objlist.deleteAll();
  api->syscb_deregisterCallback(this);
}

int ObjectDir::getNumObjects() {
  return objlist.getNumItems();
}

ObjectHandle ObjectDir::enumObject(int n) {
  ObjRec *ret = objlist[n];
  if (ret == NULL) return INVALID_OBJECT_HANDLE;
  return ret->handle;
}

void *ObjectDir::getObject(ObjectHandle handle) {
  ObjRec *or = handleToSlot(handle);
  if (or == NULL) return NULL;
  return or->getObj();
}

const char *ObjectDir::getObjectLabel(ObjectHandle handle) {
  ObjRec *or = handleToSlot(handle);
  if (or == NULL) return NULL;
  return or->label;
}

int ObjectDir::setObjectLabel(ObjectHandle handle, const char *newlabel) {
  ObjRec *or = handleToSlot(handle);
  if (or == NULL) return 0;
  or->label = newlabel;
  return 1;
}

ObjectHandle ObjectDir::insertObject(const char *filename, const char *label, const char *path) {
  ObjRec *rec = createObjRec(filename);
  rec->handle = id_gen++;
  if (label == NULL)
    rec->label = Std::filename(filename);
  else
    rec->label = label;
  rec->path = path;
  objlist.addItem(rec);
  return rec->handle;
}

int ObjectDir::removeObject(ObjectHandle handle) {
  ObjRec *or = handleToSlot(handle);
  if (or == NULL) return 0;
  objlist.removeItem(or);
  delete or;
  return 1;
}

void ObjectDir::clearAll() {
  objlist.deleteAll();
}

#if 0
void ObjectDir::runlevelcb_onAppRunning() {
  int npl = api->getIntPrivate(StringPrintf("%s/nobjects", config_prefix), 0);
  for (int i = 0; i < npl; i++) {
    char filename[WA_MAX_PATH]="";
    StringPrintf tag("%s/pl_%d", config_prefix, i);
    api->getStringPrivate(tag, filename, sizeof filename, "");
    char label[WA_MAX_PATH]="";
    tag.printf("%s/pl_%d_label", config_prefix, i);
    api->getStringPrivate(tag, label, sizeof label, "");
    insertObject(filename, label);
  }
}

void ObjectDir::runlevelcb_onShutdown() {
  StringPrintf npls("%s/objects", config_prefix);
  int prev_npl = api->getIntPrivate(StringPrintf("%s/objects", config_prefix), 0);
  foreach(objlist)
    objlist.getfor()->shutdown();
    StringPrintf tag("%s/obj_%d", config_prefix, foreach_index);
    api->setStringPrivate(tag, objlist.getfor()->filename);
    tag.printf("%s/obj_%d_label", config_prefix, foreach_index);
    api->setStringPrivate(tag, objlist.getfor()->label);
  endfor
  api->setIntPrivate(npls, objlist.getNumItems()+1);
}
#endif

ObjectDir::ObjRec *ObjectDir::handleToSlot(ObjectHandle handle) {
  foreach(objlist)
    if (objlist.getfor()->handle == handle) return objlist.getfor();
  endfor
  return NULL;
}
