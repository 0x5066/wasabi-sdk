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

#ifndef _PLSTRING_H
#define _PLSTRING_H

#include "string.h"
#include "../bfc/ptrlist.h"
#include "../studio/services/svc_playlist.h"
#include "xlatstr.h"

// intentionally weird :) --BU
template <class T, int b=0>
class PlaylistExtensionStringT : public String, private SvcEnumT<T> {
protected:
  PlaylistExtensionStringT() : String(b?0:"Playlist files (") {
    PtrList<String> exts;
    T *svc = getNext();
    for (int i = 0; svc; svc = getNext()) {
      const char *ext = svc->getExtension();
      if (ext && *ext) { exts.addItem(new String(ext)); i++; }
      release(svc);
    }
    if (i == 0) {
      cat(_("No playlist services installed"));
      cat("|*.*|");
    } else {
      if (b) {
        for (i--; i>=0; --i) {
          const char *ext = *exts.q(i);
          cat(StringPrintf("%s playlist|*.%s%s", ext, ext, i ?"|":"||"));
        }
      } else {
        for (int l = i--; i>=0; i--)
          cat(StringPrintf("%s%s", (const char *)*exts[i], i ?",":")|"));
        for (--l; l>=0; l--)
          cat(StringPrintf("*.%s%s", (const char *)*exts[l], l ? ";":"|"));
        cat(_("All files"));
        cat("(*.*)|*.*|");
      }
    }
    changeChar('|', '\0');
    exts.deleteAll();
  }
};

class PlaylistExtensionStringR : public PlaylistExtensionStringT<svc_playlistReader> { };

class PlaylistExtensionStringW : public PlaylistExtensionStringT<svc_playlistWriter, 1> { };

#endif
