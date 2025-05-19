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


#include "appcmds.h"

#define CBCLASS AppCmdsI
START_DISPATCH;
  CB(APPCMDS_GETNUMCMDS, appcmds_getNumCmds);
  CB(APPCMDS_ENUMCMD, appcmds_enumCmd);
  VCB(APPCMDS_ONCOMMAND, appcmds_onCommand);
END_DISPATCH;
#undef CBCLASS

AppCmdsI::~AppCmdsI() {
  foreach(cmds)
    if (cmds.getfor()->autodelete) delete cmds.getfor();
  endfor
}

void AppCmdsI::appcmds_addCmd(CmdRec *cmdrec) {
  cmds.addItem(cmdrec);
}

void AppCmdsI::appcmds_addCmd(const char *name, int id, int side) {
  cmds.addItem(new CmdRec(name, id, side, TRUE));
}

void AppCmdsI::appcmds_deleteAll() {
  foreach(cmds)
    if (cmds.getfor()->autodelete) delete cmds.getfor();
  endfor
  cmds.removeAll();
}

int AppCmdsI::appcmds_getNumCmds() {
  return cmds.getNumItems();
}

const char *AppCmdsI::appcmds_enumCmd(int n, int *side, int *id) {
  CmdRec *cr = cmds[n];
  if (cr == NULL) return NULL;
  if (side != NULL) *side = cr->side;
  if (id != NULL) *id = cr->id;
  return cr->cmdname;
}

void AppCmdsI::appcmds_onCommand(int id, const RECT *buttonRect) {
  foreach(cmds)
    if (cmds.getfor()->id == id) {
      cmds.getfor()->onCommand(buttonRect);
      break;
    }
  endfor
}
