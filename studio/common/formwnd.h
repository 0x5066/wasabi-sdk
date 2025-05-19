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

#ifndef _FORMWND_H
#define _FORMWND_H

#include "common.h"
#include "../common/labelwnd.h"
#include "../bfc/ptrlist.h"

class FormWnd;

/**
  This class represents a content cell for a row in a
  FormWnd.
  
  @short FormWnd cell to display content.
  @author Nullsoft
  @ver 1.0
  @see FormWnd
  @see CellRow
*/
class COMEXP FormCell {
protected:
  /**
    You are required to subclass this object to use it.
  */
  FormCell();
public:

  /**
    If the content is a BaseWnd, it is deleted.
  */
  virtual ~FormCell();

  /**
    Set the width of our cell to a fixed 
    size, in pixels.
    
    @param w The cell width (in pixels).
  */
  void setWidthFixed(int w);
    /**
    Set the height of our cell to a fixed 
    size, in pixels.
    
    @param w The cell height (in pixels).
  */
  void setHeightFixed(int h);

  /**
    Set the width of our cell to a proportionate
    value of the total width of the FormWnd (in 
    percentile).
    
    @param w The cell's proportional width, in percentile (from 0 to 100).
  */
  void setWidthProp(double w);

  /**
    Set the height of our cell to a proportionate
    value of the total width of the FormWnd (in 
    percentile).
    
    @param w The cell's proportional height, in percentile (from 0 to 100).
  */
  void setHeightProp(double h);

  /**
    This is the payload object to display in the cell area.  This is the only
    public method available to set the content of the cell area.
    
    @param rootwnd A pointer to the RootWnd to display in the cell area.
  */
  void setRootWndContent(RootWnd *rootwnd);

  // mig: made this public so you can actually alter the form
  // cells without having to delete the _entire_ formwnd.
  virtual void init(FormWnd *parent);

protected:
  friend class FormWnd;

  /**
    This event is triggered when the FormWnd is recalculating.
    Override this to implement your own behavior.
    
    Default behavior is to resize to the requested width and height
    and reposition to the requested x and y coordinates.
    
    @param x The requested X coordinate.
    @param y The requetes Y coordinate.
    @param w The requested cell width.
    @param h The the requested cell height.
  */
  void resize(int x, int y, int w, int h);

  int fixedw, fixedh;		// if not < 0, try for these fixed dimensions
  double propw, proph;		// if not < 0, ditto

  // otherwise split up what's left

  int renderw, renderh;	// final resize dimensions

  /**
    This is the payload object to display in the cell area.
    
    It's a protected method, you can't call it.  This is for
    if you subclass your own cell types.  See cells.h.
    
    @param basewnd A pointer to the BaseWnd to display in the cell area.
  */
  void setContent(BaseWnd *basewnd);

  RootWnd *getRootWnd();

private:

  FormWnd *parentWnd;
  RootWnd *rootwnd;
  BaseWnd *basewnd;
};

/**
  Represents a FormWnd row of cells.
  
  @short FormWnd row of cells.
  @author Nullsoft
  @ver 1.0
  @see FormWnd
  @see FormCell
*/
class CellRow {
public:
  /**
    Sets the default height to 0.
  */
  CellRow() { renderh = 0; }
  
  /**
    Deletes all the cells it contains.
  */
  ~CellRow() { cells.deleteAll(); }

  int renderh;
  PtrList<FormCell> cells;
};

#define FORMWND_PARENT LabelWnd
/**
  Grid / HTML table-like functionality.
  
  @short FormWnd style control (HTML tables style functionality).
  @author Nullsoft
  @ver 1.0
  @see LabelWnd
*/
class COMEXP FormWnd : public FORMWND_PARENT {
public:
  /**
    Initializes the cell count to 0.
  */
  FormWnd();
  
  /**
    Deletes all the cells.
  */
  virtual ~FormWnd();

  /**
    Event is triggered when the button is about to be initialized.
    Override this event to implement your own behavior.
  
    Defaults to a FormWnd with 0 cells, and a width and height of 0.
  
    @ret 1
  */
  virtual int onInit();
  
  /**
    Event is triggered when the FormWnd is resized.
    Override this event to implement your own behavior.
    
    The default behavior is to cause a repaint and recalculate
    the cell sizes.

    @ret 1
  */
  virtual int onResize();
  
  /**
    Pass a new cell into the form.  The cell understands its own sizing.
    
    @ret A pointer to the new cell just added.
    @param cell A pointer to the new cell to add.
  */
  FormCell *newCell(FormCell *cell=NULL);
  
  /** 
    Start a new row in the FormWnd. Call this after
    you're done adding cells to the previous row.
  */
  void nextRow();

  /** 
    Deletes all cells from the FormWnd.
  */
  void deleteAll();

  /** 
    Force a recalculation of the cell and form sizes.
  */
  void recalcForm();

private:
  PtrList<CellRow> rows;
  int currow;
  void resetCellRenderDimensions();
};

#endif
