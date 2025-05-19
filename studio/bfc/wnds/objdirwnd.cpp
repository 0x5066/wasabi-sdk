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


#include "objdirwnd.h"

#include "../../common/contextmenu.h"
#include "../../common/dragitemi.h"

ObjDirWnd::ObjDirWnd() :
   objdir(NULL) { }

ObjDirWnd::~ObjDirWnd() {
  api->service_release(objdir);
}

void ObjDirWnd::setTargetDirName(const char *dirname) {
  objectDirName = dirname;
//FUCKO: reset here
}

void ObjDirWnd::setActionTarget(const char *targetname) {
  objectDirTarget = targetname;
}

int ObjDirWnd::onInit() {
  OBJDIRWND_PARENT::onInit();

  ASSERT(!objectDirName.isempty());
  objdir = ObjectDirEnum(objectDirName).getNext();

  if (objdir != NULL) {
    setHilitedColor("pledit.currentOutline");//FUCKO from service
    viewer_addViewItem(objdir);

    int nobj = objdir->getNumObjects();
    for (int i = 0; i < nobj; i++) {
      ObjectHandle hand = objdir->enumObject(i);
      addTreeItem(createObjItem(hand), NULL);
    }
  }

  return 1;
}

int ObjDirWnd::onContextMenu(int x, int y) {
  if (objdir != NULL) ContextMenu(this, &DragItemT<svc_objectDir>(objdir));
  return 1;
}

int ObjDirWnd::viewer_onEvent(svc_objectDir *item, int event, int param2, void *ptr, int ptrlen) {
  switch (event) {
    case svc_objectDir::Event_OBJECT_ADDED: {
      int sel_after = (enumAllItems(0) == NULL);
      TreeItem *ti = addTreeItem(createObjItem(param2), NULL);
      if (sel_after) selectItemDeferred(ti);
    }
    break;
    case svc_objectDir::Event_OBJECT_REMOVED: {
      ObjItem *pli = getObjItemForHandle(param2);
      delItemDeferred(pli);
    }
    break;
    case svc_objectDir::Event_OBJECT_LABELCHANGE: {
      ObjItem *pli = getObjItemForHandle(param2);
      const char *name = objdir->getObjectLabel(pli->getObjectHandle());
      pli->setLabel(name);
    }
    break;
  }
  return 1;
}

ObjItem *ObjDirWnd::createObjItem(ObjectHandle handle) {
  return new ObjItem(handle, objdir->getObjectLabel(handle));
}

ObjItem *ObjDirWnd::getObjItemForHandle(ObjectHandle handle) {
  TreeItem *it;
  for (int i = 0; ; i++) {
    it = enumAllItems(i);
    if (it == NULL) break;
    ObjItem *plit = static_cast<ObjItem *>(it);
    if (plit->getObjectHandle() == handle) return plit;
  }
  return NULL;
}

void ObjDirWnd::onItemSelected(TreeItem *item) {
  ObjItem *objitem = static_cast<ObjItem *>(item);
  objdir->onAction(svc_objectDir::ODACTION_SELECTED, this, objectDirTarget, objitem->getObjectHandle());
}

void ObjDirWnd::onItemDeselected(TreeItem *item) {
  ObjItem *objitem = static_cast<ObjItem *>(item);
  objdir->onAction(svc_objectDir::ODACTION_DESELECTED, this, objectDirTarget, objitem->getObjectHandle());
}
