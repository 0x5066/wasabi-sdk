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
//      File:     examplevisdatawnd.cpp
//
//!##   Purpose:  Would you like to learn how visualization data works?
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

#include "../../bfc/std.h"

#include "examplevisdata.h"
#include "examplevisdatawnd.h"
#include "resource.h"

#include "../../bfc/canvas.h"


// =========================================================================
//
//  EXAMPLEVISDATA: Methods required by Window Creation Services
//
const char *ExampleVisDataWnd::getWindowTypeName() { return /**/"ExampleVisData Window"/*EDITME*/; }
GUID ExampleVisDataWnd::getWindowTypeGuid() { return the->getGUID(); }
void ExampleVisDataWnd::setIconBitmaps(ButtonWnd *button) {
  // Note that IDB_TAB_XXXXXX defines come from resource.h and the data is in resource.rc
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}


// ===========================================================================
//
//    EXAMPLEVISDATA: Timers & Timer Callbacks.
//
//    Just like Ex1, setup a 20fps timer to pump our window blitting.
//
const int myTimer = 0x0BADCAFE; // The food made me SICK last night.  Ugh.
const int myTimerDuration = 50;

int ExampleVisDataWnd::onInit() {
  int retval = EXAMPLEVISDATAWND_PARENT::onInit();

  setTimer(myTimer, myTimerDuration);
  return retval;
}

void ExampleVisDataWnd::timerCallback(int id)
{
  switch (id)
  {
    case myTimer:  
      invalidate();
      break;

    default:
      EXAMPLEVISDATAWND_PARENT::timerCallback(id);
      break;
  };
}


// ===========================================================================
//
//    EXAMPLEVISDATA: "ExampleVisDataWnd::onPaint"
//    
//    And then, when we invalidate, we're asked to paint, so we'll get vis, then paint it.
//
int ExampleVisDataWnd::onPaint(Canvas *canvas) {
  // Make sure there actually is a canvas to paint to.
  PaintBltCanvas pbc;
  if (canvas == NULL) {
    pbc.beginPaint(this);
    canvas = &pbc;
  }

  // Then, call your parent class's onpaint.
  EXAMPLEVISDATAWND_PARENT::onPaint(canvas);

  //
  //  Get the current client rect for your window.  This is the rectangle
  //  in which you should be expected to paint thyself.
  RECT crect = clientRect();

  // Fill the canvas with white and prepare to blit your little heart out.
  canvas->fillRect(&crect, RGB(0x00, 0x00, 0x00));


  //
  //
  //  *** Vis Data Methods
  //
  //
  //  This is what you wanted to see, right?
  //
  //  There's three primary API methods you can call to get back a mathematical
  //  representation of what the music "looks like."  Visualization data, wot?
  //

  //
  //  The simple data calls are for the Vu meteres for the left and right channels.
  //
  //  The "VU Meter" is simply the peak "volume" for each channel.
  //
  //  The returned values are an integer between 0 and 256
  //
  int leftVUdata = api->core_getLeftVuMeter(0); // zero is the "main" core.  
  int rightVUdata = api->core_getRightVuMeter(0); // zero is the "main" core.  
  // you're probably not using multiple cores yet.  trust me.

  //
  //  Then, after we get the VU data, we make a graphical representation of it.
  //  
  int h = crect.bottom - crect.top;
  int w = crect.right - crect.left;

  int vu_height = h / 2;

  //
  //  *** Left VU
  //
  if (leftVUdata) {
    RECT leftvu;
    // we want to make the rectangles "grow" upward, so we fix our bottom edge first.
    leftvu.bottom = crect.bottom - (h / 4);  // One quarter of the window height from the bottom.
    // and 10 pixels in?
    leftvu.left = crect.left + 10;
    // and 20 pixels wide?
    leftvu.right = leftvu.left + 20;
    // and then we calc our height.
    int leftvu_h = (vu_height * leftVUdata) / 256;  // if you tell me to shift 8, I'll say "yes, but YOU know that.  does everyone?"
    leftvu.top = (leftvu.bottom - leftvu_h) + 2;
    // and then calc the colors.
    int green = (leftVUdata < 128) ? 255 : 255 - ((leftVUdata-128) * 2);
    int red = (leftVUdata < 128) ? 255 - ((leftVUdata) * 2) : 255;
    // and then we blit a rect.
    canvas->fillRect(&leftvu, RGB(red, green, 0x00));
  }

  //
  //  *** Right VU
  //
  if (rightVUdata) {
    RECT rightvu;
    // we want to make the rectangles "grow" upward, so we fix our bottom edge first.
    rightvu.bottom = crect.bottom - (h / 4);  // One quarter of the window height from the bottom.
    // and 10 pixels in?
    rightvu.right = crect.right - 10;
    // and 20 pixels wide?
    rightvu.left = rightvu.right - 20;
    // and then we calc our height.
    int rightvu_h = (vu_height * rightVUdata) / 256;  // if you tell me to shift 8, I'll say "yes, but YOU know that.  does everyone?"
    rightvu.top = (rightvu.bottom - rightvu_h) + 2;
    // and then calc the colors.
    int green = (rightVUdata < 128) ? 255 : 255 - ((rightVUdata-128) * 2);
    int red = (rightVUdata < 128) ? 255 - ((rightVUdata) * 2) : 255;
    // and then we blit a rect.
    canvas->fillRect(&rightvu, RGB(red, green, 0x00));
  }
  


  //
  //  The more complex call is for the full vis data.
  //
  //  Essentially, you pass a memory buffer into the api and it will fill it with both
  //  types of vis data available for the current audio coming out of the core.
  //
  //  To make life a little easier on all of us, instead of passing a buffer, we're
  //  going to pass a struct (but just remember, it's treating it like a buffer).
  //
  typedef struct {
    enum {
      LEFT = 0,
      RIGHT = 1
    };
    unsigned char spectrumData[2][576];
    char waveformData[2][576];
  } VisData;
  // Many thanks to Vikki at Wild Tangent for helping me realize it's alot easier to
  // show everybody what the hell is going on if you use a struct.  :)

  // So, take the struct, fill it with the current vis data.
  VisData myVisData;
  int retval = api->core_getVisData(0, &myVisData, sizeof(VisData));

  // If there's anything playing, we'll get some, and draw.
  if (retval) {
    // Setup the pen color to draw the lines.
    canvas->pushPen(RGB(0xFF, 0xFF, 0xFF));

    //
    // Now we're going to draw the spectrum and waveform datas, like the "Render-Simple" in AVS.
    //
    // All this floating point math probably isn't a great idea if you want really FAST vis stuff, tho.)
    //
    // I'm coding for (hopefully?) clarity, not speed, here.  :)
    //

    float dataW = (float)(w - 80); // figure out the width, in pixels, we're going to draw to.
    float pointW = dataW / 576.0F; // figure out the width, in fractional pixels, for each datapoint.

    int dataH = h;  // total height of the complex vis data area we're going to blit into.
    int sectionH = dataH / 5; // 5 sections, Wave & Spect, L & R channels, and one more to center it.
    int startPointX = 40; // the X value to begin drawing at.
    int startPointY = (sectionH / 2); // the Y value to begin drawing at.
    int zeroY; // the 0 point on the y axis for our line rendering for each section.

    int lastX = 0, lastY = 0, thisX = 0, thisY = 0; // the line draw coords
    int i;

    //
    //  *** Spectrum Data
    //

    // Left channel
    zeroY = startPointY + sectionH; // bottom of the first section, draw upward.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      //
      // Our new points based on the vis data:

      // thisX just divides each point as evenly as possible across the width.
      thisX = startPointX + (int)(pointW * (float)i);  
      // and thisY treats the data as an 8bit fraction of sectionH 
      thisY = zeroY - (((myVisData.spectrumData[VisData::LEFT][i]) * sectionH) / 256);
      // Spectrum Data is UNSIGNED DATA.  Pay attention to this or you'll wind up with weird negative numbers.

      // Then draw, from the last point to the this point. 
      canvas->lineDraw(lastX, lastY, thisX, thisY);

      // And then save where we just drew so we can draw the next segment.
      lastX = thisX;
      lastY = thisY;
    }   

    // Right channel
    zeroY = startPointY + (sectionH * 2); // bottom of the second section, draw upward.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      //
      // Our new points based on the vis data:

      // thisX just divides each point as evenly as possible across the width.
      thisX = startPointX + (int)(pointW * (float)i);  
      // and thisY treats the data as an 8bit fraction of sectionH 
      thisY = zeroY - (((myVisData.spectrumData[VisData::RIGHT][i]) * sectionH) / 256);
      // Spectrum Data is UNSIGNED DATA.  Pay attention to this or you'll wind up with weird negative numbers.

      // Then draw, from the last point to the this point. 
      canvas->lineDraw(lastX, lastY, thisX, thisY);

      // And then save where we just drew so we can draw the next segment.
      lastX = thisX;
      lastY = thisY;
    }   

    //
    //  *** Waveform Data
    //

    // Left channel
    zeroY = startPointY + (sectionH * 3); // middle of the third section, draw up and down.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      //
      // Our new points based on the vis data:

      // thisX just divides each point as evenly as possible across the width.
      thisX = startPointX + (int)(pointW * (float)i);  
      // and thisY treats the data as a signed 8bit fraction of sectionH 
      thisY = zeroY - (((myVisData.waveformData[VisData::LEFT][i]) * sectionH) / 128);
      // Waveform Data is SIGNED DATA.  Feel free to pay just as much attention to this, since it's max values are 128-128

      // Then draw, from the last point to the this point. 
      canvas->lineDraw(lastX, lastY, thisX, thisY);

      // And then save where we just drew so we can draw the next segment.
      lastX = thisX;
      lastY = thisY;
    }   

    // Right channel
    zeroY = startPointY + (sectionH * 4); // middle of the fourth section, draw up and down.
    lastX = startPointX;
    lastY = zeroY;
    for (i = 0; i < 576; i++) {
      //
      // Our new points based on the vis data:

      // thisX just divides each point as evenly as possible across the width.
      thisX = startPointX + (int)(pointW * (float)i);  
      // and thisY treats the data as a sigend 8bit fraction of sectionH 
      thisY = zeroY - (((myVisData.waveformData[VisData::RIGHT][i]) * sectionH) / 128);
      // Waveform Data is SIGNED DATA.  Feel free to pay just as much attention to this, since it's max values are 128-128

      // Then draw, from the last point to the this point. 
      canvas->lineDraw(lastX, lastY, thisX, thisY);

      // And then save where we just drew so we can draw the next segment.
      lastX = thisX;
      lastY = thisY;
    }   

    // Restore the pen to its last state.
    canvas->popPen();
  }

  // Anything less than success is strictly inconceivable.
  return 1;
}

