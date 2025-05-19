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


#include "queryline.h"

#include "../../studio/api.h"
#include "../../common/metatags.h"
#include "../string.h"
#include "../../bfc/util/pathparse.h"
#include "../../common/metatags.h"

QueryLine::QueryLine(const char *query) : querytext(query), autoquery(FALSE),
  autofield(MT_NAME) {
}

void QueryLine::setQuery(const char *query) {
  querytext = query;
  String sp = querytext;
  if (autoquery && !querytext.isempty()) {
    sp = "";
    PathParser pp(querytext, " ");
    if (pp.getNumStrings() <= 0) return;
    for (int i = 0; i < pp.getNumStrings(); i++) {
      if (i != 0) sp += " and ";
      sp += StringPrintf("(\"%s\" has \"%s\")", autofield.getValue(), pp.enumString(i));
    }
  }
  sqs_setQuery(sp);
}

int QueryLine::setAuto(int bv) {
  autoquery = !!bv;
  setQuery(querytext);
  return 1;
}
