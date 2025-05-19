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

#include "autoquerylist.h"
#include "../db/subqueryserver.h"
#include "../db/multiqueryclient.h"
#include "../db/sharedscanner.h"
#include "../../common/playstring.h"
#include "../../common/metatags.h"
#include "../../common/fakedrag.h"
#include "../../common/filename.h"
#include "../../studio/api.h"
#include "../../studio/services/svc_droptarget.h"
#include "../../bfc/svc_enum.h"

#include "../../pledit/svc_pldir.h"
#include "../../pledit/playlist.h"
#include "../../pledit/editor.h"

#define TIMER_SCANNERDEL 0x6879

AutoQueryList::AutoQueryList() :
  playlist(NULL)
{
  lastpc = -1;
}

AutoQueryList::~AutoQueryList() {
  getGuiObject()->guiobject_removeAppCmds(this);
  SvcEnum::release(pldir);
}

int AutoQueryList::onInit() {
  AUTOQUERYLIST_PARENT::onInit();

  pldir = SvcEnumByGuid<svc_plDir>();
  if (pldir != NULL) {
    PlaylistHandle hand = pldir->insertPlaylist(NULL, "Media library query results", NULL, TRUE);
    pldir->setAutoSave(hand, FALSE);
    playlist = pldir->getPlaylist(hand);
    playlist->lock(TRUE);
  }

  appcmds_addCmd("Reset", 0, AppCmds::SIDE_RIGHT);
  getGuiObject()->guiobject_addAppCmds(this);

  mqs_refresh();
  return 1;
}

void AutoQueryList::mqs_onAddPlaystring(const char *playstring, int nitems, int thispos) {
  int n = (int)(thispos / (float)nitems * 100.0f);
  nfound++;
  if (n > lastpc) {
    getGuiObject()->guiobject_setCompleted(n);
    getGuiObject()->guiobject_setStatusText(StringPrintf("%d%c, %d item%s found", n, '%', nfound, (nfound > 1) ? "s" : ""), TRUE);
    lastpc = n;
  }
  if (playlist != NULL)
    playlist->addPlaylistItem(playstring);
}

void AutoQueryList::mqs_onCompleteMultiQuery() {
  getGuiObject()->guiobject_setStatusText(StringPrintf("100%c, %d item%s found", '%', nfound, (nfound > 1) ? "s" : ""), TRUE);
  lastpc = -1;
  getGuiObject()->guiobject_popCompleted();
}

void AutoQueryList::mqs_onNewMultiQuery() {
  nfound = 0;
  getGuiObject()->guiobject_setStatusText("0%", TRUE);
  getGuiObject()->guiobject_pushCompleted();
  if (playlist != NULL) {
    playlist->deleteAll();

    GuiObject *ed = getGuiObject()->guiobject_findObjectByInterface(Editor::getInterfaceGuid());
    if (ed != NULL) {
      Editor *e = static_cast<Editor*>(ed->guiobject_getRootWnd()->getInterface(Editor::getInterfaceGuid()));
      e->setPlaylistByHandle(playlist->getHandle());
    }
  }
}

void AutoQueryList::appcmds_onCommand(int id, const RECT *buttonRect) {
  switch (id) {
    case 0:
      resetSubQueries();
    break;
  }
}
