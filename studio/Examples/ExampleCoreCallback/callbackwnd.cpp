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

#include "callbackwnd.h"
#include "callbackconsole.h"
#include "resource.h"

#include "../../studio/api.h"
#include "../../bfc/wnds/buttwnd.h"

CallbackWnd::~CallbackWnd() {
  // As we die, remove this object from Core Callback object list.
  api->core_delCallback(0, this);
}

int CallbackWnd::onInit() {
  CALLBACKWND_PARENT::onInit();
  // As we init, add this object to process Core Callback events.
  api->core_addCallback(0, this);

  insertColumn(new ListColumn("Message", TRUE));

  // Set the titlebar text for the window.
  setName("CallbackWnd");

  return 1;
}

// =========================================================================
//
//  Methods required by Window Creation Services
//
const char *CallbackWnd::getWindowTypeName() { return "Example Core Callback Window"; }
GUID CallbackWnd::getWindowTypeGuid() { return the->getGUID(); }
void CallbackWnd::setIconBitmaps(ButtonWnd *button) {
  button->setBitmaps(the->gethInstance(), IDB_TAB_NORMAL, NULL, IDB_TAB_HILITED, IDB_TAB_SELECTED);
}

int CallbackWnd::corecb_outputString(const char *string) {
  addItem(string, 0);
  if (getNumItems() > 128)	// FUCKO: make configurable
    deleteByPos(0);
  ensureItemVisible(getNumItems()-1);
  return 1;
}

//
//  The COMPLETE virtual method table from "../../studio/corecb.h" -- We're going to override all of them.
//
//  These are all fairly self explanatory as far as I can tell, so this will be in lieu of your usual
//  list of referenced methods.
//

// Basic Events
int CallbackWnd::corecb_onStarted() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onStarted"));
  return 0;
}

int CallbackWnd::corecb_onStopped() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onStopped"));
  return 0;
}

int CallbackWnd::corecb_onPaused() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onPaused"));
  return 0;
}

int CallbackWnd::corecb_onUnpaused() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onUnpaused"));
  return 0;
}

int CallbackWnd::corecb_onSeeked(int newpos) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onSeeked: newpos = %d", newpos));
  return 0;
}


// Volume & EQ Events
int CallbackWnd::corecb_onVolumeChange(int newvol) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onVolumeChange: newvol = %d", newvol));
  return 0;
}

int CallbackWnd::corecb_onEQStatusChange(int newval) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onEQStatusChange: newval = %d", newval));
  return 0;
}

int CallbackWnd::corecb_onEQPreampChange(int newval) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onEQPreampChange: newval = %d", newval));
  return 0;
}

int CallbackWnd::corecb_onEQBandChange(int band, int newval) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onEQBandChange: band = %d, newval = %d", band, newval));
  return 0;
}


// Info Events
int CallbackWnd::corecb_onTitleChange(const char *title) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onTitleChange: title = %s", title));
  return 0;
}

int CallbackWnd::corecb_onTitle2Change(const char *title, const char *title2) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onTitle2Change: title = %s, title2 = %s", title, title2));
  return 0;
}

int CallbackWnd::corecb_onInfoChange(const char *info) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onInfoChange: info = %s", info));
  return 0;
}

int CallbackWnd::corecb_onUrlChange(const char *url) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onUrlChange: url = %s", url));
  return 0;
}


// Source Events
int CallbackWnd::corecb_onNextFile() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onNextFile"));
  return 0;
}

int CallbackWnd::corecb_onNeedNextFile(int fileid) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onNeedNextFile: fileid = %d", fileid));
  return 0;
}

int CallbackWnd::corecb_onAbortCurrentSong() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onAbortCurrentSong"));
  return 0;
}

int CallbackWnd::corecb_onEndOfDecode() {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onEndOfDecode"));
  return 0;
}


// Bad Events.
int CallbackWnd::corecb_onErrorOccured(int severity, const char *text) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onErrorOccured: severity = %d, text = %s", severity, text));
  return 0;
}

int CallbackWnd::corecb_onStatusMsg(const char *text) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onStatusMsg: text = %s", text));
  return 0;
}

int CallbackWnd::corecb_onWarningMsg(const char *text) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onWarningMsg: text = %s", text));
  return 0;
}

int CallbackWnd::corecb_onErrorMsg(const char *text) {
  corecb_outputString(StringPrintf("CallbackWnd::corecb_onErrorMsg: text = %s", text));
  return 0;
}


