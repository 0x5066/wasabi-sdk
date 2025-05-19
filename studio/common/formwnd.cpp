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


#include "formwnd.h"

#include "../bfc/wnds/blankwnd.h"

FormCell::FormCell() {
  fixedw = fixedh = -1;
  propw = proph = -1;
  renderw = renderh = 0;
  parentWnd = NULL;
  rootwnd = NULL;	// our content
  basewnd = NULL; // our content that we can delete
}

FormCell::~FormCell() {
  if (basewnd) delete basewnd;
}

void FormCell::setContent(BaseWnd *newbasewnd) {
  if (basewnd != NULL) delete basewnd;
  basewnd = newbasewnd;
  rootwnd = basewnd;
}

void FormCell::setRootWndContent(RootWnd *newrootwnd) {
  rootwnd = newrootwnd;
  basewnd = NULL;
}

RootWnd *FormCell::getRootWnd() {
  return rootwnd;
}

void FormCell::setWidthFixed(int w) {
  fixedw = w;
}

void FormCell::setHeightFixed(int h) {
  fixedh = h;
}

void FormCell::setWidthProp(double w) {
  ASSERT(w >= 0.0);
  ASSERT(w <= 100.0);
  propw = w;
}

void FormCell::setHeightProp(double h) {
  ASSERT(h >= 0.0);
  ASSERT(h <= 100.0);
  proph = h;
}

void FormCell::init(FormWnd *parent) {
  ASSERTPR(rootwnd != NULL, "you can't put in empty cells, use BlankWnd, or maybe you forgot to call setContent in your derived FormCell");
  parentWnd = parent;
  ASSERT(parentWnd != NULL);
  if (!rootwnd->init(parent)) {
    delete rootwnd;
    rootwnd = NULL;
  }
  if (rootwnd != NULL) rootwnd->setParent(parentWnd);
}

void FormCell::resize(int x, int y, int w, int h) {
  if (rootwnd == NULL) return;
  rootwnd->resize(x, y, w, h);
}

FormWnd::FormWnd() {
  currow = 0;
}

FormWnd::~FormWnd() {
  deleteAll();
}

int FormWnd::onInit() {
  FORMWND_PARENT::onInit();

  for (int i = 0; i < rows.getNumItems(); i++) {
    CellRow *row = rows[i];
    if (row == NULL) continue;	// empty row
    for (int j = 0; j < row->cells.getNumItems(); j++) {
      FormCell *cell = row->cells[j];
      ASSERTPR(cell != NULL, "Null cell is not ok, use a blank one");
      if (cell == NULL) continue;
      cell->init(this);
    }
  }
  return 1;
}

int FormWnd::onResize() {
  FORMWND_PARENT::onResize();
  recalcForm();
  return 1;
}

FormCell *FormWnd::newCell(FormCell *cell) {
  if (cell == NULL) {
    cell = new FormCell();
    cell->setContent(new BlankWnd());
  }

  CellRow *row = rows[currow];
  if (row == NULL) row = rows.addItem(new CellRow());
  
  row->cells.addItem(cell);

  // allow for adding cells after init
  if (isInited()) {
    cell->init(this);
//    onResize();
  }

  return cell;
}

void FormWnd::nextRow() {
  currow++;
  if (rows[currow] == NULL) rows.addItem(new CellRow());
}

void FormWnd::deleteAll() {
  invalidate();
  rows.deleteAll();
  currow = 0;
}

void FormWnd::recalcForm() {
  if (!isInited()) return;

  int widest = 0;
  int i, j;

  RECT r;
  getClientRect(&r);
  int w = r.right - r.left;
  int h = r.bottom - r.top;

  if (w <= 0 || h <= 0) return;

  // find our total cell width
  for (i = 0; i < rows.getNumItems(); i++) {
    int tw = rows[i]->cells.getNumItems();
    if (tw > widest) widest = tw;
  }

  // reset all cells' render dimensions to -1
  resetCellRenderDimensions();

  // first pass: allocate all widths per cell on a per-row basis
  int nset = 0;

  for (i = 0; i < rows.getNumItems(); i++) {
    CellRow *row = rows[i];
    if (row == NULL) continue;

    // give out fixed or proportional widths
    int aw = w;
    int nitems = row->cells.getNumItems();
    for (j = 0; j < nitems; j++) {
      FormCell *cell = row->cells[j];
      ASSERTPR(cell != NULL, "Null cell is not ok, use a blank one");

      if (cell->fixedw >= 0) {		// fixed width
        aw -= cell->fixedw;
        cell->renderw = cell->fixedw;
        nset++;
      } else if (cell->propw >= 0) {	// proportionate width
        int cw = (int)(((double)w * cell->propw) / 100.0);
        aw -= cw;
        cell->renderw = cw;
        nset++;
      }
    }

    // now evenly give out what's left
    if (nset != nitems) {
      int leftw = aw / (nitems - nset);
      for (j = 0; j < nitems; j++) {
        FormCell *cell = row->cells[j];
        if (cell == NULL) continue;
        if (cell->renderw < 0) cell->renderw = leftw;
      }
    }

  }

  // for each column
  for (i = 0; i < widest; i++) {
    // find the widest cell
    int wid = 0;
    for (j = 0; j < rows.getNumItems(); j++) {
      CellRow *row = rows[j];
      FormCell *cell = row->cells[i];
      if (cell == NULL) continue;
      if (cell->renderw > wid) wid = cell->renderw;
    }
    // now, force each cell in the column to match the widest
    for (j = 0; j < rows.getNumItems(); j++) {
      CellRow *row = rows[j];
      FormCell *cell = row->cells[i];
      if (cell == NULL) continue;
      cell->renderw = wid;
    }
  }


  // find the tallest element in each row to get row height per row
  nset = 0;
  int ah = h;
  for (i = 0; i < rows.getNumItems(); i++) {
    CellRow *row = rows[i];
    if (row == NULL) continue;
    int height = -1;
    for (j = 0; j < row->cells.getNumItems(); j++) {
      FormCell *cell = row->cells[j];
      if (cell->fixedh >= 0) {
        cell->renderh = cell->fixedh;
      } else if (cell->proph >= 0) {
        int ch = (int)(((double)h * cell->proph) / 100.0);
        cell->renderh = ch;
      }
      if (height < cell->renderh) height = cell->renderh;
    }
    row->renderh = height;
    if (height >= 0) {
      ah -= height;
      nset++;
    }
  }

  // give out all leftover height to unspecified rows
  if (nset != rows.getNumItems()) {
    int lefth = ah / (rows.getNumItems() - nset);
    for (i = 0; i < rows.getNumItems(); i++) {
      CellRow *row = rows[i];
      if (row == NULL) continue;
      if (row->renderh < 0) row->renderh = lefth;
    }
  }

  for (i = 0; i < rows.getNumItems(); i++) {
    CellRow *row = rows[i];
    if (row == NULL) continue;

    // force all non-set cells to match tallest element
    for (j = 0; j < row->cells.getNumItems(); j++) {
      FormCell *cell = row->cells[j];
      if (cell->renderh <= 0) cell->renderh = row->renderh;
    }
  }

  // apply the render widths and heights to the cells
  int y = r.top;
  for (i = 0; i < rows.getNumItems(); i++) {
    CellRow *row = rows[i];
    if (row == NULL) continue;
    int nitems = row->cells.getNumItems();
    int x = r.left;
    for (j = 0; j < nitems; j++) {
      FormCell *cell = row->cells[j];
      ASSERT(cell != NULL);
      cell->resize(x, y, cell->renderw, cell->renderh);
      x += cell->renderw;
    }

    y += row->renderh;
  }
}

void FormWnd::resetCellRenderDimensions() {
  for (int i = 0; i < rows.getNumItems(); i++) {
    CellRow *row = rows[i];
    if (row == NULL) continue;
    row->renderh = -1;
    for (int j = 0; j < row->cells.getNumItems(); j++) {
      FormCell *cell = row->cells[j];
      if (cell == NULL) continue;
      cell->renderw = cell->renderh = -1;
    }
  }
}
