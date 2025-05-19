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

// ===========================================================================
//
//    NULLSOFT WASABI SDK EXAMPLE PROJECTS
//
//      File:     examplecanvaswnd.cpp
//
//!##   Purpose:  Setup the window class for our component and have some fun
//!##             with drawing primitives.
//
//      Notes:    A note on the comments in this document:
//
//                Notes that begin with *** are important notes that everyone
//                needs to read.  The other comments assist readability or
//                explain the thinking behind sections of code which may not
//                be immediately obvious to the novice programmer.
//
//                Or I'm just typing to hear myself clickyclack.  Deal.
//
//                Because ExampleCanvas is the first of a string of example
//                projects, there are more useless comments in this
//                set of modules than any others.
//

#include "../../bfc/std.h"

#include "examplecanvas.h"
#include "examplecanvaswnd.h"
#include "resource.h"

#include "../../bfc/canvas.h"
#include "../../common/xlatstr.h"

// =========================================================================
//
//  EXAMPLECANVAS: Methods required by Window Creation Services
//
//  These are simple little static methods that are required by the WCS to exist
//  on your window class such that your window class may be identified by 
//  its name, its GUID, have a thinger icon assigned, etc etc.
//
const char *ExampleCanvasWnd::getWindowTypeName() { return /**/"ExampleCanvas Window"/*EDITME*/; }
GUID ExampleCanvasWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExampleCanvasWnd::setIconBitmaps(ButtonWnd *button) {
  // Note that IDB_TAB_XXXXXX defines come from resource.h and the data is in resource.rc
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
  // If you make different IDs for the resources, you'll have to edit this also.

  // Microsoft doesn't like people editing their .rc files by hand, but, essentially
  // all you want to do is open up my resource.rc file and change the .png filenames
  // to whatever you want your .png filenames to be and you're done.
}

// Here's our static data member's declaration and initialization.
ExampleCanvasWnd::BlitStyle ExampleCanvasWnd::staticStyle = ExampleCanvasWnd::EXCITING;


// ===========================================================================
//
//    EXAMPLECANVAS: Timers & Timer Callbacks.
//
//
//  *** NOW, in order to make sure we blit to the screen properly,
//      since we do all of our draw methods during the onPaint handler,
//      we're going to setup a timer for our window.  Pay attention,
//      here -- so your windows can be properly timed as well.
//

//
//  Timer constants.
//
//    We'll need to define 2 constants to assist in creating a
//    useful timer callback:
//
//    The first constant is the ID value of the timer we wish to use.
//    I'm sure there's a low number that you don't want to walk into,
//    so just use a nice big number that will be obviously yours when
//    or if you see it in the debugger.  
//
//    Remember, you MIGHT be deriving from objects that set their own
//    window timers, so you have to make sure you're very strict with
//    how you handle all this stuff -- otherwise you'll make the stock
//    objects possibly SNAFU your whole day.
const int myTimer = 0x0F0F0F0F;
//
//    The second constant is the duration, in ms, of our timer repetition.
const int myTimerDuration = 50;

//  So, okay, the setting of a timer falls neatly under the heading of 
//  "initialization activities."  This is how we latch our initialization
//  event and perform our method overrides.  STANDARD C++ procedure, of
//  course, but I'm gonna be explicit about it, just so there's no problems:
int ExampleCanvasWnd::onInit() {
  // By calling our parent class' onInit method, we aught to have all
  // of our children (in this case, none -- but later on this is important)
  // properly initialized as well.
  int retval = EXAMPLECANVASWND_PARENT::onInit();

  // IF YOU DON'T NEED TO SET A TIMER, DO NOT SET A TIMER.  All timers will
  // slow the system down somewhat.  Lots of timers at high resolution will
  // slow the system down considerably.  PLAY NICE WITH OTHERS.
  if (myBlitStyle == EXCITING) {
    setTimer(myTimer, myTimerDuration);
  }
  return retval;
}


//
// So, every time that our 50ms timer times out, 
// (20 fps for those who like to count that way)
// this function will get called.  If you have set
// multiple timers for your window, you'll get
// called here multiple times with the different
// ID's you've specified for the system.  Don't
// overuse this feature -- the OS can only support
// so many internal timers.
void ExampleCanvasWnd::timerCallback(int id)
{
  // anyhow, so, we test the ID here to make sure we know
  // which timer fell out.  Since we only have one timer,
  // we didn't HAVE to switch on it, but it helps to setup
  // your frameworks like this anyhow.  Multiple if-chains
  // make things hard to read. :)
  switch (id)
  {
    // For those of you unfamiliar with the joy of const int,
    // being able to use the values as the cases for a switch
    // is only one of the wonderful things that using const
    // variables allows.  Read up on it, sirrah!
    case myTimer:  
      // Anyhow, if the timer was OUR timer, we do what we 
      // want to do (and go where we're going to).
      invalidate();
      // So think for yourself, baby,
      // Cause I won't be there with you.
      break;

    default:
      // If, by some vague stretch of our crazed imaginations,
      // someone ELSE in our window derivation chain decided to
      // send up a timer, we must then pass it along up the chain.
      EXAMPLECANVASWND_PARENT::timerCallback(id);
      // NO, DO NOT QUESTION, JUST DO IT.  DO IT!  DO IT NOW!!!
      //
      //      grr!
      //
      break;
  };
  
}


// ===========================================================================
//
//  EXAMPLECANVAS: "ExampleCanvasWnd::onPaint"
//
//    Anytime you are required to redraw part of your display, you will
//    be given an onPaint call which you must service.  In nearly all cases,
//    unless your painting is very slow, feel free to refresh your entire
//    rectangle [returned by clientRect()] with whatever you're painting.
//
int ExampleCanvasWnd::onPaint(Canvas *canvas) {

  // ===========================================================================
  //
  //  EXAMPLECANVAS: "PaintBltCanvas" -- canvas.h
  //
  //    Assert the existence of a canvas upon which you shall paint thyself.
  //    The canvas contains all of the basic painting methods to allow you 
  //    to get your UI written quickly and easily.
  //
  PaintBltCanvas pbc;
  if (canvas == NULL) {
    pbc.beginPaint(this);
    canvas = &pbc;
  }

  //                                                 
  //  Call the parent window for your Wnd object with the canvas you were 
  //  given or that you created, in order to allow it to handle whatever
  //  parentclass paint handling/mangling is necessary.
  EXAMPLECANVASWND_PARENT::onPaint(canvas);

  //
  //  Get the current client rect for your window.  This is the rectangle
  //  in which you should be expected to paint thyself.
  RECT crect = clientRect();

  // Yadda yadda.... heelowurld ist SO borink.
  if (myBlitStyle == BORING)  {
    // ===========================================================================
    //
    //
    //  EXAMPLECANVAS:  "Hello, World!"
    //
    //    Okay, so, this is what we're going to do, me buckos --
    //    You're all familiar with the K&R tradition, and while we
    //    import our own sacred cows from all over the world to 
    //    offer up to the Llama gods (and then to feast upon as a 
    //    fabu lunch buffet), we would hardly call ourselves REAL 
    //    programmers without a nod to the founders of our secret
    //    society.
    //
    //    So, simple enough:  Paint the window white, then give the
    //    secret password to all the love and joy in the world today.
    //

    //
    //    Righty?  Paint it white, already.  RectFills are easy.
    //
    //    The RGB macro will generate a proper 24bit color value 
    //    from the given Red, Green, and Blue 8bit components.
    //
    canvas->fillRect(&crect, RGB(0xF0, 0xF0, 0xF0)); // RGB: #F0F0F0 for all you HTML-heads

    //
    //    Now, obviously, text is a touch trickier, but if I lose you
    //    here, well, don't get discouraged.  Just go RTM, F.
    //

    //
    //    We setup our text pen, first.  These pen states will live
    //    until you change them or the canvas is deleted from scope.
    canvas->setTextColor(RGB(0, 0, 0));   // Black pen.
    canvas->setTextSize(48);              // 48 point text.
    //
    //    Last bit:  SAY the magic words and WIN a hundred dollars!
    canvas->textOutCentered(&crect, _("Hello, World!"));
    //
    // ===========================================================================
  } else {
    // ===========================================================================
    //
    //  EXAMPLECANVAS:  "Hello, World!"
    //
    //    Hello world.
    //
    //      YAH.  So.  
    //
    //                          I got bored with that.
    //  sue me.
    //

    static int lame_frame_count = 0;

    int dx = crect.top;
    int dy = crect.left;
    int ex = crect.bottom;
    int ey = crect.right;

    int side = min(ex-dx, ey-dy) >> 1; // half the length of the shortest side

    const int stepR = 1;
    const int stepG = 1;
    const int stepB = 2;

    RECT rectSize = crect;

    // A bunch of multicolored rectangles might be a nice way to start, differently
    // from the boring white background at the very least.
    int i, j;
    for (i = 0; i < side; i++)  {
      // So lets consume a spate of shrinking rectangles.
      rectSize.top      += 1;
      rectSize.bottom   -= 1;
      rectSize.left     += 1;
      rectSize.right    -= 1;

      // And to set up the way the colors slam, multiply our frame counter by
      // a large enough stepping value.
      j = i + (lame_frame_count * 0x07);
      // Then do the colorslam.
      COLORREF rectColor = RGB((-j * stepR) & 0xFF, (j * stepG) & 0xFF, (j / stepB) & 0xFF);
      // And drop the rect in.
      canvas->drawRect(&rectSize, ((i+lame_frame_count) & 0x7) != 0 ,rectColor);
      // See the middle param there?  That's a boolean to control whether or not the rect will
      // be dotted/hatched.
    }

    //
    // And, let's make our textdrawing a bit more festive, as well, what say we?
    //
    //                  aye!
    {
      j = lame_frame_count * 0x1F;
      COLORREF textColor = RGB((-j * stepR) & 0xFF, (j * stepG) & 0xFF, (j / stepB) & 0xFF);

      const int sizeModBounds = 50;
      int sizeMod = abs((lame_frame_count % sizeModBounds) - (sizeModBounds / 2));
      
      canvas->setTextColor(textColor);
      canvas->setTextSize(25 + sizeMod);
      //
      //    Next verse.
      //                Same as the first.
      //    Little bit louder.
      //                And a whole lot worse.
      canvas->textOutCentered(&crect, _("Hello, World!"));
    }

    //
    //  And, well, I did this to show off how to do line drawing.
    //
    //          REALLY.        HONEST.
    //
    //  Why would I want to be showing off or something?
    //
    {
      //  Stealing a little algo from one of my AVS SuperScopes,
      //  'cause I'm naughty like dat, funkboy.
      static double t = 0.0;
      double i,x,y,r;
      int n,c;
      int fx,fy;
      int lastx, lasty;

      //
      //  OOOOOOH, G e O m E t R y !
      //
      //  And I'm not even going to comment any of it!
      //
      //    HAH!
      //
      //  Who says we're pansies for not having trade secrets?!@?
      //
    
      const int nModBounds = 99;
      int nMod = abs( ((lame_frame_count/32) % nModBounds) - (nModBounds / 2) );
      n = (nModBounds/2) + nMod;

      for (c = 0; c < n; c++) {

        t = t + .001;

        i = (double)c/(double)n;

        r = (i * 3.14159 * 128.0) + t; 

        x = (COS( r / 64.0 ) * 0.7) + 
            (SIN( r ) * SIN( t ) * 0.7); 
        y = (SIN( r / 64.0 ) * 0.7) + 
            (COS( r ) * SIN( t ) * 0.7); 

        int ix,iy;

        ix = (int)((double)(crect.right   >> 1 )    * x) + ( crect.right  >> 1 );
        iy = (int)((double)(crect.bottom  >> 1 )    * y) + ( crect.bottom >> 1 );

        //
        //  This, however, requires a bit of commenting (so I don't get spanked
        //  for promising to document linedraw and then teasing you with it).
        if (c == 0) {
          // This is our first point-set(-tia?)
          lastx = fx = ix; lasty = fy = iy;
          // This is also going to be our last point set where we finally land.
        } else {
          // Then we draw the line
          canvas->lineDraw(lastx, lasty, ix, iy);
          lastx = ix; lasty = iy;
        }

      }
      // And, so, see... once we're done with everything else, we draw a line from
      // the LAST point to the position the first point began.
      canvas->lineDraw(lastx, lasty, fx, fy);
      // Yes, it's kinda bass-ackwards, we know -- but doing it this way will
      // actually cause much less stack traffic and go moderately faster than 
      // any other method of line drawing (outside of using point-arrays, which
      // have their own performance issues in certain situations).
    }

    // You figured out what this does, right?
    lame_frame_count++;
    // RIGHT?!@?

    ///////////////////////////////////////////////////////////////////////////////////
  }
  // Returning one as our stasisful success is so very hip, so chic, so ''right now!''
  return 1;
}


// ===========================================================================
//
//  NEED MORE INSTRUCTIONS?@!?!@
//
//    Turn the tape over and play side ``B'' !!
//


#if 0 // Reference Stuff
// ===========================================================================
//
//  EXAMPLECANVAS: Reference Stuff
//
//  Introductory Canvas Methods
//
//    For now, ignore the derivation hierarchy, and just trust me that
//    you can use all of these methods from a PaintBltCanvas instance.
//
//    Play around with the following methods in your own copy of ExampleCanvas
//    in order to get a good feel for how the different calls work.
//
//    We'll visit most of these methods throughout the early example projects.
//

  /////////////////////////////////////////////////////////////////////////////
  // *** Rectangle Commands

      void fillRect(RECT *r, COLORREF color);
      // METHOD:  fillRect        - Draw a filled rectangle
      // PARAMS:  RECT *r         - The pointer to drawn rect, in client coords
      //          COLORREF color  - The color of the rectangle.  Use the RGB macro.


      void drawRect(RECT *r, int solid, COLORREF color);
      // METHOD:  fillRect        - Draw a filled rectangle
      // PARAMS:  RECT *r         - The pointer to drawn rect, in client coords
      //          int solid       - WIP: What does this do?
      //          COLORREF color  - The color of the rectangle.  Use the RGB macro.



  /////////////////////////////////////////////////////////////////////////////
  // *** Text Font Commands

      void setTextFont(const char *font_id_name);
      // METHOD:  setTextFont     - Set the font to be used for subesquent text rendering
      // PARAMS:  const char *font_id_name  - The name of the font to set.
      // NOTES?:  WIP: How do I get a list of available fonts?

      const char *getTextFont();
      // METHOD:  getTextFont     - Get the name of the current font
      // RETURN:  const char *    - A const pointer to the current font name

      void pushTextFont(const char *font_id_name);
      // METHOD:  pushTextFont    - Set the font to be used and push the current font onto
      //                            a font stack.
      // PARAMS:  const char *font_id_name  - The name of the font to set.
      // NOTES?:  WIP: How do I get a list of available fonts?


      const char *popTextFont();
      // METHOD:  getTextFont     - Set the current font to the last font pushed to the stack
      // RETURN:  const char *    - A const pointer to the font name being removed (NOT the
      //                            font name being set)
      // NOTES?:  If you pop past the end of the stack, the return will be a VALID string
      //          pointer to an empty string.

  /////////////////////////////////////////////////////////////////////////////
  // *** Text Style Commands

      void setTextSize(int points);
      // METHOD:  setTextSize     - Set the point size of further text output
      // PARAMS:  int points      - Uhhh... I THINK it might be the points to use
      // NOTES?:  Why am I doing this?  Isn't this obvious?  Are you as insulted
      //          by reading it as I am by writing it?  Wanna know something worse?
      //          They didn't TELL me to write this... I'm actually operating under
      //          the assumption that some people NEED to have this written for them.
      //
      //          <shudder>

      int getTextSize();
      // METHOD:  getTextSize     - Get the current font point size
      // RETURN:  int             - The current font point size.
      // NOTES?:  This is where I wish I had a voice-recognition system so I wouldn't
      //          have to actually spend the time writing all of this.

      void pushTextSize(int points);
      int popTextSize();
      // METHOD:  push & pop the font size.  Acts just like the push and pop font names.
      // NOTES?:  The font size stack and font name stack are DIFFERENT stacks.  A push
      //          and pop to this place doesn't have any effect on a push and pop of a
      //          font name.  Stack pop underflow will return 0.


      void setTextBold(int bold);
      int getTextBold();
      void pushTextBold(int bold);
      int popTextBold();
      void setTextOpaque(int op);
      int getTextOpaque();
      void pushTextOpaque(int op);
      int popTextOpaque();
      void setTextAlign(int al);
      int getTextAlign();
      void pushTextAlign(int al);
      int popTextAlign();
      void setTextColor(COLORREF color);
      COLORREF getTextColor();
      void pushTextColor(COLORREF color);
      COLORREF popTextColor();
      // METHOD:  get & set & push & pop bold, opacity, and alignment.
      //          the get/set/push/pop concepts are the same as above.
      // PARAMS:  int bold        - "1" or "0" for bold or nonbold.
      //
      //          int op          - WIP: Can't find the opacity range for this variable
      //
      //          int al          - This is an enum.  The possible values are:
      //                              ALIGN_LEFT 
      //                              ALIGN_CENTER
      //                              ALIGN_RIGHT
      //                            WIP: Shouldn't this param be to the enum?
      //
      //          COLORREF color  - Color values, create using the RGB macro.
      //





  /////////////////////////////////////////////////////////////////////////////
  // *** Text Draw Commands

      // WIP: mignote: These fxns should return a RECT set to the bounds
      // of the drawn text (clipped to window).  OR, they should return
      // a bitmap region fully describing the pixels rendered by the font
      // (which would similarily include a bounding rect).  Perhaps the
      // complex region return should be an optional penstate for drawing?
      //
      //  Drawbacks to this, of course, would be added stack traffic caused
      //    by copying 16 bytes of return value.  How much of a performance
      //    issue would that be?
      //
      //  How about extending the paramlist with a terminating pointer to
      //  RECT that has a C++ default of NULL?  Preserves legacy code and makes
      //  the copy traffic entirely voluntary.  One extra jnz per call.

      void textOut(int x, int y, const char *txt);
      // METHOD:  textOut - Draw the given string at the given point.
      // PARAMS:  x =   The x position in pixels from clientRect origin.
      //          y =   The y position in pixels from clientRect origin.
      //          txt = The string to be drawn.

      void textOut(int x, int y, int w, int h, const char *txt);
      // METHOD:  textOut         - Draw the given string at the given point,
      //          clipped to the given extents.
      // PARAMS:  int x           - The x position in pixels from clientRect origin.
      //          int y           - The y position in pixels from clientRect origin.
      //          int w           - The width extent to which to clip.
      //          int h           - The height extent to which to clip.
      //          const char *txt - The string to be drawn.

      void textOutEllipsed(int x, int y, int w, int h, const char *txt);
      // METHOD:  textOutEllipsed - Draw the given string at the given point,
      //          clipped to the given extents.  If the text is clipped out,
      //          end the drawn text with an ellipsis.
      // PARAMS:  int x           - The x position in pixels from clientRect origin.
      //          int y           - The y position in pixels from clientRect origin.
      //          int w           - The width extent to which to clip.
      //          int h           - The height extent to which to clip.
      //          const char *txt - The string to be drawn.

      void textOutWrapped(int x, int y, int w, int h, const char *txt);
      // METHOD:  textOutWrapped  - Draw the given string, wrapped to the given extents.
      // PARAMS:  int x           - The x position in pixels from clientRect origin.
      //          int y           - The y position in pixels from clientRect origin.
      //          int w           - The width extent to which to clip.
      //          int h           - The height extent to which to clip.
      //          const char *txt - The string to be drawn.


      void textOutWrappedPathed(int x, int y, int w, const char *txt);
      // METHOD:  textOutWrappedPathed  - Display a long path name properly ellipsised
      // PARAMS:  int x           - The x position in pixels from clientRect origin.
      //          int y           - The y position in pixels from clientRect origin.
      //          int w           - The width extent to which to clip.
      //          const char *txt - The string to be drawn.

      void textOutCentered(RECT *r, const char *txt);
      // METHOD:  textOutCentered - Draw the given string centered to the given rect.
      // PARAMS:  RECT *r         - The rect within which to center.
      //          const char *txt - The string to be drawn.


  /////////////////////////////////////////////////////////////////////////////
  // *** Text Preflight Commands

      int getTextWidth(const char *text);
      // METHOD:  getTextWidth    - Find out how wide the string will be in pixels,
      //          given the current text formatting settings.
      // PARAMS:  const char *txt - The string to be drawn.
      // RETURN:  int             - The width in pixels of the given string.


      int getTextHeight(const char *text);
      // METHOD:  getTextWidth    - Find out how tall the string will be in pixels,
      //          given the current text formatting settings.
      // PARAMS:  const char *txt - The string to be drawn.
      // RETURN:  int             - The width in pixels of the given string.


      void getTextExtent(const char *text, int *w, int *h);
      // METHOD:  getTextExtent   - Find out how wide and tall the string will be in pixels,
      //          given the current text formatting settings.
      // PARAMS:  const char *txt - The string to be drawn.
      // RETURN:  int             - The width in pixels of the given string.

      int getTextHeight() 
      // METHOD:  getTextHeight   - Find out the maximum height of the current font.
      // RETURN:  int             - The max height in pixels of the current font.


  /////////////////////////////////////////////////////////////////////////////
  // *** Pen Commands

      void lineDraw(int fromX, int fromY, int toX, int toY);
      // METHOD:  lineDraw        - Draw a line between two points.  This doesn't rely
      //                            on remembering a state like moveTo/lineTo does.
      // PARAMS:  int fromX       - The starting x position in pixels from clientRect origin.
      //          int fromY       - The starting y position in pixels from clientRect origin.
      //          int toX         - The ending x position in pixels from clientRect origin.
      //          int toY         - The ending y position in pixels from clientRect origin.

#endif // Reference Stuff
