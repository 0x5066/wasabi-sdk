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


#include "skinclr.h"

#include "../studio/api.h"

int *SkinColor::skin_iterator=0;

SkinColor::SkinColor(const char *_name, const char *colorgroup) : FilteredColor(0, (colorgroup == NULL || !*colorgroup) ? "Text" : colorgroup) {
  latest_iteration = -1;
  skin_iterator = NULL;
  setElementName(_name);
}

DWORD SkinColor::v() {
  if (name == NULL) return 0;

  // fetch iterator pointer if necessary
  if (skin_iterator == NULL) {
    skin_iterator = api->skin_getIterator();
    ASSERT(skin_iterator != NULL);
  }

  // see if we're current
  if (*skin_iterator != latest_iteration) {
    val = NULL;	// pointer now invalid, must re-get
    latest_iteration = *skin_iterator;	// and then we'll be current
  }

  // new pointer please
  if (val == NULL) {
    setColor(api->skin_getColorElement((char *)name));
    val = getColorRef();
  }
  if (val == NULL) return 0xFFFFFFFF;

  return *val;
}

void SkinColor::setElementName(const char *_name) {
  name = _name;
  val = NULL;
  latest_iteration = 0;
}

