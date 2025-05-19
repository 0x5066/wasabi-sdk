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

#ifndef _LISTWND_H
#define _LISTWND_H

#include "scbkgwnd.h"
#include "../../common/common.h"

#include "../string.h"
#include "../freelist.h"

#define POS_LAST -1

#define LISTWND_PARENT ScrlBkgWnd

#define LW_HT_DONTKNOW	(-1)
#define LW_HT_ABOVE	(-10)
#define LW_HT_BELOW	(-20)

class listItem;
class ListWnd;
class CompareListItem;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ListColumn : public Named {
friend ListWnd;
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ListColumn(const char *name=NULL, int isdynamic=FALSE);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getWidth();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setWidth(int newwidth);
  const char *getLabel();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setLabel(const char *newlabel);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int customDrawHeader(Canvas *c, RECT *cr);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getNumeric() { return numeric; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setDynamic(int isdynamic);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int isDynamic() { return dynamic; }

protected:  
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setIndex(int i);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getIndex();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setList(ListWnd *list);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ListWnd *getList();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setNumeric(int n) { numeric=n; }

private:
  int width;
  int index;
  int numeric;
  int dynamic;
  ListWnd *list;
};

class SelItemList;


/**
  Class 

  @short 
  @author Nullsoft
  @ver 1.0
  @see 
*/
class COMEXP ListWnd : public ScrlBkgWnd {
friend ListColumn;
friend SelItemList;
public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ListWnd();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual ~ListWnd();

  
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
  virtual int onPostOnInit();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onPaint(Canvas *canvas);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onResize();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onLeftButtonDown(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onLeftButtonUp(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onRightButtonDown(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onRightButtonUp(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onMouseMove(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onLeftButtonDblClk(int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onChar(unsigned int c);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onKeyDown(int keyCode);
  virtual int onContextMenu (int x, int y);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantAutoContextMenu();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onMouseWheelUp(int click, int lines);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onMouseWheelDown(int click, int lines);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantAutoDeselect() { return wantautodeselect; }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void setWantAutoDeselect(int want) { wantautodeselect = want; }
  
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void onSetVisible(int show);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setAutoSort(BOOL dosort);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setOwnerDraw(BOOL doownerdraw);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void timerCallback(int id);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void next(int wantcb=1);
  void selectCurrent();
  void selectFirstEntry(int wantcb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void previous(int wantcb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void pagedown(int wantcb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void pageup(int wantcb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void home(int wantcb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void end(int wantcb=1);
  
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setItemCount(int c);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void reset();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int addColumn(const char *name, int width, int numeric=0);	// adds to end
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ListColumn *getColumn(int n);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getNumColumns();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getColumnWidth(int col);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  BOOL setRedraw(BOOL redraw);	// returns prev state
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  BOOL getRedraw();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setMinimumSize(int size);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int addItem(const char *label, LPARAM lParam);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int insertItem(int pos, const char *label, LPARAM lParam);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getLastAddedItemPos();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void setSubItem(int pos, int subpos, const char *txt);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void deleteAllItems();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int deleteByPos(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getNumItems(void);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getItemLabel(int pos, int subpos, char *text, int textmax);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void setItemLabel(int pos, const char *text);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual LPARAM getItemData(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getItemRect(int pos, RECT *r);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getItemSelected(int pos);	// returns 1 if selected
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getItemFocused(int pos);	// returns 1 if focused
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getItemFocused();         // returns focused item
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setItemFocused(int pos, int ensure_visible=TRUE);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void ensureItemVisible(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void invalidateColumns();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int scrollAbsolute(int x);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int scrollRelative(int x);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void scrollLeft(int lines=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void scrollRight(int lines=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void scrollUp(int lines=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void scrollDown(int lines=1);
  virtual const char *getSubitemText(int pos, int subpos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getFirstItemSelected();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getNextItemSelected(int lastpos);	// next item AFTER given pos

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int selectAll(int cb=1);	// force all items selected
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int deselectAll(int cb=1); // force all items to be deselected
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int invertSelection(int cb=1);	// invert all selections

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int hitTest(POINT pos, int drag=0);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int hitTest(int x, int y, int drag=0);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int invalidateItem(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int locateData(LPARAM data);

  // -1 if we've never been drawn yet
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getFirstItemVisible() const { return firstItemVisible; }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getLastItemVisible() const { return lastItemVisible; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int setFontSize(int size);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getFontSize();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void jumpToNext(char c);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int wantFocus() { return 1; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void scrollToItem(int pos);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void resort();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getSortDirection();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getSortColumn();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setSortColumn(int col);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setSortDirection(int dir);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int findItemByParam(LPARAM param);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setItemParam(int pos, LPARAM param);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getItemCount() { return getNumItems(); }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setSelectionStart(int pos, int wantcb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void setSelectionEnd(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void setSelected(int pos, int selected, int cb=1);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void toggleSelection(int pos, int setfocus=TRUE, int cb=1);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getHeaderHeight();

  // this sort function just provides string/numeric comparison
  // if you need more types, just override and provide your own
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int sortCompareItem(listItem *p1, listItem *p2);


/**
  Method

  @see 
  @ret 
  @param 
*/
	int getPreventMultipleSelection() {	return preventMultipleSelection; }

/**
  Method

  @see 
  @ret 
  @param 
*/
	int setPreventMultipleSelection(int val) {	return preventMultipleSelection = val; }

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void moveItem(int from, int to);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onAcceleratorEvent(const char *name);

protected:
  // return 1 if you override this
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int ownerDraw(Canvas *canvas, int pos, RECT *r, LPARAM lParam, int selected, int focused) { return 0; };

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onPreItemDraw(Canvas *canvas, int pos, RECT *r, LPARAM lParam, int selected, int focused) { }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onPostItemDraw(Canvas *canvas, int pos, RECT *r, LPARAM lParam, int selected, int focused) { };
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getTextColor(LPARAM lParam);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getSelBgColor(LPARAM lParam);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getSelFgColor(LPARAM lParam);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getBgColor();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getFocusColor(LPARAM lParam);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual COLORREF getColumnSepColor();

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int wantColSepOnItems();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getXShift();

public:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int insertColumn(ListColumn *col, int pos=-1);// -1 is add to end

  void setHoverSelect(int a) { hoverselect = a; }
  int getHoverSelect() { return hoverselect; }

  void setSelectOnUpDown(int i) { selectonupdown = i; }
  int getSelectOnUpDown() { return selectonupdown; }
  virtual int onAction(const char *action, const char *param=NULL, int x=-1, int y=-1, int p1=0, int p2=0, void *data=NULL, int datalen=0, RootWnd *source=NULL);

protected:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getColumnsHeight();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getColumnsWidth();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getContentsWidth();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int getContentsHeight();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void drawBackground(Canvas *canvas);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void drawColumnHeaders(Canvas *c);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void drawItems(Canvas *canvas);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void updateScrollX();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void updateScrollY();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int doJumpToNext(char c, BOOL fromTop);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int fullyVisible(int pos);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onBeginDrag(int iItem);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int dragOver(int x, int y, RootWnd *sourceWnd);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onSelectAll();	// hit Control-A
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onDelete();		// hit 'delete'
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onItemDelete(LPARAM lparam) {}
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onDoubleClick(int itemnum);	// double-click on an item
  // this is called with the selected item#
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onLeftClick(int itemnum);	// left-click
  // the second time you click on an already-focused item
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onSecondLeftClick(int itemnum);
  // this is called once for the item under cursor on click
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onRightClick(int itemnum);	// right-click on item

  // override this to be notified of item selections & deselections
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual void onItemSelection(int itemnum, int selected) {}

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onColumnDblClick(int col, int x, int y) { return 0; }
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  virtual int onColumnLabelClick(int col, int x, int y);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void selectRect(int x1, int y1, int x2, int y2);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void drawRect(int x1, int y1, int x2, int y2);

  // interface to Freelist
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  listItem *createListItem();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void deleteListItem(listItem *item);

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  ListColumn *enumListColumn(int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int getColumnPosByName(const char *name);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int delColumnByPos(int pos);

protected:
  int item_invalidate_border;
  BOOL showColumnsHeaders;

  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void recalcHeaders();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void itemSelection(int itemnum, int selected);

private:
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int doAddItem(const char *label, LPARAM lParam, int pos);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int hitTestColumns(POINT p, int *origin=NULL);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  int hitTestColumnsLabel(POINT p);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void drawXorLine(int x);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void calcNewColWidth(int col, int x);
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void calcBounds();
  
  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void onDragTimer();

  /**
    Method
  
    @see 
    @ret 
    @param 
  */
  void notifySelChanged(int item=-1, int sel=-1);

  int autosort, ownerdraw;
  int textsize;
	int itemHeight;
	BOOL metrics_ok;
	BOOL redraw;
	int columnsHeight;
  int dragtimeron;

	PtrList<ListColumn> columnsList;
	PtrListQuickSorted<listItem,CompareListItem> itemList;

	int firstItemVisible;
	int lastItemVisible;

	listItem *lastItemFocused;
	int lastItemFocusedPos;

	listItem *lastAddedItem;
  SelItemList *selItemList;

  int dragskip;
  int dragskipcount;
	int selectionStart;
	int colresize;
	POINT colresizept;
	BOOL resizing_col;
	int colresizeo;

	BOOL processbup;
	BOOL bdown;
	BOOL nodrag;
  int bdownx, bdowny;
	BOOL firstComplete, lastComplete;

	int rectselecting;
	POINT selectStart;
	POINT selectLast;

  int sortdir, sortcol, lastcolsort;

	int preventMultipleSelection;
	
  Freelist<listItem> listItem_freelist;
  int wantautodeselect;

  int hoverselect;
  int selectonupdown;
  PtrList<RootWnd> tempselectnotifies;
};

#endif
