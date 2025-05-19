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


#include "selectfile.h"

#include "../studio/api.h"
#include "popup.h"
#include "../studio/services/svc_filesel.h"

SelectFile::SelectFile(RootWnd *parent, const char *menu_prefix, const char *menu_suffix) :
  parentWnd(parent),
  prefix_str(menu_prefix),
  suffix_str(menu_suffix)
{
  svc = NULL;
  ASSERT(parent != NULL);
  xpos = ypos = 0;
  pos_set = 0;
}

SelectFile::~SelectFile() {
  if (svc) api->service_release(svc);
  types.deleteAll();
}

void SelectFile::setDefaultDir(const char *dir) {
  default_dir = dir;
}

const char *SelectFile::getDirectory() {
  if (svc) return svc->getDirectory();
  return NULL;
}

void SelectFile::setIdent(const char *id) {
  ident = id;
}

void SelectFile::setPopPosition(int x, int y) {
  xpos = x;
  ypos = y;
  pos_set = 1;
}

int SelectFile::runSelector(const char *type, int allow_multiple, const char *extlist) {
  if (svc) api->service_release(svc); svc = NULL;
  types.deleteAll();
  if (type == NULL) {
    int pos = 0;
    for (;;) {
      waServiceFactory *wasvc = api->service_enumService(WaSvc::FILESELECTOR, pos++);
      if (wasvc == NULL) break;
      svc_fileSelector *sfs = castService<svc_fileSelector>(wasvc);
      const char *pref = sfs->getPrefix();
      if (pref != NULL)
        types.addItem(new String(pref));
      api->service_release(sfs);
    }
    if (types.getNumItems() <= 0) return 0;	// none?!

    PopupMenu *pop = new PopupMenu(parentWnd);
    for (int i = 0; i < types.getNumItems(); i++) {
      String str;
      str += prefix_str;
      str += types[i]->getValue();
      str += suffix_str;
      pop->addCommand(str, i);
    }
    int cmd = pos_set ? pop->popAtXY(xpos, ypos) : pop->popAtMouse();
    String *s = types[cmd];
    if (s == NULL) return 0;
    type = *s;
  }
  ASSERT(type != NULL);

  saved_type = type;

  svc = FileSelectorEnum(type).getFirst();
  ASSERT(svc != NULL);	// we just enumed it
  if (extlist != NULL) svc->setExtList(extlist);
//FUCKO : need to set open vs. save as
  api->pushModalWnd();
  int r = svc->runSelector(parentWnd, FileSel::OPEN, allow_multiple, ident.isempty() ? type : ident, default_dir);
  api->popModalWnd();
  ASSERT(svc != NULL);	// we just enumed it
  return r;
}

const char *SelectFile::getType() {
  return saved_type;
}

int SelectFile::getNumFiles() {
  return svc ? svc->getNumFilesSelected() : 0;
}

const char *SelectFile::enumFilename(int n) {
  return svc ? svc->enumFilename(n) : NULL;
}
