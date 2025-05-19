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

#ifndef _CALLBACKWND_H
#define _CALLBACKWND_H

#include "../../bfc/wnds/listwnd.h"
#include "../../studio/consolecb.h"
#include "../../studio/corecb.h"

// Forward references
class ButtonWnd;

// See, what we can do here is make our ListWnd ALSO inherit from CoreCallback...
// Why keep track of pointers and send events between objects when one object can do it all?

#define CALLBACKWND_PARENT ListWnd
class CallbackWnd : public ListWnd, public CoreCallbackI {
public:
  virtual ~CallbackWnd();

  virtual int onInit();


  // =========================================================================
  //
  //  Methods required by Window Creation Services
  //
  static const char *getWindowTypeName();
  static GUID getWindowTypeGuid();
  static void setIconBitmaps(ButtonWnd *button);


  // =========================================================================
  //
  //  The COMPLETE virtual method table from "../../studio/corecb.h" 
  //    (We're going to override all of them!  Woo hoo!)
  //

  // Basic Events
  virtual int corecb_onStarted();
  virtual int corecb_onStopped();
  virtual int corecb_onPaused();
  virtual int corecb_onUnpaused();
  virtual int corecb_onSeeked(int newpos);

  // Volume & EQ Events
  virtual int corecb_onVolumeChange(int newvol);
  virtual int corecb_onEQStatusChange(int newval);
  virtual int corecb_onEQPreampChange(int newval);
  virtual int corecb_onEQBandChange(int band, int newval);

  // Info Events
  virtual int corecb_onTitleChange(const char *title);
  virtual int corecb_onTitle2Change(const char *title, const char *title2);
  virtual int corecb_onInfoChange(const char *info);
  virtual int corecb_onUrlChange(const char *url);

  // Source Events
  virtual int corecb_onNextFile();
  virtual int corecb_onNeedNextFile(int fileid);
  virtual int corecb_onAbortCurrentSong();
  virtual int corecb_onEndOfDecode();

  // Bad Events.
  virtual int corecb_onErrorOccured(int severity, const char *text);
  virtual int corecb_onStatusMsg(const char *text);
  virtual int corecb_onWarningMsg(const char *text);
  virtual int corecb_onErrorMsg(const char *text);

  
  // Output the String
  virtual int corecb_outputString(const char *string);
};

#endif
