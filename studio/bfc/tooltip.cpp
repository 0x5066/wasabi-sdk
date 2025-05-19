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

#include "tooltip.h"
#include "../studio/api.h"
#include "../attribs/attrint.h"
#include "../attribs/cfgitem.h"
#include "../bfc/svc_enum.h"
#include "../common/xlatstr.h"

Tooltip::Tooltip(const char *txt) {
  api->appdeactivation_push_disallow(NULL);
  svc = NULL;

  if (!txt || !*txt) return;

  // {9149C445-3C30-4e04-8433-5A518ED0FDDE}
  const GUID uioptions_guid = 
  { 0x9149c445, 0x3c30, 0x4e04, { 0x84, 0x33, 0x5a, 0x51, 0x8e, 0xd0, 0xfd, 0xde } };
  if (!_intVal(api->config_getCfgItemByGuid(uioptions_guid), "Enable tooltips")) {
    // tooltips disabled
    return;
  }

  waServiceFactory *svf = api->service_enumService(WaSvc::TOOLTIPSRENDERER, 0);
  svc = castService<svc_toolTipsRenderer>(svf);

  if (!svc) {
    // no tooltips available!
    return;
  }

  svc->spawnTooltip(_(txt));
}

Tooltip::~Tooltip() {
  if (svc != NULL)
    api->service_release(svc);
  api->appdeactivation_pop_disallow(NULL);
}
