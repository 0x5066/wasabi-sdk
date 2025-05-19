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

#ifndef _STYLEPOSWND_H
#define _STYLEPOSWND_H

//!## Fairly identical to ContWnd except in the extension of how the
//!## position & size of child objects behave when resizing this object.

// I would have subclassed ContWnd, but there would be too much
// redundant, dead, code.

#include "../common/contwnd.h"
#include "../bfc/basewnd.h"
#include "../common/labelwnd.h"
#include "../bfc/tlist.h"

#define STYLEPOSWND_PARENT LabelWnd
/**
  
  
  @short Style Positioning Window.
  @author Nullsoft
  @ver 1.0
  @see ContWnd
*/
class COMEXP StylePosWnd : public STYLEPOSWND_PARENT {
public:
  /**
    Edge Positionning Constants.
    These different styles are used as parameters to the construction 
    of the ChildEdgeInfo object in the addChild method.
    
    @see ChildEdgeInfo::ChildEdgeInfo()
  */
  typedef enum {
    LOOSE,        // scalar value is extent along axis. only one opposing edge may be LOOSE.
    FIXED,        // scalar value is fixed point in client space
    LOCK_BORDER,  // scalar value is added to border (ie: right hand, use neg values)
    CENTER,       // scalar value is extent along axis.  both opposing edges must be CENTER to be valid.
    LOCK_LEFT,    // scalar value is added to the pos.left of the given BaseWnd object
    LOCK_TOP,     // scalar value is added to the pos.top of the given BaseWnd object
    LOCK_RIGHT,   // scalar value is added to the pos.right of the given BaseWnd object
    LOCK_BOTTOM,  // scalar value is added to the pos.bottom of the given BaseWnd object
    //
    NUM_CHILD_EDGE_STYLES
  } ChildEdgeStyle;

public:
  /**
    Describes how to position one edge of a rectangle.
  
    @short Single edge positionning for StylePosWnd.
    @author Nullsoft
    @ver 1.0
    @see StylePosWnd
  */
  class ChildEdgeInfo {
  public:
    /**
      This object holds the data to describe what to do with one edge of a rectangle.
      It is a simple construct-and-carry container object.

      The scalar parameter will have different meanings depending upon the ChildEdgeStyle
      bound to it.  The default, FIXED, acts the same as a normal object addition.  Some
      styles expect a BaseWnd pointer to get the position for this object-edge from the 
      an edge of the given BaseWnd obejct.

      Obviously, if you are referring to objects that are also going
      to be contained in the StylePosWnd, you _MUST_ add those objects to the StylePosWnd
      first.  Otherwise, your objects will perform unexpected tricks of positioning,
      right before your wondering eyes.
      
      @param theScalar The padding between this edge and the edge used to align against.
      @param theStyle  The style of edge positioning to use.
      @param theWho    The window against which to align the edge.
    */
    ChildEdgeInfo(int theScalar = 0, ChildEdgeStyle theStyle = FIXED, BaseWnd *theWho = NULL) :
      scalar(theScalar), style(theStyle), who(theWho) {}
  public:
    int scalar;
    ChildEdgeStyle style;
    BaseWnd *who;
  };

  /**
    Describes how to position all four edges of the bounding rectangle 
    for the given BaseWnd-derived object.
    
    @short Four edge positionning for StylePosWnd.
    @author Nullsoft
    @ver 1.0
    @see StylePosWnd
    @see ChildEdgeInfo
  */
  class ChildInfo {
  public:
    /**
      Four perpendicular edges make a rectangle.
      Four ChildEdgeInfo objects bound to a payload pointer make a ChildInfo object.
      Again, this is just a simple construct-and-carry object for the StylePosWnd object.
      
      @param theChild The child window to add.
      @param left The left edge ChildEdgeInfo.
      @param top  The top edge ChildEdgeInfo.
      @param right  The right edge ChildEdgeInfo.
      @param bottom The bottom edge ChildEdgeInfo.
      @param doinvalidate_on_resize 1, Invalidate on resize; 0, Do not invalidate on resize;
    */
    ChildInfo(BaseWnd *theChild, const ChildEdgeInfo &left, const ChildEdgeInfo &top,
      const ChildEdgeInfo &right, const ChildEdgeInfo &bottom, int doinvalidate_on_resize=1) {
      child = theChild;
      edges[LEFT] = left;
      edges[TOP] = top;
      edges[RIGHT] = right;
      edges[BOTTOM] = bottom;
      invalidate_on_resize = doinvalidate_on_resize;
      SetRect(&curpos,0,0,0,0);
    }
    
    /**
      Four perpendicular edges make a rectangle.
      Four ChildEdgeInfo objects bound to a payload pointer make a ChildInfo object.
      Again, this is just a simple construct-and-carry object for the StylePosWnd object.
      
      @param T The ChildInfo object to use.
    */
    ChildInfo(const ChildInfo& T) {
      child = T.child;
      edges[LEFT] = T.edges[LEFT];
      edges[TOP] = T.edges[TOP];
      edges[RIGHT] = T.edges[RIGHT];
      edges[BOTTOM] = T.edges[BOTTOM];
      invalidate_on_resize = T.invalidate_on_resize;
      SetRect(&curpos, T.curpos.left, T.curpos.top, T.curpos.right, T.curpos.bottom);
    }
  public:
    BaseWnd *child;
    ChildEdgeInfo edges[4];
    int invalidate_on_resize;
    RECT curpos;
  };

public:
  /**
    Sets the child position update flag to zero as default.
    This means that the children will not be repositioned when
    the StylePosWnd is constructed.
  */
  StylePosWnd();
  
  /**
    Make sure you remove everything before it deletes 
    itself if you don't want it to delete your child windows.
  */
  virtual ~StylePosWnd();

  /**
    Event is triggered when the button is about to be initialized.
    Override this event to implement your own behavior.
  
    @ret 1
  */
  virtual int onInit();
  
  /**
    Event is triggered when the window requires a repaint.
    Override this to implement your own behavior.
    
    @ret 0 for failure, 1 for success
    @param canvas The canvas on which to paint.
  */
  virtual int onPaint(Canvas *canvas);
  
  /**  
    Event is triggered when the button will be resized.
    Override this event to implement your own behavior.
    
    The default behavior is to cause a repaint.

    @ret 1
  */
  virtual int onResize();

  /**
    Add a child window.

    @ret 
    @param info
  */
  virtual int addChild(const ChildInfo &info);
  
  /**
    Deletes the desired child window.
    
    @ret 0, Failure; 1, Sucess;
    @param child A pointer to the child to remove.
  */
  int deleteChild(BaseWnd *child);
  
  /**
    Deletes all children.
    
    @ret 0, Failure; 1, Sucess;
  */
  int deleteAll();
  
  /**
    Removes the desired child window without deleting it.

    @ret 0, Failure; 1, Success;
    @param child A pointer to the child to remove.
  */
  int removeChild(BaseWnd *child);  // Will remove a child without deleting it
  
  /**
    Remove all children without deleting them.
    
    @ret 0, Failure; 1, Success;
  */
  int removeAll();                  // Will remove all without deleting them.

protected:
  /**  
    This properly repositions and resizes the child items based
    upon the information specified upon addition.
  */
  virtual void updatePositions();

  ChildInfo *findChild(BaseWnd *child);
  inline int numChildren() { return childinfo.getNumItems(); }
  inline ChildInfo *enumChildren(int child) { return childinfo[child]; }

protected:
  friend ChildInfo;
  friend ChildEdgeInfo;
  typedef enum {
    LEFT = 0,
    TOP = 1,
    RIGHT = 2,
    BOTTOM = 3,
  } Indices;

  PtrList<ChildInfo> childinfo;
  int updatechildpos;
};


#endif//_STYLEPOSWND_H

