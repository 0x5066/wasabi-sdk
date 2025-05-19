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

#include "minibrowser.h"

#define CBCLASS MiniBrowserCallbackI
START_DISPATCH;
  CB(MINIBROWSER_ONBEFORENAVIGATE,     minibrowsercb_onBeforeNavigate);
  VCB(MINIBROWSER_ONDOCUMENTCOMPLETE,  minibrowsercb_onDocumentComplete);
END_DISPATCH;
#undef CBCLASS

#define CBCLASS MiniBrowserI
START_DISPATCH;
  CB(MINIBROWSER_GETROOTWND,           minibrowser_getRootWnd);
  CB(MINIBROWSER_NAVIGATEURL,          minibrowser_navigateUrl);
  CB(MINIBROWSER_BACK,                 minibrowser_back);
  CB(MINIBROWSER_FORWARD,              minibrowser_forward);
  CB(MINIBROWSER_HOME,                 minibrowser_home);
  CB(MINIBROWSER_REFRESH,              minibrowser_refresh);
  CB(MINIBROWSER_STOP,                 minibrowser_stop);
  VCB(MINIBROWSER_SETTARGETNAME,       minibrowser_setTargetName);
  CB(MINIBROWSER_GETTARGETNAME,        minibrowser_getTargetName);
  CB(MINIBROWSER_GETCURRENTURL,        minibrowser_getCurrentUrl);
  VCB(MINIBROWSER_ADDCB,               minibrowser_addCB);
  VCB(MINIBROWSER_SETHOME,             minibrowser_setHome);
END_DISPATCH;
#undef CBCLASS

