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

//PORTABLE
#ifndef _ITEMLIST_H
#define _ITEMLIST_H

#include "listwnd.h"
#include "../canvas.h"
#include "../named.h"
#include "../ptrlist.h"
#include "../string.h"
#include "../../studio/metacb.h"

class FilenameNC;
class DragItemI;
class ContextMenu;

// this class just handles rendering the various properties of playitems
// in a listwnd... the rest is up to you, just override the convert fn

// abstract base class to render something in a column for a playstring

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP NOVTABLE ItemListColumn : public ListColumn {
protected:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ItemListColumn(const char *name=NULL) : ListColumn(name) {}
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ItemListColumn() {}

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void render(int pos, const char *playstring, Canvas &c, RECT &r)=0;
};

#define ITEMLISTWND_PARENT ListWnd

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ItemListWnd : public ListWnd, private MetaCallbackI {
friend class ItemListColumn_Callback;
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ItemListWnd();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ItemListWnd();

  
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
  int insertColumn(ItemListColumn *column, int width, int pos=-1);

protected:
  // override and return 0 to suppress auto-dragging from window
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantAutoDrag() { return 1; }
  // handles auto-adding all selected rows and calls addDragTypes
  // so you can add more via addDragItem()
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onBeginDrag(int);
  // if you return 0, the Filename version will be auto-added, otherwise not
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int addMoreDragTypes(int pos) { return 0; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int dragComplete(int success);

  // tell ListWnd we do our own drawing
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int ownerDraw(Canvas *canvas, int pos, RECT *r, LPARAM lParam, int isselected, int isfocused);

  // ItemListColumn_Callback calls this to do its rendering, lParam is what you
  // gave it to pass back to you
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void userRender(int pos, const char *playstring, Canvas &c, RECT &r, LPARAM lParam) {}

  // override this to turn the ownerdraw into a playstring
  virtual const char *convertlParam(LPARAM lParam)=0;

  // override this and return 1 if you want a "current" box around item
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getSelected(LPARAM lParam) { return 0; }

//  virtual int onRightClick(int itemnum, int x, int y);
  // automatically generated context menu (uses Filename)
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onContextMenu(int x, int y);

  // return optional DragItemI for context menu (will be deleted for you)
  virtual DragItemI *itemlistwnd_getDragItem(int x, int y) { return NULL; }
  virtual DragItemI *itemlistwnd_getSecondDragItem(int n) { return NULL; }
  virtual void itemlistwnd_addCustomContextMenuCommands(ContextMenu *cm) { }
  virtual void itemlistwnd_contextMenuResult(int res) { }

  // return TRUE if it's ok to edit in place
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int allowEdition(const char *playstring, char *field) { return 0; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void resort() { 
    //TODO> implement me!
  }

protected:
  // implement this if you want to know when an item's metadata changed
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onItemChange(int pos, const char *playstring) { }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onItemDel(int pos, const char *playstring) { }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void metacb_onItemChange(const char *playstring, const char *tag);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void metacb_onItemDel(const char *);

private:
  PtrList<FilenameNC> *keep;
};

// column class to ask ItemListWnd to do the rendering

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ItemListColumn_Callback : public ItemListColumn {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ItemListColumn_Callback(ItemListWnd *_parent, LPARAM _lparam, const char *name=NULL);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void render(int pos, const char *playstring, Canvas &c, RECT &r);

private:
  ItemListWnd *parent;
  LPARAM lparam;
};

// column class to render a metatag

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ItemListColumn_MetaTag : public ItemListColumn {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ItemListColumn_MetaTag(const char *tag, int center=0, const char *label=NULL);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ItemListColumn_MetaTag() {}

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void render(int pos, const char *playstring, Canvas &c, RECT &r);

private:
  String tag;
  int center;
  int datatype;
};

// this just renders the position of the item, starting from 1

/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
 */
class COMEXP ItemListColumn_Numbered : public ItemListColumn {
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ItemListColumn_Numbered(int _offset=0) : offset(_offset) {}
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void render(int pos, const char *playstring, Canvas &c, RECT &r);

private:
  int offset;
};

#endif
