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

#ifndef _OBJDIRWND_H
#define _OBJDIRWND_H

#include "treewnd.h"
#include "../../studio/services/svc_objectDir.h"
#include "../depview.h"


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class ObjItem : public TreeItem {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ObjItem(ObjectHandle _handle, const char *label=NULL) : handle(_handle), TreeItem(label) {}
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ObjectHandle getObjectHandle() const { return handle; }

private:
  ObjectHandle handle;
};

#define OBJDIRWND_PARENT TreeWnd

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class ObjDirWnd : public OBJDIRWND_PARENT, DependentViewerTPtr<svc_objectDir> {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ObjDirWnd();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ObjDirWnd();

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setTargetDirName(const char *dirname);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setActionTarget(const char *targetname);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onInit();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onContextMenu(int x, int y);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int viewer_onEvent(svc_objectDir *item, int event, int param2, void *ptr, int ptrlen);

protected:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ObjItem *createObjItem(ObjectHandle handle);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ObjItem *getObjItemForHandle(ObjectHandle handle);

private:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onItemSelected(TreeItem *item);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onItemDeselected(TreeItem *item);

  svc_objectDir *objdir;
  String objectDirName, objectDirTarget;
};

template <class TREEITEMCLASS>

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class ObjDirWndT : public ObjDirWnd {
public:
  virtual ObjItem *createObjItem(ObjectHandle handle) { return new TREEITEMCLASS(handle); }
};

#endif
