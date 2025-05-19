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

#ifndef _EXAMPLEEXTERNELEMENT_H
#define _EXAMPLEEXTERNELEMENT_H

#include "../../studio/wac.h"
#include "../../common/script/c_script/h_guiobject.h"
#include "../../common/script/c_script/c_guiobject.h"
#include "../../common/script/c_script/c_group.h"
#include "mainwnd.h"
#include "hooks.h"


class ExampleExternElement : public WAComponentClient {
public:
  ExampleExternElement();

  virtual const char *getName() { return "Free Form Component"; };
  virtual GUID getGUID();

private:

};

extern WAComponentClient *the;

#endif //_EXAMPLEEXTERNELEMENT_H
