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


#include "../studio/api.h"
#include "filteredcolor.h"

FilteredColor::FilteredColor(COLORREF _color, const char *colorgroupname) {
  color = _color;
  filteredcolor = _color;
  group = colorgroupname;
  skin_iterator = NULL;
  latest_iteration = -1;
  need_filter = 1;
}

FilteredColor::~FilteredColor() {
}

void FilteredColor::setColor(COLORREF _color) { 
  color = _color; 
  filteredcolor = color;
}

void FilteredColor::setColorGroup(const char *_group) {
  group = _group;
  need_filter = 1;
}

COLORREF FilteredColor::getColor() {
  ensureFiltered();
  return filteredcolor;
}

COLORREF *FilteredColor::getColorRef() {
  ensureFiltered();
  return &filteredcolor;
}

void FilteredColor::ensureFiltered() {

  // fetch iterator pointer if necessary
  if (skin_iterator == NULL) {
    skin_iterator = api->skin_getIterator();
    ASSERT(skin_iterator != NULL);
  }

  // see if we're current
  if (*skin_iterator != latest_iteration) {
    need_filter = 1;	// pointer now invalid, must re-get
    latest_iteration = *skin_iterator;	// and then we'll be current
  }


  if (need_filter) {
    filteredcolor = api->filterSkinColor(color, group);
    need_filter = 0;
  }
}

int *FilteredColor::skin_iterator=NULL;
